CC=gcc
INC=-Iinclude -I/usr/include/freetype2
LIB=-lglfw -lopenal -lfreetype -lsndfile -lassimp -ldl -lpthread -lm
CORES=-j8

CFLAGS=-std=c99

SRC=glad.c context.c tex.c font.c ui.c sound.c helpers.c shader.c file.c \
	model.c mesh.c light_point.c material.c animation.c vec2.c vec3.c \
	vec4.c mat4.c quat.c ray.c

OBJ=glad.o context.o tex.o font.o ui.o sound.o helpers.o shader.o file.o \
	model.o mesh.o light_point.o material.o animation.o vec2.o vec3.o \
	vec4.o mat4.o quat.o ray.o

BIN=libnarcotix.a

all: CFLAGS += -O2 -Wall -Wextra
all: release

release: CFLAGS += -O2 -Wall -Wextra
release: $(BIN)
	clear
	rm -rf *.o

debug: CFLAGS += -ggdb3 -Wall -Wextra -Werror -D DEBUG
debug: $(BIN)
	clear
	rm -rf *.o

test: CFLAGS += -ggdb3 -Wall -Wextra -Werror -D DEBUG
test:
	rm -f $(BIN)

test: $(BIN)
	clear
	$(CC) $(CFLAGS) $(INC) -o test src/test.c -L. -lnarcotix -Llib $(LIB)
	rm -rf *.o
	gdb ./test --tui

run: CFLAGS += -O2 -Wall -Wextra
run:
	clear
	make clean
	make test -j8
	./test

$(BIN): $(OBJ)
	ar rcs $(BIN) $^
	rm -rf *.o
	@echo "COMPILED SUCCESSFULLY"

%.o: src/%.c
	$(CC) $(CFLAGS) $(INC) -c $^ $(LIB)

clean:
	rm -rf $(BIN) test *.o src/*.orig include/*.orig
	clear
