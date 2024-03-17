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
    s8 offsetX;
    s8 offsetY;
    s16 rot = 0;
    SpriteData(std::string spriteFile, u8 spriteWidth, u8 spriteHeight) : spriteFile(spriteFile), spriteWidth(spriteWidth), spriteHeight(spriteHeight) {}
    SpriteData(std::string spriteFile, u8 spriteWidth, u8 spriteHeight, s8 offsetX, s8 offsetY, s16 rot) : spriteFile(spriteFile), spriteWidth(spriteWidth), spriteHeight(spriteHeight), offsetX(offsetX), offsetY(offsetY), rot(rot){}

    void init(u8 spriteId, u8 rotId);
   
};

class Sprite {
    u8 spriteId;
    s8 rotId;
    u8 spriteWidth;
    u8 spriteHeight;
    s8 offsetX=0;
    s8 offsetY=0;
    s8 screenIds[2]={-1,-1};
    u8 radius = 0;

    IdStorage *idStorage[2];

    void drawOnScreen(s16 x, s16 y, u8 screen);
    void deleteFromScreen(u8 screen);

    public:
    Sprite(u8 spriteId, u8 rotId, u8 spriteWidth, u8 spriteHeight) : spriteId(spriteId), rotId(rotId), spriteWidth(spriteWidth), spriteHeight(spriteHeight){}
    Sprite(SpriteData spriteData) : spriteId(spriteData.spriteId), rotId(spriteData.rotId), spriteWidth(spriteData.spriteWidth), spriteHeight(spriteData.spriteHeight), offsetX(spriteData.offsetX), offsetY(spriteData.offsetY){}
    void draw(s16 x, s16 y);
    void setRealRadius(u8 radius);
    void setIDStorage(IdStorage* topIdStorage, IdStorage* bottomIdStorage);

    ~Sprite();
};