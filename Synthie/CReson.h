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
    void SetFrequency(double f) { m_frequency = f; }
    void SetBandwidth(double bw) { m_bandwidth = bw; }

private:
    CAudioNode* m_source;
    double m_frequency;
    double m_bandwidth;

    double m_y1[2];
    double m_y2[2];

    double m_a0;
    double m_a1;
    double m_a2;
};

