#pragma once
#include "Effect.h"

class CompressorEffect : public Effect
{
public:
    CompressorEffect();
    virtual ~CompressorEffect();

    void Start() override;
    void Process(double* input, double* output) override;

    void SetThreshold(double threshold);
    void SetRatio(double ratio);
    void SetAttackTime(double attackTime);
    void SetReleaseTime(double releaseTime);

private:
    double m_threshold;    // Threshold level for compression
    double m_ratio;        // Compression ratio
    double m_attackTime;   // Attack time in seconds
    double m_releaseTime;  // Release time in seconds
    double m_gain;         // Current gain reduction
    double m_attackCoef;
    double m_releaseCoef;
};