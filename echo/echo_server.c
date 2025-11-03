#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#define ECHO_PORT 7777
#define BUFFER_SIZE 1024

volatile sig_atomic_t keep_running = 1;

void handle_signal(int signal) {
    keep_running = 0;
    printf("\nПолучен сигнал завершения. Завершение работы сервера...\n");
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    
    // Установка обработчика сигналов для корректного завершения
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    
    // Создание TCP сокета
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Ошибка создания сокета");
        return 1;
    }
    
    // Настройка опции повторного использования адреса
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Ошибка установки SO_REUSEADDR");
        close(server_fd);
        return 1;
    }
    
    // Заполнение структуры адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Принимаем соединения с любого интерфейса
    server_addr.sin_port = htons(ECHO_PORT);
    
    // Привязка сокета к адресу
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка привязки сокета");
        close(server_fd);
        return 1;
    }
    
    // Начало прослушивания порта
    if (listen(server_fd, 5) < 0) {
        perror("Ошибка прослушивания");
        close(server_fd);
        return 1;
    }
    
    printf("Echo сервер запущен на порту %d\n", ECHO_PORT);
    printf("Ожидание подключений...\n");
    printf("Для завершения работы нажмите Ctrl+C\n\n");
    
    // Основной цикл сервера
    while (keep_running) {
        // Принятие входящего соединения
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        
        if (client_fd < 0) {
            if (errno == EINTR) {
                // Прервано сигналом
                continue;
            }
            perror("Ошибка принятия соединения");
            continue;
        }
        
        // Вывод информации о клиенте
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        printf("Подключен клиент: %s:%d\n", client_ip, ntohs(client_addr.sin_port));
        
        // Обработка данных от клиента
        while ((bytes_read = recv(client_fd, buffer, BUFFER_SIZE - 1, 0)) > 0) {
            // Добавляем нулевой терминатор
            buffer[bytes_read] = '\0';
            
            // Вывод полученных данных
            printf("Получено от %s: %s", client_ip, buffer);
            
            // Отправка данных обратно клиенту (echo)
            if (send(client_fd, buffer, bytes_read, 0) < 0) {
                perror("Ошибка отправки данных");
                break;
            }
            
            printf("Отправлено эхо: %s", buffer);
            
            // Проверка на команду завершения
            if (strncmp(buffer, "quit", 4) == 0 || strncmp(buffer, "exit", 4) == 0) {
                printf("Клиент %s запросил отключение\n", client_ip);
                break;
            }
        }
        
        if (bytes_read == 0) {
            printf("Клиент %s отключился\n", client_ip);
        } else if (bytes_read < 0) {
            perror("Ошибка чтения данных");
        }
        
        // Закрытие соединения с клиентом
        close(client_fd);
        printf("Соединение с клиентом %s закрыто\n\n", client_ip);
    }
    
    // Закрытие серверного сокета
    close(server_fd);
    printf("Сервер остановлен\n");
    
    return 0;
}