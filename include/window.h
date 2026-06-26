
#ifndef WINDOW_H
    #define WINDOW_H

    #define WINDOW_WIDTH 800
    #define WINDOW_HEIGHT 450
    #define TITLE "CRISP8"

    typedef struct cpu Cpu;

    void init_window(); 
    void update_user_input(Cpu *cpu);
    void draw_content(Cpu *cpu);

#endif
