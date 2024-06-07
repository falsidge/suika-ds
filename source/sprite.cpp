#include "sprite.hpp"

void SpriteData::init(u8 spriteId, u8 rotId)
{
    this->spriteId = spriteId;
    this->rotId = rotId;
}

void Sprite::setIDStorage(IdStorage *topidStorage, IdStorage *bottomidStorage)
{
    idStorage[0] = topidStorage;
    idStorage[1] = bottomidStorage;
}

void Sprite::setRealRadius(s32 radius)
{
    this->radius = radius;
}

void Sprite::drawOnScreen(s32 x, s32 y, int screen)
{
    if (screenIds[screen] == -1)
    {

        s16 id = idStorage[screen]->getId();
        if (id < 0 || id > 127)
        {
            return;
        }
        this->screenIds[screen] = id;
        NF_CreateSprite(screen, screenIds[screen], spriteId, spriteId, x, y);
        if (rotId != -1)
        {
            NF_EnableSpriteRotScale(screen, id, rotId, true);
        }
    }
    else
    {

        NF_MoveSprite(screen, screenIds[screen], x, y);
    }
}

void Sprite::deleteFromScreen(int screen)
{
    if (screenIds[screen] != -1)
    {

        // printf("Deleting sprite %d from screen %d\n", spriteId, screen);
        NF_DeleteSprite(screen, screenIds[screen]);
        idStorage[screen]->releaseId(screenIds[screen]);
        screenIds[screen] = -1;
    }
}

void Sprite::draw(s32 x, s32 y)
{
    x += offsetX;
    y += offsetY;
    x = x - radius;
    y = y - radius;
    if (radius > 32)
    {
        x -= (64 - radius);
        y -= (32 - radius / 2);
    }
    // printf("Drawing sprite %d at %d %d offset %d %d radius %d\n", spriteId, x, y, offsetX, offsetY, radius);

    // bottom screen
    if (y > 192 - radius - (radius <= 32 ? radius : 64 - radius))
    {
        this->drawOnScreen(x, y - 192, 1);
    }
    else
    {
        // this->deleteFromScreen(1);
    }

    // top screen
    if (y <= 192)
    {
        this->drawOnScreen(x, y, 0);
    }
    else
    {
        this->deleteFromScreen(0);
    }
}

Sprite::~Sprite()
{
    deleteFromScreen(0);
    deleteFromScreen(1);
}
