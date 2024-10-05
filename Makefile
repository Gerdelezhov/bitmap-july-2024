CC = gcc
CFLAGS = -std=c11 -g -fPIC
GCOVFLAGS = -fprofile-arcs -ftest-coverage
INC = include
SRC = src
TEST = test
HEAD = $(INC)/bitmap.h
PLUGINHEAD = $(INC)/bitmap_plugin.h
TESTFLAGS = -lcheck -lm
PLUGINS = $(SRC)/bitmap_and_op.c \
          $(SRC)/bitmap_or_op.c \
          $(SRC)/bitmap_xor_op.c \
          $(SRC)/bitmap_not_op.c
PLUGINS_OBJS = $(PLUGINS:.c=.o)
PLUGINS_LIBS = $(PLUGINS:.c=.so)
SRCS = $(SRC)/bitmap_command_op.c \
       $(SRC)/bitmap_compare_op.c
OBJS = $(SRCS:.c=.o)
UNIT_TEST = $(TEST)/bitmap_test.c
LOGFILE = build_errors.log
UIHEAD = $(INC)/ui.h
UISRC = $(SRC)/ui.c
UIOBJ = $(UISRC:.c=.o)

ifeq ($(shell uname -s), Linux)
    TESTFLAGS += -lpthread -lrt -lsubunit
endif

.PHONY: all clean static dyn demo plugins test gcov_report style run run_demo

all: static dyn demo plugins

static: clean static_lib build

dyn: clean dyn_lib build

demo: clean static_lib demo_exec build

plugins: $(PLUGINS_LIBS)

static_lib: bitmap.a

bitmap.a: $(OBJS)
	$(AR) rcs $@ $^ 2>> $(LOGFILE)

dyn_lib: libbitmap.so

libbitmap.so: $(OBJS)
	$(CC) -shared -o $@ $^ 2>> $(LOGFILE)

demo_exec: main_demo.o
	$(CC) -o main_demo $^ -L. -l:bitmap.a -ldl 2>> $(LOGFILE)

build: $(UIOBJ) main.o bitmap.a
	$(CC) -o main $^ -L. -l:bitmap.a -lncursesw -ldl 2>> $(LOGFILE)

main.o: $(SRC)/main.c $(HEAD)
	$(CC) $(CFLAGS) -I$(INC) -c -o $@ $< 2>> $(LOGFILE)

main_demo.o: $(SRC)/main_demo.c $(HEAD)
	$(CC) $(CFLAGS) -I$(INC) -c -o $@ $< 2>> $(LOGFILE)

$(SRC)/%.o: $(SRC)/%.c $(HEAD) $(PLUGINHEAD) $(OBJS)
	$(CC) $(CFLAGS) -I$(INC) -c -o $@ $< 2>> $(LOGFILE)

$(SRC)/%.so: $(SRC)/%.c $(HEAD) $(PLUGINHEAD)
	$(CC) $(CFLAGS) $(OBJS) -shared -o $@ $< 2>> $(LOGFILE)

$(UIOBJ): $(UISRC) $(UIHEAD)
	$(CC) $(CFLAGS) -I$(INC) -c -o $@ $(UISRC)

test: build $(PLUGINS_LIBS) bitmap.a $(UNIT_TEST)
	$(CC) $(GCOVFLAGS) $(UNIT_TEST) $(PLUGINS_LIBS) bitmap.a -o test.out -ldl $(TESTFLAGS) 2>> $(LOGFILE)
	./test.out 2>> $(LOGFILE)

gcov_report: build $(PLUGINS_LIBS) bitmap.a $(UNIT_TEST)
	$(CC) $(GCOVFLAGS) $(UNIT_TEST) $(SRCS) -o report.out -ldl $(TESTFLAGS) 2>> $(LOGFILE)
	./report.out 2>> $(LOGFILE)
	lcov -t "gcov_report" -o coverage_table.info -c -d . 2>> $(LOGFILE)
	genhtml -o ./report coverage_table.info 2>> $(LOGFILE)

style:
	clang-format -style="{BasedOnStyle: Google, IndentWidth: 4}" -i $(SRCS) $(INC)/*.h $(SRC)/*.c $(TEST)/*.c

clean:
	rm -rf *.o *.a *.so main main_demo test.out report.out coverage_table.info report *.gcda *.gcno test_load.bin test_save.bin *.log
	rm -rf $(SRC)/*.o $(SRC)/*.so *.bin

run: static $(PLUGINS_LIBS) bitmap.a
	./main 2>> $(LOGFILE)

run_demo: static $(PLUGINS_LIBS) demo
	./main_demo 2>> $(LOGFILE)
