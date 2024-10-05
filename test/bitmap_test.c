#include "../include/bitmap.h"

#include <check.h>

#include "../include/bitmap_plugin.h"

#define PLUGIN_PATH "src/"

//ТЕСТЫ ДЛЯ ПОБИТОВЫХ ОПЕРАЦИЙ

// bitmap_and - проверка работы
START_TEST(bitmap_and_1) {
    void *handle;
    bitmap_plugin ops;

    handle = dlopen(PLUGIN_PATH "bitmap_and_op.so", RTLD_LAZY);
    if (!handle) {
        perror("Ошибка загрузки плагина\n");
        exit(EXIT_FAILURE);
    }
    ops.bitmap_and = dlsym(handle, "bitmap_and");
    if (dlerror() != NULL) {
        perror("Ошибка загрузки плагина\n");
        dlclose(handle);
        exit(EXIT_FAILURE);
    }

    Bitmap bitmap1;
    Bitmap bitmap2;

    ck_assert_int_eq(bitmap_create(&bitmap1, 16), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 16), SUCCESS);

    // 10101010 10101010
    bitmap_set_bit(&bitmap1, 1, 1);
    bitmap_set_bit(&bitmap1, 3, 1);
    bitmap_set_bit(&bitmap1, 5, 1);
    bitmap_set_bit(&bitmap1, 7, 1);
    bitmap_set_bit(&bitmap1, 9, 1);
    bitmap_set_bit(&bitmap1, 11, 1);
    bitmap_set_bit(&bitmap1, 13, 1);
    bitmap_set_bit(&bitmap1, 15, 1);

    // 11001100 11001100
    bitmap_set_bit(&bitmap2, 2, 1);
    bitmap_set_bit(&bitmap2, 3, 1);
    bitmap_set_bit(&bitmap2, 6, 1);
    bitmap_set_bit(&bitmap2, 7, 1);
    bitmap_set_bit(&bitmap2, 10, 1);
    bitmap_set_bit(&bitmap2, 11, 1);
    bitmap_set_bit(&bitmap2, 14, 1);
    bitmap_set_bit(&bitmap2, 15, 1);

    ck_assert_int_eq(ops.bitmap_and(&bitmap1, &bitmap2), SUCCESS);

    // Ожидаем: 10001000 10001000
    ck_assert_int_eq(bitmap1.bits[0], 0x88);
    ck_assert_int_eq(bitmap1.bits[1], 0x88);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

// bitmap_and - тест проверки на null
START_TEST(bitmap_and_2) {
    void *handle;
    bitmap_plugin ops;

    handle = dlopen(PLUGIN_PATH "bitmap_and_op.so", RTLD_LAZY);
    if (!handle) {
        perror("Ошибка загрузки плагина\n");
        exit(EXIT_FAILURE);
    }
    ops.bitmap_and = dlsym(handle, "bitmap_and");
    if (dlerror() != NULL) {
        perror("Ошибка загрузки плагина\n");
        dlclose(handle);
        exit(EXIT_FAILURE);
    }

    Bitmap bitmap1;
    Bitmap bitmap2;

    ck_assert_int_eq(ops.bitmap_and(NULL, &bitmap2), ERROR);
    ck_assert_int_eq(ops.bitmap_and(&bitmap1, NULL), ERROR);

    bitmap1.bits = NULL;
    bitmap1.size = 16;
    ck_assert_int_eq(ops.bitmap_and(&bitmap1, &bitmap2), ERROR);

    bitmap1.bits = (uint8_t *)malloc(2);
    bitmap2.bits = NULL;
    bitmap2.size = 16;
    ck_assert_int_eq(ops.bitmap_and(&bitmap1, &bitmap2), ERROR);

    free(bitmap1.bits);
}
END_TEST

// bitmap_and - проверка работы на битмапах разной длинны
START_TEST(bitmap_and_3) {
    void *handle;
    bitmap_plugin ops;

    handle = dlopen(PLUGIN_PATH "bitmap_and_op.so", RTLD_LAZY);
    if (!handle) {
        perror("Ошибка загрузки плагина\n");
        exit(EXIT_FAILURE);
    }
    ops.bitmap_and = dlsym(handle, "bitmap_and");
    if (dlerror() != NULL) {
        perror("Ошибка загрузки плагина\n");
        dlclose(handle);
        exit(EXIT_FAILURE);
    }
    Bitmap bitmap1;
    Bitmap bitmap2;

    ck_assert_int_eq(bitmap_create(&bitmap1, 32), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 16), SUCCESS);

    // 10101010 10101010
    bitmap_set_bit(&bitmap1, 1, 1);
    bitmap_set_bit(&bitmap1, 3, 1);
    bitmap_set_bit(&bitmap1, 5, 1);
    bitmap_set_bit(&bitmap1, 7, 1);
    bitmap_set_bit(&bitmap1, 9, 1);
    bitmap_set_bit(&bitmap1, 11, 1);
    bitmap_set_bit(&bitmap1, 13, 1);
    bitmap_set_bit(&bitmap1, 15, 1);

    // 11001100 11001100
    bitmap_set_bit(&bitmap2, 2, 1);
    bitmap_set_bit(&bitmap2, 3, 1);
    bitmap_set_bit(&bitmap2, 6, 1);
    bitmap_set_bit(&bitmap2, 7, 1);
    bitmap_set_bit(&bitmap2, 10, 1);
    bitmap_set_bit(&bitmap2, 11, 1);
    bitmap_set_bit(&bitmap2, 14, 1);
    bitmap_set_bit(&bitmap2, 15, 1);

    ck_assert_int_eq(ops.bitmap_and(&bitmap1, &bitmap2), SUCCESS);

    // Ожидаем: 10001000 10001000
    ck_assert_int_eq(bitmap1.bits[0], 0x88);
    ck_assert_int_eq(bitmap1.bits[1], 0x88);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

// bitmap_and - проверка работы на битмапах разной длинны
START_TEST(bitmap_and_4) {
    void *handle;
    bitmap_plugin ops;

    handle = dlopen(PLUGIN_PATH "bitmap_and_op.so", RTLD_LAZY);
    if (!handle) {
        perror("Ошибка загрузки плагина\n");
        exit(EXIT_FAILURE);
    }
    ops.bitmap_and = dlsym(handle, "bitmap_and");
    if (dlerror() != NULL) {
        perror("Ошибка загрузки плагина\n");
        dlclose(handle);
        exit(EXIT_FAILURE);
    }

    Bitmap bitmap1;
    Bitmap bitmap2;

    ck_assert_int_eq(bitmap_create(&bitmap1, 16), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 32), SUCCESS);

    // 10101010 10101010
    bitmap_set_bit(&bitmap1, 1, 1);
    bitmap_set_bit(&bitmap1, 3, 1);
    bitmap_set_bit(&bitmap1, 5, 1);
    bitmap_set_bit(&bitmap1, 7, 1);
    bitmap_set_bit(&bitmap1, 9, 1);
    bitmap_set_bit(&bitmap1, 11, 1);
    bitmap_set_bit(&bitmap1, 13, 1);
    bitmap_set_bit(&bitmap1, 15, 1);

    // 11001100 11001100
    bitmap_set_bit(&bitmap2, 2, 1);
    bitmap_set_bit(&bitmap2, 3, 1);
    bitmap_set_bit(&bitmap2, 6, 1);
    bitmap_set_bit(&bitmap2, 7, 1);
    bitmap_set_bit(&bitmap2, 10, 1);
    bitmap_set_bit(&bitmap2, 11, 1);
    bitmap_set_bit(&bitmap2, 14, 1);
    bitmap_set_bit(&bitmap2, 15, 1);

    ck_assert_int_eq(ops.bitmap_and(&bitmap1, &bitmap2), SUCCESS);

    // Ожидаем: 10001000 10001000
    ck_assert_int_eq(bitmap1.bits[0], 0x88);
    ck_assert_int_eq(bitmap1.bits[1], 0x88);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

