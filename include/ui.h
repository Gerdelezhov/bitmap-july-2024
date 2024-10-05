#ifndef UI_H
#define UI_H

#include <locale.h>
#include <ncurses.h>

#include "bitmap.h"

typedef struct {
    Bitmap *bitmaps;
    int size;
    int capacity;
} BitmapArray;

typedef struct {
    WINDOW *main_win;
    WINDOW *menu_win;
    WINDOW *input_win;
    WINDOW *out_win;
} Windows;

// Функции для создания, работы с TUI.
void create_windows(Windows *windows);
void draw_borders(WINDOW *win);
void draw_menu(WINDOW *menu_win);
void compare_menu(int bitmap1_id, int bitmap2_id, BitmapArray *array,
                  Windows windows);
void load_bitmap_menu(Windows windows, BitmapArray *array);
int redraw_windows(Windows windows, const BitmapArray *array);

// Функции для работы с массивом битмапов.
int bitmap_show_ncurses(const Bitmap *bitmap, int id, WINDOW *win, int line);
int show_from_array(const BitmapArray *array, int id, WINDOW *win, int line);
int is_valid_bitmap_id(int id, const BitmapArray *array, WINDOW *win);
void init_bitmap_array(BitmapArray *array);
void resize_bitmap_array(BitmapArray *array, int new_capacity);
void add_bitmap(BitmapArray *array, Bitmap bitmap);
void remove_bitmap(BitmapArray *array, int index);

#endif