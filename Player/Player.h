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
    int currentFrame;
    int frameSpeed;
    int frameCount;
    int totalFrames;
    float frameTimer;
    bool isRunning;
    bool isIdle;
    bool isFacingRight;
    bool isAttacking;
    Texture2D playerIdleTexture;
    Texture2D playerRunTexture;
    Texture2D playerAttackTexture;
    Rectangle playerRectangle;
    Rectangle destRec;
    Rectangle sourceRec;
    Vector2 playerPosition;
    Vector2 origin;
};

void InitPlayer(struct Player *player);
void UpdatePlayer(struct Player *player, float deltaTime);
void DrawPlayer(const struct Player *player);

void DestroyPlayer(const struct Player *player);



#endif //PLAYER_H
