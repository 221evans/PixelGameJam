//
// Created by Jason Evans on 12/05/2025.
//

#ifndef PLAYER_H
#define PLAYER_H
#include "raylib.h"

struct Player
{
    int health;
    int damage;
    int speed;
    Texture2D playerTexture;
    Rectangle playerRectangle;
    Rectangle destRec;
    Rectangle sourceRec;
    Vector2 playerPosition;
    Vector2 origin;
};

void InitPlayer(struct Player *player);
void UpdatePlayer(struct Player *player, float deltaTime);
void DrawPlayer(const struct Player *player);
void DestroyPlayer(struct Player *player);


#endif //PLAYER_H
