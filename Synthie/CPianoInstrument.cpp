#include "stdafx.h"
#include "CPianoInstrument.h"
#include "Notes.h"

CPianoInstrument::CPianoInstrument()
{
    m_duration = 0.1;
}

CPianoInstrument::CPianoInstrument(double bpm)
{
    m_duration = 0.1;
    m_bpm = bpm;
}

CPianoInstrument::~CPianoInstrument()
{
}

void CPianoInstrument::Start()
{
    m_sinewave.SetSampleRate(GetSampleRate());
    m_sinewave.Start();
    m_ar.SetSource(&m_sinewave);
    m_ar.SetSampleRate(GetSampleRate());
    m_ar.SetAttack(0.01); // Attack time in seconds
    m_ar.SetRelease(m_duration); // Release over the duration
    m_ar.SetDuration(m_duration); // Total duration equals release time
    m_ar.Start();
    m_time = 0;
}

bool CPianoInstrument::Generate()
{
    bool valid = m_ar.Generate();

    // Generate the piano sound
    m_frame[0] = m_ar.Frame(0);
    m_frame[1] = m_ar.Frame(1);

    // Advance time
    m_time += GetSamplePeriod();

    // Check if the note duration is complete
    return valid;
}

void CPianoInstrument::SetNote(CNote* note)
{
    // Retrieve attributes from the note
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    note->Node()->get_attributes(&attributes);
    long len;
    attributes->get_length(&len);

    for (int i = 0; i < len; i++)
    {
        CComPtr<IXMLDOMNode> attrib;
        attributes->get_item(i, &attrib);

        CComBSTR name;
        attrib->get_nodeName(&name);

        CComVariant value;
        attrib->get_nodeValue(&value);

        if (name == L"duration")
        {
            value.ChangeType(VT_R8);
            m_duration = value.dblVal;
            m_ar.SetDuration(m_duration);
        }
        else if (name == L"note")
        {
            SetFreq(NoteToFrequency(value.bstrVal));
        }
    }
}