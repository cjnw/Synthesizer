#include "stdafx.h"
#include "Sample.h"
#include <fstream>
#include <stdexcept>
#include <cmath>
#include <iostream>

Sample::Sample() : m_sampleRate(44100), m_channels(2), m_currentPosition(0), m_playing(false) {}

bool Sample::Load(const std::wstring& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) return false;

    char header[44];
    file.read(header, 44);
    if (!file) return false;

    if (header[22] != 2 || header[34] != 16) {
        return false;
    }

    m_data.clear();
    short sample;
    while (file.read(reinterpret_cast<char*>(&sample), sizeof(sample))) {
        m_data.push_back(sample);
    }

    m_playing = false;
    m_currentPosition = 0;
    return true;
}

void Sample::Start() {
    m_playing = true;
    m_currentPosition = 0;
}

void Sample::Stop() {
    m_playing = false;
}

bool Sample::IsPlaying() const {
    return m_playing;
}

double Sample::Generate() {
    if (!m_playing || m_currentPosition >= m_data.size()) {
        m_playing = false;
        return 0.0;
    }

    double sampleValue = m_data[m_currentPosition] / 32768.0;
    m_currentPosition += m_channels;
    return sampleValue;
}