// bitmap_and - проверка работы на нулевых битмапах
START_TEST(bitmap_and_5) {
    void *handle;
    bitmap_plugin ops;

    handle = dlopen(PLUGIN_PATH "bitmap_and_op.so", RTLD_LAZY);
    if (!handle) {
        perror("Ошибка загрузки плагина\n");
        exit(EXIT_FAILURE);
    }
    ops.bitmap_and = dlsym(handle, "bitmap_and");
    if (dlerror() != NULL) {
        perror("Ошибка загрузки плагина\n");
        dlclose(handle);
        exit(EXIT_FAILURE);
    }

    Bitmap bitmap1;
    Bitmap bitmap2;

    ck_assert_int_eq(bitmap_create(&bitmap1, 16), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 16), SUCCESS);

    ck_assert_int_eq(ops.bitmap_and(&bitmap1, &bitmap2), SUCCESS);

    // Ожидаем: 00000000 00000000
    ck_assert_int_eq(bitmap1.bits[0], 0x00);
    ck_assert_int_eq(bitmap1.bits[1], 0x00);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

// bitmap_or - проверка работы
START_TEST(bitmap_or_1) {
    void *handle;
    bitmap_plugin ops;

    handle = dlopen(PLUGIN_PATH "bitmap_or_op.so", RTLD_LAZY);
    if (!handle) {
        perror("Ошибка загрузки плагина\n");
        exit(EXIT_FAILURE);
    }
    ops.bitmap_or = dlsym(handle, "bitmap_or");
    if (dlerror() != NULL) {
        perror("Ошибка загрузки плагина\n");
        dlclose(handle);
        exit(EXIT_FAILURE);
    }
    Bitmap bitmap1;
    Bitmap bitmap2;

    ck_assert_int_eq(bitmap_create(&bitmap1, 16), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 16), SUCCESS);

    // 10101010 10101010
    bitmap_set_bit(&bitmap1, 1, 1);
    bitmap_set_bit(&bitmap1, 3, 1);
    bitmap_set_bit(&bitmap1, 5, 1);
    bitmap_set_bit(&bitmap1, 7, 1);
    bitmap_set_bit(&bitmap1, 9, 1);
    bitmap_set_bit(&bitmap1, 11, 1);
    bitmap_set_bit(&bitmap1, 13, 1);
    bitmap_set_bit(&bitmap1, 15, 1);

    // 11001100 11001100
    bitmap_set_bit(&bitmap2, 2, 1);
    bitmap_set_bit(&bitmap2, 3, 1);
    bitmap_set_bit(&bitmap2, 6, 1);
    bitmap_set_bit(&bitmap2, 7, 1);
    bitmap_set_bit(&bitmap2, 10, 1);
    bitmap_set_bit(&bitmap2, 11, 1);
    bitmap_set_bit(&bitmap2, 14, 1);
    bitmap_set_bit(&bitmap2, 15, 1);

    ck_assert_int_eq(ops.bitmap_or(&bitmap1, &bitmap2), SUCCESS);

    // Ожидаем: 11101110 11101110
    ck_assert_int_eq(bitmap1.bits[0], 0xEE);
    ck_assert_int_eq(bitmap1.bits[1], 0xEE);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

// bitmap_or - тест проверки на null
START_TEST(bitmap_or_2) {
    void *handle;
    bitmap_plugin ops;

    handle = dlopen(PLUGIN_PATH "bitmap_or_op.so", RTLD_LAZY);
    if (!handle) {
        perror("Ошибка загрузки плагина\n");
        exit(EXIT_FAILURE);
    }
    ops.bitmap_or = dlsym(handle, "bitmap_or");
    if (dlerror() != NULL) {
        perror("Ошибка загрузки плагина\n");
        dlclose(handle);
        exit(EXIT_FAILURE);
    }
    Bitmap bitmap1;
    Bitmap bitmap2;
    ck_assert_int_eq(ops.bitmap_or(NULL, &bitmap2), ERROR);
    ck_assert_int_eq(ops.bitmap_or(&bitmap1, NULL), ERROR);
    bitmap1.bits = NULL;
    bitmap1.size = 16;
    ck_assert_int_eq(ops.bitmap_or(&bitmap1, &bitmap2), ERROR);
    bitmap2.bits = NULL;
    bitmap2.size = 16;
    bitmap1.bits = (uint8_t *)malloc(2);
    ck_assert_int_eq(ops.bitmap_or(&bitmap1, &bitmap2), ERROR);
    free(bitmap1.bits);
}
END_TEST

// bitmap_or - проверка работы на битмапах разной длинны
START_TEST(bitmap_or_3) {
    void *handle;
    bitmap_plugin ops;

    handle = dlopen(PLUGIN_PATH "bitmap_or_op.so", RTLD_LAZY);
    if (!handle) {
        perror("Ошибка загрузки плагина\n");
        exit(EXIT_FAILURE);
    }
    ops.bitmap_or = dlsym(handle, "bitmap_or");
    if (dlerror() != NULL) {
        perror("Ошибка загрузки плагина\n");
        dlclose(handle);
        exit(EXIT_FAILURE);
    }
    Bitmap bitmap1;
    Bitmap bitmap2;

    ck_assert_int_eq(bitmap_create(&bitmap1, 32), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 16), SUCCESS);

    // 10101010 10101010
    bitmap_set_bit(&bitmap1, 1, 1);
    bitmap_set_bit(&bitmap1, 3, 1);
    bitmap_set_bit(&bitmap1, 5, 1);
    bitmap_set_bit(&bitmap1, 7, 1);
    bitmap_set_bit(&bitmap1, 9, 1);
    bitmap_set_bit(&bitmap1, 11, 1);
    bitmap_set_bit(&bitmap1, 13, 1);
    bitmap_set_bit(&bitmap1, 15, 1);

    // 11001100 11001100
    bitmap_set_bit(&bitmap2, 2, 1);
    bitmap_set_bit(&bitmap2, 3, 1);
    bitmap_set_bit(&bitmap2, 6, 1);
    bitmap_set_bit(&bitmap2, 7, 1);
    bitmap_set_bit(&bitmap2, 10, 1);
    bitmap_set_bit(&bitmap2, 11, 1);
    bitmap_set_bit(&bitmap2, 14, 1);
    bitmap_set_bit(&bitmap2, 15, 1);

    ck_assert_int_eq(ops.bitmap_or(&bitmap1, &bitmap2), SUCCESS);

    // Ожидаем: 11101110 11101110
    ck_assert_int_eq(bitmap1.bits[0], 0xEE);
    ck_assert_int_eq(bitmap1.bits[1], 0xEE);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

