#include <stdio.h>
#include <unistd.h>

#include "Game/Game.h"


int main(void)
{
    chdir("/Users/jasonevans/Documents/PixelGameJam");

    struct Game game;

    RunGame(&game);
}
