#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define DAYTIME_PORT 13
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Использование: %s <IP-адрес сервера>\n", argv[0]);
        printf("Пример: %s 172.16.40.1\n", argv[0]);
        return 1;
    }

    printf("Daytime UDP клиент для сервера: %s\n", argv[1]);
    printf("Реализация будет добавлена в следующем задании...\n");
    
    return 0;
}