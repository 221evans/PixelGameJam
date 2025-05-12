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
}


void InitGame()
{

    InitPlayer(&player);
}

void DrawGame()
{
    DrawPlayer(&player);
}

void UpdateGame(float deltaTime)
{
    UpdatePlayer(&player, deltaTime);
}