// bitmap_or - проверка работы на битмапах разной длинны
START_TEST(bitmap_or_4) {
    void *handle;
    bitmap_plugin ops;

    handle = dlopen(PLUGIN_PATH "bitmap_or_op.so", RTLD_LAZY);
    if (!handle) {
        perror("Ошибка загрузки плагина\n");
        exit(EXIT_FAILURE);
    }
    ops.bitmap_or = dlsym(handle, "bitmap_or");
    if (dlerror() != NULL) {
        perror("Ошибка загрузки плагина\n");
        dlclose(handle);
        exit(EXIT_FAILURE);
    }
    Bitmap bitmap1;
    Bitmap bitmap2;

    ck_assert_int_eq(bitmap_create(&bitmap1, 16), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 32), SUCCESS);

    // 10101010 10101010
    bitmap_set_bit(&bitmap1, 1, 1);
    bitmap_set_bit(&bitmap1, 3, 1);
    bitmap_set_bit(&bitmap1, 5, 1);
    bitmap_set_bit(&bitmap1, 7, 1);
    bitmap_set_bit(&bitmap1, 9, 1);
    bitmap_set_bit(&bitmap1, 11, 1);
    bitmap_set_bit(&bitmap1, 13, 1);
    bitmap_set_bit(&bitmap1, 15, 1);

    // 11001100 11001100
    bitmap_set_bit(&bitmap2, 2, 1);
    bitmap_set_bit(&bitmap2, 3, 1);
    bitmap_set_bit(&bitmap2, 6, 1);
    bitmap_set_bit(&bitmap2, 7, 1);
    bitmap_set_bit(&bitmap2, 10, 1);
    bitmap_set_bit(&bitmap2, 11, 1);
    bitmap_set_bit(&bitmap2, 14, 1);
    bitmap_set_bit(&bitmap2, 15, 1);

    ck_assert_int_eq(ops.bitmap_or(&bitmap1, &bitmap2), SUCCESS);

    // Ожидаем: 11101110 11101110
    ck_assert_int_eq(bitmap1.bits[0], 0xEE);
    ck_assert_int_eq(bitmap1.bits[1], 0xEE);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

// bitmap_or - проверка работы на нулевых битмапах
START_TEST(bitmap_or_5) {
    void *handle;
    bitmap_plugin ops;

    handle = dlopen(PLUGIN_PATH "bitmap_or_op.so", RTLD_LAZY);
    if (!handle) {
        perror("Ошибка загрузки плагина\n");
        exit(EXIT_FAILURE);
    }
    ops.bitmap_or = dlsym(handle, "bitmap_or");
    if (dlerror() != NULL) {
        perror("Ошибка загрузки плагина\n");
        dlclose(handle);
        exit(EXIT_FAILURE);
    }
    Bitmap bitmap1;
    Bitmap bitmap2;

    ck_assert_int_eq(bitmap_create(&bitmap1, 16), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 16), SUCCESS);

    ck_assert_int_eq(ops.bitmap_or(&bitmap1, &bitmap2), SUCCESS);

    // Ожидаем: 00000000 00000000
    ck_assert_int_eq(bitmap1.bits[0], 0x00);
    ck_assert_int_eq(bitmap1.bits[1], 0x00);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

// bitmap_xor - проверка работы
START_TEST(bitmap_xor_1) {
    void *handle;
    bitmap_plugin ops;

    handle = dlopen(PLUGIN_PATH "bitmap_xor_op.so", RTLD_LAZY);
    if (!handle) {
        perror("Ошибка загрузки плагина\n");
        exit(EXIT_FAILURE);
    }
    ops.bitmap_xor = dlsym(handle, "bitmap_xor");
    if (dlerror() != NULL) {
        perror("Ошибка загрузки плагина\n");
        dlclose(handle);
        exit(EXIT_FAILURE);
    }
    Bitmap bitmap1;
    Bitmap bitmap2;

    ck_assert_int_eq(bitmap_create(&bitmap1, 16), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 16), SUCCESS);

    // 10101010 10101010
    bitmap_set_bit(&bitmap1, 1, 1);
    bitmap_set_bit(&bitmap1, 3, 1);
    bitmap_set_bit(&bitmap1, 5, 1);
    bitmap_set_bit(&bitmap1, 7, 1);
    bitmap_set_bit(&bitmap1, 9, 1);
    bitmap_set_bit(&bitmap1, 11, 1);
    bitmap_set_bit(&bitmap1, 13, 1);
    bitmap_set_bit(&bitmap1, 15, 1);

    // 11001100 11001100
    bitmap_set_bit(&bitmap2, 2, 1);
    bitmap_set_bit(&bitmap2, 3, 1);
    bitmap_set_bit(&bitmap2, 6, 1);
    bitmap_set_bit(&bitmap2, 7, 1);
    bitmap_set_bit(&bitmap2, 10, 1);
    bitmap_set_bit(&bitmap2, 11, 1);
    bitmap_set_bit(&bitmap2, 14, 1);
    bitmap_set_bit(&bitmap2, 15, 1);

    ck_assert_int_eq(ops.bitmap_xor(&bitmap1, &bitmap2), SUCCESS);

    // Ожидаем: 01100110 01100110
    ck_assert_int_eq(bitmap1.bits[0], 0x66);
    ck_assert_int_eq(bitmap1.bits[1], 0x66);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

// bitmap_xor - тест проверки на null
START_TEST(bitmap_xor_2) {
    void *handle;
    bitmap_plugin ops;

    handle = dlopen(PLUGIN_PATH "bitmap_xor_op.so", RTLD_LAZY);
    if (!handle) {
        perror("Ошибка загрузки плагина\n");
        exit(EXIT_FAILURE);
    }
    ops.bitmap_xor = dlsym(handle, "bitmap_xor");
    if (dlerror() != NULL) {
        perror("Ошибка загрузки плагина\n");
        dlclose(handle);
        exit(EXIT_FAILURE);
    }
    Bitmap bitmap1;
    Bitmap bitmap2;
    ck_assert_int_eq(ops.bitmap_xor(NULL, &bitmap2), ERROR);
    ck_assert_int_eq(ops.bitmap_xor(&bitmap1, NULL), ERROR);
    bitmap1.bits = NULL;
    bitmap1.size = 16;
    ck_assert_int_eq(ops.bitmap_xor(&bitmap1, &bitmap2), ERROR);
    bitmap2.bits = NULL;
    bitmap2.size = 16;
    bitmap1.bits = (uint8_t *)malloc(2);
    ck_assert_int_eq(ops.bitmap_xor(&bitmap1, &bitmap2), ERROR);
    free(bitmap1.bits);
}
END_TEST

// bitmap_xor - проверка работы на битмапах разной длинны
START_TEST(bitmap_xor_3) {
    void *handle;
    bitmap_plugin ops;

    handle = dlopen(PLUGIN_PATH "bitmap_xor_op.so", RTLD_LAZY);
    if (!handle) {
        perror("Ошибка загрузки плагина\n");
        exit(EXIT_FAILURE);
    }
    ops.bitmap_xor = dlsym(handle, "bitmap_xor");
    if (dlerror() != NULL) {
        perror("Ошибка загрузки плагина\n");
        dlclose(handle);
        exit(EXIT_FAILURE);
    }
    Bitmap bitmap1;
    Bitmap bitmap2;

    ck_assert_int_eq(bitmap_create(&bitmap1, 32), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 16), SUCCESS);

    // 10101010 10101010
    bitmap_set_bit(&bitmap1, 1, 1);
    bitmap_set_bit(&bitmap1, 3, 1);
    bitmap_set_bit(&bitmap1, 5, 1);
    bitmap_set_bit(&bitmap1, 7, 1);
    bitmap_set_bit(&bitmap1, 9, 1);
    bitmap_set_bit(&bitmap1, 11, 1);
    bitmap_set_bit(&bitmap1, 13, 1);
    bitmap_set_bit(&bitmap1, 15, 1);

    // 11001100 11001100
    bitmap_set_bit(&bitmap2, 2, 1);
    bitmap_set_bit(&bitmap2, 3, 1);
    bitmap_set_bit(&bitmap2, 6, 1);
    bitmap_set_bit(&bitmap2, 7, 1);
    bitmap_set_bit(&bitmap2, 10, 1);
    bitmap_set_bit(&bitmap2, 11, 1);
    bitmap_set_bit(&bitmap2, 14, 1);
    bitmap_set_bit(&bitmap2, 15, 1);

    ck_assert_int_eq(ops.bitmap_xor(&bitmap1, &bitmap2), SUCCESS);

    // Ожидаем: 01100110 01100110
    ck_assert_int_eq(bitmap1.bits[0], 0x66);
    ck_assert_int_eq(bitmap1.bits[1], 0x66);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

