//
// Created by Jason Evans on 14/05/2025.
//

#include "Background.h"

void InitBackground(struct Background *background)
{
    background->backgroundTexture = LoadTexture("resources/Background-Image.png");
}
void DrawBackground(const struct Background *background)
{
    DrawTexture(background->backgroundTexture, 0, 0, WHITE);
}
void DestroyBackground(struct Background *background)
{
    UnloadTexture(background->backgroundTexture);
}