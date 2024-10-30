#pragma once
#include "Note.h"
#include "AR.h"
#include "SineWave.h"
#include "Instrument.h"

class CAdditiveSynthesizer : public CInstrument
{
public:
	CAdditiveSynthesizer();
	CAdditiveSynthesizer(double f);
	virtual ~CAdditiveSynthesizer();

	const double NUM_SECS_IN_MINUTE = 60.0;
	virtual void Start();
	virtual bool Generate();

	void SetFreq(double f) { m_frequency = f; }
	void SetAmplitude(double a) { m_sinewave.SetAmplitude(a); }
	void SetDuration(double d) { m_duration = d; }
	virtual void SetNote(CNote* note);
private:
	std::vector<CSineWave> m_sinewaves;
	std::vector<double> nums;
	CSineWave m_sinewave;
	CAR m_ar;
	double m_frequency;
	double m_duration;
	double m_time;
	double m_vibratoDepth;
	double m_vibratoPhaseIncrement;
	double m_vibratoRate;
	double m_vibratoPhase;
	double m_fundamental_frequency;
};