// bitmap_xor - проверка работы на битмапах разной длинны
START_TEST(bitmap_xor_4) {
    void *handle;
    bitmap_plugin ops;

    handle = dlopen(PLUGIN_PATH "bitmap_xor_op.so", RTLD_LAZY);
    if (!handle) {
        perror("Ошибка загрузки плагина\n");
        exit(EXIT_FAILURE);
    }
    ops.bitmap_xor = dlsym(handle, "bitmap_xor");
    if (dlerror() != NULL) {
        perror("Ошибка загрузки плагина\n");
        dlclose(handle);
        exit(EXIT_FAILURE);
    }
    Bitmap bitmap1;
    Bitmap bitmap2;

    ck_assert_int_eq(bitmap_create(&bitmap1, 16), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 32), SUCCESS);

    // 10101010 10101010
    bitmap_set_bit(&bitmap1, 1, 1);
    bitmap_set_bit(&bitmap1, 3, 1);
    bitmap_set_bit(&bitmap1, 5, 1);
    bitmap_set_bit(&bitmap1, 7, 1);
    bitmap_set_bit(&bitmap1, 9, 1);
    bitmap_set_bit(&bitmap1, 11, 1);
    bitmap_set_bit(&bitmap1, 13, 1);
    bitmap_set_bit(&bitmap1, 15, 1);

    // 11001100 11001100
    bitmap_set_bit(&bitmap2, 2, 1);
    bitmap_set_bit(&bitmap2, 3, 1);
    bitmap_set_bit(&bitmap2, 6, 1);
    bitmap_set_bit(&bitmap2, 7, 1);
    bitmap_set_bit(&bitmap2, 10, 1);
    bitmap_set_bit(&bitmap2, 11, 1);
    bitmap_set_bit(&bitmap2, 14, 1);
    bitmap_set_bit(&bitmap2, 15, 1);

    ck_assert_int_eq(ops.bitmap_xor(&bitmap1, &bitmap2), SUCCESS);

    // Ожидаем: 01100110 01100110
    ck_assert_int_eq(bitmap1.bits[0], 0x66);
    ck_assert_int_eq(bitmap1.bits[1], 0x66);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

// bitmap_xor - проверка работы на нулевых битмапах
START_TEST(bitmap_xor_5) {
    void *handle;
    bitmap_plugin ops;

    handle = dlopen(PLUGIN_PATH "bitmap_xor_op.so", RTLD_LAZY);
    if (!handle) {
        perror("Ошибка загрузки плагина\n");
        exit(EXIT_FAILURE);
    }
    ops.bitmap_xor = dlsym(handle, "bitmap_xor");
    if (dlerror() != NULL) {
        perror("Ошибка загрузки плагина\n");
        dlclose(handle);
        exit(EXIT_FAILURE);
    }
    Bitmap bitmap1;
    Bitmap bitmap2;

    ck_assert_int_eq(bitmap_create(&bitmap1, 16), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 16), SUCCESS);

    ck_assert_int_eq(ops.bitmap_xor(&bitmap1, &bitmap2), SUCCESS);

    // Ожидаем: 00000000 00000000
    ck_assert_int_eq(bitmap1.bits[0], 0x00);
    ck_assert_int_eq(bitmap1.bits[1], 0x00);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

// bitmap_not - проверка работы
START_TEST(bitmap_not_1) {
    void *handle;
    bitmap_plugin ops;

    handle = dlopen(PLUGIN_PATH "bitmap_not_op.so", RTLD_LAZY);
    if (!handle) {
        perror("Ошибка загрузки плагина\n");
        exit(EXIT_FAILURE);
    }
    ops.bitmap_not = dlsym(handle, "bitmap_not");
    if (dlerror() != NULL) {
        perror("Ошибка загрузки плагина\n");
        dlclose(handle);
        exit(EXIT_FAILURE);
    }
    Bitmap bitmap1;

    ck_assert_int_eq(bitmap_create(&bitmap1, 16), SUCCESS);

    // 10101010 10101010
    bitmap_set_bit(&bitmap1, 1, 1);
    bitmap_set_bit(&bitmap1, 3, 1);
    bitmap_set_bit(&bitmap1, 5, 1);
    bitmap_set_bit(&bitmap1, 7, 1);
    bitmap_set_bit(&bitmap1, 9, 1);
    bitmap_set_bit(&bitmap1, 11, 1);
    bitmap_set_bit(&bitmap1, 13, 1);
    bitmap_set_bit(&bitmap1, 15, 1);

    ck_assert_int_eq(ops.bitmap_not(&bitmap1), SUCCESS);

    // Ожидаем: 01010101 01010101
    ck_assert_int_eq(bitmap1.bits[0], 0x55);
    ck_assert_int_eq(bitmap1.bits[1], 0x55);

    bitmap_delete(&bitmap1);
}
END_TEST

// bitmap_not - тест проверки на null
START_TEST(bitmap_not_2) {
    void *handle;
    bitmap_plugin ops;

    handle = dlopen(PLUGIN_PATH "bitmap_not_op.so", RTLD_LAZY);
    if (!handle) {
        perror("Ошибка загрузки плагина\n");
        exit(EXIT_FAILURE);
    }
    ops.bitmap_not = dlsym(handle, "bitmap_not");
    if (dlerror() != NULL) {
        perror("Ошибка загрузки плагина\n");
        dlclose(handle);
        exit(EXIT_FAILURE);
    }
    Bitmap bitmap;
    ck_assert_int_eq(ops.bitmap_not(NULL), ERROR);
    bitmap.bits = NULL;
    ck_assert_int_eq(ops.bitmap_not(&bitmap), ERROR);
}
END_TEST

// ТЕСТЫ ДЛЯ КОМАНДНЫХ ОПЕРАЦИЙ

// bits_to_byte
START_TEST(bitmap_bits_to_byte_1) { ck_assert_int_eq(bits_to_byte(0), 0); }
END_TEST

START_TEST(bitmap_bits_to_byte_2) { ck_assert_int_eq(bits_to_byte(1), 1); }
END_TEST

START_TEST(bitmap_bits_to_byte_3) { ck_assert_int_eq(bits_to_byte(8), 1); }
END_TEST

START_TEST(bitmap_bits_to_byte_4) { ck_assert_int_eq(bits_to_byte(9), 2); }
END_TEST

// bitmap_create
START_TEST(bitmap_create_1) {
    Bitmap bitmap;
    ck_assert_int_eq(bitmap_create(NULL, 8), ERROR);
}
END_TEST

START_TEST(bitmap_create_2) {
    Bitmap bitmap;
    ck_assert_int_eq(bitmap_create(&bitmap, 0), ERROR);
}
END_TEST

START_TEST(bitmap_create_3) {
    Bitmap bitmap;
    ck_assert_int_eq(bitmap_create(&bitmap, 8), SUCCESS);
    bitmap_delete(&bitmap);
}
END_TEST

// bitmap_clone
START_TEST(bitmap_clone_1) {
    Bitmap source;
    Bitmap clone;
    ck_assert_int_eq(bitmap_clone(NULL, &clone), ERROR);
}
END_TEST

START_TEST(bitmap_clone_2) {
    Bitmap source;
    Bitmap clone;
    ck_assert_int_eq(bitmap_clone(&source, NULL), ERROR);
}
END_TEST

START_TEST(bitmap_clone_3) {
    Bitmap source;
    Bitmap clone;
    ck_assert_int_eq(bitmap_create(&source, 8), SUCCESS);
    source.bits[0] = 0xFF;
    ck_assert_int_eq(bitmap_clone(&source, &clone), SUCCESS);
    ck_assert_int_eq(clone.bits[0], 0xFF);
    bitmap_delete(&source);
    bitmap_delete(&clone);
}
END_TEST

// bitmap_show
START_TEST(bitmap_show_1) {
    Bitmap bitmap;
    ck_assert_int_eq(bitmap_show(NULL), ERROR);
}
END_TEST

