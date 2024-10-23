#include "Effect.h"
#include <vector>

class EchoEffect : public Effect
{
public:
    EchoEffect();
    virtual ~EchoEffect();

    void Start() override;
    void Process(double* input, double* output) override;

    void SetDelay(double delayTime);
    void SetWet(double wetLevel);
    void SetDry(double dryLevel);

private:
    std::vector<double> m_delayBuffer[2]; // For stereo channels
    size_t m_writeIndex;
    size_t m_delaySamples;
    double m_wetLevel;
    double m_dryLevel;
};

