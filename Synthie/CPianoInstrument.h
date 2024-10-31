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

//#pragma once
//#include "instrument.h"
//#include "PianoWave.h"
//#include "Notes.h"
//#include "Envelope.h"
//
//class CPianoInstrument :
//	public CInstrument
//{
//public:
//	CPianoInstrument(void);
//	~CPianoInstrument(void);
//	std::wstring Name() const { return L"Piano"; }
//
//public:
//	virtual void Start();
//	virtual bool Generate();
//
//	void SetFreq(double f) { m_pianowave.SetFreq(f); }
//	void SetAmplitude(double a) { m_pianowave.SetAmplitude(a); }
//	void SetDuration(double d) { m_ar.SetDuration(d); }
//	void SetNote(CNote* note);
//	void Sustain(bool s) { m_ar.Sustain(s); }
//	CPianoWave* GetPlayer() { return &m_pianowave; }
//	void GetFrame(double frame[2]) { frame[0] = m_frame[0]; frame[1] = m_frame[1]; }
//
//
//private:
//	CPianoWave m_pianowave;
//	double m_duration;
//	double m_time;
//	Envelope m_ar;
//};