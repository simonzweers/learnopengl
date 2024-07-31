CC=/usr/bin/gcc
CXX=/usr/bin/g++

CFLAGS += -Wall -g
CFLAGS += -Iinclude 
CFLAGS += -Ilib/stb/include 
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

libs: lib/glad lib/stb

lib/glad:
	glad --profile core --out-path $@ --generator c
	$(CC) $@/src/glad.c -o $@/src/glad.o -c $(CFLAGS)

lib/stb:
	@mkdir -p $@
	@mkdir -p $@/include
	curl -s https://raw.githubusercontent.com/nothings/stb/master/stb_image.h -o $@/include/stb_image.h
