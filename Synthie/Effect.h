#pragma once
class Effect
{
public:
	Effect() : wetMix(0.5), dryMix(0.5) {}
	virtual ~Effect() {}

	virtual void Process(double* input, double* output) = 0;

	void SetWetMix(double mix) { wetMix = mix; }
	void SetDryMix(double mix) { dryMix = mix; }

protected:
	double wetMix;
	double dryMix;



};

