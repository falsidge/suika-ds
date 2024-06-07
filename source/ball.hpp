
#pragma once
#include "sprite.hpp"

struct BallData
{
    u8 id;
    std::vector<SpriteData> spriteFiles;
    float physicsRadius;
    s32 pixelRadius;
};

class Ball
{
    IdStorage *topidStorage;
    IdStorage *bottomidStorage;
    
public:
    s32 pixelRadius;
    float physicsRadius;
    
    u8 id;

    s32 x;
    s32 y;

    bool dropped;

    std::vector<Sprite> ballSprites;

    Ball(IdStorage *topidStorage, IdStorage *bottomidStorage, BallData balldata);

    void moveLeft(float delta);
    void moveRight(float delta);

    void checkBounds();
    void drop();
    void draw();
};