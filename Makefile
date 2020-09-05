
CC = gcc

DIR_SRC = ./src
DIR_OBJ = ./build/obj
DIR_BIN = ./build/bin

# C 文件
SRC = $(wildcard ${DIR_SRC}/*.c)
# O 后缀文件
OBJ = $(patsubst %.c,${DIR_OBJ}/%.o,$(notdir ${SRC}))

# 输出可执行文件名
TARGET = myurl
BIN_TARGET = ${DIR_BIN}/$(TARGET)


${BIN_TARGET}:${OBJ}
	@mkdir -p $(@D)
	$(CC) $(OBJ) -o $@

${DIR_OBJ}/%.o:${DIR_SRC}/%.c
	@mkdir -p $(@D)
	$(CC) -c $< -o $@

.PHONY:clean
clean:
	find ${DIR_OBJ} -name *.o -exec rm -rf {}
