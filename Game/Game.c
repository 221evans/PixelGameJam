//
// Created by Jason Evans on 12/05/2025.
//

#include "Game.h"

#include <stdio.h>


void RunGame(struct Game *game)
{
    InitWindow(800, 450, "Game");

    InitGame(game);


    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawGame(game);
        UpdateGame(game,deltaTime);
        EndDrawing();
    }
    DestroyGame();
}


void InitGame(struct Game *game)
{

    InitPlayer(&player);
    game->waveTimer = 0;
    game->timeBetweenWaves = 30.0f;
    game->currentWave = 1;
    game->enemySpawnTimer = 0;
    game->enemySpawnRate = 3.0f;

    game->enemyCount = 0;
    game->maxEnemies = 3;

    SpawnEnemyWave(game);
}

void DrawGame(struct Game *game) {
    // Draw all active enemies
    for (int i = 0; i < game->enemyCount; i++) {
        EnemyDraw(&game->enemies[i]);
    }

    // Draw player on top
    DrawPlayer(&player);

    // Draw UI - wave info
    char waveText[50];
    sprintf(waveText, "WAVE: %d", game->currentWave);
    DrawText(waveText, 20, 20, 20, WHITE);

    // Draw progress to next wave
    float waveProgress = game->waveTimer / game->timeBetweenWaves;
    DrawRectangle(20, 50, 200, 10, GRAY);
    DrawRectangle(20, 50, (int)(200 * waveProgress), 10, RED);
}

void UpdateGame( struct Game *game, float deltaTime)
{
    UpdatePlayer(&player, deltaTime);

    // Wave timer
    game->waveTimer += deltaTime;
    if (game->waveTimer >= game->timeBetweenWaves)
    {
        SpawnEnemyWave(game);
        game->waveTimer = 0;
    }

    // Enemy spawner (gradually fill up to max enemies)
    game->enemySpawnTimer += deltaTime;
    if (game->enemySpawnTimer >= game->enemySpawnRate && game->enemyCount < game->maxEnemies)
    {
        // Randomly choose a side of the screen
        int side = GetRandomValue(0, 3);

        Vector2 position = {};
        switch (side) {
        case 0:  // Top
            position.x = GetRandomValue(50, 750);
            position.y = -50;
            break;
        case 1:  // Right
            position.x = 850;
            position.y = GetRandomValue(50, 400);
            break;
        case 2:  // Bottom
            position.x = GetRandomValue(50, 750);
            position.y = 500;
            break;
        case 3:  // Left
            position.x = -50;
            position.y = GetRandomValue(50, 400);
            break;
        }

        SpawnEnemy(game, position);
        game->enemySpawnTimer = 0;
    }

    // Update active enemies
    for (int i = 0; i < game->enemyCount; i++)
    {
        EnemyUpdate(&game->enemies[i], deltaTime);

        // Check if this enemy should be removed (e.g., if health <= 0)
        if (game->enemies[i].health <= 0)
        {
            // Remove this enemy by replacing it with the last active enemy
            EnemyDestroy(&game->enemies[i]);
            game->enemies[i] = game->enemies[game->enemyCount - 1];
            game->enemyCount--;
            i--; // Check the newly swapped enemy in this position
        }
    }
}

void DestroyGame()
{
    DestroyPlayer(&player);

    CloseWindow();
}

// Spawn a single enemy at the specified position
void SpawnEnemy(struct Game* game, Vector2 position) {
    // Only spawn if we haven't reached the maximum
    if (game->enemyCount < MAX_ENEMIES) {
        struct Enemy* enemy = &game->enemies[game->enemyCount];

        // Initialize the enemy
        EnemyInit(enemy);

        // Set the spawn position
        enemy->enemyPosition = position;
        enemy->destRec.x = position.x;
        enemy->destRec.y = position.y;

        // Increment the enemy count
        game->enemyCount++;
    }
}

void SpawnEnemyWave(struct Game *game)
{
    // Increase the maximum enemies based on wave number
    game->maxEnemies = 3 + (game->currentWave * 2);

    if (game->maxEnemies > MAX_ENEMIES)
    {
        game->maxEnemies = MAX_ENEMIES;
    }

    // Number of enemies to spawn immediately with the new wave
    int enemiesToSpawn = game->currentWave + 2;

    // Cap immediate spawns to prevent overwhelming the player
    if (enemiesToSpawn > 10) {
        enemiesToSpawn = 10;
    }

    // Spawn enemies at random positions around the edges of the screen

    for (int i = 0; i < enemiesToSpawn; i++)
    {
        if (game->enemyCount >= game->maxEnemies)
        {
            break;
        }
        // Randomly choose a side of the screen
        int side = GetRandomValue(0, 3);  // 0: top, 1: right, 2: bottom, 3: left

        Vector2 position = {};

        switch (side) {
        case 0:  // Top
            position.x = GetRandomValue(50, 750);
            position.y = -50;
            break;
        case 1:  // Right
            position.x = 850;
            position.y = GetRandomValue(50, 400);
            break;
        case 2:  // Bottom
            position.x = GetRandomValue(50, 750);
            position.y = 500;
            break;
        case 3:  // Left
            position.x = -50;
            position.y = GetRandomValue(50, 400);
            break;
        }

        SpawnEnemy(game, position);

    }

    // Decrease time between waves (make it faster as game progresses)
    game->timeBetweenWaves *= 0.9f;
    if (game->timeBetweenWaves < 10.0f) {
        game->timeBetweenWaves = 10.0f;  // Minimum 10 seconds between waves
    }

    // Decrease enemy spawn rate (make it faster as game progresses)
    game->enemySpawnRate *= 0.9f;
    if (game->enemySpawnRate < 0.5f) {
        game->enemySpawnRate = 0.5f;  // Minimum 0.5 seconds between spawns
    }

    // Increment wave counter
    game->currentWave++;
}