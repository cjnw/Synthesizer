#pragma once

class Dynamics {
public:
    Dynamics();
    void SetDynamicRange(double minLevel, double maxLevel);
    double ApplyDynamics(double input);

private:
    double m_minLevel;
    double m_maxLevel;

    double clamp(double value, double minVal, double maxVal);
};