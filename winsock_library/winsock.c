#pragma comment(lib, "Ws2_32.lib") // �Ǵ� -lws2_32 �����Ϸ� �ɼ� �߰�
#define _WINSOCK_DEPRECATED_NO_WARNINGS // Visual Studio IDE �ɼ� �߰�

#include <stdio.h>
#include <winsock2.h>

#define CLIENT_MAX 10
#define BUFFER_SIZE 1024

int main(int argc, char* argv[])
{
	WSADATA wsa;
	SOCKET server_socket, client_socket[CLIENT_MAX], new_socket;
	struct sockaddr_in server_addr, client_addr;
	unsigned long mode = 1;
	int i, select, port, client_len, bytes_received;
	char client_status[CLIENT_MAX], buffer[BUFFER_SIZE];

	if (argc == 1)
	{
		system("start winsock.exe 1");
		system("start winsock.exe 2");
	}

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
			printf("���� ��Ʈ ��ȣ : ");
			break;
		case 2:
			printf("���� ��Ʈ ��ȣ : ");
			break;
		default:
			printf("�߸� �Է� �Ͽ����ϴ�.\n");
			continue;
		}
		fscanf(stdin, "%d", &port);
		getchar();

		// Winsock �ʱ�ȭ
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		{
			printf("Winsock �ʱ�ȭ ����\n");
			break;
		}

		// ���� �ּ� ����
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		server_addr.sin_port = htons(port);

		switch (select)
		{
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
		case 1:
			// ���� ���� ����
			if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
			{
				printf("���� ���� ���� ����\n");
				break;
			}

			// ���� Ŭ���̾�Ʈ ó�� ����(mode = 1 // accept ���ŷ ����)
			ioctlsocket(server_socket, FIONBIO, &mode);

			// ���ϰ� �ּ� ���ε�
			if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
			{
				printf("���ε� ����\n");
				break;
			}

			// Ŭ���̾�Ʈ�� ���� ��û ���
			if (listen(server_socket, 1) == SOCKET_ERROR)
			{
				printf("���� ����\n");
				break;
			}

			client_len = sizeof(client_addr);

			printf("���� Ȱ��ȭ :: Ŭ���̾�Ʈ ���� ��� ��\n");

			while (1)
			{
				// Ŭ���̾�Ʈ�� ���� ��û�� ����
				new_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);

				for (i = 0; i < CLIENT_MAX; i++)
				{
					if (new_socket != INVALID_SOCKET && client_status[i] != 1)
					{
						client_socket[i] = new_socket;
						client_status[i] = 1;
						printf("%s:%d Ŭ���̾�Ʈ %lld ����\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), client_socket[i]);
						break;
					}
				}

				for (i = 0; i < CLIENT_MAX; i++)
				{
					if (client_status[i] == 1)
					{
						// Ŭ���̾�Ʈ�κ��� ������ ���� �� ���
						bytes_received = recv(client_socket[i], buffer, BUFFER_SIZE, 0);
						if (bytes_received > 0)
						{
							buffer[bytes_received - 1] = '\0';
							printf("Ŭ���̾�Ʈ %lld : %s\n", client_socket[i], buffer);
						}
						if (bytes_received == 0)
						{
							closesocket(client_socket[i]);
							client_status[i] = 0;
							printf("%s:%d Ŭ���̾�Ʈ %lld ���� ����\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), client_socket[i]);
						}
					}
				}
			}
			break;
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
		case 2:
			// Ŭ���̾�Ʈ ���� ����
			if ((client_socket[0] = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
			{
				printf("Ŭ���̾�Ʈ ���� ���� ����\n");
				break;
			}

			// ������ ����
			if (connect(client_socket[0], (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
			{
				printf("���� ���� ����\n");
				break;
			}

			printf("%s:%d ���� ���� �Ϸ�(���� : 0)\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));

			while (1)
			{
				printf(":: ");
				fgets(buffer, sizeof(buffer), stdin);

				// Ŭ���̾�Ʈ ����
				if (strstr(buffer, "0") != 0)
					break;

				// ������ ������ �۽�
				int bytes_sent = send(client_socket[0], buffer, strlen(buffer), 0);
				if (bytes_sent == SOCKET_ERROR)
				{
					printf("�������� ������ ���������ϴ�.\n");
					break;
				}
			}
			break;
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
		}

		// ���� ����
		if (select == 1)
			closesocket(server_socket);
		else
			closesocket(client_socket[0]);

		// Winsock ����
		WSACleanup();

		continue;
	}

	return 0;
}
