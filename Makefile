CC=/usr/bin/gcc
CXX=/usr/bin/g++

CFLAGS += -Werror -Wall -g
CFLAGS += -Iinclude
LFLAGS=

SRC=$(wildcard src/*.cpp)
BLD=build
OBJ=$(SRC:%.cpp=${BLD}/%.o)
EXE=learnopengl

all: ${EXE}

${EXE}: ${OBJ}
	${CXX} -o $@ $^ ${CFLAGS}

${BLD}/%.o: %.cpp
	mkdir -p ${dir $@}
	${CXX} -o $@ $^ -c ${CFLAGS}

cdb: clean
	bear -- make

clean: 
	rm -rf ${EXE} *.o ${BLD}
