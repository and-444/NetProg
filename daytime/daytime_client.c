#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>

#define DAYTIME_PORT 13
#define BUFFER_SIZE 1024
#define TIMEOUT_SEC 5

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Использование: %s <IP-адрес сервера>\n", argv[0]);
        printf("Пример: %s 172.16.40.1\n", argv[0]);
        return 1;
    }

    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(server_addr);
    
    // Создание UDP сокета
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Ошибка создания сокета");
        return 1;
    }
    
    // Настройка таймаута на прием данных
    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_SEC;
    timeout.tv_usec = 0;
    
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("Ошибка установки таймаута");
        close(sockfd);
        return 1;
    }
    
    // Заполнение структуры адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(DAYTIME_PORT);
    
    // Преобразование IP-адреса
    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0) {
        fprintf(stderr, "Ошибка: неверный IP-адрес\n");
        close(sockfd);
        return 1;
    }
    
    printf("Подключение к daytime серверу: %s:%d\n", argv[1], DAYTIME_PORT);
    
    // Отправка пустого датаграммы для запроса времени
    // Согласно RFC 867, для UDP daytime сервер отвечает на любой полученный пакет
    if (sendto(sockfd, NULL, 0, 0, (struct sockaddr*)&server_addr, addr_len) < 0) {
        perror("Ошибка отправки запроса");
        close(sockfd);
        return 1;
    }
    
    printf("Запрос отправлен. Ожидание ответа...\n");
    
    // Прием ответа от сервера
    ssize_t recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0, 
                               (struct sockaddr*)&server_addr, &addr_len);
    
    if (recv_len < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            fprintf(stderr, "Таймаут: сервер не ответил в течение %d секунд\n", TIMEOUT_SEC);
        } else {
            perror("Ошибка приема данных");
        }
        close(sockfd);
        return 1;
    }
    
    // Завершаем строку
    buffer[recv_len] = '\0';
    
    // Вывод полученного времени
    printf("Текущее время сервера:\n%s", buffer);
    
    // Закрытие сокета
    close(sockfd);
    
    return 0;
}