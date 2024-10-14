#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <iostream>
#include <windows.h>

#include <ctime>
#include <stdexcept>
#include <conio.h>
#include <string>

using namespace std;

const int game_width = 80;
const int game_height = 20;

// Переменные для игры
bool game_over;
int head_x, head_y, fruit_x, fruit_y, player_points;
int* tail_x;
int* tail_y;
int tail_length;
enum eDirection { stop = 0, dir_left, dir_right, dir_up, dir_down } dir;

// Прототипы функций
void init_game();
void hide_cursor();
void render_game(string player_name);
void update_game();
void process_input();
int set_game_difficulty();
void set_console_cursor(int x, int y);
void set_text_color(int color);
void set_window_title(const string& title);
void free_memory();

#endif
