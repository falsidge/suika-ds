#pragma once
#include <vector>
#include <string>
#include <nds.h>
#include <nf_lib.h>

#include "idstorage.hpp"

class SpriteData{
    public:
    std::string spriteFile;
    u8 spriteId;
    u8 rotId;
    u8 spriteWidth;
    u8 spriteHeight;
    s32 offsetX = 0;
    s32 offsetY = 0;
    s16 rot = -1;
    SpriteData(std::string spriteFile, u8 spriteWidth, u8 spriteHeight) : spriteFile(spriteFile), spriteWidth(spriteWidth), spriteHeight(spriteHeight) {}
    SpriteData(std::string spriteFile, u8 spriteWidth, u8 spriteHeight, s32 offsetX, s32 offsetY, s16 rot) : spriteFile(spriteFile), spriteWidth(spriteWidth), spriteHeight(spriteHeight), offsetX(offsetX), offsetY(offsetY), rot(rot){}

    void init(u8 spriteId, u8 rotId);
   
};

class Sprite {
    u8 spriteId;
    s8 rotId;
    u8 spriteWidth;
    u8 spriteHeight;
    s32 offsetX=0;
    s32 offsetY=0;
    int screenIds[2]={-1,-1};
    s32 radius = 0;

    IdStorage *idStorage[2];

    void drawOnScreen(s32 x, s32 y, int screen);
    void deleteFromScreen(int screen);

    public:
    Sprite(u8 spriteId, u8 rotId, u8 spriteWidth, u8 spriteHeight) : spriteId(spriteId), rotId(rotId), spriteWidth(spriteWidth), spriteHeight(spriteHeight), offsetX(0), offsetY(0){}
    Sprite(SpriteData spriteData) : spriteId(spriteData.spriteId), rotId(spriteData.rotId), spriteWidth(spriteData.spriteWidth), spriteHeight(spriteData.spriteHeight), offsetX(spriteData.offsetX), offsetY(spriteData.offsetY){}
    void draw(s32 x, s32 y);
    void setRealRadius(s32 radius);
    void setIDStorage(IdStorage* topIdStorage, IdStorage* bottomIdStorage);

    ~Sprite();
};