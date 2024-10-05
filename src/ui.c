#include "../include/ui.h"

#include "../include/bitmap.h"

// Создает и инициализирует окна интерфейса.
void create_windows(Windows *windows) {
    int term_height, term_width;
    getmaxyx(stdscr, term_height, term_width);

    windows->main_win = newwin(term_height, term_width, 0, 0);
    draw_borders(windows->main_win);

    int menu_height = term_height - 7;
    windows->menu_win = newwin(menu_height, 35, 1, 1);
    draw_borders(windows->menu_win);
    draw_menu(windows->menu_win);

    windows->input_win = newwin(5, term_width - 2, term_height - 6, 1);
    draw_borders(windows->input_win);

    int out_win_height = term_height - 7;
    windows->out_win = newwin(out_win_height, term_width - 37, 1, 36);
    draw_borders(windows->out_win);
}

// Рисует рамку вокруг заданного окна и обновляет его содержимое на экране.
void draw_borders(WINDOW *win) {
    box(win, 0, 0);
    wrefresh(win);
}

// Выводит меню на экран.
void draw_menu(WINDOW *menu_win) {
    mvwprintw(menu_win, 1, 2, "Меню операций:");
    mvwprintw(menu_win, 2, 2, "1. Создать Bitmap");
    mvwprintw(menu_win, 3, 2, "2. Клонировать Bitmap");
    mvwprintw(menu_win, 4, 2, "3. Копировать часть Bitmap");
    mvwprintw(menu_win, 5, 2, "4. Установить бит");
    mvwprintw(menu_win, 6, 2, "5. Очистить бит");
    mvwprintw(menu_win, 7, 2, "6. Сбросить Bitmap");
    mvwprintw(menu_win, 8, 2, "7. Удалить Bitmap");
    mvwprintw(menu_win, 9, 2, "8. AND Bitmaps");
    mvwprintw(menu_win, 10, 2, "9. OR Bitmaps");
    mvwprintw(menu_win, 11, 2, "10. XOR Bitmaps");
    mvwprintw(menu_win, 12, 2, "11. NOT Bitmap");
    mvwprintw(menu_win, 13, 2, "12. Сравнить Bitmaps");
    mvwprintw(menu_win, 14, 2, "13. Сохранить Bitmap в файл");
    mvwprintw(menu_win, 15, 2, "14. Загрузить Bitmap из файла");
    mvwprintw(menu_win, 16, 2, "15. Выход");
    wrefresh(menu_win);
}

/* Принимает идентификаторы двух битмапов и указатель на структуру BitmapArray.
 * Выводит меню сравнения двух битмапов по их идентификаторам.*/
