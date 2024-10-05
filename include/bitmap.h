#ifndef BITMAP_H
#define BITMAP_H

#include <dlfcn.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME_SIZE 30
#define PLUGIN_PATH "src/"
#define BYTE_SIZE 8
#define SUCCESS 0
#define ERROR -1

typedef struct {
    uint8_t *bits;
    size_t size;
} Bitmap;

// Командные функции
size_t bits_to_byte(size_t bits);
int bitmap_create(Bitmap *bitmap, size_t bits);
int bitmap_clone(const Bitmap *source, Bitmap *clone);
int bitmap_increase(Bitmap *bitmap, size_t length);
int bitmap_copy(const Bitmap *source, Bitmap *dest, size_t src_start,
                size_t length, size_t dest_start);
int bitmap_show(const Bitmap *bitmap);
int bitmap_set_bit(Bitmap *bitmap, size_t num, int val);
int bitmap_clear_bit(Bitmap *bitmap, size_t num);
int bitmap_reset(Bitmap *bitmap);
int bitmap_delete(Bitmap *bitmap);
int bitmap_file_save(const Bitmap *bitmap, const char *fname);
int bitmap_file_load(Bitmap *bitmap, const char *fname);
int bitmap_concat_in_place(Bitmap *bitmap1, const Bitmap *bitmap2);

// Операции сравнения
int bitmap_compare(Bitmap *bitmap1, const Bitmap *bitmap2);
int bitmap_equal(Bitmap *bitmap1, const Bitmap *bitmap2);
int bitmap_strictly_greater(Bitmap *bitmap1, const Bitmap *bitmap2);
int bitmap_greater(Bitmap *bitmap1, const Bitmap *bitmap2);
int bitmap_strictly_less(Bitmap *bitmap1, const Bitmap *bitmap2);
int bitmap_less(Bitmap *bitmap1, const Bitmap *bitmap2);

#endif