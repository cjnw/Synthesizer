#pragma once
#include "AudioNode.h"
#include <cmath>

#ifndef M_PI
constexpr double M_PI = 3.14159265358979323846;
#endif

class CSineWave :
	public CAudioNode
{
public:
	CSineWave();
	virtual ~CSineWave();
public:
	//! Start audio generation
	virtual void Start();

	//! Generate one frame of audio
	virtual bool Generate();

	//! Set the sine wave frequency
	void SetFreq(double f) { m_freq = f; }
	double GetFreq() const { return m_freq; }

	//! Set the sine wave amplitude
	void SetAmplitude(double a) { m_amp = a; }
	double GetAmplitude() const { return m_amp; }

	// Function to generate a sample at a specific time
	double GenerateSample(double time) const {
		// Calculate the sample value based on amplitude and frequency
		return m_amp * sin(2 * M_PI * m_freq * time);
	}

private:
	double m_freq;
	double m_amp;
	double m_phase;
};

