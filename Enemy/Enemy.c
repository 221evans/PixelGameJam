//
// Created by Jason Evans on 13/05/2025.
//

#include "Enemy.h"
#include <math.h>
#include "raylib.h"
#include "../Game/Game.h"

void EnemyInit(struct Enemy* enemy)
{
    enemy->health = 10;
    enemy->damage = 5;
    enemy->speed = 20;
    enemy->enemyIdleTexture = LoadTexture("Resources/Zombie//Muscle/Idle-Sheet.png");
    enemy->enemyRunTexture = LoadTexture("Resources/Zombie//Muscle/Run-Sheet.png");
    enemy->enemyRectangle = (Rectangle) {0,0,64,64};
    enemy->enemyPosition = (Vector2) {300, 300};
    enemy->sourceRec = (Rectangle) {0,0,64,64};
    enemy->destRec = (Rectangle) {

        enemy->enemyPosition.x,
        enemy->enemyPosition.y,
        enemy->sourceRec.width * 1.5f,
        enemy->sourceRec.height * 1.5f
    };

    // Init animations
    enemy->currentFrame = 0;
    enemy->frameCount = 0;
    enemy->frameSpeed = 8;
    enemy->totalFrames = 4;
    enemy->isRunning = false;
    enemy->isIdle = true;
    enemy->frameTimer = 0;
    enemy->isFacingRight = true;
}

void EnemyDraw(struct Enemy *enemy)
{

    Texture2D currentTexture = enemy->isRunning ? enemy->enemyRunTexture : enemy->enemyIdleTexture;

    Rectangle srcRec = enemy->sourceRec;

    if (!enemy->isFacingRight)
    {
        srcRec.width = -srcRec.width;
    }

    DrawTexturePro(currentTexture,srcRec,enemy->destRec,enemy->origin,0,RAYWHITE);



}

void EnemyUpdate(struct Enemy* enemy, const float deltaTime)
{
    bool wasRunning = enemy->isRunning;

    // Calculate direction to player (vector from enemy to player)
    Vector2 direction = {
        player.playerPosition.x - enemy->enemyPosition.x,
        player.playerPosition.y - enemy->enemyPosition.y
    };

    if (direction.x > 0)
    {
        enemy->isFacingRight = true;
    }
    else if (direction.x < 0)
    {
        enemy->isFacingRight = false;
    }

    // Calculate distance to player
    float distance = sqrtf(direction.x * direction.x + direction.y * direction.y);

    // Set the enemy's state based on distance
    if (distance > 10.0f) {  // Only move if not already touching player
        // Set animation state to running
        enemy->isRunning = true;
        enemy->isIdle = false;

        // Normalize direction vector (make it length 1)
        if (distance > 0) {
            direction.x /= distance;
            direction.y /= distance;
        }

        // Move enemy toward player (only once)
        enemy->enemyPosition.x += direction.x * enemy->speed * deltaTime;
        enemy->enemyPosition.y += direction.y * enemy->speed * deltaTime;
    } else {
        // Close enough to player, stop and idle
        enemy->isRunning = false;
        enemy->isIdle = true;
    }

    // Update destination rectangle for drawing
    enemy->destRec.x = enemy->enemyPosition.x;
    enemy->destRec.y = enemy->enemyPosition.y;

    // Check if animation state changed
    if (wasRunning != enemy->isRunning) {
        enemy->currentFrame = 0;
        enemy->frameTimer = 0;
        enemy->sourceRec.x = 0;
    }

    // Set animation parameters based on state
    if (enemy->isRunning) {
        enemy->totalFrames = 6;
        enemy->frameSpeed = 10;
    } else if (enemy->isIdle) {
        enemy->totalFrames = 4;
        enemy->frameSpeed = 6;
    }

    // Animation frame timing - ALWAYS update animation
    enemy->frameTimer += deltaTime;

    float frameDuration = 1.0f / (float)enemy->frameSpeed;

    if (enemy->frameTimer >= frameDuration) {
        enemy->currentFrame = (enemy->currentFrame + 1) % enemy->totalFrames;
        enemy->frameTimer -= frameDuration;
        enemy->sourceRec.x = (float)enemy->currentFrame * enemy->sourceRec.width;
    }
}

void EnemyDestroy(struct Enemy* enemy)
{
    UnloadTexture(enemy->enemyRunTexture);
    UnloadTexture(enemy->enemyIdleTexture);
}

bool DamageEnemy(struct Enemy* enemy, int damage)
{
    enemy->health -= damage;

    if (enemy->health <= 0)
    {
        return true;
    }

    return false;
}

