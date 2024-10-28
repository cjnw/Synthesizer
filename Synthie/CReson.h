#pragma once
#include "AudioNode.h"
class CReson :
    public CAudioNode
{
public:
    CReson();
    virtual ~CReson();

    virtual void Start();
    virtual bool Generate();

    void SetSource(CAudioNode* source) { m_source = source; }
    void SetStartFrequency(double f) { m_startFrequency = f; }
    void SetEndFrequency(double f) { m_endFrequency = f; }
    void SetBandwidth(double bw) { m_bandwidth = bw; }
    void SetDuration(double duration) { m_duration = duration; }

private:
    void UpdateCoefficients();

    CAudioNode* m_source;
    double m_startFrequency;
    double m_endFrequency;
    double m_bandwidth;
    double m_duration;
    double m_time;

    double m_y1[2];
    double m_y2[2];

    double m_a0;
    double m_a1;
    double m_a2;
};

