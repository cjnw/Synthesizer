#pragma once
#include "AudioNode.h"

class CNoise : public CAudioNode
{
public:
    CNoise();
    virtual ~CNoise();

    virtual void Start();
    virtual bool Generate();
};
