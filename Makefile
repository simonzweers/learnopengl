CC=/usr/bin/gcc
CXX=/usr/bin/g++

CFLAGS += -Werror -Wall -g
CFLAGS += -Iinclude
LFLAGS=-lglfw

SRC=$(wildcard src/*.cpp) $(wildcard src/*.c)
BLD=build
OBJ=$(SRC:%.cpp=${BLD}/%.o)
EXE=learnopengl

all: ${EXE}

${EXE}: ${OBJ}
	${CXX} -o $@ $^ ${CFLAGS} ${LFLAGS}

${BLD}/%.o: %.cpp
	mkdir -p ${dir $@}
	${CXX} -o $@ $^ -c ${CFLAGS}

cdb: clean
	bear -- make

clean: 
	rm -rf ${EXE} *.o ${BLD}
