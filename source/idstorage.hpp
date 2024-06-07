#pragma once

#include <nds.h>
class IdStorage
{
    int N = 127;
    bool ids[127] = {false};

public:
    s16 getId();

    void releaseId(int i);
};