void compare_menu(int bitmap1_id, int bitmap2_id, BitmapArray *array,
                  Windows windows) {
    if (!is_valid_bitmap_id(bitmap1_id, array, windows.out_win) ||
        !is_valid_bitmap_id(bitmap2_id, array, windows.out_win)) {
        return;
    }

    werase(windows.out_win);
    mvwprintw(windows.out_win, 1, 2, "1-общее сравнение");
    mvwprintw(windows.out_win, 2, 2, "2-меньше");
    mvwprintw(windows.out_win, 3, 2, "3-строго меньше");
    mvwprintw(windows.out_win, 4, 2, "4-равно");
    mvwprintw(windows.out_win, 5, 2, "5-больше");
    mvwprintw(windows.out_win, 6, 2, "6-строго больше");
    for (int i = 0; i < array->size; i++) {
        show_from_array(array, i, windows.out_win, i + 8);
    }
    draw_borders(windows.out_win);
    wrefresh(windows.out_win);

    werase(windows.input_win);
    mvwprintw(windows.input_win, 1, 1, "Выберите операцию:");
    draw_borders(windows.input_win);
    wrefresh(windows.input_win);

    int choice, result;
    wscanw(windows.input_win, "%d", &choice);
    werase(windows.out_win);
    draw_borders(windows.out_win);
    wrefresh(windows.out_win);

    switch (choice) {
        case 1:
            result = bitmap_compare(&array->bitmaps[bitmap1_id],
                                    &array->bitmaps[bitmap2_id]);

            if (result == 0) {
                mvwprintw(windows.out_win, 1, 2, "Битмапы равны.");
            } else if (result == 1) {
                mvwprintw(windows.out_win, 1, 2, "Первый битмап больше.");
            } else if (result == 2) {
                mvwprintw(windows.out_win, 1, 2, "Второй битмап больше.");
            } else {
                mvwprintw(windows.out_win, 1, 2, "Произошла ошибка сравнения.");
                perror("Произошла ошибка сравнения.");
            }
            break;
        case 2:
            result = bitmap_less(&array->bitmaps[bitmap1_id],
                                 &array->bitmaps[bitmap2_id]);

            if (result == 0) {
                mvwprintw(windows.out_win, 1, 2,
                          "Первый битмап НЕ меньше и НЕ равен второму");
            } else if (result == 1) {
                mvwprintw(windows.out_win, 1, 2,
                          "Первый битмап меньше или равен второму");
            } else {
                mvwprintw(windows.out_win, 1, 2, "Произошла ошибка сравнения.");
                perror("Произошла ошибка сравнения.");
            }
            break;
        case 3:
            result = bitmap_strictly_less(&array->bitmaps[bitmap1_id],
                                          &array->bitmaps[bitmap2_id]);

            if (result == 0) {
                mvwprintw(windows.out_win, 1, 2,
                          "Первый битмап НЕ меньше второго");
            } else if (result == 1) {
                mvwprintw(windows.out_win, 1, 2,
                          "Первый битмап меньше второго");
            } else {
                mvwprintw(windows.out_win, 1, 2, "Произошла ошибка сравнения.");
                perror("Произошла ошибка сравнения.");
            }
            break;
        case 4:
            result = bitmap_equal(&array->bitmaps[bitmap1_id],
                                  &array->bitmaps[bitmap2_id]);

            if (result == 1) {
                mvwprintw(windows.out_win, 1, 2, "Битмапы равны.");
            } else {
                mvwprintw(windows.out_win, 1, 2, "Битмапы не равны.");
            }
            break;
        case 5:
            result = bitmap_greater(&array->bitmaps[bitmap1_id],
                                    &array->bitmaps[bitmap2_id]);

            if (result == 0) {
                mvwprintw(windows.out_win, 1, 2,
                          "Первый битмап НЕ больше и НЕ равен второму");
            } else if (result == 1) {
                mvwprintw(windows.out_win, 1, 2,
                          "Первый битмап больше или равен второму");
            } else {
                mvwprintw(windows.out_win, 1, 2, "Произошла ошибка сравнения.");
                perror("Произошла ошибка сравнения.");
            }
            break;
        case 6:
            result = bitmap_strictly_greater(&array->bitmaps[bitmap1_id],
                                             &array->bitmaps[bitmap2_id]);

            if (result == 0) {
                mvwprintw(windows.out_win, 1, 2,
                          "Первый битмап НЕ больше второго");
            } else if (result == 1) {
                mvwprintw(windows.out_win, 1, 2,
                          "Первый битмап больше второго");
            } else {
                mvwprintw(windows.out_win, 1, 2, "Произошла ошибка сравнения.");
                perror("Произошла ошибка сравнения.");
            }
            break;
        default:
            mvwprintw(windows.out_win, 1, 2, "Неверная операция.");
            break;
    }
    show_from_array(array, bitmap1_id, windows.out_win, 2);
    show_from_array(array, bitmap2_id, windows.out_win, 3);
}

/* Принимает указатель на структуру BitmapArray. Дает пользователю выбрать
 * загрузить в существующий битмап с указанием id или создать новый. */
