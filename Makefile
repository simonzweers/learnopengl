CC=/usr/bin/gcc
CXX=/usr/bin/g++

CFLAGS += -Werror -Wall -g
CFLAGS += -Iinclude 
CFLAGS += -Ilib/glad/include

LFLAGS=-lglfw lib/glad/src/glad.o

SRC=$(wildcard src/*.cpp) $(wildcard src/*.c)
BLD=build
OBJ=$(SRC:%=${BLD}/%.o)
EXE=learnopengl

.PHONY: clean libs cdb all

all: ${EXE}

${EXE}: ${OBJ}
	${CXX} -o $@ $^ ${CFLAGS} ${LFLAGS}

${BLD}/%.cpp.o: %.cpp
	mkdir -p ${dir $@}
	${CXX} -o $@ $^ -c ${CFLAGS}

${BLD}/%.c.o: %.c
	mkdir -p ${dir $@}
	${CC} -o $@ $^ -c ${CFLAGS}

cdb: clean libs
	bear -- make

clean: 
	rm -rf ${EXE} *.o ${BLD} lib/

libs: lib/glad

lib/glad:
	glad --profile core --out-path $@ --generator c
	$(CC) $@/src/glad.c -o $@/src/glad.o -c $(CFLAGS)
