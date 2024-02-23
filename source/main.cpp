// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: NightFox & Co., 2009-2011
//
// Sprite loading example
// http://www.nightfoxandco.com

#include <stdio.h>
#include <time.h>
#include <vector>
#include <string>
#include <unordered_set>
// #include <chrono>

#include <nds.h>
#include <filesystem.h>

#include <nf_lib.h>

#include "ball.hpp"
#include "idstorage.hpp"
// using namespace std::chrono;

// 16 20 28 32 42 55 64 78 88 108 128
std::vector<BallData> ballData = {
    {0, {"sprite/ball0"}, 0.8f, 8, 16, {}},
    {1, {"sprite/ball1"}, 1.0f, 10, 32, {}},
    {2, {"sprite/ball2"}, 1.4f, 14, 32, {}},
    {3, {"sprite/ball3"}, 1.6f, 16, 32, {}},
    {4, {"sprite/ball4"}, 2.1f, 21, 64, {}},
    {5, {"sprite/ball5"}, 2.75f, 27, 64, {}},
    {6, {"sprite/ball6"}, 3.2f, 32, 64, {}},
    // {7,
    //     {"sprite/ball7","sprite/ball700", "sprite/ball701", "sprite/ball702", "sprite/ball703"},
    //     3.9f,
    //     39,
    //     32,
    //     {}
    // },
    // {8,
    //     {"sprite/ball8","sprite/ball800", "sprite/ball801", "sprite/ball802", "sprite/ball803"},
    //     4.4f,
    //     44,
    //     32,
    //     {}
    // },
    // {9,
    //     {"sprite/ball9","sprite/ball90", "sprite/ball91", "sprite/ball92", "sprite/ball93"},
    //     5.9f,
    //     59,
    //     32,
    //     {}
    // },
    // {10,
    //     {"sprite/ball10","sprite/ball1000", "sprite/ball1001", "sprite/ball1002", "sprite/ball1003"},
    //     6.4f,
    //     64,
    //     64,
    //     {}
    // }
};

