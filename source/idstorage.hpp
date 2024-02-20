#ifndef IDSTORAGE_HPP

class IdStorage
{
    int N = 127;
    bool ids[127];

public:
    int getId()
    {

        for (int i = 0; i < N; ++i)
        {
            if (!ids[i])
            {
                ids[i] = true;
                return i;
            }
        }
    }

    void releaseId(int i)
    {
        ids[i] = false;
    }
};


#define IDSTORAGE_HPP
#endif