START_TEST(bitmap_show_2) {
    Bitmap bitmap;
    bitmap.size = 0;
    ck_assert_int_eq(bitmap_show(&bitmap), ERROR);
}
END_TEST

START_TEST(bitmap_show_3) {
    Bitmap bitmap;
    ck_assert_int_eq(bitmap_create(&bitmap, 8), SUCCESS);
    bitmap.bits[0] = 0xFF;
    ck_assert_int_eq(bitmap_show(&bitmap), SUCCESS);
    bitmap_delete(&bitmap);
}
END_TEST

// bitmap_set_bit
START_TEST(bitmap_set_bit_1) {
    Bitmap bitmap;
    ck_assert_int_eq(bitmap_set_bit(NULL, 0, 1), ERROR);
}
END_TEST

START_TEST(bitmap_set_bit_2) {
    Bitmap bitmap;
    bitmap.size = 0;
    ck_assert_int_eq(bitmap_set_bit(&bitmap, 0, 1), ERROR);
}
END_TEST

START_TEST(bitmap_set_bit_3) {
    Bitmap bitmap;
    ck_assert_int_eq(bitmap_create(&bitmap, 8), SUCCESS);
    ck_assert_int_eq(bitmap_set_bit(&bitmap, 0, 2), ERROR);
    bitmap_delete(&bitmap);
}
END_TEST

START_TEST(bitmap_set_bit_4) {
    Bitmap bitmap;
    ck_assert_int_eq(bitmap_create(&bitmap, 8), SUCCESS);
    ck_assert_int_eq(bitmap_set_bit(&bitmap, 8, 1), ERROR);
    bitmap_delete(&bitmap);
}
END_TEST

START_TEST(bitmap_set_bit_5) {
    Bitmap bitmap;
    ck_assert_int_eq(bitmap_create(&bitmap, 8), SUCCESS);
    ck_assert_int_eq(bitmap_set_bit(&bitmap, 0, 1), SUCCESS);
    ck_assert_int_eq((bitmap.bits[0] & 1), 1);
    bitmap_delete(&bitmap);
}
END_TEST

// bitmap_clear_bit
START_TEST(bitmap_clear_bit_1) {
    Bitmap bitmap;
    ck_assert_int_eq(bitmap_clear_bit(NULL, 0), ERROR);
}
END_TEST

START_TEST(bitmap_clear_bit_2) {
    Bitmap bitmap;
    bitmap.size = 0;
    ck_assert_int_eq(bitmap_clear_bit(&bitmap, 0), ERROR);
}
END_TEST

START_TEST(bitmap_clear_bit_3) {
    Bitmap bitmap;
    ck_assert_int_eq(bitmap_create(&bitmap, 8), SUCCESS);
    ck_assert_int_eq(bitmap_clear_bit(&bitmap, 8), ERROR);
    bitmap_delete(&bitmap);
}
END_TEST

START_TEST(bitmap_clear_bit_4) {
    Bitmap bitmap;
    ck_assert_int_eq(bitmap_create(&bitmap, 8), SUCCESS);
    ck_assert_int_eq(bitmap_set_bit(&bitmap, 0, 1), SUCCESS);
    ck_assert_int_eq(bitmap_clear_bit(&bitmap, 0), SUCCESS);
    ck_assert_int_eq((bitmap.bits[0] & 1), 0);
    bitmap_delete(&bitmap);
}
END_TEST

// bitmap_reset
START_TEST(bitmap_reset_1) {
    Bitmap bitmap;
    ck_assert_int_eq(bitmap_reset(NULL), ERROR);
}
END_TEST

START_TEST(bitmap_reset_3) {
    Bitmap bitmap;
    ck_assert_int_eq(bitmap_create(&bitmap, 8), SUCCESS);
    ck_assert_int_eq(bitmap_set_bit(&bitmap, 0, 1), SUCCESS);
    ck_assert_int_eq(bitmap_reset(&bitmap), SUCCESS);
    ck_assert_int_eq(bitmap.bits[0], 0);
    bitmap_delete(&bitmap);
}
END_TEST

// bitmap_delete
START_TEST(bitmap_delete_1) { ck_assert_int_eq(bitmap_delete(NULL), ERROR); }
END_TEST

START_TEST(bitmap_delete_2) {
    Bitmap bitmap;
    ck_assert_int_eq(bitmap_create(&bitmap, 8), SUCCESS);
    ck_assert_int_eq(bitmap_delete(&bitmap), SUCCESS);
}
END_TEST

// bitmap_increase
START_TEST(bitmap_increase_1) {
    Bitmap bitmap;
    ck_assert_int_eq(bitmap_create(&bitmap, 8), SUCCESS);

    bitmap_set_bit(&bitmap, 0, 1);
    bitmap_set_bit(&bitmap, 1, 0);
    bitmap_set_bit(&bitmap, 2, 1);
    bitmap_set_bit(&bitmap, 3, 0);

    ck_assert_int_eq(bitmap_increase(&bitmap, 16), SUCCESS);

    ck_assert((bitmap.bits[0] & 1) == 1);
    ck_assert((bitmap.bits[0] & (1 << 1)) == 0);
    ck_assert((bitmap.bits[0] & (1 << 2)) != 0);
    ck_assert((bitmap.bits[0] & (1 << 3)) == 0);

    for (size_t i = 8; i < 16; i++) {
        ck_assert((bitmap.bits[i / 8] & (1 << (i % 8))) == 0);
    }

    bitmap_delete(&bitmap);
}
END_TEST

// bitmap_copy
START_TEST(bitmap_copy_1) {
    Bitmap bitmap1, bitmap2;
    ck_assert_int_eq(bitmap_create(&bitmap1, 8), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 8), SUCCESS);

    bitmap_set_bit(&bitmap1, 0, 1);
    bitmap_set_bit(&bitmap1, 1, 1);
    bitmap_set_bit(&bitmap1, 2, 0);
    bitmap_set_bit(&bitmap1, 3, 1);

    ck_assert_int_eq(bitmap_copy(&bitmap1, &bitmap2, 0, 4, 0), SUCCESS);

    ck_assert((bitmap2.bits[0] & 1) == 1);
    ck_assert((bitmap2.bits[0] & (1 << 1)) != 0);
    ck_assert((bitmap2.bits[0] & (1 << 2)) == 0);
    ck_assert((bitmap2.bits[0] & (1 << 3)) != 0);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

START_TEST(bitmap_copy_2) {
    Bitmap bitmap1, bitmap2;
    ck_assert_int_eq(bitmap_create(&bitmap1, 8), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 16), SUCCESS);

    bitmap_set_bit(&bitmap1, 0, 1);
    bitmap_set_bit(&bitmap1, 1, 1);
    bitmap_set_bit(&bitmap1, 2, 0);
    bitmap_set_bit(&bitmap1, 3, 1);

    ck_assert_int_eq(bitmap_copy(&bitmap1, &bitmap2, 0, 4, 8), SUCCESS);

    ck_assert((bitmap2.bits[1] & 1) == 1);
    ck_assert((bitmap2.bits[1] & (1 << 1)) != 0);
    ck_assert((bitmap2.bits[1] & (1 << 2)) == 0);
    ck_assert((bitmap2.bits[1] & (1 << 3)) != 0);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

START_TEST(bitmap_copy_3) {
    Bitmap bitmap1, bitmap2;
    ck_assert_int_eq(bitmap_create(&bitmap1, 8), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 4), SUCCESS);

    bitmap_set_bit(&bitmap1, 0, 1);
    bitmap_set_bit(&bitmap1, 1, 0);
    bitmap_set_bit(&bitmap1, 2, 1);
    bitmap_set_bit(&bitmap1, 3, 0);

    ck_assert_int_eq(bitmap_copy(&bitmap1, &bitmap2, 0, 8, 0), SUCCESS);

    ck_assert((bitmap2.bits[0] & 1) == 1);
    ck_assert((bitmap2.bits[0] & (1 << 1)) == 0);
    ck_assert((bitmap2.bits[0] & (1 << 2)) != 0);
    ck_assert((bitmap2.bits[0] & (1 << 3)) == 0);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

