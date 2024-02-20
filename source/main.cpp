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
// #include <chrono>

#include <nds.h>
#include <filesystem.h>

#include <nf_lib.h>

#include "ball.hpp"
#include "idstorage.hpp"
// using namespace std::chrono;



// 16 20 28 32 42 55 64 78 88 108 128
std::vector<BallData> ballData = {
    {
        {"sprite/ball0"},
        0.8f,
        16
    },
    {
        {"sprite/ball1"},
        1.0f,
        32
    }, 
    {
        {"sprite/ball2"},
        1.4f,
        32
    },
    {
        {"sprite/ball3"},
        1.6f,
        32
    },
    {
        {"sprite/ball4"},
        2.1f,
        32
    },
    {
        {"sprite/ball5"},
        2.75f,
        32
    },
    {
        {"sprite/ball6"},
        3.2f,
        32
    },
    {
        {"sprite/ball700", "sprite/ball701", "sprite/ball702", "sprite/ball703"},
        3.9f,
        32
    },
    {
        {"sprite/ball800", "sprite/ball801", "sprite/ball802", "sprite/ball803"},
        4.4f,
        32
    },
    {
        {"sprite/ball90", "sprite/ball91", "sprite/ball92", "sprite/ball93"},
        5.9f,
        32
    },
    {
        {"sprite/ball1000", "sprite/ball1001", "sprite/ball1002", "sprite/ball1003"},
        6.4f,
        64
    }
};

