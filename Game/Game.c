//
// Created by Jason Evans on 12/05/2025.
//

#include "Game.h"



void RunGame(struct Game *game)
{
    InitWindow(800, 450, "Game");

    InitGame();


    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawGame();
        UpdateGame(deltaTime);
        EndDrawing();
    }
    DestroyGame();
}


void InitGame()
{

    InitPlayer(&player);
    EnemyInit(&enemy);
}

void DrawGame()
{
    DrawPlayer(&player);
    EnemyDraw(&enemy);
}

void UpdateGame(float deltaTime)
{
    UpdatePlayer(&player, deltaTime);
    EnemyUpdate(&enemy, deltaTime);
}

void DestroyGame()
{
    DestroyPlayer(&player);
    EnemyDestroy(&enemy);
    CloseWindow();
}

