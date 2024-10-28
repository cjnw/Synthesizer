#include "stdafx.h"
#include "CReson.h"
#include <cmath>

#define PI 3.14159265358979323846

CReson::CReson()
{
    m_source = NULL;
    m_startFrequency = 1000.0;
    m_endFrequency = 500.0;
    m_bandwidth = 100.0;
    m_duration = 1.0;
    m_time = 0.0;

    m_y1[0] = m_y1[1] = 0.0;
    m_y2[0] = m_y2[1] = 0.0;
}

CReson::~CReson()
{
}

void CReson::Start()
{
    m_source->SetSampleRate(GetSampleRate());
    m_source->Start();

    m_y1[0] = m_y1[1] = 0.0;
    m_y2[0] = m_y2[1] = 0.0;

    m_time = 0.0;
}

void CReson::UpdateCoefficients()
{
    // Calculate the current frequency
    double t = m_time / m_duration;
    if (t > 1.0) t = 1.0; // Clamp to 1.0
    double frequency = m_startFrequency + t * (m_endFrequency - m_startFrequency);

    // Compute filter coefficients
    double d = exp(-PI * m_bandwidth / GetSampleRate());
    double c = 2.0 * d * cos(2.0 * PI * frequency / GetSampleRate());
    m_a0 = 1.0 - d;
    m_a1 = -c;
    m_a2 = d;
}

bool CReson::Generate()
{
    m_source->Generate();

    UpdateCoefficients();

    for (int c = 0; c < 2; c++)
    {
        double x = m_source->Frame(c);

        double y = m_a0 * x - m_a1 * m_y1[c] - m_a2 * m_y2[c];

        m_y2[c] = m_y1[c];
        m_y1[c] = y;

        m_frame[c] = y;
    }

    m_time += GetSamplePeriod();

    return true;
}
