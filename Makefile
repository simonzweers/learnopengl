CC=/usr/bin/gcc
CXX=/usr/bin/g++

CFLAGS += -Werror -Wall -g
CFLAGS += -Iinclude
LFLAGS=-lglfw

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

cdb: clean
	bear -- make

clean: 
	rm -rf ${EXE} *.o ${BLD}
