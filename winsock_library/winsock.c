#pragma comment(lib, "Ws2_32.lib") // �Ǵ� -lws2_32 �����Ϸ� �ɼ� �߰�

#include <stdio.h>
#include <winsock2.h>

#define BUFFER_SIZE 1024

int main()
{
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int select;
    int port;
    int addr_len;
    int bytes_received;
    char buffer[BUFFER_SIZE];

    while (1)
    {
        printf("1. ���� ���� :: 2.���� ���� :: 0.���� :: ");
        fscanf(stdin, "%d", &select);
        getchar();

        switch (select)
        {
        case 0:
            return 0;
        case 1:
            printf("������ ��Ʈ ��ȣ : ");
            break;
        case 2:
            printf("������ ��Ʈ ��ȣ : ");
            break;
        default:
            printf("�߸� �Է� �ϼ̽��ϴ�.\n");
            continue;
        }
        fscanf(stdin, "%d", &port);
        getchar();

        // Winsock �ʱ�ȭ
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        {
            printf("Winsock �ʱ�ȭ ����\n");
            return 1;
        }

        // ���� �ּ� ����
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        server_addr.sin_port = htons(port);

        switch (select)
        {
            ////////////////////////////////////////////////// ���� ���� ���� �κ� //////////////////////////////////////////////////
        case 1:
            // ���� ���� ����
            if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
            {
                printf("���� ���� ���� ����\n");
                getchar();
                return 1;
            }

            // ���ϰ� �ּ� ���ε�
            if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
            {
                printf("���ε� ����\n");
                getchar();
                closesocket(server_socket);
                return 1;
            }

            // Ŭ���̾�Ʈ�� ���� ��û ���
            if (listen(server_socket, 1) == SOCKET_ERROR)
            {
                printf("���� ����\n");
                getchar();
                closesocket(server_socket);
                return 1;
            }

            printf("���� Ȱ��ȭ ��\n");

            // Ŭ���̾�Ʈ�� ���� ��û�� ����
            // Ŭ���̾�Ʈ�κ��� ������ ���� �� ���
            int status = 0;
            while (1)
            {
                if (status == 0)
                {
                    // Ŭ���̾�Ʈ�� ���� ��û�� ����
                    addr_len = sizeof(client_addr);
                    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
                    if (client_socket == INVALID_SOCKET)
                    {
                        printf("Ŭ���̾�Ʈ�� ���� ����\n");
                        closesocket(server_socket);
                        return 1;
                    }
                    else
                        printf("Ŭ���̾�Ʈ ���� �Ϸ� : %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

                    status = 1;
                }

                bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
                if (bytes_received > 0)
                {
                    buffer[bytes_received - 1] = '\0';
                    printf("%d : %s\n", client_socket, buffer);
                }
                else if (bytes_received == 0)
                {
                    printf("Ŭ���̾�Ʈ�� ���� ���� ����\n");
                    status = 0;
                    continue;
                }
                else
                {
                    printf("Ŭ���̾�Ʈ�� ������ ���� ����\n");
                    status = 0;
                    continue;
                }
                Sleep(10);
            }
            break;
            ////////////////////////////////////////////////// ���� ���� �� �κ� //////////////////////////////////////////////////
            ////////////////////////////////////////////// Ŭ���̾�Ʈ ���� ���� �κ� ///////////////////////////////////////////////
        case 2:
            // Ŭ���̾�Ʈ ���� ����
            if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
            {
                printf("Ŭ���̾�Ʈ ���� ���� ����\n");
                break;
            }

            // ������ ����
            if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
            {
                printf("���� ���� ����\n");
                closesocket(client_socket);
                WSACleanup();
                break;
            }
            else
                printf("%s:%d ���� ���� �Ϸ�(���� : 0)\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));

            printf("�߽Ű���");
            while (1)
            {
                printf(" :: ");
                fgets(buffer, sizeof(buffer), stdin);

                if (strstr(buffer, "0") != 0)
                    break;

                int bytes_sent = send(client_socket, buffer, strlen(buffer), 0);
                if (bytes_sent == SOCKET_ERROR)
                    printf("�߽źҰ�");
                else
                    printf("�߽Ű���");
            }
            break;
            ////////////////////////////////////////////// Ŭ���̾�Ʈ ���� �� �κ� //////////////////////////////////////////////////
        }

        // ���� ����
        closesocket(client_socket);
        closesocket(server_socket);

        // Winsock ����
        WSACleanup();

        continue;
    }

    return 0;
}
