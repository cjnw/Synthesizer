#pragma once
#include "Instrument.h"
#include "SineWave.h"
#include "AR.h"

class CPianoInstrument : public CInstrument
{
public:
    CPianoInstrument();
    CPianoInstrument(double bpm);
    virtual ~CPianoInstrument();

    const double NUM_SECS_IN_MINUTE = 60.0;
    virtual void Start();
    virtual bool Generate();

    void SetFreq(double f) { m_sinewave.SetFreq(f); }
    void SetAmplitude(double a) { m_sinewave.SetAmplitude(a); }
    void SetDuration(double d) { m_duration = d; }
    virtual void SetNote(CNote* note);

private:
    CSineWave m_sinewave;
    CAR m_ar;
    double m_duration;
    double m_time;
    double m_bpm;
};