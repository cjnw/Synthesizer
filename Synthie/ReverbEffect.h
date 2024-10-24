#pragma once
#include "Effect.h"
#include <vector>

class ReverbEffect : public Effect
{
public:
    ReverbEffect();
    virtual ~ReverbEffect();

    void Start() override;
    void Process(double* input, double* output) override;

    void SetRoomSize(double size);   // Sets the room size, affecting delay times
    void SetDamping(double damping); // Sets the damping factor
    void SetWet(double wetLevel);    // Sets the wet mix level
    void SetDry(double dryLevel);    // Sets the dry mix level

private:
    // Comb filter delay lines
    std::vector<double> m_combDelayBuffer[2][4]; // 2 channels, 4 comb filters
    size_t m_combIndices[2][4];

    // All-pass filter delay lines
    std::vector<double> m_allpassDelayBuffer[2][2]; // 2 channels, 2 all-pass filters
    size_t m_allpassIndices[2][2];

    // Parameters
    double m_roomSize; // Controls the delay lengths
    double m_damping;  // Controls the feedback damping
    double m_wetLevel;
    double m_dryLevel;

    // Internal variables
    double m_combFeedback[4];
    double m_allpassFeedback;

    void InitializeDelays();
};