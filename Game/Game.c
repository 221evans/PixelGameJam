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
        DrawBackground(&background);
        DrawGame(game);
        UpdateGame(game,deltaTime);
        EndDrawing();
    }
    DestroyGame();
}


void InitGame(struct Game *game)
{
    InitBackground(&background);
    InitPlayer(&player);
    game->waveTimer = 0;
    game->timeBetweenWaves = 30.0f;
    game->currentWave = 1;
    game->enemySpawnTimer = 0;
    game->enemySpawnRate = 3.0f;
    game->playerDamageCooldown = 0.0f;
    game->enemyCount = 0;
    game->maxEnemies = 3;
    game->state = GAME_PLAYING;
    game->gameOverTimer = 0.0f;

    SpawnEnemyWave(game);

}

void DrawGame(struct Game *game) {
    // Always draw the game world
    // Draw all active enemies
    for (int i = 0; i < game->enemyCount; i++) {
        EnemyDraw(&game->enemies[i]);
    }

    // Draw player on top
    DrawPlayer(&player);

    // Draw UI based on game state
    switch (game->state)
    {
        case GAME_PLAYING:
            // Draw UI - wave info
            char waveText[50];
            sprintf(waveText, "WAVE: %d", game->currentWave);
            DrawText(waveText, 20, 20, 20, WHITE);

            // Draw progress to next wave
            float waveProgress = game->waveTimer / game->timeBetweenWaves;
            DrawRectangle(20, 50, 200, 10, GRAY);
            DrawRectangle(20, 50, (int)(200 * waveProgress), 10, RED);

            // Draw player health
            char healthText[50];
            sprintf(healthText, "HEALTH: %d", player.health);
            DrawText(healthText, 20, 70, 20, WHITE);
            
            // Health bar
            DrawRectangle(20, 100, 200, 10, GRAY);
            DrawRectangle(20, 100, (int)(200 * player.health / 100.0f), 10, GREEN);
            
            // Draw damage cooldown indicator (optional)
            if (game->playerDamageCooldown > 0) {
                DrawRectangle(20, 115, (int)(200 * game->playerDamageCooldown), 5, RED);
            }
            break;
            
        case GAME_OVER:
            // Draw the game over screen on top of the game world
            DrawGameOverScreen(game);
            break;
    }
}

void UpdateGame(struct Game *game, float deltaTime)
{
    // Handle different game states
    switch (game->state)
    {
        case GAME_PLAYING:
            // Normal gameplay updates
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

            // Handle player attacking enemies
            HandlePlayerAttack(game);
            
            // Handle enemies damaging player
            HandleDamageToPlayer(game, deltaTime);
            break;
            
        case GAME_OVER:
            // Update game over timer for effects
            game->gameOverTimer += deltaTime;
            
            // Check for restart input
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
            {
                RestartGame(game);
            }
            break;
    }
}

void DestroyGame()
{
    DestroyPlayer(&player);
    DestroyBackground(&background);
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

void HandlePlayerAttack(struct Game* game)
{
    if (!player.isAttacking)
    {
        return;
    }

    // Only deal damage on specific attack frames (middle of animation)
    // For example, frames 3-5 of an 8-frame attack animation

    if (player.currentFrame < 3 || player.currentFrame > 5)
    {
        return;
    }

    // Get players attackHitbox
    Rectangle attackHitbox = GetPlayerAttackHitbox(&player);

    // check collision with each enemy

    for (int i = 0; i < game->enemyCount; i++) {
        if (CheckCollisionRecs(attackHitbox, game->enemies[i].destRec)) {
            // Damage the enemy and check if it died
            bool died = DamageEnemy(&game->enemies[i], player.damage);

            if (died) {
                // Remove the enemy (swap with the last one and decrease count)
                game->enemies[i] = game->enemies[game->enemyCount - 1];
                game->enemyCount--;
                i--; // Recheck the same index since we swapped in a new enemy
            }
        }
    }



}

void HandleDamageToPlayer(struct Game *game, float deltaTime)
{
    // Update cooldown timer
    if (game->playerDamageCooldown > 0)
    {
        game->playerDamageCooldown -= deltaTime;
    }
    
    // Only check for damage if cooldown has expired and game is in playing state
    if (game->playerDamageCooldown <= 0 && game->state == GAME_PLAYING)
    {
        // Check each enemy's distance to player
        for (int i = 0; i < game->enemyCount; i++)
        {
            // Use rectangle collision
            if (CheckCollisionRecs(game->enemies[i].destRec, player.destRec))
            {
                // Enemy is close enough to damage player
                bool playerDied = DamagePlayer(&player, game->enemies[i].damage);
                
                // Reset cooldown so player isn't damaged immediately again
                game->playerDamageCooldown = 1.0f;  // 1 second cooldown
                
                // Handle player death
                if (playerDied)
                {
                    // Change game state to GAME_OVER
                    game->state = GAME_OVER;
                    game->gameOverTimer = 0.0f;
                    printf("Game Over! Player has died.\n");
                }
                
                // Only take damage from one enemy per cooldown period
                break;
            }
        }
    }
}

void DrawGameOverScreen(struct Game *game)
{
    // Draw semi-transparent overlay
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ColorAlpha(BLACK, 0.7f));
    
    // Draw game over text
    const char *gameOverText = "GAME OVER";
    int fontSize = 50;
    int textWidth = MeasureText(gameOverText, fontSize);
    DrawText(gameOverText, (GetScreenWidth() - textWidth) / 2, GetScreenHeight() / 3, fontSize, RED);
    
    // Draw wave survived text
    char waveText[50];
    sprintf(waveText, "You survived %d waves", game->currentWave - 1);
    fontSize = 30;
    textWidth = MeasureText(waveText, fontSize);
    DrawText(waveText, (GetScreenWidth() - textWidth) / 2, GetScreenHeight() / 2, fontSize, WHITE);
    
    // Draw restart instruction
    const char *restartText = "Press ENTER to restart";
    fontSize = 25;
    textWidth = MeasureText(restartText, fontSize);
    DrawText(restartText, (GetScreenWidth() - textWidth) / 2, GetScreenHeight() * 2/3, fontSize, WHITE);
    
    // Blink the restart text
    if ((int)(game->gameOverTimer * 2) % 2 == 0)
    {
        DrawText(restartText, (GetScreenWidth() - textWidth) / 2, GetScreenHeight() * 2/3, fontSize, WHITE);
    }
}

void RestartGame(struct Game *game)
{
    // Clean up existing enemies
    for (int i = 0; i < game->enemyCount; i++)
    {
        EnemyDestroy(&game->enemies[i]);
    }
    
    // Reset game state
    game->waveTimer = 0;
    game->timeBetweenWaves = 30.0f;
    game->currentWave = 1;
    game->enemySpawnTimer = 0;
    game->enemySpawnRate = 3.0f;
    game->playerDamageCooldown = 0.0f;
    game->state = GAME_PLAYING;
    game->gameOverTimer = 0.0f;
    game->enemyCount = 0;
    game->maxEnemies = 3;
    
    // Reset player
    InitPlayer(&player);
    
    // Start a new wave
    SpawnEnemyWave(game);
}