#include "stdafx.h"
#include "CNoise.h"
#include <cstdlib>

CNoise::CNoise()
{
}

CNoise::~CNoise()
{
}

void CNoise::Start()
{
}

bool CNoise::Generate()
{
    m_frame[0] = ((double)rand() / RAND_MAX) * 2.0 - 1.0; // Random value between -1 and 1
    m_frame[1] = m_frame[0]; // Mono output
    return true;
}
