#include "stdafx.h"
#include "COrgan.h"

COrgan::COrgan()
{
	m_duration = 0.1;
}

COrgan::COrgan(int bpm)
{
	m_duration = 0.1;
	m_bpm = bpm;
}

COrgan::~COrgan()
{
}

void COrgan::Start()
{
	m_ar.SetSource(&m_sinewave);
	m_ar.SetSampleRate(GetSampleRate());
	m_ar.Start();
}

bool COrgan::Generate()
{
	return false;
}

void COrgan::SetNote(CNote* note)
{
}
