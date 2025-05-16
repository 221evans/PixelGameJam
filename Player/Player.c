//
// Created by Jason Evans on 12/05/2025.
//

#include "Player.h"

#include <stdio.h>

void InitPlayer(struct Player *player)
{

    player->health = 100;
    player->damage = 10;
    player->speed = 100;
    player->playerIdleTexture = LoadTexture("Resources/Hunter/Idle-Side-Sheet.png");
    player->playerRunTexture = LoadTexture("Resources/Hunter/Walk-Side-Sheet.png");
    player->playerAttackTexture = LoadTexture("Resources/Hunter/Slice-Side-Sheet.png");
    player->playerRectangle = (Rectangle) { 0, 0, 64, 64 };
    player->playerPosition = (Vector2) { 100, 300 };
    player->origin = (Vector2) { 0, 0 };
    player->sourceRec = (Rectangle) {0,0,64,64};
    player->destRec = (Rectangle) {
        player->playerPosition.x,
        player->playerPosition.y,
        player->playerRectangle.width * 1.5f,
        player->playerRectangle.height *1.5f,
    };

    // Init Animations
    player->currentFrame = 0;
    player->frameCount = 0;
    player->frameSpeed = 8;
    player->totalFrames = 4;
    player->isRunning = false;
    player->isIdle = true;
    player->frameTimer = 0.0f;
    player->isFacingRight = true;
    player->isAttacking = false;
}



void DrawPlayer(const struct Player *player)
{
    Texture2D currentTexture;

    // Select the appropriate texture based on state
    if (player->isAttacking) {
        currentTexture = player->playerAttackTexture;
    } else if (player->isRunning) {
        currentTexture = player->playerRunTexture;
    } else {
        currentTexture = player->playerIdleTexture;
    }

    // Create a source rectangle that can be flipped
    Rectangle srcRec = player->sourceRec;

    // When facing left, flip the sprite horizontally by making width negative
    if (!player->isFacingRight) {
        srcRec.width = -srcRec.width;
    }

    DrawTexturePro(currentTexture, srcRec, player->destRec, player->origin, 0, RAYWHITE);
}


void UpdatePlayer(struct Player *player, const float deltaTime)
{
    // Reset animation states
    bool wasRunning = player->isRunning;
    player->isRunning = false;
    player->isIdle = true;

    // Handle attack input
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        player->isAttacking = true;
        player->isIdle = false;
        player->currentFrame = 0;  // Reset animation frame on attack start
        player->frameTimer = 0.0f;
    }

    // Handle movement
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D) || IsKeyDown(KEY_W) || IsKeyDown(KEY_S))
    {
        player->isRunning = true;
        player->isIdle = false;

        // Handle actual movement
        if (IsKeyDown(KEY_A))
        {
            player->playerPosition.x -= player->speed * deltaTime;
            player->isFacingRight = false;
        }
        if (IsKeyDown(KEY_D))
        {
            player->playerPosition.x += player->speed * deltaTime;
            player->isFacingRight = true;
        }
        if (IsKeyDown(KEY_W)) player->playerPosition.y -= player->speed * deltaTime;
        if (IsKeyDown(KEY_S)) player->playerPosition.y += player->speed * deltaTime;
    }

    // Update position for drawing
    player->destRec.x = player->playerPosition.x;
    player->destRec.y = player->playerPosition.y;

    // Reset animation when state changes
    if ((wasRunning != player->isRunning) && !player->isAttacking) {
        player->currentFrame = 0;
        player->frameTimer = 0.0f;
        player->sourceRec.x = 0;
    }

    // Update animation properties based on state
    if (player->isAttacking)
    {
        player->totalFrames = 8;
        player->frameSpeed = 10;
    }
    else if (player->isRunning)
    {
        player->totalFrames = 6;
        player->frameSpeed = 10;
    }
    else // isIdle
    {
        player->totalFrames = 4;
        player->frameSpeed = 6;
    }

    // Animation timing
    player->frameTimer += deltaTime;

    // Convert frameSpeed to the time each frame should be displayed
    float frameDuration = 1.0f / (float) player->frameSpeed;

    // Check if it's time for the next frame
    if (player->frameTimer >= frameDuration) {
        // Update to next frame
        player->currentFrame = (player->currentFrame + 1) % player->totalFrames;

        // Reset timer, but keep remainder for smoother animation
        player->frameTimer -= frameDuration;

        // Update the source rectangle x position
        player->sourceRec.x = (float)player->currentFrame * player->sourceRec.width;

        // End attack animation when it completes one cycle
        if (player->isAttacking && player->currentFrame == 0) {
            player->isAttacking = false;
            player->isIdle = true;
        }

        // Debugging
        printf("New frame: %d, SourceRec.x: %.1f, Attacking: %d\n",
               player->currentFrame, player->sourceRec.x, player->isAttacking);
    }
}

void DestroyPlayer(const struct Player *player)
{
    UnloadTexture(player->playerIdleTexture);
    UnloadTexture(player->playerRunTexture);
    UnloadTexture(player->playerAttackTexture);
}

Rectangle GetPlayerAttackHitbox(const struct Player* player)
{
    Rectangle attackHitbox = player->destRec;

    if (player->isFacingRight)
    {
        attackHitbox.x += attackHitbox.width;
        attackHitbox.width = 1.0f;
    }
    else
    {
        attackHitbox.x -= 1.0f;
        attackHitbox.width = 1.0f;
    }

    return attackHitbox;
}

bool DamagePlayer(struct Player* player, int damage)
{
    player->health -= damage;
    if (player->health <= 0)
    {
        return true;
    }
    return false;
}
