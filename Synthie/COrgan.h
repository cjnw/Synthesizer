#pragma once
#include "Instrument.h"
#include <SineWave.h>
#include <AR.h>

class COrgan :
    public CInstrument
{
private:
	COrgan();
	COrgan(int bpm);
	virtual ~COrgan();
public:
	const double NUM_SECS_IN_MINUTE = 60.0;
	virtual void Start();
	virtual bool Generate();

	void SetFreq(double f) { m_sinewave.SetFreq(f); }
	void SetAmplitude(double a) { m_sinewave.SetAmplitude(a); }
	void SetDuration(double d) { m_duration = d; }
	virtual void SetNote(CNote* note);
private:
	CSineWave   m_sinewave;
	CAR m_ar;
	double m_duration;
	double m_time;
};

