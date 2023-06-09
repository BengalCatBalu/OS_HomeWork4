#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    srand(time(NULL));
    if (argc != 3) {
        printf("Использование: %s <IP-адрес сервера> <порт сервера>\n", argv[0]);
        return 1;
    }

    char *server_ip_address = argv[1];  // IP-адрес сервера
    int server_port = atoi(argv[2]);    // Номер порта сервера

    // Создание сокета
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);

    // Параметры адреса сервера
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    server_address.sin_addr.s_addr = inet_addr(server_ip_address);

    printf("Клиент запущен. Подключение к серверу...\n");

    while (1) {
        // Отправка запроса на получение задания
        char buffer[BUFFER_SIZE] = "Request";
        sendto(client_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_address, sizeof(server_address));

        // Ожидание ответа от сервера
        memset(buffer, 0, sizeof(buffer));
        recvfrom(client_socket, buffer, sizeof(buffer), 0, NULL, NULL);

        int sector = atoi(buffer);

        // Вывод информации о номере участка, который получил клиент
        printf("Получено задание. Исследовать сектор: %d\n", sector);

        // Задержка на исследование участка (3 секунды)
        sleep(rand() % 5);

        // Проверка условия нахождения Винни-Пуха (для тестирования, пусть всегда будет 10% шанс)
        int found = (rand() % 100) < 5;
        // Отправка результата поиска серверу
        sprintf(buffer, "%d", found);
        sendto(client_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_address, sizeof(server_address));

        if (found) {
            // Вывод информации о нахождении Винни-Пуха
            printf("Винни-Пух найден на участке %d! Отправка сообщения серверу...\n", sector);
            break;
        }
    }

    // Закрытие сокета клиента
    close(client_socket);

    return 0;
}


