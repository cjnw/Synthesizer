#pragma once
#include "Effect.h"
#include <vector>

class ChorusEffect : public Effect
{
public:
    ChorusEffect();
    virtual ~ChorusEffect();

    void Start() override;
    void Process(double* input, double* output) override;

    void SetDepth(double depth);
    void SetRate(double rate);


private:
    std::vector<double> m_delayBuffer[2];
    size_t m_bufferSize;
    double m_depth;        // Depth of modulation in samples
    double m_rate;         // Modulation rate in Hz
    double m_phase;
    int m_writeIndex;
};