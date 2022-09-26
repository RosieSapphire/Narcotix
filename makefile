CC=gcc
INC=-Iinclude -I/usr/include/freetype2
LIB=-lglfw -lcglm -lopenal -lfreetype -lsndfile -lassimp -ldl -lpthread -lm
CORES=-j8

CFLAGS=-std=c99

SRC=glad.c renderer.c texture.c font.c ui.c sound_engine.c sound.c model.c helpers.c screen_buffer.c shader.c file.c mesh.c light_point.c material.c
OBJ=glad.o renderer.o texture.o font.o ui.o sound_engine.o sound.o model.o helpers.o screen_buffer.o shader.o file.o mesh.o light_point.o material.o

BIN=libnarcotix.a

all:
	make clean
	make release

release: CFLAGS += -O2 -Wall -Wextra
release: $(BIN)

debug: CFLAGS += -ggdb3 -Wall -Wextra -Werror -D DEBUG
debug: $(BIN)

test: CFLAGS += -ggdb3 -Wall -Wextra -Werror -D DEBUG
test: $(BIN)
	$(CC) $(CFLAGS) $(INC) -o test src/test.c -L. -lnarcotix -Llib $(LIB)

$(BIN): $(OBJ)
	ar rcs $(BIN) $^
	rm -rf *.o
	@echo "COMPILED SUCCESSFULLY"

%.o: src/%.c
	$(CC) $(CFLAGS) $(INC) -c $^ $(LIB)

clean:
	rm -rf $(BIN) test *.o src/*.orig include/*.orig
	clear
