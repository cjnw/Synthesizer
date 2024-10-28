#pragma once
#include "Instrument.h"
#include "SineWave.h"
#include "AR.h"
#include "CNoise.h"
#include "CReson.h"

class CDrums:
	public CInstrument
{
public:
    CDrums();
    virtual ~CDrums();

    virtual void Start();
    virtual bool Generate();

    void SetDuration(double d) { m_duration = d; }
    void SetDrumType(const std::wstring& type);  // Identify drum type and set defaults

    virtual void SetNote(CNote* note);

private:
    CNoise m_noise;
    CReson m_reson;
    CAR m_ar;
    double m_duration;
    double m_time;

    std::wstring m_drumType;
};

