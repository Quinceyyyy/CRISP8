
#include "window.h"

#include "raylib.h"


void init_window()
{
    InitWindow(SCREEN_HEIGHT, SCREEN_HEIGHT, TITLE);
    SetTargetFPS(60);

}