int main(int argc, char **argv)
{
    // Set random seed based on the current time
    srand(time(NULL));
    u32 currentTimeMS = 0;

    // timerStart(0, ClockDivider_1024, TIMER_FREQ_1024(10000), timerCallBack);

    // Prepare a NitroFS initialization screen
    NF_Set2D(0, 0);
    NF_Set2D(1, 0);
    consoleDemoInit();
    printf("\n NitroFS init. Please wait.\n\n");
    printf(" Iniciando NitroFS,\n por favor, espere.\n\n");
    swiWaitForVBlank();

    // Initialize NitroFS and set it as the root folder of the filesystem
    nitroFSInit(NULL);
    NF_SetRootFolder("NITROFS");

    IdStorage topidstorage;
    IdStorage bottomidstorage;

    // Initialize 2D engine in both screens and use mode 0
    NF_Set2D(0, 0);
    NF_Set2D(1, 0);

    // Initialize tiled backgrounds system
    NF_InitTiledBgBuffers(); // Initialize storage buffers
    NF_InitTiledBgSys(0);    // Top screen
    NF_InitTiledBgSys(1);    // Bottom screen

    // Initialize sprite system
    NF_InitSpriteBuffers(); // Initialize storage buffers
    NF_InitSpriteSys(0);    // Top screen
    NF_InitSpriteSys(1);    // Bottom screen

    // Load background files from NitroFS
    NF_LoadTiledBg("bg/suika_top", "suika_top", 256, 256);
    NF_LoadTiledBg("bg/suika_bottom", "suika_bottom", 256, 256);
    // NF_LoadTiledBg("bg/bg2", "layer_2", 1024, 256);

    // Create top screen background
    NF_CreateTiledBg(0, 3, "suika_top");

    // Create bottom screen backgrounds
    NF_CreateTiledBg(1, 3, "suika_bottom");

    u8 spriteId = 0;
    for (auto &balldata : ballData)
    {

        std::string filename = balldata.spriteFiles[0];
        const char *filename_c = filename.c_str();
        NF_LoadSpriteGfx(filename_c, spriteId, balldata.spriteSize, balldata.spriteSize);
        NF_LoadSpritePal(filename_c, spriteId);
        NF_VramSpriteGfx(1, spriteId, spriteId, true); // Ball: Keep all frames in VRAM
        NF_VramSpritePal(1, spriteId, spriteId);

        NF_VramSpriteGfx(0, spriteId, spriteId, true); // Ball: Keep all frames in VRAM
        NF_VramSpritePal(0, spriteId, spriteId);
        balldata.spriteIds.push_back(spriteId);
        spriteId++;
    }

    std::unordered_set<b2Body *> ballBodies;

    BallData currentballdata = ballData[rand() % (ballData.size() - 2)];

    // Ball *currentBall = new Ball(idstorage, currentballdata.spriteIds[0], currentballdata.pixelRadius, ballBodies.size());
    Ball *currentBall = new Ball(&topidstorage, &bottomidstorage, currentballdata);
    b2Body *currentBody = nullptr;
    u8 debounce = 0;

    b2AABB worldAABB;
    worldAABB.minVertex.Set(-12.8f * 2.0f, -19.2f * 2.0f);
    worldAABB.maxVertex.Set(12.8f * 2.0f, 19.2f * 2.0f);

    b2Vec2 gravity(0.0f, -5.0f);
    bool doSleep = true;

    b2World world(worldAABB, gravity, doSleep);

    b2BoxDef groundBoxDef;
    groundBoxDef.extents.Set(12.8f, 1.0f);
    groundBoxDef.density = 0.0f;

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -18.8f);
    groundBodyDef.AddShape(&groundBoxDef);
    world.CreateBody(&groundBodyDef);

    b2BoxDef leftWallBoxDef;
    leftWallBoxDef.extents.Set(1.6f, 19.2f);
    leftWallBoxDef.density = 0.0f;

    b2BodyDef leftWallBodyDef;
    leftWallBodyDef.position.Set(-12.8f, 0.0f);
    leftWallBodyDef.AddShape(&leftWallBoxDef);
    world.CreateBody(&leftWallBodyDef);

    b2BoxDef rightWallBoxDef;
    rightWallBoxDef.extents.Set(1.6f, 19.2f);
    rightWallBoxDef.density = 0.0f;

    b2BodyDef rightWallBodyDef;
    rightWallBodyDef.position.Set(12.8f, 0.0f);
    rightWallBodyDef.AddShape(&rightWallBoxDef);
    world.CreateBody(&rightWallBodyDef);

    cpuStartTiming(0);
    u32 prev = 0;
    float delta = 0;
    bool touched = false;
    bool dropped = false;
    float deltadropped = 0;
    while (1)
    {

        // Animate character
        // time_t currentTime = time(NULL);
        currentTimeMS = cpuGetTiming();
        delta = timerTicks2msec(currentTimeMS - prev) / 1000.0f;
        world.Step(delta, 5);
        prev = currentTimeMS;

        scanKeys();
        u16 keys = keysHeld();
        if (keys & KEY_START)
        {
            break;
        }
        if (keys & KEY_LEFT)
        {
            currentBall->moveLeft(delta);
        }
        if (keys & KEY_RIGHT)
        {
            currentBall->moveRight(delta);
        }
        touchPosition touch;
        touchRead(&touch);
        if (touch.z1 != 0)
        {
            currentBall->x = touch.px;
            currentBall->checkBounds();
            touched = true;
        }

        keys = keysDown();

        if ((keys & KEY_A || (touched && touch.z1 == 0)) && !dropped)
        {
            touched = false;
            dropped = true;
            deltadropped = currentTimeMS;

            b2CircleDef circle1;
            circle1.radius = currentballdata.radius - 0.05;
            circle1.density = 1.0f;
            circle1.friction = 0.5f;

            b2BodyDef bodyDef;
            // bodyDef.position.Set(0.0f, 12.2f);
            bodyDef.userData = currentBall;
            bodyDef.AddShape(&circle1);
            // circle1.categoryBits = 0x0002;
            bodyDef.linearDamping = 0.001f;
            bodyDef.angularDamping = 0.01f;
            bodyDef.position.Set((currentBall->x) / 10.0f - 12.8f, (192 - currentBall->y) / 10.0f);
            // bodyDef.position.Set(0.0, 12.2f);
            currentBody = world.CreateBody(&bodyDef);
            ballBodies.insert(currentBody);
        }

        if (timerTicks2msec(currentTimeMS - deltadropped) > 1000 && dropped)
        {

            currentballdata = ballData[rand() % (ballData.size() - 2)];

            currentBall = new Ball(&topidstorage, &bottomidstorage, currentballdata);
            currentBody = nullptr;
            dropped = false;
        }
        else if (debounce > 0 && dropped)
        {
            debounce--;
        }

        std::unordered_set<b2Body *> destroyedBodies;
        for (b2Contact *c = world.GetContactList(); c; c = c->GetNext())
        {
            b2Body *body1 = c->GetShape1()->GetBody();
            b2Body *body2 = c->GetShape2()->GetBody();
            if (body1->m_userData == nullptr || body2->m_userData == nullptr)
            {
                continue;
            }
            if (destroyedBodies.find(body1) != destroyedBodies.end() || destroyedBodies.find(body2) != destroyedBodies.end())
            {
                continue;
            }
            if (((Ball *)body1->GetUserData())->radius == ((Ball *)body2->GetUserData())->radius)
            {

                if (((Ball *)body1->m_userData)->id == ballData.size() - 1)
                {
                    continue;
                }

                if (body1 == currentBody || body2 == currentBody)
                {
                    currentBall = nullptr;
                }
                destroyedBodies.insert(body1);
                destroyedBodies.insert(body2);

                BallData newballdata = ballData[((Ball *)body1->m_userData)->id + 1];
                b2CircleDef circle1;
                circle1.radius = newballdata.radius - 0.05;
                circle1.density = 1.0f;
                circle1.friction = 0.5f;

                b2BodyDef bodyDef;
                // bodyDef.position.Set(0.0f, 12.2f);
                Ball *newBall = new Ball(&topidstorage, &bottomidstorage, newballdata);

                bodyDef.userData = newBall;
                bodyDef.AddShape(&circle1);
                // circle1.categoryBits = 0x0002;
                bodyDef.linearDamping = 0.001f;
                bodyDef.angularDamping = 0.01f;
                // get midpoint of body1 and body2
                b2Vec2 position1 = body1->GetOriginPosition();
                b2Vec2 position2 = body2->GetOriginPosition();
                bodyDef.position.Set((position1.x + position2.x) / 2.0f, (position1.y + position2.y) / 2.0f);
                // bodyDef.position.Set(0.0, 12.2f);
                ballBodies.insert(world.CreateBody(&bodyDef));

                delete (Ball *)body1->m_userData;
                delete (Ball *)body2->m_userData;
                body1->m_userData = nullptr;
                body2->m_userData = nullptr;

                ballBodies.erase(body1);
                ballBodies.erase(body2);
                world.DestroyBody(body1);
                world.DestroyBody(body2);
            }
        }
        if (!dropped && currentBall != nullptr)
        {
            currentBall->draw();
        }

        for (auto it : ballBodies)
        {
            if ((Ball *)it->m_userData == nullptr)
            {
                continue;
            }
            b2Vec2 position = it->GetOriginPosition();
            ((Ball *)it->m_userData)->x = (int)((double)position.x * 10.0f) + 128;
            ((Ball *)it->m_userData)->y = 192 - (int)((double)position.y * 10.0f);
            ((Ball *)it->m_userData)->draw();
        }

        // Update OAM array
        NF_SpriteOamSet(0);
        NF_SpriteOamSet(1);

        // Wait for the screen refresh
        swiWaitForVBlank();

        // Update OAM
        oamUpdate(&oamMain);
        oamUpdate(&oamSub);
    }

    return 0;
}
