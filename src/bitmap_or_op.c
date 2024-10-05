#include "../include/bitmap.h"
#include "../include/bitmap_plugin.h"

// Функция выполняет побитовое or на пересекающихся частях двух битмапов
int bitmap_or(Bitmap *bitmap1, const Bitmap *bitmap2) {
    if (bitmap1 == NULL || bitmap2 == NULL) {
        return ERROR;
    }

    if (bitmap1->bits == NULL || bitmap2->bits == NULL) {
        return ERROR;
    }

    size_t min_size =
        bitmap1->size < bitmap2->size ? bitmap1->size : bitmap2->size;
    size_t bytes = bits_to_byte(min_size);

    for (size_t i = 0; i < bytes; i++) {
        bitmap1->bits[i] |= bitmap2->bits[i];
    }

    return SUCCESS;
}