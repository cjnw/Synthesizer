#pragma once
#include "Effect.h"

class NoiseGateEffect : public Effect
{
public:
    NoiseGateEffect();
    virtual ~NoiseGateEffect();

    void Start() override;
    void Process(double* input, double* output) override;

    void SetThreshold(double threshold);
    void SetAttackTime(double attackTime);
    void SetReleaseTime(double releaseTime);

private:
    double m_threshold;    // Threshold level for gating
    double m_attackTime;   // Time to open the gate
    double m_releaseTime;  // Time to close the gate
    double m_gain;         // Current gain
    double m_attackCoef;
    double m_releaseCoef;
};

