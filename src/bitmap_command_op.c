#include "../include/bitmap.h"

/* Функция преобразует количество битов в количество байтов,
округляя результат вверх до ближайшего целого */
size_t bits_to_byte(size_t bits) {
    return (bits + (BYTE_SIZE - 1)) / BYTE_SIZE;
}

// Функция принимает указатель на bitmap и количество бит
int bitmap_create(Bitmap *bitmap, size_t bits) {
    if (bitmap == NULL) {
        return ERROR;
    }

    if (bits == 0) {
        return ERROR;
    }

    size_t adjusted_bits = (bits + BYTE_SIZE - 1) & ~(BYTE_SIZE - 1);
    bitmap->bits =
        (uint8_t *)calloc(adjusted_bits / BYTE_SIZE, sizeof(uint8_t));
    if (bitmap->bits == NULL) {
        return ERROR;
    }

    bitmap->size = adjusted_bits;
    return SUCCESS;
}

/* Функция принимает указатель на исходный битмап bitmap и на целевой, после
   чего заполняет целевой содержимым исходного. */
int bitmap_clone(const Bitmap *source, Bitmap *clone) {
    if (source == NULL || source->bits == NULL || source->size == 0 ||
        clone == NULL) {
        return ERROR;
    }

    if (bitmap_create(clone, source->size) == ERROR) {
        return ERROR;
    }

    size_t bytes_needed = bits_to_byte(source->size);
    memcpy(clone->bits, source->bits, bytes_needed);

    return SUCCESS;
}

/* Функция принимает указатель на bitmap и его конечный размер. После
 * чего увеличивает его размер с сохранение содержимого. */
int bitmap_increase(Bitmap *bitmap, size_t length) {
    Bitmap buff;
    bitmap_create(&buff, BYTE_SIZE);
    bitmap_clone(bitmap, &buff);
    bitmap_create(bitmap, length);
    size_t bytes_needed = bits_to_byte((&buff)->size);
    memcpy(bitmap->bits, (&buff)->bits, bytes_needed);
    return SUCCESS;
}

/* Функция принимает указатель на исходный битмап bitmap и на целевой, а так же
 * индекс начала копируемого отрезка и его длину. После чего копирует
 * содержимое отрезка в целевой bitmap.*/
int bitmap_copy(const Bitmap *source, Bitmap *dest, size_t src_start,
                size_t length, size_t dest_start) {
    if (source == NULL || dest == NULL || source->bits == NULL ||
        dest->bits == NULL) {
        return ERROR;
    }

    if (src_start > source->size || src_start + length > source->size ||
        dest_start > dest->size) {
        return ERROR;
    }

    if (dest_start + length > dest->size) {
        size_t new_size = bits_to_byte(length) * BYTE_SIZE + dest->size;
        bitmap_increase(dest, new_size);
    }

    for (size_t i = 0; i < length; ++i) {
        size_t src_index = src_start + i;
        size_t dest_index = dest_start + i;
        size_t src_byte = src_index / BYTE_SIZE;
        size_t src_bit = src_index % BYTE_SIZE;
        size_t dest_byte = dest_index / BYTE_SIZE;
        size_t dest_bit = dest_index % BYTE_SIZE;

        uint8_t bit_value = (source->bits[src_byte] >> src_bit) & 1;

        if (bit_value) {
            dest->bits[dest_byte] |= (1 << dest_bit);
        } else {
            dest->bits[dest_byte] &= ~(1 << dest_bit);
        }
    }
    return SUCCESS;
}

/* Функция принимает указатель на структуру bitmap. Выводит на экран содержимое
 bitmap справа налево. */
int bitmap_show(const Bitmap *bitmap) {
    if (!bitmap || !bitmap->bits || bitmap->size == 0) {
        printf("Bitmap пуст или неинициализирован.\n");
        perror("Bitmap пуст или неинициализирован.\n");
        return ERROR;
    }

    size_t bytes = bits_to_byte(bitmap->size);

    for (size_t i = bytes; i > 0; --i) {
        if (i == bytes) {
            size_t remaining_bits = bitmap->size % BYTE_SIZE;
            if (remaining_bits == 0) {
                remaining_bits = BYTE_SIZE;
            }

            for (size_t j = remaining_bits; j > 0; --j) {
                printf("%d", (bitmap->bits[i - 1] >> (j - 1)) & 1);
            }

            if (remaining_bits != BYTE_SIZE) {
                printf(" ");
            }
        } else {
            printf(
                " %d%d%d%d%d%d%d%d", (bitmap->bits[i - 1] >> 7) & 1,
                (bitmap->bits[i - 1] >> 6) & 1, (bitmap->bits[i - 1] >> 5) & 1,
                (bitmap->bits[i - 1] >> 4) & 1, (bitmap->bits[i - 1] >> 3) & 1,
                (bitmap->bits[i - 1] >> 2) & 1, (bitmap->bits[i - 1] >> 1) & 1,
                (bitmap->bits[i - 1] >> 0) & 1);
        }
    }
    printf("\n");
    return SUCCESS;
}

