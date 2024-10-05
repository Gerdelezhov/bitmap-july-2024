#include "../include/bitmap.h"
#include "../include/bitmap_plugin.h"

// Функция выполняет побитовое not на битмапе
int bitmap_not(Bitmap *bitmap) {
    if (bitmap == NULL || bitmap->bits == NULL) {
        return ERROR;
    }

    size_t bytes = bits_to_byte(bitmap->size);

    for (size_t i = 0; i < bytes; i++) {
        bitmap->bits[i] = ~bitmap->bits[i];
    }

    return SUCCESS;
}