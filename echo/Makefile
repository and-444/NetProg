# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic
DEBUG_FLAGS = -g
RELEASE_FLAGS = -O2

# Имя исполняемого файла
TARGET = echo_server

# Исходные файлы
SRCS = echo_server.c
OBJS = $(SRCS:.c=.o)

# Правила сборки
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -c $< -o $@

# Отладочная сборка
debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

# Релизная сборка
release: CFLAGS += $(RELEASE_FLAGS)
release: $(TARGET)

# Очистка
clean:
	rm -f $(OBJS) $(TARGET)

# Запуск сервера
run: $(TARGET)
	./$(TARGET)

.PHONY: all debug release clean run