// Функция установки значения бита
int bitmap_set_bit(Bitmap *bitmap, size_t num, int val) {
    if (bitmap == NULL || bitmap->bits == NULL) {
        return ERROR;
    }

    if (num >= bitmap->size) {
        return ERROR;
    }

    if (val < 0 || val > 1) {
        return ERROR;
    }

    size_t byte_index = num / BYTE_SIZE;
    uint8_t bit_mask = 1 << (num % BYTE_SIZE);

    if (val) {
        bitmap->bits[byte_index] |= bit_mask;  // Установить бит в 1
    } else {
        bitmap->bits[byte_index] &= ~bit_mask;  // Установить бит в 0
    }

    return SUCCESS;
}

/*Функция сбрасывает бит по индексу*/
int bitmap_clear_bit(Bitmap *bitmap, size_t num) {
    if (bitmap == NULL || bitmap->bits == NULL) {
        return ERROR;
    }

    if (num >= bitmap->size) {
        return ERROR;
    }

    size_t byte_index = num / BYTE_SIZE;
    uint8_t bit_mask = 1 << (num % BYTE_SIZE);

    bitmap->bits[byte_index] &= ~bit_mask;  // Установка бита в 0

    return SUCCESS;
}
/* Функция принимает указатель на структуру bitmap. Зануляет все биты массива
 * bits. */
int bitmap_reset(Bitmap *bitmap) {
    if (bitmap == NULL || bitmap->bits == NULL) {
        return ERROR;
    }

    size_t bytes = bits_to_byte(bitmap->size);

    for (size_t i = 0; i < bytes; i++) {
        bitmap->bits[i] &= 0;
    }

    return SUCCESS;
}

/* Функция принимает указатель на структуру bitmap. Освобождает память из-под
битов и устанавливает размер 0. */
int bitmap_delete(Bitmap *bitmap) {
    if (bitmap == NULL) {
        return ERROR;
    }

    free(bitmap->bits);
    bitmap->size = 0;

    return SUCCESS;
}

/* Функция принимает указатель на  bitmap и имя файла. Функция создает или
открывает существующий файл с указанным именем и записавает в него size и bits.
*/
int bitmap_file_save(const Bitmap *bitmap, const char *fname) {
    if (bitmap == NULL || bitmap->bits == NULL || fname == NULL) {
        return ERROR;
    }

    FILE *file = fopen(fname, "wb");
    if (file == NULL) {
        return ERROR;
    }

    if (fwrite(&bitmap->size, sizeof(size_t), 1, file) != 1) {
        fclose(file);
        return ERROR;
    }

    size_t written = fwrite(bitmap->bits, sizeof(uint8_t), bitmap->size, file);
    if (written != bitmap->size) {
        fclose(file);
        return ERROR;
    }

    fclose(file);

    return SUCCESS;
}
/* Функция принимает указатель на bitmap и имя файла. Функция
открывает файл с указанным именем и загружает из него size и bits в переданную
структуру. Передовать можно как указатель на пустой битмап(в этом случае
создастся новый битмап с размером из файла), так и на уже созданный(в этом
случае битмап удалится, и создастся новый по размеру из файла)*/
int bitmap_file_load(Bitmap *bitmap, const char *fname) {
    if (bitmap == NULL || fname == NULL) {
        return ERROR;
    }
    if (bitmap->bits != NULL) {
        bitmap_delete(bitmap);
    }

    FILE *file = fopen(fname, "rb");
    if (file == NULL) {
        return ERROR;
    }
    size_t size;

    if (fread(&size, sizeof(size_t), 1, file) != 1) {
        fclose(file);
        return ERROR;
    }
    bitmap_create(bitmap, size);

    size_t bytesRead = fread(bitmap->bits, sizeof(uint8_t), bitmap->size, file);
    if (bytesRead != bitmap->size) {
        bitmap_delete(bitmap);
        fclose(file);
        return ERROR;
    }

    fclose(file);

    return SUCCESS;
}

/* Функция принимает 2 указателя на битмап.
Склеивает два битмапа с сохранением в bitmap1 */
int bitmap_concat_in_place(Bitmap *bitmap1, const Bitmap *bitmap2) {
    if (bitmap1 == NULL || bitmap2 == NULL) {
        return ERROR;
    }

    if (bitmap1->bits == NULL || bitmap2->bits == NULL) {
        return ERROR;
    }

    if (bitmap1->size == 0 || bitmap2->size == 0) {
        return ERROR;
    }

    size_t bitmap1_size = bitmap1->size;
    size_t concat_size = bitmap1->size + bitmap2->size;

    if (bitmap_increase(bitmap1, concat_size) == ERROR) {
        return ERROR;
    }

    if (bitmap_copy(bitmap2, bitmap1, 0, bitmap2->size, bitmap1_size) ==
        ERROR) {
        return ERROR;
    }

    return SUCCESS;
}