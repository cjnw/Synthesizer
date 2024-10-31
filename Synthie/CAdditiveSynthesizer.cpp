#include "stdafx.h"
#include "CAdditiveSynthesizer.h"

CAdditiveSynthesizer::CAdditiveSynthesizer() {
	m_fundamental_frequency = 600;
	m_time = 0;
}

CAdditiveSynthesizer::CAdditiveSynthesizer(double fundamentalFrequency)
    : m_fundamental_frequency(fundamentalFrequency), m_duration(0){
    
}

CAdditiveSynthesizer::~CAdditiveSynthesizer() {
	m_fundamental_frequency = 0;
}

void CAdditiveSynthesizer::Start()
{
    m_sinewave.SetSampleRate(GetSampleRate());
    m_sinewave.Start();
    m_ar.SetSource(&m_sinewave);
    m_ar.SetSampleRate(GetSampleRate());
    m_ar.Start();
    m_time = 0;
    m_vibratoRate = 5;
    m_vibratoDepth = .05;
    // Calculate vibrato phase increment
    m_vibratoPhaseIncrement = 2 * M_PI * m_vibratoRate / GetSampleRate();
    m_vibratoPhase = 0;
}

bool CAdditiveSynthesizer::Generate()
{

    // Calculate the fundamental frequency and harmonics
    double harmonics[] = { 1,2,3,4,5,6,7,8,9}; // Harmonic multipliers
    

    // Calculate vibrato effect
    double vibrato = 1 + m_vibratoDepth * sin(m_vibratoPhase);
    m_vibratoPhase += m_vibratoPhaseIncrement;
    if (m_vibratoPhase > 2 * M_PI)
    {
        m_vibratoPhase -= 2 * M_PI;
    }

    // Generate the organ sound by adding harmonics
    double sampleL = 0;
    double sampleR = 0;
    int l = m_sinewaves.size();
    for (int i = 0; i < l; i++)
    {
        m_sinewaves[i].SetFreq(m_fundamental_frequency * harmonics[i] * vibrato);
        m_sinewaves[i].Generate();  // Generate the sine wave
        sampleL += nums[i] * m_sinewaves[i].Frame(0); // Add to the output
        sampleR += nums[i] * m_sinewaves[i].Frame(1);
    }

    // Apply envelope
    bool valid = m_ar.Generate();
    m_ar.SetDuration(m_duration);
    double envelope1 = m_ar.Frame(0);
    double envelope2 = m_ar.Frame(1);
    sampleL *= envelope1;
    sampleR *= envelope2;

    m_frame[0] = sampleL;
    m_frame[1] = sampleR;

    // Advance time
    m_time += GetSamplePeriod();

    // Check if the note duration is complete
    return valid;
}

void CAdditiveSynthesizer::SetNote(CNote* note)
{
    // Get a list of all attribute nodes and the
    // length of that list
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    note->Node()->get_attributes(&attributes);
    long len;
    attributes->get_length(&len);

    // Loop over the list of attributes
    for (int i = 0; i < len; i++)
    {
        // Get attribute i
        CComPtr<IXMLDOMNode> attrib;
        attributes->get_item(i, &attrib);

        // Get the name of the attribute
        CComBSTR name;
        attrib->get_nodeName(&name);

        // Get the value of the attribute.  A CComVariant is a variable
        // that can have any type. It loads the attribute value as a
        // string (UNICODE), but we can then change it to an integer 
        // (VT_I4) or double (VT_R8) using the ChangeType function 
        // and then read its integer or double value from a member variable.
        CComVariant value;
        attrib->get_nodeValue(&value);

        if (name == "duration")
        {
            value.ChangeType(VT_R8);
            // SetDuration(value.dblVal); // play the note for the duration in terms of seconds
            m_ar.SetDuration(value.dblVal * (NUM_SECS_IN_MINUTE / m_bpm));
            m_duration = value.dblVal * (NUM_SECS_IN_MINUTE / m_bpm);

        }
        else if (name == "harmonics")
        {
            _bstr_t bstrValue(value.bstrVal);
            std::string harmonicsStr(static_cast<const char*>(bstrValue));

            std::string a;
            double num;
            for (char i : harmonicsStr) {
                if (i == ',') {
                    num = std::stod(a);
                    nums.push_back(num);
                    a = "";
                }
                else {
                    a = a + i;
                }
            }
            num = std::stod(a);
            nums.push_back(num);
            CSineWave wave;
            int d = 0;
            for (double b : nums) {
                d = d + 1;
                wave.SetAmplitude(b*200);
                wave.SetFreq(m_fundamental_frequency * d);
                m_sinewaves.push_back(wave);
            }

        }
    }
    // Apply attack and release envelope
    m_ar.SetAttack(0.01);  // Attack time in seconds
    m_ar.SetRelease(0.01); // Release time in seconds
}