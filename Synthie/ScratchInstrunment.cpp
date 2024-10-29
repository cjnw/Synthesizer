//
// Created by neelm on 10/28/2024.
//

#include "stdafx.h"
#include "ScratchInstrument.h"
#include "ScratchInstrunment.h"

ScratchInstrument::ScratchInstrument(Synthesizer& synth) : m_synth(synth) {}

void ScratchInstrument::BabyScratch(float speed) {
    SetPlaybackSpeed(speed);
    ApplyAmplitudeEnvelope(speed);
}

void ScratchInstrument::ScribbleScratch() {
    for (float speed = 2.0f; speed >= 0.5f; speed -= 0.1f) {
        SetPlaybackSpeed(speed);
        m_synth.ProcessAudioFrame();
    }
}

void ScratchInstrument::ChirpScratch() {
    BabyScratch();
    ApplyFader(true, true); // Quick attack and decay
}

void ScratchInstrument::TransformScratch() {
    while (recordIsMoving) {
        ToggleFader();
        m_synth.ProcessAudioFrame();
    }
}

void ScratchInstrument::OrbitScratch() {
    while (recordIsMoving) {
        SetPlaybackSpeed(1.0f); // Forward
        ApplyAmplitudeEnvelope(1.0f);
        SetPlaybackSpeed(-1.0f); // Backward
        ApplyAmplitudeEnvelope(1.0f);
    }
}

// Private methods for internal handling

void ScratchInstrument::SetPlaybackSpeed(float speed) {
    m_synth.SetPlaybackSpeed(speed);
}

void ScratchInstrument::ApplyAmplitudeEnvelope(float speed) {
    float amplitude = speed < 1.0f ? speed : 1.0f; // Example calculation
    m_synth.SetAmplitude(amplitude);
}

void ScratchInstrument::ApplyFader(bool attack, bool decay) {
    if (attack) m_synth.SetAttack(true);
    if (decay) m_synth.SetDecay(true);
}

void ScratchInstrument::ToggleFader() {
    m_synth.ToggleFader();
}

