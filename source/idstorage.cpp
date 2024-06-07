#include "idstorage.hpp"

s16 IdStorage::getId()
{

    for (int i = 0; i < N; ++i)
    {
        if (!ids[i])
        {
            ids[i] = true;
            return i;
        }
    }
    return -1;
}

void IdStorage::releaseId(int i)
{
    ids[i] = false;
}