START_TEST(bitmap_copy_4) {
    Bitmap bitmap1, bitmap2;
    ck_assert_int_eq(bitmap_create(&bitmap1, 8), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 8), SUCCESS);

    ck_assert_int_eq(bitmap_copy(NULL, &bitmap2, 0, 4, 0), ERROR);

    ck_assert_int_eq(bitmap_copy(&bitmap1, NULL, 0, 4, 0), ERROR);

    bitmap1.bits = NULL;
    ck_assert_int_eq(bitmap_copy(&bitmap1, &bitmap2, 0, 4, 0), ERROR);

    bitmap2.bits = NULL;
    ck_assert_int_eq(bitmap_copy(&bitmap1, &bitmap2, 0, 4, 0), ERROR);
}
END_TEST

// bitmap_file_save
START_TEST(bitmap_file_save1) {
    Bitmap bitmap1;
    ck_assert_int_eq(bitmap_create(&bitmap1, 32), SUCCESS);
    bitmap_set_bit(&bitmap1, 0, 1);
    bitmap_set_bit(&bitmap1, 31, 1);

    ck_assert_int_eq(bitmap_file_save(&bitmap1, "test_save.bin"), SUCCESS);

    bitmap_delete(&bitmap1);
}
END_TEST

START_TEST(bitmap_file_save2) {
    ck_assert_int_eq(bitmap_file_save(NULL, "test_save.bin"), ERROR);
}
END_TEST

START_TEST(bitmap_file_save3) {
    Bitmap bitmap1;
    bitmap1.bits = NULL;
    bitmap1.size = 32;
    ck_assert_int_eq(bitmap_file_save(&bitmap1, "test_save.bin"), ERROR);
}
END_TEST

START_TEST(bitmap_file_save4) {
    Bitmap bitmap1;
    ck_assert_int_eq(bitmap_create(&bitmap1, 32), SUCCESS);
    ck_assert_int_eq(bitmap_file_save(&bitmap1, NULL), ERROR);
    bitmap_delete(&bitmap1);
}
END_TEST

// bitmap_file_load
START_TEST(bitmap_file_load1) {
    Bitmap bitmap1;
    ck_assert_int_eq(bitmap_create(&bitmap1, 32), SUCCESS);
    bitmap_set_bit(&bitmap1, 0, 1);
    bitmap_set_bit(&bitmap1, 31, 1);
    ck_assert_int_eq(bitmap_file_save(&bitmap1, "test_load.bin"), SUCCESS);

    Bitmap bitmap2;
    bitmap2.bits = NULL;

    ck_assert_int_eq(bitmap_file_load(&bitmap2, "test_load.bin"), SUCCESS);
    ck_assert_int_eq(bitmap2.size, 32);
    ck_assert_int_eq(bitmap2.bits[0], bitmap1.bits[0]);
    ck_assert_int_eq(bitmap2.bits[3], bitmap1.bits[3]);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

START_TEST(bitmap_file_load2) {
    ck_assert_int_eq(bitmap_file_load(NULL, "test_load.bin"), ERROR);
}
END_TEST

START_TEST(bitmap_file_load3) {
    Bitmap bitmap1;
    ck_assert_int_eq(bitmap_file_load(&bitmap1, NULL), ERROR);
}
END_TEST

START_TEST(bitmap_file_load4) {
    Bitmap bitmap1;
    ck_assert_int_eq(bitmap_create(&bitmap1, 32), SUCCESS);
    ck_assert_int_eq(bitmap_file_load(&bitmap1, "non_exist_file.bin"), ERROR);
}
END_TEST

START_TEST(bitmap_file_load5) {
    Bitmap bitmap1;
    ck_assert_int_eq(bitmap_create(&bitmap1, 32), SUCCESS);
    bitmap_set_bit(&bitmap1, 0, 1);
    bitmap_set_bit(&bitmap1, 31, 1);
    ck_assert_int_eq(bitmap_file_save(&bitmap1, "test_load.bin"), SUCCESS);

    Bitmap bitmap2;
    ck_assert_int_eq(bitmap_create(&bitmap2, 1), SUCCESS);

    ck_assert_int_eq(bitmap_file_load(&bitmap2, "test_load.bin"), SUCCESS);

    ck_assert_int_eq(bitmap_equal(&bitmap1, &bitmap2), 1);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

//ТЕСТЫ ДЛЯ ОПЕРАЦИЙ СРАВНЕНИЯ

// bitmap_compare
START_TEST(bitmap_compare_1) {
    Bitmap bitmap1, bitmap2;
    ck_assert_int_eq(bitmap_create(&bitmap1, 8), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 8), SUCCESS);

    bitmap_set_bit(&bitmap1, 0, 1);
    bitmap_set_bit(&bitmap2, 0, 1);

    ck_assert_int_eq(bitmap_compare(&bitmap1, &bitmap2), 0);

    bitmap_set_bit(&bitmap1, 1, 1);
    ck_assert_int_eq(bitmap_compare(&bitmap1, &bitmap2), 1);
    bitmap_set_bit(&bitmap1, 1, 0);
    bitmap_set_bit(&bitmap2, 1, 1);
    ck_assert_int_eq(bitmap_compare(&bitmap1, &bitmap2), 1);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

START_TEST(bitmap_compare_2) {
    Bitmap bitmap1, bitmap2;
    ck_assert_int_eq(bitmap_create(&bitmap1, 8), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 4), SUCCESS);

    ck_assert_int_eq(bitmap_compare(&bitmap1, &bitmap2), 0);
    ck_assert_int_eq(bitmap_compare(&bitmap2, &bitmap1), 0);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

START_TEST(bitmap_compare_3) {
    Bitmap bitmap1, bitmap2;
    ck_assert_int_eq(bitmap_create(&bitmap1, 8), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 8), SUCCESS);

    ck_assert_int_eq(bitmap_compare(NULL, &bitmap2), ERROR);
    ck_assert_int_eq(bitmap_compare(&bitmap1, NULL), ERROR);
    ck_assert_int_eq(bitmap_compare(NULL, NULL), ERROR);

    bitmap1.bits = NULL;
    ck_assert_int_eq(bitmap_compare(&bitmap1, &bitmap2), ERROR);

    bitmap2.bits = NULL;
    ck_assert_int_eq(bitmap_compare(&bitmap1, &bitmap2), ERROR);
}
END_TEST

// bitmap_equal
START_TEST(bitmap_equal_1) {
    Bitmap bitmap1, bitmap2;
    ck_assert_int_eq(bitmap_create(&bitmap1, 8), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 8), SUCCESS);

    bitmap_set_bit(&bitmap1, 0, 1);
    bitmap_set_bit(&bitmap2, 0, 1);

    ck_assert_int_eq(bitmap_equal(&bitmap1, &bitmap2), 1);

    bitmap_set_bit(&bitmap1, 1, 1);
    ck_assert_int_eq(bitmap_equal(&bitmap1, &bitmap2), 0);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

START_TEST(bitmap_equal_2) {
    Bitmap bitmap1, bitmap2;
    ck_assert_int_eq(bitmap_create(&bitmap1, 8), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 4), SUCCESS);

    ck_assert_int_eq(bitmap_equal(&bitmap1, &bitmap2), 1);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

START_TEST(bitmap_equal_3) {
    Bitmap bitmap1, bitmap2;
    ck_assert_int_eq(bitmap_create(&bitmap1, 8), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 8), SUCCESS);

    ck_assert_int_eq(bitmap_equal(NULL, &bitmap2), ERROR);
    ck_assert_int_eq(bitmap_equal(&bitmap1, NULL), ERROR);
    ck_assert_int_eq(bitmap_equal(NULL, NULL), ERROR);

    bitmap1.bits = NULL;
    ck_assert_int_eq(bitmap_equal(&bitmap1, &bitmap2), ERROR);

    bitmap2.bits = NULL;
    ck_assert_int_eq(bitmap_equal(&bitmap1, &bitmap2), ERROR);
}
END_TEST

