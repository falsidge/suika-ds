#pragma once

#include <nds.h>
class IdStorage
{
    u8 N = 127;
    bool ids[127] = {false};

public:
    s16 getId();

    void releaseId(u8 i);
};
