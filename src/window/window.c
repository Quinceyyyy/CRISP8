
#include "window.h"

#include "cpu.h"
#include "raylib.h"


static const int keymap[MAX_INPUTS] = {
    KEY_X,      // 0
    KEY_ONE,    // 1
    KEY_TWO,    // 2
    KEY_THREE,  // 3
    KEY_Q,      // 4
    KEY_W,      // 5
    KEY_E,      // 6
    KEY_A,      // 7
    KEY_S,      // 8
    KEY_D,      // 9
    KEY_Z,      // A
    KEY_C,      // B
    KEY_FOUR,   // C
    KEY_R,      // D
    KEY_F,      // E
    KEY_V       // F
};


void draw_content(Cpu *cpu)
{
    BeginDrawing();
    ClearBackground(BLACK);

    float scale_x = (float)WINDOW_WIDTH / SCREEN_WIDTH;
    float scale_y = (float)WINDOW_HEIGHT / SCREEN_HEIGHT;

    for (int i = 0; i < SCREEN_SIZE; i++) {
        if (cpu->display_buffer[i] == 1) {
            int x = i % SCREEN_WIDTH;
            int y = i / SCREEN_WIDTH;

            DrawRectangle(x * scale_x, y * scale_y, scale_x, scale_y, WHITE);
        }
    }

    EndDrawing();
}

void update_user_input(Cpu *cpu)
{
    for (int i = 0; i < MAX_INPUTS; i++) {
        if (IsKeyDown(keymap[i])) {
            cpu->input_pad[i] = 1;
        } else {
            cpu->input_pad[i] = 0;
        }
    }
}

void init_window()
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, TITLE);
    SetTargetFPS(60);

}
