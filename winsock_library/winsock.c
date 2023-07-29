#pragma comment(lib, "Ws2_32.lib") // 또는 -lws2_32 컴파일러 옵션 추가

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
        printf("1. 서버 생성 :: 2.서버 접속 :: 0.종료 :: ");
        fscanf(stdin, "%d", &select);
        getchar();

        switch (select)
        {
        case 0:
            return 0;
        case 1:
            printf("생성할 포트 번호 : ");
            break;
        case 2:
            printf("접속할 포트 번호 : ");
            break;
        default:
            printf("잘못 입력 하셨습니다.\n");
            continue;
        }
        fscanf(stdin, "%d", &port);
        getchar();

        // Winsock 초기화
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        {
            printf("Winsock 초기화 실패\n");
            return 1;
        }

        // 서버 주소 설정
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        server_addr.sin_port = htons(port);

        switch (select)
        {
            ////////////////////////////////////////////////// 서버 생성 시작 부분 //////////////////////////////////////////////////
        case 1:
            // 서버 소켓 생성
            if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
            {
                printf("서버 소켓 생성 실패\n");
                getchar();
                return 1;
            }

            // 소켓과 주소 바인딩
            if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
            {
                printf("바인딩 실패\n");
                getchar();
                closesocket(server_socket);
                return 1;
            }

            // 클라이언트의 연결 요청 대기
            if (listen(server_socket, 1) == SOCKET_ERROR)
            {
                printf("리슨 실패\n");
                getchar();
                closesocket(server_socket);
                return 1;
            }

            printf("서버 활성화 중\n");

            // 클라이언트의 연결 요청을 수락
            // 클라이언트로부터 데이터 수신 및 출력
            int status = 0;
            while (1)
            {
                if (status == 0)
                {
                    // 클라이언트의 연결 요청을 수락
                    addr_len = sizeof(client_addr);
                    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
                    if (client_socket == INVALID_SOCKET)
                    {
                        printf("클라이언트측 연결 실패\n");
                        closesocket(server_socket);
                        return 1;
                    }
                    else
                        printf("클라이언트 연결 완료 : %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

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
                    printf("클라이언트측 정상 연결 해제\n");
                    status = 0;
                    continue;
                }
                else
                {
                    printf("클라이언트측 비정상 연결 해제\n");
                    status = 0;
                    continue;
                }
                Sleep(10);
            }
            break;
            ////////////////////////////////////////////////// 서버 생성 끝 부분 //////////////////////////////////////////////////
            ////////////////////////////////////////////// 클라이언트 생성 시작 부분 ///////////////////////////////////////////////
        case 2:
            // 클라이언트 소켓 생성
            if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
            {
                printf("클라이언트 소켓 생성 실패\n");
                break;
            }

            // 서버에 연결
            if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
            {
                printf("서버 접속 실패\n");
                closesocket(client_socket);
                WSACleanup();
                break;
            }
            else
                printf("%s:%d 서버 접속 완료(종료 : 0)\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));

            printf("발신가능");
            while (1)
            {
                printf(" :: ");
                fgets(buffer, sizeof(buffer), stdin);

                if (strstr(buffer, "0") != 0)
                    break;

                int bytes_sent = send(client_socket, buffer, strlen(buffer), 0);
                if (bytes_sent == SOCKET_ERROR)
                    printf("발신불가");
                else
                    printf("발신가능");
            }
            break;
            ////////////////////////////////////////////// 클라이언트 생성 끝 부분 //////////////////////////////////////////////////
        }

        // 소켓 해제
        closesocket(client_socket);
        closesocket(server_socket);

        // Winsock 정리
        WSACleanup();

        continue;
    }

    return 0;
}