void load_bitmap_menu(Windows windows, BitmapArray *array) {
    werase(windows.input_win);
    mvwprintw(windows.input_win, 1, 1, "1-загрузить в существующий битмап");
    mvwprintw(windows.input_win, 2, 1, "2-создать новый битмап: ");
    draw_borders(windows.input_win);
    wrefresh(windows.input_win);

    int choice;
    wscanw(windows.input_win, "%d", &choice);
    char file_name_in[FILE_NAME_SIZE] = {' '};
    int bitmap1_id;

    if (choice == 1) {
        mvwprintw(
            windows.input_win, 3, 1,
            "Введите id битмапа и имя файла (до %d символов):", FILE_NAME_SIZE);
        wscanw(windows.input_win, "%d %s", &bitmap1_id, file_name_in);

        if (!is_valid_bitmap_id(bitmap1_id, array, windows.out_win)) {
            return;
        }

        if (bitmap_file_load(&array->bitmaps[bitmap1_id], file_name_in) ==
            ERROR) {
            mvwprintw(windows.out_win, 1, 1, "Ошибка загрузки из файла.");
            perror("Ошибка загрузки из файла.");
        } else {
            mvwprintw(windows.out_win, 1, 1, "Битмап успешно загружен.");
            show_from_array(array, bitmap1_id, windows.out_win, 2);
        }

    } else if (choice == 2) {
        mvwprintw(windows.input_win, 3, 1,
                  "Введите имя файла (до %d символов):", FILE_NAME_SIZE);
        wrefresh(windows.input_win);
        wscanw(windows.input_win, "%s", file_name_in);
        Bitmap new_bitmap;
        bitmap_create(&new_bitmap, BYTE_SIZE);
        if (bitmap_file_load(&new_bitmap, file_name_in) == ERROR) {
            mvwprintw(windows.out_win, 1, 1, "Ошибка загрузки из файла.");
            perror("Ошибка загрузки из файла.");
        } else {
            add_bitmap(array, new_bitmap);
            mvwprintw(windows.out_win, 1, 1,
                      "Новый битмап успешно создан и загружен.");
            show_from_array(array, -1, windows.out_win, 2);
        }
    } else {
        mvwprintw(windows.out_win, 1, 1, "Неверный выбор.");
    }
}

/* Функция принимает структуру окон и массив битмапов, обновляет окна интерфейса
 * и запрашивает у пользователя выбор операции. */
int redraw_windows(Windows windows, const BitmapArray *array) {
    int choice;
    draw_menu(windows.menu_win);

    for (int i = 0; i < array->size; i++) {
        show_from_array(array, i, windows.out_win, i + 8);
    }
    wrefresh(windows.out_win);

    werase(windows.input_win);
    mvwprintw(windows.input_win, 1, 1, "Введите номер операции: ");
    draw_borders(windows.input_win);
    wrefresh(windows.input_win);

    wscanw(windows.input_win, "%d", &choice);
    werase(windows.input_win);
    wrefresh(windows.input_win);

    werase(windows.out_win);
    for (int i = 0; i < array->size; i++) {
        show_from_array(array, i, windows.out_win, i + 8);
    }
    draw_borders(windows.out_win);
    wrefresh(windows.out_win);

    return choice;
}

/* Функция принимает указатель на битмап, id битмапа, окно интерфеса и номер
 * строки. Отображает содержимое битмапа в указанном окне, показывая его в виде
 * битовой строки. */
int bitmap_show_ncurses(const Bitmap *bitmap, int id, WINDOW *win, int line) {
    if (!bitmap || !bitmap->bits || bitmap->size == 0) {
        mvwprintw(win, 1, 1, "Bitmap пуст или неинициализирован.");
        perror("Bitmap пуст или неинициализирован.");
        wrefresh(win);
        return ERROR;
    }

    size_t bytes = bits_to_byte(bitmap->size);
    int col = 1;

    wmove(win, line, col);
    wprintw(win, "ID: %d | ", id);

    for (size_t i = bytes; i > 0; --i) {
        if (i == bytes) {
            size_t remaining_bits = bitmap->size % BYTE_SIZE;
            if (remaining_bits == 0) {
                remaining_bits = BYTE_SIZE;
            }

            for (size_t j = remaining_bits; j > 0; --j) {
                wprintw(win, "%d", (bitmap->bits[i - 1] >> (j - 1)) & 1);
            }

            if (remaining_bits != BYTE_SIZE) {
                wprintw(win, " ");
            }
        } else {
            wprintw(
                win, " %d%d%d%d%d%d%d%d", (bitmap->bits[i - 1] >> 7) & 1,
                (bitmap->bits[i - 1] >> 6) & 1, (bitmap->bits[i - 1] >> 5) & 1,
                (bitmap->bits[i - 1] >> 4) & 1, (bitmap->bits[i - 1] >> 3) & 1,
                (bitmap->bits[i - 1] >> 2) & 1, (bitmap->bits[i - 1] >> 1) & 1,
                (bitmap->bits[i - 1] >> 0) & 1);
        }
    }

    wrefresh(win);
    return SUCCESS;
}

