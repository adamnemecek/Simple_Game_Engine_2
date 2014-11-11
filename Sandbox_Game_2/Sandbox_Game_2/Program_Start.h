#ifndef SANDBOX_GAME_PROGRAM_START_H
#define SANDBOX_GAME_PROGRAM_START_H

// These functions must be defined in the game project
void init();
void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);

unsigned int defaults(unsigned int displayMode, int &width, int &height);


#endif
