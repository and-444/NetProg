# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic
DEBUG_FLAGS = -g
RELEASE_FLAGS = -O2

# Имя исполняемого файла
TARGET = daytime_client

# Исходные файлы
SRCS = daytime_client.c
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

# Установка (копирование в /usr/local/bin)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Удаление установленной программы
uninstall:
	rm -f /usr/local/bin/$(TARGET)

.PHONY: all debug release clean install uninstall
