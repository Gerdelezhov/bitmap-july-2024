#ifndef BITMAP_PLUGIN_H
#define BITMAP_PLUGIN_H

#include "bitmap.h"

typedef struct {
    int (*bitmap_and)(Bitmap *bitmap1, const Bitmap *bitmap2);
    int (*bitmap_or)(Bitmap *bitmap1, const Bitmap *bitmap2);
    int (*bitmap_xor)(Bitmap *bitmap1, const Bitmap *bitmap2);
    int (*bitmap_not)(Bitmap *bitmap);
} bitmap_plugin;

#endif  // BITMAP_PLUGIN_H
