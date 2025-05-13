//
// Created by Jason Evans on 12/05/2025.
//

#ifndef GAME_H
#define GAME_H
#include "raylib.h"
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"

struct Player player;
struct Enemy enemy;
struct Game
{

};

void RunGame(struct Game *game);
void InitGame();
void UpdateGame(float deltaTime);
void DrawGame();
void DestroyGame();


#endif //GAME_H
