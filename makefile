# Makefile

# 编译器
CC = gcc

# 编译选项
CFLAGS = -Wall -I./3rdtools/include -I./include

# 链接选项
LDFLAGS = -L./3rdtools/lib -lcurl -lz -lnghttp2

# 目标文件
TARGET = bin/mnist_main

# 源文件
SRCS = src/mnist_download.c src/main.c src/read_mnist_image.c

# 对象文件
OBJS = $(SRCS:.c=.o)
BUILD_OBJS = $(patsubst %.o,build/%.o,$(OBJS))

# 目录
BUILD_DIR = build
BIN_DIR = bin

all: $(TARGET)

$(TARGET): $(BUILD_DIR) $(BIN_DIR) $(BUILD_OBJS)
	$(CC) $(CFLAGS) -o $@ $(BUILD_OBJS) $(LDFLAGS)

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $@

$(BIN_DIR):
	@mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean