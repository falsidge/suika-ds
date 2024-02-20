
#ifndef BALL_HPP
#include <nds.h>
#include <filesystem.h>

#include <nf_lib.h>
#include "IdStorage.hpp"

#include <Box2D.h>
struct BallData{
    std::vector<std::string> spriteFiles;
    float radius;
    u8 pixelRadius;
    u8 spriteSize;
};

class Ball
{
public:
    s16 x;
    s16 y;
    s8 vx;
    s8 vy;
    s8 ax;
    s8 ay;

    u8 radius;


    s8 topSpriteId;
    s8 bottomSpriteId;

    bool dropped;
    IdStorage idStorage;

    Ball(IdStorage storage)
    {
        x = 20;
        y = 70;
        vx = 1;
        vy = 2;
        ax = 0;
        ay = 0;
        topSpriteId = -1;
        bottomSpriteId = -1;
        radius = 16;

        dropped = false;
        idStorage = storage;
    }

    void update()
    {
        if (dropped)
        {
            if (y <= 192 * 2 - radius*2 - 10)
            {

                y += vy;
            }
            // if (y > 160)
            // {
            //     y = 70;
            //     dropped = false;
            // }
        }
    }

    void moveLeft()
    {
        if (dropped)
        {
            return;
        }
        x -= 2;
        if (x < 20)
        {
            x = 20;
        }
    }

    void moveRight()
    {
        if (dropped)
        {
            return;
        }
        x += 2;
        if (x > 200)
        {
            x = 200;
        }
    }

    void drop()
    {
        dropped = true;
    }
    void draw()
    {
        if (y > 192 - radius*2)
        {
            if (bottomSpriteId < 0)
            {
                bottomSpriteId = idStorage.getId();
                NF_CreateSprite(1, bottomSpriteId, 0, 0, x, y - 192);
            }
            else
            {
                NF_MoveSprite(1, bottomSpriteId, x, y - 192);
            }
        }
        if (y < 192)
        {
            if (topSpriteId < 0)
            {
                topSpriteId = idStorage.getId();
                NF_CreateSprite(0, topSpriteId, 0, 0, x, y);
            }
            else
            {
                NF_MoveSprite(0, topSpriteId, x, y);
            }
        }
    }
};
#define BALL_HPP
#endif