// bitmap_strictly_greater
START_TEST(bitmap_strictly_greater_1) {
    Bitmap bitmap1, bitmap2;
    ck_assert_int_eq(bitmap_create(&bitmap1, 8), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 8), SUCCESS);

    bitmap_set_bit(&bitmap1, 0, 1);
    bitmap_set_bit(&bitmap2, 0, 0);

    ck_assert_int_eq(bitmap_strictly_greater(&bitmap1, &bitmap2), 1);
    ck_assert_int_eq(bitmap_strictly_greater(&bitmap2, &bitmap1), 0);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

START_TEST(bitmap_strictly_greater_2) {
    Bitmap bitmap1, bitmap2;
    ck_assert_int_eq(bitmap_create(&bitmap1, 8), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 4), SUCCESS);

    ck_assert_int_eq(bitmap_strictly_greater(&bitmap1, &bitmap2), 0);
    ck_assert_int_eq(bitmap_strictly_greater(&bitmap2, &bitmap1), 0);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

START_TEST(bitmap_strictly_greater_3) {
    Bitmap bitmap1, bitmap2;
    ck_assert_int_eq(bitmap_create(&bitmap1, 8), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 8), SUCCESS);

    ck_assert_int_eq(bitmap_strictly_greater(NULL, &bitmap2), ERROR);
    ck_assert_int_eq(bitmap_strictly_greater(&bitmap1, NULL), ERROR);
    ck_assert_int_eq(bitmap_strictly_greater(NULL, NULL), ERROR);

    bitmap1.bits = NULL;
    ck_assert_int_eq(bitmap_strictly_greater(&bitmap1, &bitmap2), ERROR);

    bitmap2.bits = NULL;
    ck_assert_int_eq(bitmap_strictly_greater(&bitmap1, &bitmap2), ERROR);
}
END_TEST

// bitmap_greater
START_TEST(bitmap_greater_1) {
    Bitmap bitmap1, bitmap2;
    ck_assert_int_eq(bitmap_create(&bitmap1, 8), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 8), SUCCESS);

    bitmap_set_bit(&bitmap1, 0, 1);
    bitmap_set_bit(&bitmap2, 0, 0);

    ck_assert_int_eq(bitmap_greater(&bitmap1, &bitmap2), 1);
    ck_assert_int_eq(bitmap_greater(&bitmap2, &bitmap1), 0);
    ck_assert_int_eq(bitmap_greater(&bitmap1, &bitmap1), 1);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

START_TEST(bitmap_greater_2) {
    Bitmap bitmap1, bitmap2;
    ck_assert_int_eq(bitmap_create(&bitmap1, 8), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 4), SUCCESS);

    ck_assert_int_eq(bitmap_greater(&bitmap1, &bitmap2), 1);
    ck_assert_int_eq(bitmap_greater(&bitmap2, &bitmap1), 1);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

START_TEST(bitmap_greater_3) {
    Bitmap bitmap1, bitmap2;
    ck_assert_int_eq(bitmap_create(&bitmap1, 8), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 8), SUCCESS);

    ck_assert_int_eq(bitmap_greater(NULL, &bitmap2), ERROR);
    ck_assert_int_eq(bitmap_greater(&bitmap1, NULL), ERROR);
    ck_assert_int_eq(bitmap_greater(NULL, NULL), ERROR);

    bitmap1.bits = NULL;
    ck_assert_int_eq(bitmap_greater(&bitmap1, &bitmap2), ERROR);

    bitmap2.bits = NULL;
    ck_assert_int_eq(bitmap_greater(&bitmap1, &bitmap2), ERROR);
}
END_TEST

// bitmap_strictly_less
START_TEST(bitmap_strictly_less_1) {
    Bitmap bitmap1, bitmap2;
    ck_assert_int_eq(bitmap_create(&bitmap1, 8), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 8), SUCCESS);

    bitmap_set_bit(&bitmap1, 0, 0);
    bitmap_set_bit(&bitmap2, 0, 1);

    ck_assert_int_eq(bitmap_strictly_less(&bitmap1, &bitmap2), 1);
    ck_assert_int_eq(bitmap_strictly_less(&bitmap2, &bitmap1), 0);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

START_TEST(bitmap_strictly_less_2) {
    Bitmap bitmap1, bitmap2;
    ck_assert_int_eq(bitmap_create(&bitmap1, 4), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 8), SUCCESS);

    ck_assert_int_eq(bitmap_strictly_less(&bitmap1, &bitmap2), 0);
    ck_assert_int_eq(bitmap_strictly_less(&bitmap2, &bitmap1), 0);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

START_TEST(bitmap_strictly_less_3) {
    Bitmap bitmap1, bitmap2;
    ck_assert_int_eq(bitmap_create(&bitmap1, 8), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 8), SUCCESS);

    ck_assert_int_eq(bitmap_strictly_less(NULL, &bitmap2), ERROR);
    ck_assert_int_eq(bitmap_strictly_less(&bitmap1, NULL), ERROR);
    ck_assert_int_eq(bitmap_strictly_less(NULL, NULL), ERROR);

    bitmap1.bits = NULL;
    ck_assert_int_eq(bitmap_strictly_less(&bitmap1, &bitmap2), ERROR);

    bitmap2.bits = NULL;
    ck_assert_int_eq(bitmap_strictly_less(&bitmap1, &bitmap2), ERROR);
}
END_TEST

// bitmap_less
START_TEST(bitmap_less_1) {
    Bitmap bitmap1, bitmap2;
    ck_assert_int_eq(bitmap_create(&bitmap1, 8), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 8), SUCCESS);

    bitmap_set_bit(&bitmap1, 0, 0);
    bitmap_set_bit(&bitmap2, 0, 1);

    ck_assert_int_eq(bitmap_less(&bitmap1, &bitmap2), 1);
    ck_assert_int_eq(bitmap_less(&bitmap2, &bitmap1), 0);
    ck_assert_int_eq(bitmap_less(&bitmap1, &bitmap1), 1);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

START_TEST(bitmap_less_2) {
    Bitmap bitmap1, bitmap2;
    ck_assert_int_eq(bitmap_create(&bitmap1, 4), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 8), SUCCESS);

    ck_assert_int_eq(bitmap_less(&bitmap1, &bitmap2), 1);
    ck_assert_int_eq(bitmap_less(&bitmap2, &bitmap1), 1);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

START_TEST(bitmap_less_3) {
    Bitmap bitmap1, bitmap2;
    ck_assert_int_eq(bitmap_create(&bitmap1, 8), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 8), SUCCESS);

    ck_assert_int_eq(bitmap_less(NULL, &bitmap2), ERROR);
    ck_assert_int_eq(bitmap_less(&bitmap1, NULL), ERROR);
    ck_assert_int_eq(bitmap_less(NULL, NULL), ERROR);

    bitmap1.bits = NULL;
    ck_assert_int_eq(bitmap_less(&bitmap1, &bitmap2), ERROR);

    bitmap2.bits = NULL;
    ck_assert_int_eq(bitmap_less(&bitmap1, &bitmap2), ERROR);
}
END_TEST

// bitmap_concat_in_place
START_TEST(bitmap_concat_in_place_1) {
    Bitmap bitmap1;
    ck_assert_int_eq(bitmap_concat_in_place(&bitmap1, NULL), ERROR);
}
END_TEST

