#ifndef _H_FIGURE_H_
#define _H_FIGURE_H_

#include <GL/glut.h>

#define TIMER_ID 0
#define TIMER_INTERVAL 20
#define JUMP_LEN 5
#define JUMP_HEIGHT 2
#define FORWARD 0
#define LEFT 1
#define RIGHT 2
#define DIAG_LEFT 3
#define DIAG_RIGHT 4
#define BACK 5

#define LAVATEXTURE "lava.bmp"

extern GLuint lava_texture;
extern char str[15];
extern int num_of_rocks;
extern int path_x[25];
extern int path_z[25];
extern int new_level;
extern int jump_ongoing;
extern int level;
extern int left_island;
extern double x;
extern double y;
extern double z;
extern double jumped;
extern int direction;

void lego_man();
void lava_floor(GLuint lava_texture);
void rock();
int check_existing_rock(int x, int z, int num_of_rocks);
void rock_line(int z_coord);
void draw_path();
void generate_path();
void draw_islands();
void island();
void level_text(const char* tekst);

#endif
