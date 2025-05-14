//
// Created by Jason Evans on 14/05/2025.
//

#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "raylib.h"

struct Background
{
    Texture2D backgroundTexture;
};

void InitBackground(struct Background *background);
void DrawBackground(const struct Background *background);
void DestroyBackground(struct Background *background);

#endif //BACKGROUND_H
