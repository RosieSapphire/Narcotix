CC=gcc
INC=-Iinclude -I/usr/include/freetype2
LIB=-lglfw -lopenal -lfreetype -lsndfile -lassimp -ldl -lpthread -lm
CORES=-j8

CFLAGS=-std=c99 -Wall -Wextra

SRC=gl.c context.c tex.c font.c ui.c sound.c helpers.c shader.c file.c \
	model.c mesh.c light_point.c material.c animation.c vec2.c vec3.c \
	vec4.c mat4.c quat.c ray.c

OBJ=gl.o context.o tex.o font.o ui.o sound.o helpers.o shader.o file.o \
	model.o mesh.o light_point.o material.o animation.o vec2.o vec3.o \
	vec4.o mat4.o quat.o ray.o

BIN=libnarcotix.a

all: CFLAGS += -Ofast
all: release

release: CFLAGS += -Ofast
release: $(BIN)
	rm -rf *.o

test: release
	$(CC) $(CFLAGS) $(INC) -o test src/test.c -L. -lnarcotix -Llib $(LIB)
	rm -rf *.o
	./test

debug: CFLAGS += -Og -ggdb3 -Wall -Wextra -Werror -D DEBUG
debug: $(BIN)
	rm -rf *.o

testd: debug
	$(CC) $(CFLAGS) $(INC) -o test src/test.c -L. -lnarcotix -Llib $(LIB)
	rm -rf *.o
	gdb ./test --tui

$(BIN): $(OBJ)
	ar rcs $(BIN) $^
	rm -rf *.o
	@echo "COMPILED SUCCESSFULLY"

%.o: src/%.c
	$(CC) $(CFLAGS) $(INC) -c $^ $(LIB)

clean:
	clear
	rm -rf $(BIN) test *.o src/*.orig include/*.orig
