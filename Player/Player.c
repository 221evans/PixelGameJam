//
// Created by Jason Evans on 12/05/2025.
//

#include "Player.h"

void InitPlayer(struct Player *player)
{
    player->health = 100;
    player->damage = 10;
    player->speed = 10;
    player->playerTexture = LoadTexture("Resources/Idle-Sheet.png");
    player->playerRectangle = (Rectangle) { 0, 0, 64, 64 };
    player->playerPosition = (Vector2) { 100, 385 };
    player->origin = (Vector2) { 0, 0 };
    player->sourceRec = (Rectangle) {0,0,64,64};
    player->destRec = (Rectangle) {
        player->playerPosition.x,
        player->playerPosition.y,
        player->playerRectangle.width,
        player->playerRectangle.height
    };
}

void UpdatePlayer(struct Player *player, const float deltaTime)
{

    if (IsKeyDown(KEY_A))
    {
        player->playerPosition.x -= (float)player->speed * deltaTime;
    }
    if (IsKeyDown(KEY_D))
    {
        player->playerPosition.x += (float)player->speed * deltaTime;
    }
    if (IsKeyDown(KEY_W))
    {
        player->playerPosition.y -= (float)player->speed * deltaTime;
    }
    if (IsKeyDown(KEY_S))
    {
        player->playerPosition.y += (float)player->speed * deltaTime;
    }

    player->destRec.x = player->playerPosition.x;
    player->destRec.y = player->playerPosition.y;

}

void DrawPlayer(const struct Player *player)
{
    DrawTexturePro(player->playerTexture, player->sourceRec,player->destRec,player->origin,0,RAYWHITE);
}