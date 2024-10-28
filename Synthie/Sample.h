#pragma once
#include <string>
#include <vector>

class Sample {
public:
    Sample();
    bool Load(const std::wstring& filename);
    void Start();
    void Stop();
    bool IsPlaying() const;
    double Generate();

private:
    std::vector<short> m_data;
    int m_sampleRate;
    int m_channels;
    size_t m_currentPosition;
    bool m_playing;
};