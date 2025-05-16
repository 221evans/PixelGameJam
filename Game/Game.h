//
// Created by Jason Evans on 12/05/2025.
//

#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"
#include "../Background/Background.h"
#define MAX_ENEMIES 500

typedef enum {
    GAME_PLAYING,
    GAME_OVER
} GameState;


struct Player player;
struct Background background;

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
    float playerDamageCooldown;
    GameState state;
    float gameOverTimer;
};

void RunGame(struct Game *game);
void InitGame(struct Game *game);
void UpdateGame(struct Game *game, float deltaTime);
void DrawGame(struct Game *game);
void DestroyGame();
void SpawnEnemy(struct Game* game, Vector2 position);
void SpawnEnemyWave(struct Game *game);
void HandlePlayerAttack(struct Game *game);
void HandleDamageToPlayer(struct Game *game, float deltaTime);
void DrawGameOverScreen(struct Game *game);
void RestartGame(struct Game *game);



#endif //GAME_H
