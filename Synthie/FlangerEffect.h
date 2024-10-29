#pragma once
#include "Effect.h"
#include <vector>

class FlangerEffect : public Effect
{
public:
    FlangerEffect();
    virtual ~FlangerEffect();

    void Start() override;
    void Process(double* input, double* output) override;

    void SetDepth(double depth);
    void SetRate(double rate);
    void SetFeedback(double feedback);
    void SetWet(double wetLevel);
    void SetDry(double dryLevel);

private:
    std::vector<double> m_delayBuffer[2];
    size_t m_bufferSize;
    double m_depth;        // Max delay modulation in samples
    double m_rate;         // LFO rate in Hz
    double m_feedback;     // Feedback amount
    double m_phase;
    double m_lastOutput[2];
};