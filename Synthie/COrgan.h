#pragma once
#include "Instrument.h"
#include <SineWave.h>
#include <AR.h>

class COrgan :
    public CInstrument
{
public:
	COrgan();
	COrgan(int bpm);
	virtual ~COrgan();

	const double NUM_SECS_IN_MINUTE = 60.0;
	virtual void Start();
	virtual bool Generate();

	void SetFreq(double f) { m_frequency = f; }
	void SetAmplitude(double a) { m_sinewave.SetAmplitude(a); }
	void SetDuration(double d) { m_duration = d; }
	virtual void SetNote(CNote* note);
private:
	std::vector<CSineWave> m_sinewaves;
	CSineWave m_sinewave;
	CAR m_ar;
	double m_frequency;
	double m_duration;
	std::wstring m_drawbars;
	double m_time;
	double m_vibratoDepth;
	double m_vibratoPhaseIncrement;
	double m_vibratoRate;
	double m_vibratoPhase;
};