int main(int argc, char **argv)
{
    // Set random seed based on the current time
    srand(time(NULL));


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

    IdStorage idstorage;

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
    NF_LoadTiledBg("bg/suika_top", "nfl", 256, 256);
    NF_LoadTiledBg("bg/suika_bottom", "layer_3", 256, 256);
    // NF_LoadTiledBg("bg/bg2", "layer_2", 1024, 256);

    // Load sprite files from NitroFS
    // NF_LoadSpriteGfx("sprite/character", 0, 64, 64);
    // NF_LoadSpritePal("sprite/character", 0);

    NF_LoadSpriteGfx("sprite/ball10", 0, 64, 64);
    NF_LoadSpritePal("sprite/ball10", 0);

    // Create top screen background
    NF_CreateTiledBg(0, 3, "nfl");

    // Create bottom screen backgrounds
    NF_CreateTiledBg(1, 3, "layer_3");

    // Transfer the required sprites to VRAM
    NF_VramSpriteGfx(1, 0, 0, true); // Ball: Keep all frames in VRAM
    NF_VramSpritePal(1, 0, 0);

    NF_VramSpriteGfx(0, 0, 0, true); 
    NF_VramSpritePal(0, 0, 0);

    // Setup ball sprites

    // for (int n = 0; n < 32; n++)
    // {
    //     ball_x[n] = rand() % 223;
    //     ball_y[n] = rand() % 159;
    //     ball_spx[n] = (rand() % 3) + 1;
    //     ball_spy[n] = (rand() % 3) + 1;
    //     NF_CreateSprite(0, n, 0, 0, ball_x[n], ball_y[n]);
    // }
    s16 dropBall_x = 20;
    s16 dropBall_y = 70;
    s8 dropBall_spx = 1;
    s8 dropBall_spy = 1;

    std::vector<Ball> ballSprites;
    std::vector<b2Body*> ballBodies;

    Ball currentBall(idstorage);
    b2Body* currentBody;
    u8 debounce = 0;

    // b2AABB worldAABB;
    // worldAABB.minVertex.Set(-100.0f, -100.0f);
    // worldAABB.maxVertex.Set(500.0f, 500.0f);

    // b2Vec2 gravity(0.0f, -10.0f);
    // bool doSleep = true;
    // b2World world(worldAABB, gravity, doSleep);

    // b2BoxDef groundBoxDef;
    // groundBoxDef.extents.Set(128.0f, 2.0f);
    // // groundBoxDef.density = 0.0f;

    // b2BodyDef groundBodyDef;
    // groundBodyDef.position.Set(128.0f, 0.0f);
    // groundBodyDef.AddShape(&groundBoxDef);
    // b2Body* ground = world.CreateBody(&groundBodyDef);

    // b2CircleDef circle1;
    // circle1.localPosition.Set(0.0f, 0.0f);

    // circle1.radius = 1.0f;
    // circle1.density = 1.0f;
    // circle1.friction = 0.2f;


    // b2BodyDef bodyDef;
    // bodyDef.position.Set(128.0f, 128.0f);
    // bodyDef.AddShape(&circle1);

    b2AABB worldAABB;
    worldAABB.minVertex.Set(-12.8f*2.0f, -19.2f*2.0f);
    worldAABB.maxVertex.Set(12.8f*2.0f, 19.2f*2.0f);

    b2Vec2 gravity(0.0f, -2.0f);
    bool doSleep = true;

    b2World world(worldAABB, gravity, doSleep);

    b2BoxDef groundBoxDef;
    groundBoxDef.extents.Set(12.8f, 1.0f);
    groundBoxDef.density = 0.0f;

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -16.7f);
    groundBodyDef.AddShape(&groundBoxDef);
    b2Body* ground = world.CreateBody(&groundBodyDef);

    b2BoxDef leftWallBoxDef;
    leftWallBoxDef.extents.Set(1.0f, 19.2f);
    leftWallBoxDef.density = 0.0f;

    b2BodyDef leftWallBodyDef;
    leftWallBodyDef.position.Set(-12.8f, 0.0f);
    leftWallBodyDef.AddShape(&leftWallBoxDef);
    b2Body* leftWallBody = world.CreateBody(&leftWallBodyDef);

    b2BoxDef rightWallBoxDef;
    rightWallBoxDef.extents.Set(1.0f, 19.2f);
    rightWallBoxDef.density = 0.0f;

    b2BodyDef rightWallBodyDef;
    rightWallBodyDef.position.Set(12.8f, 0.0f);
    rightWallBodyDef.AddShape(&rightWallBoxDef);
    b2Body* rightWallBody = world.CreateBody(&rightWallBodyDef);
    // b2CircleDef circle1;
    // circle1.radius = 0.1f;
    // circle1.density = 1.0f;
    // circle1.friction = 0.3f;

    // b2BodyDef bodyDef;
    // // bodyDef.position.Set(0.0f, 12.2f);
    // bodyDef.AddShape(&circle1);

    // b2Body* body = world.CreateBody(&bodyDef);

    // b2Body* body = world.CreateBody(&bodyDef);

    while (1)
    {

        // Animate character
        // time_t currentTime = time(NULL);

        world.Step(1/60.0f,  2);
        // b2Vec2 position = body->GetOriginPosition();
        // b2Vec2 position2 = ground->GetOriginPosition();
        // prevTimeMS = currentTimeMS;


        scanKeys();
        u16 keys = keysHeld();
        if (keys & KEY_START)
        {
            break;
        }
        if (keys & KEY_LEFT)
        {
            currentBall.moveLeft();
        }
        if (keys & KEY_RIGHT)
        {
            currentBall.moveRight();
        }

      
        keys = keysDown();

        if (keys & KEY_A && !currentBall.dropped)
        {
            currentBall.drop();
            debounce = 30;
            b2CircleDef circle1;
            circle1.radius = 2.7f;
            circle1.density = 1.0f;
            circle1.friction = 0.5f;

            b2BodyDef bodyDef;
            // bodyDef.position.Set(0.0f, 12.2f);
            bodyDef.AddShape(&circle1);
            // circle1.categoryBits = 0x0002;
            // bodyDef.linearDamping = 0.1f;
            bodyDef.angularDamping = 0.01f;
            bodyDef.position.Set((currentBall.x) / 10.0f - 12.8f, (192 - currentBall.y) / 10.0f) ;
            // bodyDef.position.Set(0.0, 12.2f);
            currentBody = world.CreateBody(&bodyDef);

        }
        // if (debounce > 0)
        // {
        //     debounce--;
        // }
        if (debounce == 0 && currentBall.dropped)
        {
            ballSprites.push_back(currentBall);
            ballBodies.push_back(currentBody);
            currentBall = Ball(idstorage);

        }
        else if (debounce > 0 && currentBall.dropped)
        {
            debounce--;
            b2Vec2 position = currentBody->GetOriginPosition();
            currentBall.x = (int) ((double) position.x * 10.0f )+ 128 ;
            currentBall.y = 192 - (int)((double) position.y * 10.0f);
        }
        // else if (!currentBall.dropped)
        // {
        //     currentBall.update();
        // }

        currentBall.draw();

        


        // ball.update();
        // ball.x = (unsigned short) position.x + 128 - 16;
        // ball.y = 192 - (int)((double) position.y * 10.0f);
        // ball.draw();
        // printf("ball %4.2f %4.2f ground %4.2f %4.2f \n ", (double) position.x, (double) position.y, (double) position2.x, (double) position2.y);
        for (u8 i = 0; i < ballSprites.size(); i++)
        {
            // ballSprites[i].update();
            b2Vec2 position = ballBodies[i]->GetOriginPosition();
            ballSprites[i].x = (int) ((double) position.x  * 10.0f) + 128 - 16;
            ballSprites[i].y = 192 - (int)((double) position.y * 10.0f);
            ballSprites[i].draw();
        }

        // Move balls
        // for (int n = 0; n < 32; n++)
        // {
        //     ball_x[n] += ball_spx[n];
        //     if ((ball_x[n] < 0) || (ball_x[n] > 223))
        //         ball_spx[n] *= -1;

        //     ball_y[n] += ball_spy[n];
        //     if ((ball_y[n] < 0) || (ball_y[n] > 159))
        //         ball_spy[n] *= -1;

        //     NF_MoveSprite(0, n, ball_x[n], ball_y[n]);
        // }

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
