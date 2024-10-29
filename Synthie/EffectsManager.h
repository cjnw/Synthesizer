#pragma once
#include <vector>


class EffectsManager
{
public:
	EffectsManager(int channels);

    // Default constructor
    EffectsManager() = default;

    // Copy constructor
    EffectsManager(const EffectsManager& other) : wetLevels(other.wetLevels), dryLevels(other.dryLevels) {}

    // Copy assignment operator
    EffectsManager& operator=(const EffectsManager& other) {
        if (this != &other) {
            dryLevels = other.dryLevels; // Copy the vector
            wetLevels = other.wetLevels; // Copy the vector

        }
        return *this;
    }

	void addEffectXML(IXMLDOMNode* xml);

    double WetLevel(int i) const { return wetLevels[i]; }
    double DryLevel(int i) const { return dryLevels[i]; }
    void SetWetLevel(int i, double value) { wetLevels[i] = value; }
    void SetDryLevel(int i, double value) { dryLevels[i] = value; }


private:
    std::vector<double> wetLevels;
    std::vector<double> dryLevels;

};

