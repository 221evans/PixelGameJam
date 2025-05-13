//
// Created by Jason Evans on 13/05/2025.
//

#include "Enemy.h"

#include <stdio.h>

#include "raylib.h"

void EnemyInit(struct Enemy* enemy)
{
    enemy->health = 10;
    enemy->damage = 5;
    enemy->enemyIdleTexture = LoadTexture("Resources/Zombie//Muscle/Idle-Sheet.png");
    enemy->enemyRunTexture = LoadTexture("Resources/Zombie//Muscle/Run-Sheet.png");
    enemy->enemyRectangle = (Rectangle) {0,0,64,64};
    enemy->enemyPosition = (Vector2) {300, 385};
    enemy->sourceRec = (Rectangle) {0,0,64,64};
    enemy->destRec = (Rectangle) {

        enemy->enemyPosition.x,
        enemy->enemyPosition.y,
        enemy->sourceRec.width,
        enemy->sourceRec.height
    };

    // Init animations
    enemy->currentFrame = 0;
    enemy->frameCount = 0;
    enemy->frameSpeed = 8;
    enemy->totalFrames = 4;
    enemy->isRunning = false;
    enemy->isIdle = true;
    enemy->frameTimer = 0;
}

void EnemyDraw(struct Enemy* enemy)
{
    if (enemy->isRunning)
    {
        DrawTexturePro(enemy->enemyRunTexture,enemy->sourceRec,enemy->destRec,enemy->origin,0,RAYWHITE);
    }
    else if (enemy->isIdle)
    {
        DrawTexturePro(enemy->enemyIdleTexture,enemy->sourceRec,enemy->destRec,enemy->origin,0,RAYWHITE);
    }
}

void EnemyUpdate(struct Enemy* enemy, const float deltaTime)
{
    bool wasRunning = enemy->isRunning;


    enemy->destRec.x = enemy->enemyPosition.x;
    enemy->destRec.y = enemy->enemyPosition.y;

    if (wasRunning != enemy->isRunning)
    {
        enemy->currentFrame = 0;
        enemy->frameTimer = 0;
        enemy->sourceRec.x = 0;
    }

    if (enemy->isRunning)
    {
        enemy->totalFrames = 6;
        enemy->frameSpeed = 10;
        printf("Enemy Frame Speed: %d\n", enemy->frameSpeed );
    }
    else if (enemy->isIdle)
    {
        enemy->totalFrames = 4;
        enemy->frameSpeed = 6;
        printf("Enemy Frame Speed: %d\n", enemy->frameSpeed);
    }

    // Animation frame timing
    enemy->frameTimer += deltaTime;

    float frameDuration = 1.0f / (float) enemy->frameSpeed;

    if (enemy->frameTimer >= frameDuration)
    {
        enemy->currentFrame = (enemy->currentFrame + 1) % enemy->totalFrames;

        enemy->frameTimer -= frameDuration;

        enemy->sourceRec.x = (float)enemy->currentFrame * enemy->sourceRec.width;
    }

}
