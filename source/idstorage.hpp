#ifndef IDSTORAGE_HPP

class IdStorage
{
    u8 N = 127;
    bool ids[127] = {false};

public:
    s16 getId()
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

    void releaseId(u8 i)
    {
        ids[i] = false;
    }
};


#define IDSTORAGE_HPP
#endif