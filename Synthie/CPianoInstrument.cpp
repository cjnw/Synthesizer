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

//#include "StdAfx.h"
//#include "CPianoInstrument.h"
//
//CPianoInstrument::CPianoInstrument(void)
//{
//    m_duration = 0.1;
//}
//
//CPianoInstrument::~CPianoInstrument(void)
//{
//}
//void CPianoInstrument::Start()
//{
//    m_pianowave.SetSampleRate(GetSampleRate());
//    m_pianowave.Start();
//    m_time = 0;
//
//    // Tell the AR object it gets its samples from 
//    // the pianowave object.
//    m_ar.SetSource(&m_pianowave);
//    m_ar.SetSampleRate(GetSampleRate());
//    m_ar.Start();
//
//}
//
//
//bool CPianoInstrument::Generate()
//{
//    // Tell the component to generate an audio sample
//    m_pianowave.Generate();
//    bool valid = m_ar.Generate();
//
//    // Read the component's sample and make it our resulting frame.
//    m_frame[0] = m_ar.Frame(0);
//    m_frame[1] = m_ar.Frame(1);
//
//    // Update time
//    m_time += GetSamplePeriod();
//    return valid;
//}
//void CPianoInstrument::SetNote(CNote* note)
//{
//    // Get a list of all attribute nodes and the
//    // length of that list
//    CComPtr<IXMLDOMNamedNodeMap> attributes;
//    note->Node()->get_attributes(&attributes);
//    long len;
//    attributes->get_length(&len);
//
//    // Loop over the list of attributes
//    for (int i = 0; i < len; i++)
//    {
//        // Get attribute i
//        CComPtr<IXMLDOMNode> attrib;
//        attributes->get_item(i, &attrib);
//
//        // Get the name of the attribute
//        CComBSTR name;
//        attrib->get_nodeName(&name);
//
//        // Get the value of the attribute.  A CComVariant is a variable
//        // that can have any type. It loads the attribute value as a
//        // string (UNICODE), but we can then change it to an integer 
//        // (VT_I4) or double (VT_R8) using the ChangeType function 
//        // and then read its integer or double value from a member variable.
//        CComVariant value;
//        attrib->get_nodeValue(&value);
//
//        if (name == "duration")
//        {
//            value.ChangeType(VT_R8);
//            SetDuration(value.dblVal);
//        }
//        else if (name == "note")
//        {
//            SetFreq(NoteToFrequency(value.bstrVal));
//        }
//        else if (name == "attack")
//        {
//            value.ChangeType(VT_R8);
//            m_ar.SetAttack(value.dblVal);
//        }
//        else if (name == "release")
//        {
//            value.ChangeType(VT_R8);
//            m_ar.SetRelease(value.dblVal);
//        }
//    }
//}
