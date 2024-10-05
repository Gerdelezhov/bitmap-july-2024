#include "../include/bitmap.h"

/*Функция принимает указатели на два битмапа и сравнивает их. Функция возвращает
0 если битмапы равны, 1 если первый больше вторго, 2 если второй больше
первого, -1 если произошла ошибка*/
int bitmap_compare(Bitmap *bitmap1, const Bitmap *bitmap2) {
    if (bitmap1 == NULL || bitmap2 == NULL) {
        return ERROR;
    }

    if (bitmap1->bits == NULL || bitmap2->bits == NULL) {
        return ERROR;
    }

    size_t min_size =
        bitmap1->size < bitmap2->size ? bitmap1->size : bitmap2->size;

    if (bitmap1->size > bitmap2->size) {
        return 1;
    }
    if (bitmap1->size < bitmap2->size) {
        return 2;
    }

    int comp_res = memcmp(bitmap1->bits, bitmap2->bits, min_size);

    if (comp_res != 0) {
        return (comp_res = 1) ? 1 : 2;
    }

    return 0;
}
/*Функция принимает указатели на два битмапа и проверяет на равенство. Функция
возвращает 1 если битмапы равны, 0 если битмапы не равны, -1 если произошла
ошибка*/
int bitmap_equal(Bitmap *bitmap1, const Bitmap *bitmap2) {
    if (bitmap1 == NULL || bitmap2 == NULL) {
        return ERROR;
    }

    if (bitmap1->bits == NULL || bitmap2->bits == NULL) {
        return ERROR;
    }

    if (bitmap1->size != bitmap2->size) {
        return 0;
    }

    int comp_res = memcmp(bitmap1->bits, bitmap2->bits, bitmap1->size);

    if (comp_res != 0) {
        return 0;
    }

    return 1;
}

/*Функция принимает указатели на два битмапа и проверяет больше ли первый чем
второй. Функция возвращает 1 если первый больше второго, 0если первый меньше
или равен второму, -1 если произошла ошибка*/
int bitmap_strictly_greater(Bitmap *bitmap1, const Bitmap *bitmap2) {
    if (bitmap1 == NULL || bitmap2 == NULL) {
        return ERROR;
    }

    if (bitmap1->bits == NULL || bitmap2->bits == NULL) {
        return ERROR;
    }

    size_t min_size =
        bitmap1->size < bitmap2->size ? bitmap1->size : bitmap2->size;

    if (bitmap1->size > bitmap2->size) {
        return 1;
    }

    int comp_res = memcmp(bitmap1->bits, bitmap2->bits, min_size);

    if (comp_res == 1) {
        return 1;
    }
    return 0;
}

/*Функция принимает указатели на два битмапа и проверяет больше ли первый чем
второй или они равны. Функция возвращает 1 если первый больше или равен второму,
0 если первый меньше второго, -1 если произошла ошибка*/
int bitmap_greater(Bitmap *bitmap1, const Bitmap *bitmap2) {
    if (bitmap1 == NULL || bitmap2 == NULL) {
        return ERROR;
    }

    if (bitmap1->bits == NULL || bitmap2->bits == NULL) {
        return ERROR;
    }
    int size_equal = 0;

    size_t min_size =
        bitmap1->size < bitmap2->size ? bitmap1->size : bitmap2->size;

    if (bitmap1->size > bitmap2->size) {
        return 1;
    }
    if (bitmap1->size == bitmap2->size) {
        size_equal = 1;
    }

    int comp_res = memcmp(bitmap1->bits, bitmap2->bits, min_size);

    if (((comp_res == 1) || ((comp_res == 0) && (size_equal == 1)))) {
        return 1;
    }
    return 0;
}

/*Функция принимает указатели на два битмапа и проверяет меньше ли первый чем
второй. Функция возвращает 1 если первый меньше второго, 0 если первый
больше или равен второму, -1 если произошла ошибка*/
int bitmap_strictly_less(Bitmap *bitmap1, const Bitmap *bitmap2) {
    if (bitmap1 == NULL || bitmap2 == NULL) {
        return ERROR;
    }

    if (bitmap1->bits == NULL || bitmap2->bits == NULL) {
        return ERROR;
    }
    if (bitmap1->size < bitmap2->size) {
        return 1;
    }

    size_t min_size =
        bitmap1->size < bitmap2->size ? bitmap1->size : bitmap2->size;

    int comp_res = memcmp(bitmap1->bits, bitmap2->bits, min_size);

    if (comp_res < 0) {
        return 1;
    }
    return 0;
}

/*Функция принимает указатели на два битмапа и проверяет меньше ли первый чем
второй или они равны. Функция возвращает 1 если первый меньше или равен
второму, 0 если первый больше второго, -1 если произошла ошибка*/
int bitmap_less(Bitmap *bitmap1, const Bitmap *bitmap2) {
    if (bitmap1 == NULL || bitmap2 == NULL) {
        return ERROR;
    }

    if (bitmap1->bits == NULL || bitmap2->bits == NULL) {
        return ERROR;
    }
    int size_equal = 0;
    if (bitmap1->size < bitmap2->size) {
        return 1;
    }

    size_t min_size =
        bitmap1->size < bitmap2->size ? bitmap1->size : bitmap2->size;

    if (bitmap1->size == bitmap2->size) {
        size_equal = 1;
    }

    int comp_res = memcmp(bitmap1->bits, bitmap2->bits, min_size);

    if (((comp_res < 0) || ((comp_res == 0) && (size_equal == 1)))) {
        return 1;
    }
    return 0;
}