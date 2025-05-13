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
    player->playerRectangle = (Rectangle) { 0, 0, 64, 64 };
    player->playerPosition = (Vector2) { 100, 385 };
    player->origin = (Vector2) { 0, 0 };
    player->sourceRec = (Rectangle) {0,0,64,64};
    player->destRec = (Rectangle) {
        player->playerPosition.x,
        player->playerPosition.y,
        player->playerRectangle.width,
        player->playerRectangle.height,
    };

    // Init Animations
    player->currentFrame = 0;
    player->frameCount = 0;
    player->frameSpeed = 8;
    player->totalFrames = 4;
    player->isRunning = false;
    player->isIdle = true;
    player->frameTimer = 0.0f;
}



void DrawPlayer(const struct Player *player)
{
    if (player->isRunning)
    {
        DrawTexturePro(player->playerRunTexture, player->sourceRec,player->destRec,player->origin,0,RAYWHITE);
    }
    else if (player->isIdle)
    {

        DrawTexturePro(player->playerIdleTexture, player->sourceRec,player->destRec,player->origin,0,RAYWHITE);
    }


}

void UpdatePlayer(struct Player *player, const float deltaTime)
{
    // Reset animation states
    bool wasRunning = player->isRunning;
    player->isRunning = false;
    player->isIdle = true;

    // Handle movement
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D) || IsKeyDown(KEY_W) || IsKeyDown(KEY_S))
    {
        player->isRunning = true;
        player->isIdle = false;

        // Handle actual movement
        if (IsKeyDown(KEY_A)) player->playerPosition.x -= player->speed * deltaTime;
        if (IsKeyDown(KEY_D)) player->playerPosition.x += player->speed * deltaTime;
        if (IsKeyDown(KEY_W)) player->playerPosition.y -= player->speed * deltaTime;
        if (IsKeyDown(KEY_S)) player->playerPosition.y += player->speed * deltaTime;
    }

    // Update position for drawing
    player->destRec.x = player->playerPosition.x;
    player->destRec.y = player->playerPosition.y;

    // Reset animation when state changes
    if (wasRunning != player->isRunning) {
        player->currentFrame = 0;
        player->frameTimer = 0.0f;
        player->sourceRec.x = 0;
    }


    if (player->isRunning)
    {
        player->totalFrames = 6;
        player->frameSpeed = 10;
        printf("Frame Speed: %d\n", player->frameSpeed);
    }
    else if (player->isIdle)
    {
        player->totalFrames = 4;
        player->frameSpeed = 6;
        printf("Frame Speed: %d\n", player->frameSpeed);
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

        // Debugging
        printf("New frame: %d, SourceRec.x: %.1f\n",
               player->currentFrame, player->sourceRec.x);
    }
}

void DestroyPlayer(const struct Player *player)
{
    UnloadTexture(player->playerIdleTexture);
    UnloadTexture(player->playerRunTexture);
}