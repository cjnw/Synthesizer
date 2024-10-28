#pragma once


#include "Sample.h"

class Pedal {
public:
    Pedal();
    void Press();
    void Release();
    bool IsPressed() const;
    void AddNoiseSample(const Sample& down, const Sample& up);
    Sample& GetNoise(bool isPressing);

    void GenerateNoiseFrame(double* frame, bool isPressing, int channels);

private:
    bool m_isPressed;
    Sample m_noiseDown;
    Sample m_noiseUp;
};

