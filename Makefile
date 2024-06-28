CC=/usr/bin/gcc
CXX=/usr/bin/g++

CFLAGS += -Werror -Wall -g
LFLAGS=

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
EXE=learnopengl

all: ${EXE}

${EXE}: main.o
	${CXX} -o $@ $^ ${CFLAGS}

%.o: %.c
	${CXX} -o $@ $^ -c ${CFLAGS}

clean: 
	rm -rf ${EXE} *.o
