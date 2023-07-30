#pragma comment(lib, "Ws2_32.lib") // 또는 -lws2_32 컴파일러 옵션 추가
#define _WINSOCK_DEPRECATED_NO_WARNINGS // Visual Studio IDE 옵션 추가

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
		printf("1. 서버 생성 :: 2.서버 접속 :: 0.종료 :: ");
		fscanf(stdin, "%d", &select);
		getchar();

		switch (select)
		{
		case 0:
			return 0;
		case 1:
			printf("생성 포트 번호 : ");
			break;
		case 2:
			printf("접속 포트 번호 : ");
			break;
		default:
			printf("잘못 입력 하였습니다.\n");
			continue;
		}
		fscanf(stdin, "%d", &port);
		getchar();

		// Winsock 초기화
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		{
			printf("Winsock 초기화 실패\n");
			break;
		}

		// 서버 주소 설정
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		server_addr.sin_port = htons(port);

		switch (select)
		{
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
		case 1:
			// 서버 소켓 생성
			if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
			{
				printf("서버 소켓 생성 실패\n");
				break;
			}

			// 다중 클라이언트 처리 설정(mode = 1 // accept 블로킹 해제)
			ioctlsocket(server_socket, FIONBIO, &mode);

			// 소켓과 주소 바인딩
			if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
			{
				printf("바인딩 실패\n");
				break;
			}

			// 클라이언트의 연결 요청 대기
			if (listen(server_socket, 1) == SOCKET_ERROR)
			{
				printf("리슨 실패\n");
				break;
			}

			client_len = sizeof(client_addr);

			printf("서버 활성화 :: 클라이언트 접속 대기 중\n");

			while (1)
			{
				// 클라이언트의 연결 요청을 수락
				new_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);

				for (i = 0; i < CLIENT_MAX; i++)
				{
					if (new_socket != INVALID_SOCKET && client_status[i] != 1)
					{
						client_socket[i] = new_socket;
						client_status[i] = 1;
						printf("%s:%d 클라이언트 %lld 연결\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), client_socket[i]);
						break;
					}
				}

				for (i = 0; i < CLIENT_MAX; i++)
				{
					if (client_status[i] == 1)
					{
						// 클라이언트로부터 데이터 수신 및 출력
						bytes_received = recv(client_socket[i], buffer, BUFFER_SIZE, 0);
						if (bytes_received > 0)
						{
							buffer[bytes_received - 1] = '\0';
							printf("클라이언트 %lld : %s\n", client_socket[i], buffer);
						}
						if (bytes_received == 0)
						{
							closesocket(client_socket[i]);
							client_status[i] = 0;
							printf("%s:%d 클라이언트 %lld 연결 해제\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), client_socket[i]);
						}
					}
				}
			}
			break;
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
		case 2:
			// 클라이언트 소켓 생성
			if ((client_socket[0] = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
			{
				printf("클라이언트 소켓 생성 실패\n");
				break;
			}

			// 서버에 연결
			if (connect(client_socket[0], (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
			{
				printf("서버 접속 실패\n");
				break;
			}

			printf("%s:%d 서버 접속 완료(종료 : 0)\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));

			while (1)
			{
				printf(":: ");
				fgets(buffer, sizeof(buffer), stdin);

				// 클라이언트 종료
				if (strstr(buffer, "0") != 0)
					break;

				// 서버로 데이터 송신
				int bytes_sent = send(client_socket[0], buffer, strlen(buffer), 0);
				if (bytes_sent == SOCKET_ERROR)
				{
					printf("서버와의 접속이 끊어졌습니다.\n");
					break;
				}
			}
			break;
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////
		}

		// 소켓 해제
		if (select == 1)
			closesocket(server_socket);
		else
			closesocket(client_socket[0]);

		// Winsock 정리
		WSACleanup();

		continue;
	}

	return 0;
}
