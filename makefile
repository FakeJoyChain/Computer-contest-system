CC=gcc
TARGET=main.exe
TARGET_FILE=main.c
INCLUDE_PATH=./include
LIB_PATH=./lib
SOURCE_CODE_PATH=./src

vpath %c src
vpath %h include 

${TARGET}:${TARGET_FILE} competitor.o
	${CC} -g -Wall -I${INCLUDE_PATH}/ ${TARGET_FILE} ${LIB_PATH}/competitor.o \
	-o ${TARGET} 

competitor.o: competitor.h competitor.c
	${CC} -c -Wall -I${INCLUDE_PATH}/ ${SOURCE_CODE_PATH}/competitor.c \
	-o ${LIB_PATH}/competitor.o 

run:
	./${TARGET}
