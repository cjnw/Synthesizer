#include "stdafx.h"
#include "COrgan.h"
#include "Notes.h"
#include <corecrt_math_defines.h>

COrgan::COrgan()
{
    m_duration = 0;
    m_time = 0;
    m_frequency = 0;
    m_vibratoDepth = 0.1; // Default vibrato depth
    m_vibratoRate = 5;   // Default vibrato rate in Hz

    // Initialize sine wave generators for each harmonic
    for (int i = 0; i < 9; i++) {
        m_sinewaves.push_back(CSineWave());
    }
}

COrgan::COrgan(int bpm)
{
	m_duration = 0.1;
	m_bpm = bpm;

    // Initialize sine wave generators for each harmonic
    for (int i = 0; i < 9; i++) {
        m_sinewaves.push_back(CSineWave());
    }
}

COrgan::~COrgan()
{
}

void COrgan::Start()
{
    m_sinewave.SetSampleRate(GetSampleRate());
    m_sinewave.Start();
    m_ar.SetSource(&m_sinewave);
    m_ar.SetSampleRate(GetSampleRate());
    m_ar.Start();
    m_time = 0;

    // Calculate vibrato phase increment
    m_vibratoPhaseIncrement = 2 * M_PI * m_vibratoRate / GetSampleRate();
    m_vibratoPhase = 0;
}

bool COrgan::Generate()
{

    // Calculate the fundamental frequency and harmonics
    double harmonics[] = { 1, 3, 2, 4, 6, 8, 10, 12, 16 }; // Harmonic multipliers
    double amplitudes[9] = { 0 };

    // Get drawbar settings from the note
    if (m_drawbars.length() == 9)
    {
        for (int i = 0; i < 9; i++)
        {
            int drawbarValue = m_drawbars[i] - '0'; // Convert character to integer
            if (drawbarValue > 0)
            {
                double dbReduction = -3 * (8 - drawbarValue);
                amplitudes[i] = pow(10, dbReduction / 20.0); // Calculate amplitude based on drawbar value
            }
        }
    }

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
    for (int i = 0; i < 9; i++)
    {
        m_sinewaves[i].SetFreq(m_frequency * harmonics[i] * vibrato);
        m_sinewaves[i].Generate();  // Generate the sine wave
        sampleL += amplitudes[i] * m_sinewaves[i].Frame(0); // Add to the output
        sampleR += amplitudes[i] * m_sinewaves[i].Frame(1);
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

void COrgan::SetNote(CNote* note)
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
        else if (name == "note")
        {
            SetFreq(NoteToFrequency(value.bstrVal));
        }
        else if (name == "drawbars")
        {
            m_drawbars = value.bstrVal;
        }
    }
    // Apply attack and release envelope
    m_ar.SetAttack(0.01);  // Attack time in seconds
    m_ar.SetRelease(0.01); // Release time in seconds
}
