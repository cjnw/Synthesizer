#pragma once
#include <vector>


class EffectsManager
{
public:
	EffectsManager(int channels);

    // Default constructor
    EffectsManager() = default;

    // Copy constructor
    EffectsManager(const EffectsManager& other) : sends(other.sends) {}

    // Copy assignment operator
    EffectsManager& operator=(const EffectsManager& other) {
        if (this != &other) {
            sends = other.sends; // Copy the sends vector
        }
        return *this;
    }

	void addEffectXML(IXMLDOMNode* xml);

	double Send(int i) { return sends[i]; }
	void SetSend(int i, double value) { sends[i] = value; }



private:
	std::vector<double> sends;

};

