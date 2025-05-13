//
// Created by Jason Evans on 13/05/2025.
//

#ifndef ENEMY_H
#define ENEMY_H
#include "raylib.h"

struct Enemy
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
    Rectangle enemyRectangle;
    Rectangle destRec;
    Rectangle sourceRec;
    Texture2D enemyIdleTexture;
    Texture2D enemyRunTexture;
    Vector2 enemyPosition;
    Vector2 origin;
};

void EnemyInit(struct Enemy *enemy);
void EnemyDraw(struct Enemy *enemy);
void EnemyUpdate(struct Enemy *enemy, const float deltaTime);
void EnemyDestroy(struct Enemy *enemy);


#endif //ENEMY_H


