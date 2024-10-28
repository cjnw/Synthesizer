#include "stdafx.h"
#include "Dynamics.h"

Dynamics::Dynamics()
    : m_minLevel(0.0), m_maxLevel(1.0)
{
}

void Dynamics::SetDynamicRange(double minLevel, double maxLevel) {
    m_minLevel = minLevel;
    m_maxLevel = maxLevel;
}

double Dynamics::ApplyDynamics(double input) {
    return clamp(input, m_minLevel, m_maxLevel);
}

double Dynamics::clamp(double value, double minVal, double maxVal) {
    return (value < minVal) ? minVal : (value > maxVal) ? maxVal : value;
}