/* Функция принимает указатель на массив битмапов, id битмапа, окно интерфеса и
 * номер строки. Создает битмап, после чего отображает ег на экран с помощью
 * функции bitmap_show_ncurses. Если id = -1, функция работает с последним
 * созданым битмапом. */
int show_from_array(const BitmapArray *array, int id, WINDOW *win, int line) {
    if (!array || id < -1 || id >= array->size) {
        mvwprintw(win, 1, 1, "Неверный ID битмапа.");
        perror("Неверный ID битмапа.");
        wrefresh(win);
        return ERROR;
    }

    Bitmap *bitmap;
    if (id == -1) {
        id = array->size;
        bitmap = &array->bitmaps[array->size - 1];
        id--;
    } else {
        bitmap = &array->bitmaps[id];
    }

    if (!bitmap->bits || bitmap->size == 0) {
        mvwprintw(win, 1, 1, "Bitmap пуст или неинициализирован.");
        perror("Bitmap пуст или неинициализирован.");
        wrefresh(win);
        return ERROR;
    }

    bitmap_show_ncurses(bitmap, id, win, line);
    return SUCCESS;
}

/* Принимает идентификатор и указатель на структуру BitmapArray. Возвращает 1,
 * если идентификатор допустим, 0 в противном случае. */
int is_valid_bitmap_id(int id, const BitmapArray *array, WINDOW *win) {
    if (id >= 0 && id < array->size) {
        return 1;
    }
    mvwprintw(win, 1, 1, "Неверный ID битмапа: %d", id);
    perror("Неверный id битмапа.");
    wrefresh(win);
    return 0;
}

/* Инициализирует массив битмапов.
 * Принимает указатель на структуру BitmapArray.*/
void init_bitmap_array(BitmapArray *array) {
    array->bitmaps = NULL;
    array->size = 0;
    array->capacity = 0;
}

/* Изменяет ёмкость массива битмапов. Принимает указатель на структуру
 * BitmapArray и новую ёмкость. */
void resize_bitmap_array(BitmapArray *array, int new_capacity) {
    Bitmap *new_bitmaps =
        (Bitmap *)realloc(array->bitmaps, new_capacity * sizeof(Bitmap));
    if (new_bitmaps != NULL) {
        array->bitmaps = new_bitmaps;
        array->capacity = new_capacity;
    } else {
        perror("Ошибка перераспределения памяти.");
        exit(EXIT_FAILURE);
    }
}

/* Добавляет новую битмап в массив. Принимает указатель на структуру BitmapArray
 * и битмап для добавления. */
void add_bitmap(BitmapArray *array, Bitmap bitmap) {
    if (array->size == array->capacity) {
        int new_capacity;
        if (array->capacity == 0) {
            new_capacity = 1;
        } else {
            new_capacity = array->capacity * 2;
        }
        resize_bitmap_array(array, new_capacity);
    }
    array->bitmaps[array->size++] = bitmap;
}

/* Удаляет битмап из массива по указанному индексу. Принимает указатель на
 * структуру BitmapArray и индекс удаляемого битмапа. */
void remove_bitmap(BitmapArray *array, int index) {
    for (int i = index; i < array->size - 1; i++) {
        array->bitmaps[i] = array->bitmaps[i + 1];
    }
    array->size--;
    if (array->size > 0 && array->size == array->capacity / 4) {
        resize_bitmap_array(array, array->capacity / 2);
    } else if (array->size == 0) {
        free(array->bitmaps);
        array->bitmaps = NULL;
        array->capacity = 0;
    }
}