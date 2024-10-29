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
    m_ar.SetAttack(0.0);              // Immediate attack
    m_ar.SetRelease(m_duration);      // Release over the duration
    m_ar.SetDuration(m_duration);     // Total duration equals release time
    m_ar.Start();

    m_time = 0;
}

bool CDrums::Generate()
{
    bool valid = m_ar.Generate();

    // Add a burst of noise only for the initial attack (e.g., first 5ms)
    double noiseBurst = (m_time < 0.005) ? m_noise.Frame(0) * 0.7 : 0.0; // Slightly higher for punch
    double volumeScale = 0.6; // Overall volume control to avoid clipping

    m_frame[0] = (m_ar.Frame(0) + noiseBurst) * volumeScale;
    m_frame[1] = (m_ar.Frame(1) + noiseBurst) * volumeScale;

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
    m_reson.SetDuration(m_duration);

    if (type == L"snare")
    {
        m_reson.SetStartFrequency(400.0);
        m_reson.SetEndFrequency(200.0);
        m_reson.SetBandwidth(2000.0);
        m_duration = 0.3;  // Set specific duration for snare

        // Snare-specific envelope
        m_ar.SetAttack(0.005);
        m_ar.SetRelease(0.195);
    }
    else if (type == L"bass")
    {
        // Strong, punchy frequency sweep for the bass drum
        m_reson.SetStartFrequency(120.0);   // Start frequency for a punchy bass
        m_reson.SetEndFrequency(40.0);      // End at very low frequency for depth
        m_reson.SetBandwidth(100.0);        // Narrow bandwidth for a focused low-end sound
        m_duration = 0.3;                   // Adjust duration for more decay

        // Envelope with a fast attack and moderate decay for bass drum resonance
        m_ar.SetAttack(0.001);              // Very fast attack for punch
        m_ar.SetRelease(0.28);              // Slightly longer release for resonance
    }

    else if (type == L"high_tom")
    {
        // High Tom settings for a brighter, punchier sound
        m_reson.SetStartFrequency(300.0);    // Higher start frequency for a clear, punchy tone
        m_reson.SetEndFrequency(180.0);      // End frequency for a noticeable drop
        m_reson.SetBandwidth(900.0);         // Wider bandwidth for an open resonance
        m_duration = 0.18;                   // Shorter duration for a quick decay

        // Envelope settings for high tom
        m_ar.SetAttack(0.002);               // Quick attack for punch
        m_ar.SetRelease(0.16);               // Quick decay to match duration
    }
    else if (type == L"medium_tom")
    {
        // Lower Medium Tom settings for a deeper, resonant sound
        m_reson.SetStartFrequency(150.0);    // Lower start frequency for depth
        m_reson.SetEndFrequency(80.0);       // Lower end frequency for a warm decay
        m_reson.SetBandwidth(600.0);         // Narrower bandwidth for a focused sound
        m_duration = 0.3;                    // Slightly longer duration to add resonance

        // Envelope settings for medium tom
        m_ar.SetAttack(0.002);               // Quick attack for tom punch
        m_ar.SetRelease(0.27);               // Slightly longer release to match duration
    }

    else if (type == L"cymbal")
    {
        // Cymbal settings
        m_reson.SetStartFrequency(8000.0);
        m_reson.SetEndFrequency(5000.0);
        m_reson.SetBandwidth(12000.0);
        m_duration = 1.0;  // Set specific duration for cymbal

        // Cymbal-specific envelope
        m_ar.SetAttack(0.0);
        m_ar.SetRelease(1.0);  // Longer release for cymbals
    }

}


