
#ifndef BALL_HPP
#include <nds.h>
#include <filesystem.h>

#include <nf_lib.h>
#include "IdStorage.hpp"

#include <Box2D.h>
struct BallData
{
    u8 id;
    std::vector<std::string> spriteFiles;
    float radius;
    u8 pixelRadius;
    u8 spriteSize;
    std::vector<u8> spriteIds;
};

class Ball
{
public:
    IdStorage *topidStorage;
    IdStorage *bottomidStorage;
    u8 spriteId;
    u8 radius;
    u8 id;
    BallData currentballdata;

    s16 x;
    s16 y;

    s16 topSpriteId;
    s16 bottomSpriteId;
    bool dropped;
    bool destroyed;
    // Ball *currentBall = new Ball(idstorage, currentballdata.spriteIds[0], currentballdata.pixelRadius, ballBodies.size());

    Ball(IdStorage *topstorage, IdStorage *bottomstorage, BallData balldata) : topidStorage(topstorage), bottomidStorage(bottomstorage), spriteId(balldata.spriteIds[0]), radius(balldata.pixelRadius), id(balldata.id), currentballdata(balldata)
    {
        x = 20;
        y = 70;

        topSpriteId = -1;
        bottomSpriteId = -1;

        dropped = false;
        destroyed = false;
    }

    void update()
    {
        if (dropped)
        {
            if (y <= 192 * 2 - radius * 2 - 10)
            {

                // y += vy;
            }
            // if (y > 160)
            // {
            //     y = 70;
            //     dropped = false;
            // }
        }
    }

    void moveLeft(float delta)
    {
        if (dropped)
        {
            return;
        }
        x -= 200 * delta;
        if (x < 20)
        {
            x = 20;
        }
    }

    void moveRight(float delta)
    {
        if (dropped)
        {
            return;
        }
        x += 200 * delta;
        if (x > 200)
        {
            x = 200;
        }
    }

    void checkBounds()
    {
        if (x > 200)
        {
            x = 200;
        }
        if (x < 20)
        {
            x = 20;
        }
    }

    void drop()
    {
        dropped = true;
    }
    void resize(u16 spriteId, u8 screen)
    {
        if (radius > 32)
        {
            NF_EnableSpriteRotScale(screen, spriteId, id, true);
            // NF_EnableSpriteRotScale(0, spriteId, id, true);
        }
    }
    s32 getScaleError()
    {
        if (radius <= 32)
        {
            return 0;
        }
        return -(64 - radius);
    }
    void draw()
    {
        if (destroyed)
        {
            return;
        }

        if (y > 192 - radius)
        {
            if (bottomSpriteId < 0)
            {
                s16 newId = bottomidStorage->getId();
                if (newId < 0 || newId > 127)
                {
                    return;
                }
                bottomSpriteId = newId;

                NF_CreateSprite(1, bottomSpriteId, spriteId, spriteId, x - radius + getScaleError(), y - 192 - radius + getScaleError());

                // NF_CreateSprite(1, bottomSpriteId, spriteId, spriteId, x - radius, y - 192 - radius);
                resize(bottomSpriteId, 1);
            }
            else
            {
                NF_MoveSprite(1, bottomSpriteId, x - radius + getScaleError(), y - 192 - radius + getScaleError());
            }
        }
        else
        {
            if (bottomSpriteId >= 0)
            {
                NF_DeleteSprite(1, bottomSpriteId);
                bottomidStorage->releaseId(bottomSpriteId);
                bottomSpriteId = -1;
            }
        }
        if (y <= 192 + radius )
        {
            if (topSpriteId < 0)
            {
                // topSpriteId = topidStorage.getId();
                s16 newId = topidStorage->getId();
                if (newId < 0 || newId > 127)
                {
                    return;
                }
                topSpriteId = newId;

                NF_CreateSprite(0, topSpriteId, spriteId, spriteId, x - radius + getScaleError(), y - radius + getScaleError());

                resize(topSpriteId, 0);
            }
            else
            {

                NF_MoveSprite(0, topSpriteId, x - radius + getScaleError(), y - radius + getScaleError());
            }
        }
        else
        {
            if (topSpriteId >= 0)
            {
                NF_DeleteSprite(0, topSpriteId);
                topidStorage->releaseId(topSpriteId);
                topSpriteId = -1;
            }
        }
    }
    void destroy()
    {

        if (destroyed)
        {
            return;
        }
        destroyed = true;
        if (topSpriteId >= 0)
        {
            NF_DeleteSprite(0, topSpriteId);
            topidStorage->releaseId(topSpriteId);
            topSpriteId = -1;
        }
        if (bottomSpriteId >= 0)
        {
            NF_DeleteSprite(1, bottomSpriteId);
            bottomidStorage->releaseId(bottomSpriteId);
            bottomSpriteId = -1;
        }
    }

    ~Ball()
    {
        this->destroy();
    }
};
#define BALL_HPP
#endif