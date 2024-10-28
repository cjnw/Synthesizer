#include "stdafx.h"
#include "Envelope.h"
#include <algorithm>

Envelope::Envelope(double attack, double release)
    : m_attack(attack), m_release(release), m_duration(0) {}

void Envelope::Start(double duration) {
    m_duration = duration;
}

double Envelope::Apply(double time) {
    if (time < m_attack)
        return time / m_attack;
    else if (time > m_duration - m_release)
        return (std::max)(0.0, (m_duration - time) / m_release);
    return 1.0;
}

bool Envelope::IsActive(double time) const {
    return time < m_duration;
}