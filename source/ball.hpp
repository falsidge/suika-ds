
#pragma once
#include "sprite.hpp"

struct BallData
{
    u8 id;
    std::vector<SpriteData> spriteFiles;
    float physicsRadius;
    u8 pixelRadius;
};

class Ball
{
    IdStorage *topidStorage;
    IdStorage *bottomidStorage;
    
public:
    u8 pixelRadius;
    float physicsRadius;
    
    u8 id;

    s16 x;
    s16 y;

    bool dropped;

    std::vector<Sprite> ballSprites;

    Ball(IdStorage *topidStorage, IdStorage *bottomidStorage, BallData& balldata);

    void moveLeft(float delta);
    void moveRight(float delta);

    void checkBounds();
    void drop();
    void draw();
};