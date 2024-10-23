#pragma once

class Effect
{
public:
    Effect() : m_sampleRate(44100.0) {}
    virtual ~Effect() {}

    virtual void Start() {}
    virtual void Process(double* input, double* output) = 0;

    void SetSampleRate(double sampleRate) { m_sampleRate = sampleRate; }

protected:
    double m_sampleRate;
};