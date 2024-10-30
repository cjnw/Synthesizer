#pragma once
#include "SineWave.h"
#include <vector>
#include <memory>

class MultiSoundHandler {
public:
    void AddSineWave(const CSineWave& wave) {
        sineWaves.push_back(wave);
    }

    double GenerateMixedSample(double time) {
        double mixedSample = 0.0;
        for (auto& wave : sineWaves) {
            mixedSample += wave.GenerateSample(time); // Assuming GenerateSample(time) returns a sample
        }
        return mixedSample / sineWaves.size(); // Normalize by the number of waves
    }

private:
    std::vector<CSineWave> sineWaves;
};
