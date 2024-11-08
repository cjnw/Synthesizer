#include "stdafx.h"
#include "Sample.h"
#include <fstream>
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <windows.h> // Include for MessageBox

Sample::Sample() : m_sampleRate(44100), m_channels(2), m_currentPosition(0), m_playing(false) {}

bool Sample::Load(const std::wstring& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        MessageBox(NULL, L"Failed to open file.", L"Error", MB_OK | MB_ICONERROR);
        return false;
    }

    char header[44];
    file.read(header, 44);
    if (!file) {
        MessageBox(NULL, L"Failed to read header from file.", L"Error", MB_OK | MB_ICONERROR);
        return false;
    }

    // Check for stereo (2 channels) and 16-bit audio
    if (header[22] != 2 || header[34] != 16) {
        MessageBox(NULL, L"Unsupported format for file.", L"Error", MB_OK | MB_ICONERROR);
        return false;
    }

    m_data.clear();
    short sample;
    while (file.read(reinterpret_cast<char*>(&sample), sizeof(sample))) {
        m_data.push_back(sample);
    }

    // Check if any samples were loaded
    if (m_data.empty()) {
        MessageBox(NULL, L"No samples were loaded from the file.", L"Warning", MB_OK | MB_ICONWARNING);
    }
    else {
        std::wstring message = L"Loaded " + std::to_wstring(m_data.size()) + L" samples from " + filename;
        MessageBox(NULL, message.c_str(), L"Info", MB_OK); // Show message box with loaded samples info
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

    double sampleValue = m_data[m_currentPosition] / 32768.0; // Normalize to [-1.0, 1.0]
    m_currentPosition += m_channels; // Increment position by number of channels
    return sampleValue;
}