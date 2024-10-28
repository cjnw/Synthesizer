#include "stdafx.h"
#include "Pedal.h"

Pedal::Pedal() : m_isPressed(false) {}

void Pedal::Press() {
    m_isPressed = true;
}

void Pedal::Release() {
    m_isPressed = false;
}

bool Pedal::IsPressed() const {
    return m_isPressed;
}

void Pedal::AddNoiseSample(const Sample& down, const Sample& up) {
    m_noiseDown = down;
    m_noiseUp = up;
}

Sample& Pedal::GetNoise(bool isPressing)  {
    return isPressing ? m_noiseDown : m_noiseUp;
}