#include "stdafx.h"
#include "CDrums.h"

CDrums::CDrums()
{
    m_duration = 0.1;
}

CDrums::~CDrums()
{
}

void CDrums::Start()
{
    m_noise.SetSampleRate(GetSampleRate());
    m_noise.Start();

    m_reson.SetSampleRate(GetSampleRate());
    m_reson.SetSource(&m_noise);
    m_reson.Start();

    m_ar.SetSource(&m_reson);
    m_ar.SetSampleRate(GetSampleRate());
    m_ar.SetDuration(m_duration);
    m_ar.Start();

    m_time = 0;
}

bool CDrums::Generate()
{
    bool valid = m_ar.Generate();
    m_frame[0] = m_ar.Frame(0);
    m_frame[1] = m_ar.Frame(1);

    m_time += GetSamplePeriod();

    return valid;
}

void CDrums::SetNote(CNote* note)
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
        }
        else if (name == L"type")
        {
            value.ChangeType(VT_BSTR);
            m_drumType = value.bstrVal;
            SetDrumType(m_drumType);  // Set defaults based on drum type
        }
    }
}

void CDrums::SetDrumType(const std::wstring& type)
{
    if (type == L"snare")
    {
        m_reson.SetFrequency(180.0);
        m_reson.SetBandwidth(2000.0);
    }
    else if (type == L"bass")
    {
        m_reson.SetFrequency(100.0);
        m_reson.SetBandwidth(500.0);
    }
    else if (type == L"tom")
    {
        m_reson.SetFrequency(150.0);
        m_reson.SetBandwidth(1000.0);
    }
    else if (type == L"cymbal")
    {
        m_reson.SetFrequency(3000.0);
        m_reson.SetBandwidth(8000.0);
    }
    else
    {
        // Default drum parameters for unspecified types
        m_reson.SetFrequency(200.0);
        m_reson.SetBandwidth(1000.0);
    }
}

