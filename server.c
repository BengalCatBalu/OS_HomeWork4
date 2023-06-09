#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>

#define BUFFER_SIZE 1024
#define SECTORS 20

int main(int argc, char *argv[]) {
    srand(time(NULL));
    if (argc != 2) {
        printf("Использование: %s <порт сервера>\n", argv[0]);
        return 1;
    }

    int server_port = atoi(argv[1]);    // Номер порта сервера

    // Создание сокета
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);

    // Параметры адреса сервера
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    // Привязка сокета к IP-адресу и порту
    bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    printf("Сервер запущен и ожидает подключения клиентов...\n");

    int sectors[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};  // Список участков
    int num_sectors = sizeof(sectors) / sizeof(sectors[0]);

    int explored_sectors[num_sectors];
    memset(explored_sectors, 0, sizeof(explored_sectors));
    int random_index = 0;
    while (1) {
        // Ожидание запроса от клиента
        char buffer[BUFFER_SIZE];
        struct sockaddr_in client_address;
        socklen_t client_address_length = sizeof(client_address);
        recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_address, &client_address_length);
        if (strcmp(buffer, "Request") == 0) {
            // Генерация случайного номера участка для клиента
            int sector = sectors[random_index];

            // Отправка номера участка клиенту
            sprintf(buffer, "%d", sector);
            sendto(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_address, client_address_length);

            // Ожидание результата поиска от клиента
            memset(buffer, 0, sizeof(buffer));
            recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_address, &client_address_length);

            int found = atoi(buffer);

            if (found) {
                // Вывод информации о нахождении Винни-Пуха
                printf("Винни-Пух найден на участке %d! Наказание будет применено...\n", sector);

                // Завершение работы сервера и клиента
                break;
            } else {
                // Отметить участок как исследованный
                printf("Винни-Пух не найден на участке %d!\n", sector);
                explored_sectors[random_index] = 1;;
            }
        }
        random_index++;
        if (random_index == num_sectors) {
            printf("Винни-Пух нигде не обнаружен :(\n");
            break;
        }
    }

    // Закрытие сокета сервера
    close(server_socket);

    return 0;
}