START_TEST(bitmap_concat_in_place_2) {
    Bitmap bitmap1, bitmap2;
    ck_assert_int_eq(bitmap_create(&bitmap1, 4), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 4), SUCCESS);

    bitmap_set_bit(&bitmap1, 0, 1);
    bitmap_set_bit(&bitmap2, 0, 1);

    ck_assert_int_eq(bitmap_concat_in_place(&bitmap1, &bitmap2), SUCCESS);
    ck_assert_int_eq(bitmap1.size, 16);

    ck_assert_int_eq(bitmap1.bits[0], 0x01);
    ck_assert_int_eq(bitmap1.bits[1], 0x01);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

START_TEST(bitmap_concat_in_place_3) {
    Bitmap bitmap1, bitmap2;
    ck_assert_int_eq(bitmap_create(&bitmap1, 4), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 8), SUCCESS);

    bitmap_set_bit(&bitmap1, 0, 1);
    bitmap_set_bit(&bitmap2, 0, 1);
    bitmap_set_bit(&bitmap2, 4, 1);

    ck_assert_int_eq(bitmap_concat_in_place(&bitmap1, &bitmap2), SUCCESS);
    ck_assert_int_eq(bitmap1.size, 16);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

START_TEST(bitmap_concat_in_place_4) {
    Bitmap bitmap1, bitmap2;
    ck_assert_int_eq(bitmap_create(&bitmap1, 4), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 4), SUCCESS);

    ck_assert_int_eq(bitmap_concat_in_place(NULL, &bitmap2), ERROR);
    ck_assert_int_eq(bitmap_concat_in_place(&bitmap1, NULL), ERROR);
    ck_assert_int_eq(bitmap_concat_in_place(NULL, NULL), ERROR);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

START_TEST(bitmap_concat_in_place_5) {
    Bitmap bitmap1, bitmap2;
    ck_assert_int_eq(bitmap_create(&bitmap1, 4), SUCCESS);
    ck_assert_int_eq(bitmap_create(&bitmap2, 4), SUCCESS);

    bitmap1.bits = NULL;
    ck_assert_int_eq(bitmap_concat_in_place(&bitmap1, &bitmap2), ERROR);

    bitmap2.bits = NULL;
    ck_assert_int_eq(bitmap_concat_in_place(&bitmap1, &bitmap2), ERROR);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
}
END_TEST

Suite *bitmap_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Bitmap");

    tc_core = tcase_create("Core");

    // Тесты побитовых операций
    tcase_add_test(tc_core, bitmap_and_1);
    tcase_add_test(tc_core, bitmap_and_2);
    tcase_add_test(tc_core, bitmap_and_3);
    tcase_add_test(tc_core, bitmap_and_4);
    tcase_add_test(tc_core, bitmap_and_5);

    tcase_add_test(tc_core, bitmap_or_1);
    tcase_add_test(tc_core, bitmap_or_2);
    tcase_add_test(tc_core, bitmap_or_3);
    tcase_add_test(tc_core, bitmap_or_4);
    tcase_add_test(tc_core, bitmap_or_5);

    tcase_add_test(tc_core, bitmap_xor_1);
    tcase_add_test(tc_core, bitmap_xor_2);
    tcase_add_test(tc_core, bitmap_xor_3);
    tcase_add_test(tc_core, bitmap_xor_4);
    tcase_add_test(tc_core, bitmap_xor_5);

    tcase_add_test(tc_core, bitmap_not_1);
    tcase_add_test(tc_core, bitmap_not_2);

    // Тесты командных операций
    tcase_add_test(tc_core, bitmap_bits_to_byte_1);
    tcase_add_test(tc_core, bitmap_bits_to_byte_2);
    tcase_add_test(tc_core, bitmap_bits_to_byte_3);
    tcase_add_test(tc_core, bitmap_bits_to_byte_4);

    tcase_add_test(tc_core, bitmap_create_1);
    tcase_add_test(tc_core, bitmap_create_2);
    tcase_add_test(tc_core, bitmap_create_3);

    tcase_add_test(tc_core, bitmap_clone_1);
    tcase_add_test(tc_core, bitmap_clone_2);
    tcase_add_test(tc_core, bitmap_clone_3);

    tcase_add_test(tc_core, bitmap_show_1);
    tcase_add_test(tc_core, bitmap_show_2);
    tcase_add_test(tc_core, bitmap_show_3);

    tcase_add_test(tc_core, bitmap_set_bit_1);
    tcase_add_test(tc_core, bitmap_set_bit_2);
    tcase_add_test(tc_core, bitmap_set_bit_3);
    tcase_add_test(tc_core, bitmap_set_bit_4);
    tcase_add_test(tc_core, bitmap_set_bit_5);

    tcase_add_test(tc_core, bitmap_clear_bit_1);
    tcase_add_test(tc_core, bitmap_clear_bit_2);
    tcase_add_test(tc_core, bitmap_clear_bit_3);
    tcase_add_test(tc_core, bitmap_clear_bit_4);

    tcase_add_test(tc_core, bitmap_reset_1);
    tcase_add_test(tc_core, bitmap_reset_3);

    tcase_add_test(tc_core, bitmap_delete_1);
    tcase_add_test(tc_core, bitmap_delete_2);

    tcase_add_test(tc_core, bitmap_increase_1);

    tcase_add_test(tc_core, bitmap_copy_1);
    tcase_add_test(tc_core, bitmap_copy_2);
    tcase_add_test(tc_core, bitmap_copy_3);
    tcase_add_test(tc_core, bitmap_copy_4);

    tcase_add_test(tc_core, bitmap_file_save1);
    tcase_add_test(tc_core, bitmap_file_save2);
    tcase_add_test(tc_core, bitmap_file_save3);
    tcase_add_test(tc_core, bitmap_file_save4);

    tcase_add_test(tc_core, bitmap_file_load1);
    tcase_add_test(tc_core, bitmap_file_load2);
    tcase_add_test(tc_core, bitmap_file_load3);
    tcase_add_test(tc_core, bitmap_file_load4);
    tcase_add_test(tc_core, bitmap_file_load5);

    //Тесты операций сравнения
    tcase_add_test(tc_core, bitmap_compare_1);
    tcase_add_test(tc_core, bitmap_compare_2);
    tcase_add_test(tc_core, bitmap_compare_3);

    tcase_add_test(tc_core, bitmap_equal_1);
    tcase_add_test(tc_core, bitmap_equal_2);
    tcase_add_test(tc_core, bitmap_equal_3);

    tcase_add_test(tc_core, bitmap_strictly_greater_1);
    tcase_add_test(tc_core, bitmap_strictly_greater_2);
    tcase_add_test(tc_core, bitmap_strictly_greater_3);

    tcase_add_test(tc_core, bitmap_greater_1);
    tcase_add_test(tc_core, bitmap_greater_2);
    tcase_add_test(tc_core, bitmap_greater_3);

    tcase_add_test(tc_core, bitmap_strictly_less_1);
    tcase_add_test(tc_core, bitmap_strictly_less_2);
    tcase_add_test(tc_core, bitmap_strictly_less_3);

    tcase_add_test(tc_core, bitmap_less_1);
    tcase_add_test(tc_core, bitmap_less_2);
    tcase_add_test(tc_core, bitmap_less_3);

    tcase_add_test(tc_core, bitmap_concat_in_place_1);
    tcase_add_test(tc_core, bitmap_concat_in_place_2);
    tcase_add_test(tc_core, bitmap_concat_in_place_3);
    tcase_add_test(tc_core, bitmap_concat_in_place_4);
    tcase_add_test(tc_core, bitmap_concat_in_place_5);

    suite_add_tcase(s, tc_core);

    return s;
}

int main() {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = bitmap_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
