//
// Created by Jason Evans on 12/05/2025.
//

#ifndef GAME_H
#define GAME_H
#include "raylib.h"
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"
#define MAX_ENEMIES 50

struct Player player;

struct Game
{
    float waveTimer;
    float timeBetweenWaves;
    int currentWave;
    float enemySpawnTimer;
    float enemySpawnRate;
    struct Enemy enemies[MAX_ENEMIES];
    int enemyCount;
    int maxEnemies;
};

void RunGame(struct Game *game);
void InitGame(struct Game *game);
void UpdateGame(struct Game *game, float deltaTime);
void DrawGame(struct Game *game);
void DestroyGame();
void SpawnEnemy(struct Game* game, Vector2 position);
void SpawnEnemyWave(struct Game *game);


#endif //GAME_H
