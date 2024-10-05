#include <ncurses.h>

#include "../include/bitmap.h"
#include "../include/bitmap_plugin.h"
#include "../include/ui.h"

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

    setlocale(LC_ALL, "");
    initscr();
    echo();
    nocbreak();
    Windows windows;
    create_windows(&windows);
    BitmapArray array;
    init_bitmap_array(&array);
    Bitmap buffer1, buffer2;

    size_t size, length, src_start, dest_start, bit_num;
    int choice, result, val, bitmap1_id = 0, bitmap2_id = 0;
    char file_name_out[FILE_NAME_SIZE] = {' '};

    while (1) {
        choice = redraw_windows(windows, &array);

        switch (choice) {
            case 1:
                mvwprintw(windows.input_win, 1, 1, "Введите количество бит: ");
                draw_borders(windows.input_win);
                wrefresh(windows.input_win);
                wscanw(windows.input_win, "%zu", &size);

                if (size == 0 || size * 2 < size) {
                    mvwprintw(windows.out_win, 1, 1,
                              "Размер должен быть больше 0.");
                    perror("Ошибка: Размер должен быть больше 0.");
                    break;
                }

                Bitmap new_bitmap;
                result = bitmap_create(&new_bitmap, size);

                if (result == 0) {
                    mvwprintw(windows.out_win, 1, 1, "Битмап успешно создан.");
                    add_bitmap(&array, new_bitmap);
                    show_from_array(&array, -1, windows.out_win, 2);
                } else {
                    mvwprintw(windows.out_win, 1, 1,
                              "Ошибка создания битмапа.");
                    perror("Ошибка создания битмапа.");
                }
                wrefresh(windows.out_win);
                break;
            case 2:
                mvwprintw(
                    windows.input_win, 1, 1,
                    "Введите id исходного битмапа, и id целевого битмапа: ");
                draw_borders(windows.input_win);
                wrefresh(windows.input_win);
                wscanw(windows.input_win, "%d %d", &bitmap1_id, &bitmap2_id);

                if (!is_valid_bitmap_id(bitmap1_id, &array, windows.out_win) ||
                    !is_valid_bitmap_id(bitmap2_id, &array, windows.out_win)) {
                    break;
                }

                if (bitmap_clone(&array.bitmaps[bitmap2_id], &buffer2) ==
                    ERROR) {
                    mvwprintw(windows.out_win, 1, 1,
                              "Ошибка клонирования битмапа.");
                    perror("Ошибка клонирования битмапа.");
                }

                result = bitmap_clone(&array.bitmaps[bitmap1_id],
                                      &array.bitmaps[bitmap2_id]);
                if (result == 0) {
                    werase(windows.out_win);
                    draw_borders(windows.out_win);
                    mvwprintw(windows.out_win, 1, 1,
                              "Битмап успешно клонирован.");
                    show_from_array(&array, bitmap1_id, windows.out_win, 2);
                    bitmap_show_ncurses(&buffer2, bitmap2_id, windows.out_win,
                                        3);
                    mvwprintw(windows.out_win, 4, 1, "----------------------");
                    show_from_array(&array, bitmap2_id, windows.out_win, 5);
                } else {
                    mvwprintw(windows.out_win, 1, 1,
                              "Ошибка клонирования битмапа.");
                    perror("Ошибка клонирования битмапа.");
                }
                wrefresh(windows.out_win);
                break;
            case 3:
                mvwprintw(windows.input_win, 1, 1,
                          "Введите id исходного битмапа, целевого, начало "
                          "копируемого отрезка, длину отрезка, начало места "
                          "вставки:");
                mvwprintw(windows.input_win, 2, 1, " ");
                draw_borders(windows.input_win);
                wrefresh(windows.input_win);
                wscanw(windows.input_win, "%d %d %zu %zu %zu", &bitmap1_id,
                       &bitmap2_id, &src_start, &length, &dest_start);

                if (!is_valid_bitmap_id(bitmap1_id, &array, windows.out_win) ||
                    !is_valid_bitmap_id(bitmap2_id, &array, windows.out_win)) {
                    break;
                }

                bitmap_clone(&array.bitmaps[bitmap2_id], &buffer2);
                result = bitmap_copy(&array.bitmaps[bitmap1_id],
                                     &array.bitmaps[bitmap2_id], src_start,
                                     length, dest_start);
                if (result == 0) {
                    werase(windows.out_win);
                    draw_borders(windows.out_win);
                    mvwprintw(windows.out_win, 1, 1,
                              "Копирование прошло успешно.");
                    show_from_array(&array, bitmap1_id, windows.out_win, 2);
                    bitmap_show_ncurses(&buffer2, bitmap2_id, windows.out_win,
                                        3);
                    mvwprintw(windows.out_win, 4, 1, "----------------------");
                    show_from_array(&array, bitmap2_id, windows.out_win, 5);
                } else {
                    mvwprintw(windows.out_win, 1, 1, "Ошибка копирования.");
                    perror("Ошибка клонирования битмапа.");
                }
                wrefresh(windows.out_win);
                break;
            case 4:
                mvwprintw(windows.input_win, 1, 1,
                          "Введите id битмапа, номер бита и значение: ");
                draw_borders(windows.input_win);
                wrefresh(windows.input_win);
                wscanw(windows.input_win, "%d %zu %d", &bitmap1_id, &bit_num,
                       &val);

                if (!is_valid_bitmap_id(bitmap1_id, &array, windows.out_win)) {
                    break;
                }

                bitmap_clone(&array.bitmaps[bitmap1_id], &buffer1);

                result =
                    bitmap_set_bit(&array.bitmaps[bitmap1_id], bit_num, val);
                if (result == 0) {
                    werase(windows.out_win);
                    draw_borders(windows.out_win);
                    mvwprintw(windows.out_win, 1, 1, "Бит задан успешно.");
                } else {
                    mvwprintw(windows.out_win, 1, 1, "Ошибка установки бита.");
                    perror("Ошибка установки бита.");
                }

                bitmap_show_ncurses(&buffer1, bitmap1_id, windows.out_win, 2);
                mvwprintw(windows.out_win, 3, 1, "----------------------");
                show_from_array(&array, bitmap1_id, windows.out_win, 4);
                wrefresh(windows.out_win);
                break;

            case 5:
                mvwprintw(windows.input_win, 1, 1,
                          "Введите id битмапа и номер бита: ");
                draw_borders(windows.input_win);
                wrefresh(windows.input_win);
                wscanw(windows.input_win, "%d %zu", &bitmap1_id, &bit_num);

                if (!is_valid_bitmap_id(bitmap1_id, &array, windows.out_win)) {
                    break;
                }

                bitmap_clone(&array.bitmaps[bitmap1_id], &buffer1);

                result = bitmap_clear_bit(&array.bitmaps[bitmap1_id], bit_num);
                if (result == 0) {
                    werase(windows.out_win);
                    draw_borders(windows.out_win);
                    mvwprintw(windows.out_win, 1, 1, "Бит очищен успешно.");
                } else {
                    mvwprintw(windows.out_win, 1, 1, "Ошибка очистки бита.");
                    perror("Ошибка очистки бита.");
                }

                bitmap_show_ncurses(&buffer1, bitmap1_id, windows.out_win, 2);
                mvwprintw(windows.out_win, 3, 1, "----------------------");
                show_from_array(&array, bitmap1_id, windows.out_win, 4);
                wrefresh(windows.out_win);
                break;

            case 6:
                mvwprintw(windows.input_win, 1, 1, "Введите id битмапа: ");
                draw_borders(windows.input_win);
                wrefresh(windows.input_win);
                wscanw(windows.input_win, "%d", &bitmap1_id);

                if (!is_valid_bitmap_id(bitmap1_id, &array, windows.out_win)) {
                    break;
                }

                bitmap_clone(&array.bitmaps[bitmap1_id], &buffer1);

                result = bitmap_reset(&array.bitmaps[bitmap1_id]);
                if (result == 0) {
                    mvwprintw(windows.out_win, 1, 1, "Битмап сброшен успешно.");
                } else {
                    mvwprintw(windows.out_win, 1, 1, "Ошибка сброса битмапа.");
                    perror("Ошибка сброса битмапа.");
                }

                bitmap_show_ncurses(&buffer1, bitmap1_id, windows.out_win, 2);
                mvwprintw(windows.out_win, 3, 1, "----------------------");
                show_from_array(&array, bitmap1_id, windows.out_win, 4);
                wrefresh(windows.out_win);
                break;

            case 7:
                mvwprintw(windows.input_win, 1, 1, "Введите id битмапа: ");
                draw_borders(windows.input_win);
                wrefresh(windows.input_win);
                wscanw(windows.input_win, "%d", &bitmap1_id);

                if (!is_valid_bitmap_id(bitmap1_id, &array, windows.out_win)) {
                    break;
                }

                result = bitmap_delete(&array.bitmaps[bitmap1_id]);
                if (result == 0) {
                    werase(windows.out_win);
                    mvwprintw(windows.out_win, 1, 1, "Битмап успешно удален.");
                    remove_bitmap(&array, bitmap1_id);
                    draw_borders(windows.out_win);
                    wrefresh(windows.out_win);
                } else {
                    mvwprintw(windows.out_win, 1, 1,
                              "Ошибка удаления битмапа.");
                    perror("Ошибка удаления бита.");
                }
                wrefresh(windows.out_win);
                break;

            case 8:
                mvwprintw(windows.input_win, 1, 1,
                          "Введите id первого битмапа, и id второго битмапа: ");
                draw_borders(windows.input_win);
                wrefresh(windows.input_win);
                wscanw(windows.input_win, "%d %d", &bitmap1_id, &bitmap2_id);

                if (!is_valid_bitmap_id(bitmap1_id, &array, windows.out_win) ||
                    !is_valid_bitmap_id(bitmap2_id, &array, windows.out_win)) {
                    break;
                }

                bitmap_clone(&array.bitmaps[bitmap1_id], &buffer1);

                result = ops.bitmap_and(&array.bitmaps[bitmap1_id],
                                        &array.bitmaps[bitmap2_id]);
                if (result == 0) {
                    werase(windows.out_win);
                    draw_borders(windows.out_win);
                    mvwprintw(windows.out_win, 1, 1,
                              "Операция AND выполнена успешно.");
                } else {
                    mvwprintw(windows.out_win, 1, 1,
                              "Произошла ошибка при выполнении операции AND.");
                    perror("Произошла ошибка при выполнении операции AND.");
                }
                bitmap_show_ncurses(&buffer1, bitmap1_id, windows.out_win, 2);
                show_from_array(&array, bitmap2_id, windows.out_win, 3);
                mvwprintw(windows.out_win, 4, 1, "----------------------");
                show_from_array(&array, bitmap1_id, windows.out_win, 5);
                wrefresh(windows.out_win);
                break;

            case 9:
                mvwprintw(windows.input_win, 1, 1,
                          "Введите id первого битмапа, и id второго битмапа: ");
                draw_borders(windows.input_win);
                wrefresh(windows.input_win);
                wscanw(windows.input_win, "%d %d", &bitmap1_id, &bitmap2_id);

                if (!is_valid_bitmap_id(bitmap1_id, &array, windows.out_win) ||
                    !is_valid_bitmap_id(bitmap2_id, &array, windows.out_win)) {
                    break;
                }

                bitmap_clone(&array.bitmaps[bitmap1_id], &buffer1);
                result = ops.bitmap_or(&array.bitmaps[bitmap1_id],
                                       &array.bitmaps[bitmap2_id]);
                if (result == 0) {
                    werase(windows.out_win);
                    draw_borders(windows.out_win);
                    mvwprintw(windows.out_win, 1, 1,
                              "Операция OR выполнена успешно.");
                } else {
                    mvwprintw(windows.out_win, 1, 1,
                              "Произошла ошибка при выполнении операции OR.");
                    perror("Произошла ошибка при выполнении операции OR.");
                }
                bitmap_show_ncurses(&buffer1, bitmap1_id, windows.out_win, 2);
                show_from_array(&array, bitmap2_id, windows.out_win, 3);
                mvwprintw(windows.out_win, 4, 1, "----------------------");
                show_from_array(&array, bitmap1_id, windows.out_win, 5);
                wrefresh(windows.out_win);
                break;

            case 10:
                mvwprintw(windows.input_win, 1, 1,
                          "Введите id первого битмапа, и id второго битмапа: ");
                draw_borders(windows.input_win);
                wrefresh(windows.input_win);
                wscanw(windows.input_win, "%d %d", &bitmap1_id, &bitmap2_id);

                if (!is_valid_bitmap_id(bitmap1_id, &array, windows.out_win) ||
                    !is_valid_bitmap_id(bitmap2_id, &array, windows.out_win)) {
                    break;
                }

                bitmap_clone(&array.bitmaps[bitmap1_id], &buffer1);
                result = ops.bitmap_xor(&array.bitmaps[bitmap1_id],
                                        &array.bitmaps[bitmap2_id]);
                if (result == 0) {
                    werase(windows.out_win);
                    draw_borders(windows.out_win);
                    mvwprintw(windows.out_win, 1, 1,
                              "Операция XOR выполнена успешно.");
                } else {
                    mvwprintw(windows.out_win, 1, 1,
                              "Произошла ошибка при выполнении операции XOR.");
                    perror("Произошла ошибка при выполнении операции XOR.");
                }
                bitmap_show_ncurses(&buffer1, bitmap1_id, windows.out_win, 2);
                show_from_array(&array, bitmap2_id, windows.out_win, 3);
                mvwprintw(windows.out_win, 4, 1, "----------------------");
                show_from_array(&array, bitmap1_id, windows.out_win, 5);
                wrefresh(windows.out_win);
                break;

            case 11:
                mvwprintw(windows.input_win, 1, 1, "Введите id битмапа: ");
                draw_borders(windows.input_win);
                wrefresh(windows.input_win);
                wscanw(windows.input_win, "%d", &bitmap1_id);

                if (!is_valid_bitmap_id(bitmap1_id, &array, windows.out_win)) {
                    break;
                }
                bitmap_clone(&array.bitmaps[bitmap1_id], &buffer1);
                result = ops.bitmap_not(&array.bitmaps[bitmap1_id]);
                if (result == 0) {
                    werase(windows.out_win);
                    draw_borders(windows.out_win);
                    mvwprintw(windows.out_win, 1, 1,
                              "Операция NOT выполнена успешно.");
                } else {
                    mvwprintw(windows.out_win, 1, 1,
                              "Произошла ошибка при выполнении операции NOT.");
                    perror("Произошла ошибка при выполнении операции NOT.");
                }
                bitmap_show_ncurses(&buffer1, bitmap1_id, windows.out_win, 2);
                mvwprintw(windows.out_win, 3, 1, "----------------------");
                show_from_array(&array, bitmap1_id, windows.out_win, 4);
                wrefresh(windows.out_win);
                break;

            case 12:
                mvwprintw(windows.input_win, 1, 1,
                          "Введите id первого битмапа, и id второго битмапа: ");
                draw_borders(windows.input_win);
                wrefresh(windows.input_win);
                wscanw(windows.input_win, "%d %d", &bitmap1_id, &bitmap2_id);

                if (!is_valid_bitmap_id(bitmap1_id, &array, windows.out_win) ||
                    !is_valid_bitmap_id(bitmap2_id, &array, windows.out_win)) {
                    break;
                }
                compare_menu(bitmap1_id, bitmap2_id, &array, windows);
                break;

            case 13:
                mvwprintw(windows.input_win, 1, 1,
                          "Введите id битмапа, который хотите сохранить и имя "
                          "файла (до %d символов): ",
                          FILE_NAME_SIZE);
                draw_borders(windows.input_win);
                wrefresh(windows.input_win);
                wscanw(windows.input_win, "%d %s", &bitmap1_id, file_name_out);

                if (!is_valid_bitmap_id(bitmap1_id, &array, windows.out_win)) {
                    break;
                }

                if (bitmap_file_save(&array.bitmaps[bitmap1_id],
                                     file_name_out) == ERROR) {
                    mvwprintw(windows.out_win, 1, 1, "Ошибка записи в файл.");
                    perror("Произошла ошибка при записи битмапа в файл.");
                } else {
                    mvwprintw(windows.out_win, 1, 1,
                              "Битмап успешно сохранен.");
                }
                show_from_array(&array, bitmap1_id, windows.out_win, 2);
                wrefresh(windows.out_win);
                break;

            case 14:
                load_bitmap_menu(windows, &array);
                break;

            case 15:
                mvwprintw(windows.out_win, 1, 1, "Выход...");
                wrefresh(windows.out_win);
                free(array.bitmaps);
                endwin();
                return 0;

            default:
                mvwprintw(windows.out_win, 1, 1, "Неверная операция.");
                wrefresh(windows.out_win);
                break;
        }

        wrefresh(windows.out_win);
    }
    endwin();
    return 0;
}