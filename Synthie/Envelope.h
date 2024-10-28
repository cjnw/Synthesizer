#pragma once


class Envelope {
public:
    Envelope(double attack = 0.05, double release = 0.05);
    void Start(double duration);
    double Apply(double time);
    bool IsActive(double time) const;

private:
    double m_attack;
    double m_release;
    double m_duration;
};

