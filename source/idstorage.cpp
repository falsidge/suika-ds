#include "idstorage.hpp"

s16 IdStorage::getId()
{

    for (u8 i = 0; i < N; ++i)
    {
        if (!ids[i])
        {
            ids[i] = true;
            return i;
        }
    }
    return -1;
}

void IdStorage::releaseId(u8 i)
{
    ids[i] = false;
}