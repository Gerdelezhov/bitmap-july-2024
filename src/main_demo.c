#include "../include/bitmap.h"
#include "../include/bitmap_plugin.h"

int main() {
    void *handle;
    bitmap_plugin ops;

    handle = dlopen(PLUGIN_PATH "bitmap_and_op.so", RTLD_LAZY);
    if (!handle) {
        perror("Ошибка загрузки плагина\n");
        return ERROR;
    }
    ops.bitmap_and = dlsym(handle, "bitmap_and");
    if (dlerror() != NULL) {
        perror("Ошибка загрузки плагина\n");
        dlclose(handle);
        return ERROR;
    }

    handle = dlopen(PLUGIN_PATH "bitmap_or_op.so", RTLD_LAZY);
    if (!handle) {
        perror("Ошибка загрузки плагина\n");
        return ERROR;
    }
    ops.bitmap_or = dlsym(handle, "bitmap_or");
    if (dlerror() != NULL) {
        perror("Ошибка загрузки плагина\n");
        dlclose(handle);
        return ERROR;
    }

    handle = dlopen(PLUGIN_PATH "bitmap_xor_op.so", RTLD_LAZY);
    if (!handle) {
        perror("Ошибка загрузки плагина\n");
        return ERROR;
    }
    ops.bitmap_xor = dlsym(handle, "bitmap_xor");
    if (dlerror() != NULL) {
        perror("Ошибка загрузки плагина\n");
        dlclose(handle);
        return ERROR;
    }

    handle = dlopen(PLUGIN_PATH "bitmap_not_op.so", RTLD_LAZY);
    if (!handle) {
        perror("Ошибка загрузки плагина\n");
        return ERROR;
    }
    ops.bitmap_not = dlsym(handle, "bitmap_not");
    if (dlerror() != NULL) {
        perror("Ошибка загрузки плагина\n");
        dlclose(handle);
        return ERROR;
    }
    Bitmap bitmap;
    size_t bits = 32;

    if (bitmap_create(&bitmap, bits) == ERROR) {
        printf("Не удалось создать битмап.\n");
        perror("Не удалось создать битмап.\n");
        return ERROR;
    }

    /* Устанавливаем третий бит в битовой карте в значение 1, отсчет байтов идет
    от меньшего разряда к большему */
    bitmap.bits[0] |= (1 << 2);
    /* Устанавливаем 12 бит в битовой карте в значение 1 */
    bitmap.bits[1] |= (1 << 4);
    printf("\nbitmap: ");
    bitmap_show(&bitmap);

    printf("Устанавливаем в bitmap 0 бит в значение 1\n");
    bitmap_set_bit(&bitmap, 0, 1);
    bitmap_show(&bitmap);

    printf("Очищаем в bitmap 12 бит\n");
    bitmap_clear_bit(&bitmap, 12);
    bitmap_show(&bitmap);

    /* Удаляем битовую карту*/
    bitmap_delete(&bitmap);
    bitmap_show(&bitmap);

    Bitmap bitmap1, bitmap2;

    // Выполнение bitmap_and
    printf("\nВЫПОЛНЕНИЕ ОПЕРАЦИИ AND\n");
    if (bitmap_create(&bitmap1, 16) == ERROR) {
        printf("Не удалось создать bitmap1\n");
        perror("Не удалось создать bitmap1\n");
        return ERROR;
    }
    if (bitmap_create(&bitmap2, 24) == ERROR) {
        printf("Не удалось создать bitmap2\n");
        perror("Не удалось создать bitmap2\n");
        bitmap_delete(&bitmap1);
        return ERROR;
    }

    bitmap_set_bit(&bitmap1, 0, 1);
    bitmap_set_bit(&bitmap1, 2, 1);
    bitmap_set_bit(&bitmap1, 4, 1);
    bitmap_set_bit(&bitmap2, 0, 1);
    bitmap_set_bit(&bitmap2, 3, 1);
    bitmap_set_bit(&bitmap2, 4, 1);

    printf("bitmap1 до AND\n");
    bitmap_show(&bitmap1);
    printf("bitmap2\n");
    bitmap_show(&bitmap2);

    if (ops.bitmap_and(&bitmap1, &bitmap2) == ERROR) {
        printf("Не удалось выполнить AND\n");
        perror("Не удалось выполнить AND\n");
        bitmap_delete(&bitmap1);
        bitmap_delete(&bitmap2);
        return ERROR;
    }

    printf("Bitmap1 после AND\n");
    bitmap_show(&bitmap1);

    Bitmap clone;
    bitmap_create(&clone, 64);
    printf("\nКлонируем bitmap1: addr:%p содержимое:", &bitmap1);
    bitmap_show(&bitmap1);
    bitmap_clone(&bitmap1, &clone);
    bitmap1.bits[0] |= (1 << 2);
    printf("После клонирования низменим третий бит bitmap1 на 1:");
    bitmap_show(&bitmap1);
    printf("Содеримое битмапа clone не изменилось: addr:%p содержимое:",
           &clone);
    bitmap_show(&clone);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);

    printf("\nВЫПОЛНЕНИЕ ОПЕРАЦИИ OR\n");
    if (bitmap_create(&bitmap1, 32) == ERROR) {
        printf("Не удалось создать bitmap1\n");
        perror("Не удалось создать bitmap1\n");
        return ERROR;
    }
    if (bitmap_create(&bitmap2, 16) == ERROR) {
        printf("Не удалось создать bitmap2\n");
        perror("Не удалось создать bitmap2\n");
        bitmap_delete(&bitmap1);
        return ERROR;
    }

    bitmap_set_bit(&bitmap1, 0, 1);
    bitmap_set_bit(&bitmap1, 15, 1);
    bitmap_set_bit(&bitmap1, 31, 1);

    bitmap_set_bit(&bitmap2, 0, 1);
    bitmap_set_bit(&bitmap2, 3, 1);
    bitmap_set_bit(&bitmap2, 15, 1);

    printf("bitmap1 до OR\n");
    bitmap_show(&bitmap1);

    printf("bitmap2\n");
    bitmap_show(&bitmap2);

    if (ops.bitmap_or(&bitmap1, &bitmap2) == ERROR) {
        printf("Не удалось выполнить побитовую операцию OR\n");
        perror("Не удалось выполнить побитовую операцию OR\n");
        bitmap_delete(&bitmap1);
        bitmap_delete(&bitmap2);
        return ERROR;
    }

    printf("Bitmap1 после OR\n");
    bitmap_show(&bitmap1);

    bitmap_reset(&bitmap1);
    printf("Bitmap1 после сброса\n");
    bitmap_show(&bitmap1);

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);

    printf("\nВЫПОЛНЕНИЕ ОПЕРАЦИИ XOR\n");
    if (bitmap_create(&bitmap1, 32) == ERROR) {
        printf("Не удалось создать bitmap1\n");
        perror("Не удалось создать bitmap1\n");
        return ERROR;
    }
    if (bitmap_create(&bitmap2, 16) == ERROR) {
        printf("Не удалось создать bitmap2\n");
        perror("Не удалось создать bitmap2\n");
        bitmap_delete(&bitmap1);
        return ERROR;
    }

    bitmap_set_bit(&bitmap1, 0, 1);
    bitmap_set_bit(&bitmap1, 15, 1);
    bitmap_set_bit(&bitmap1, 31, 1);

    bitmap_set_bit(&bitmap2, 0, 1);
    bitmap_set_bit(&bitmap2, 3, 1);
    bitmap_set_bit(&bitmap2, 15, 1);

    printf("bitmap1 до OR\n");
    bitmap_show(&bitmap1);

    printf("bitmap2\n");
    bitmap_show(&bitmap2);

    if (ops.bitmap_xor(&bitmap1, &bitmap2) == ERROR) {
        printf("Не удалось выполнить побитовую операцию XOR\n");
        perror("Не удалось выполнить побитовую операцию XOR\n");
        bitmap_delete(&bitmap1);
        bitmap_delete(&bitmap2);
        return ERROR;
    }

    printf("Bitmap1 после XOR\n");
    bitmap_show(&bitmap1);

    if (ops.bitmap_not(&bitmap1) == ERROR) {
        printf("Не удалось выполнить побитовую операцию not\n");
        perror("Не удалось выполнить побитовую операцию not\n");
        bitmap_delete(&bitmap1);
        bitmap_delete(&bitmap2);
        return ERROR;
    }

    printf("Bitmap1 после NOT\n");
    bitmap_show(&bitmap1);
    //Проверка операций сравнения
    printf("\n");
    printf("Bitmap1: ");
    bitmap_show(&bitmap1);
    printf("Bitmap2: ");
    bitmap_show(&bitmap2);
    int comp_res = bitmap_compare(&bitmap1, &bitmap2);
    if (comp_res == ERROR) {
        printf("Не удалось выполнить  операцию compare\n");
        perror("Не удалось выполнить  операцию compare\n");
        bitmap_delete(&bitmap1);
        bitmap_delete(&bitmap2);
        return ERROR;
    }
    if (comp_res == 0) {
        printf("bitmap1 и bitmap2 равны\n");

    } else {
        printf("bitmap%d больше\n", comp_res);
    }
    int greater_res = bitmap_greater(&bitmap1, &bitmap2);
    if (greater_res == ERROR) {
        printf("Не удалось выполнить  операцию greater\n");
        perror("Не удалось выполнить  операцию greater\n");
        bitmap_delete(&bitmap1);
        bitmap_delete(&bitmap2);
        return ERROR;
    }
    if (greater_res) {
        printf("bitmap1 больше или равен bitmap2 \n");
    } else {
        printf("bitmap1 НЕ больше или равен bitmap2\n");
    }

    int greater_strict_res = bitmap_strictly_greater(&bitmap1, &bitmap2);
    if (greater_strict_res == ERROR) {
        printf("Не удалось выполнить  операцию strictly_greater\n");
        perror("Не удалось выполнить  операцию strictly_greater\n");
        bitmap_delete(&bitmap1);
        bitmap_delete(&bitmap2);
        return ERROR;
    }
    if (greater_strict_res) {
        printf("bitmap1 больше bitmap2 \n");
    } else {
        printf("bitmap1 НЕ больше bitmap2\n");
    }

    int less_res = bitmap_less(&bitmap1, &bitmap2);
    if (less_res == ERROR) {
        printf("Не удалось выполнить  операцию less\n");
        perror("Не удалось выполнить  операцию less\n");
        bitmap_delete(&bitmap1);
        bitmap_delete(&bitmap2);
        return ERROR;
    }
    if (less_res) {
        printf("bitmap1 меньше или равен bitmap2 \n");
    } else {
        printf("bitmap1 НЕ меньше или равен bitmap2\n");
    }

    int less_strict_res = bitmap_strictly_less(&bitmap1, &bitmap2);
    if (less_strict_res == ERROR) {
        printf("Не удалось выполнить  операцию less_greater\n");
        perror("Не удалось выполнить  операцию less_greater\n");
        bitmap_delete(&bitmap1);
        bitmap_delete(&bitmap2);
        return ERROR;
    }
    if (less_strict_res) {
        printf("bitmap1 меньше bitmap2 \n");
    } else {
        printf("bitmap1 НЕ меньше bitmap2\n");
    }
    int equal_res = bitmap_equal(&bitmap1, &bitmap2);
    if (equal_res == ERROR) {
        printf("Не удалось выполнить  операцию equal\n");
        perror("Не удалось выполнить  операцию equal\n");
        bitmap_delete(&bitmap1);
        bitmap_delete(&bitmap2);
        return ERROR;
    }
    if (equal_res) {
        printf("bitmap1 и bitmap2 равны\n");
    } else {
        printf("bitmap1 и bitmap2 не равны\n");
    }
    printf("\n");

    printf("bitmap1 до сброса\n");
    bitmap_show(&bitmap1);
    if (bitmap_reset(&bitmap1) == ERROR) {
        printf("Не удалось выполнить  операцию reset\n");
        perror("Не удалось выполнить  операцию reset\n");
        bitmap_delete(&bitmap1);
        return ERROR;
    }
    printf("bitmap1 после сброса\n");
    bitmap_show(&bitmap1);

    printf("bitmap2 до сброса\n");
    bitmap_show(&bitmap2);
    if (bitmap_reset(&bitmap2) == ERROR) {
        printf("Не удалось выполнить  операцию reset\n");
        perror("Не удалось выполнить  операцию reset\n");
        bitmap_delete(&bitmap2);
        return ERROR;
    }
    printf("bitmap2 после сброса\n");
    bitmap_show(&bitmap2);

    printf("\nВЫПОЛНЕНИЕ ОПЕРАЦИИ COPY\n");
    printf("Изменим биты в bitmap1:");
    bitmap_set_bit(&bitmap1, 15, 1);
    bitmap_set_bit(&bitmap1, 14, 1);
    bitmap_set_bit(&bitmap1, 13, 1);
    bitmap_set_bit(&bitmap1, 10, 1);
    bitmap_show(&bitmap1);
    if (bitmap_file_save(&bitmap1, "bitmap1.bin") == ERROR) {
        printf("Не удалось выполнить  операцию file_save\n");
        bitmap_delete(&bitmap1);
        return ERROR;
    }
    printf("(Сохраняем bitmap1 в файл bitmap1.bin)\n");
    printf("Скопируем биты 14-10 из bitmap1 в clone:\nclone до изменений:");
    bitmap_show(&clone);
    bitmap_copy(&bitmap1, &clone, 10, 5, 15);
    printf("Вставим биты в clone начиная c 9.\nclone после изменений:");
    bitmap_show(&clone);

    // bitmap_file_save(&clone, "test.txt");
    if (bitmap_file_load(&clone, "bitmap1.bin") == ERROR) {
        printf("Не удалось выполнить  операцию file_load\n");
        perror("Не удалось выполнить  операцию file_load\n");
        bitmap_delete(&bitmap2);
        return ERROR;
    }
    printf("(Загружаем в clone битмап из файла bitmap1.bin)\n");
    bitmap_show(&clone);

    bitmap_delete(&clone);
    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);

    printf("СКЛЕИВАНИЕ ДВУХ БИТМАПОВ\n");
    bitmap_create(&bitmap1, 4);
    bitmap_set_bit(&bitmap1, 0, 1);
    bitmap_set_bit(&bitmap1, 1, 1);

    bitmap_create(&bitmap2, 4);
    bitmap_set_bit(&bitmap2, 2, 1);
    bitmap_set_bit(&bitmap2, 3, 1);

    printf("bitmap1:");
    bitmap_show(&bitmap1);
    printf("bitmap2:");
    bitmap_show(&bitmap2);
    printf("result: ");
    if (bitmap_concat_in_place(&bitmap1, &bitmap2) == SUCCESS) {
        bitmap_show(&bitmap1);
    } else {
        printf("Ошибка при склеивании битмапов\n");
        perror("Ошибка при склеивании битмапов\n");
    }

    bitmap_delete(&bitmap1);
    bitmap_delete(&bitmap2);
    dlclose(handle);

    return SUCCESS;
}