#include "ball.hpp"
#include "IdStorage.hpp"

Ball::Ball(IdStorage *topidStorage, IdStorage *bottomidStorage, BallData &balldata) : topidStorage(topidStorage), bottomidStorage(bottomidStorage), pixelRadius(balldata.pixelRadius), physicsRadius(balldata.physicsRadius), id(balldata.id)
{
    x = 60;
    y = 70;

    dropped = false;

    for (auto &i : balldata.spriteFiles)
    {
        Sprite n(i);
        n.setIDStorage(topidStorage, bottomidStorage);
        n.setRealRadius(balldata.pixelRadius);
        ballSprites.push_back(n);
    }
}

void Ball::moveLeft(float delta)
{
    if (dropped)
    {
        return;
    }
    x -= 200 * delta;
    if (x < 46)
    {
        x = 46;
    }
}

void Ball::moveRight(float delta)
{
    if (dropped)
    {
        return;
    }
    x += 200 * delta;
    if (x > 210)
    {
        x = 210;
    }
}

void Ball::checkBounds()
{
    if (x > 210)
    {
        x = 210;
    }
    if (x < 46)
    {
        x = 46;
    }
}

void Ball::drop()
{
    dropped = true;
}

void Ball::draw()
{
    for (auto &i : ballSprites)
    {
        i.draw(x, y);
    }
}