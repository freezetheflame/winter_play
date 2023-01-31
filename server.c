#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tools.h"
#include "map.h"
#include "ball.h"
#include "winter_playh.h"
#include "board.h"
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

DWORD WINAPI ThreadProc(LPVOID lpParam);    /*线程执行函数*/
DWORD WINAPI CheckThreadProc(LPVOID lpParam);
void send_msg(char *msg, int len, int client_sock);            /*消息发送函数*/
void buildConnection(SOCKET *listen_socket);

HANDLE g_hMutex = NULL;
clock_t start, end;
int st = 0, en = 0;
int P1_key,P2_key;
int playernum = 0;
int clnt_cnt = 0;//collect the number
int clnt_socks[5];//manage the sockets
HANDLE Threads[10];//manage the threads
int main() {
    g_hMutex = CreateMutex(NULL, FALSE, NULL);
    WSADATA Wsadata;
    WORD Sockversion = MAKEWORD(2, 2);
    WSAStartup(Sockversion, &Wsadata);
    struct addrinfo *result, *ptr, hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    //TCP协议
    hints.ai_protocol = IPPROTO_TCP;
    //服务端被动绑定
    hints.ai_flags = AI_PASSIVE;
    int iResult = getaddrinfo(NULL, "8080", &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        exit(1);
    }
    //build a listening socket
    SOCKET listen_socket = INVALID_SOCKET;
    listen_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listen_socket == INVALID_SOCKET) {
        printf("socket() failed: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        exit(1);
    }
    //bind the address and release the space of result
    iResult = bind(listen_socket, result->ai_addr, (int) result->ai_addrlen);
    freeaddrinfo(result);
    if (iResult == SOCKET_ERROR) {
        printf("bind() failed: %d\n", WSAGetLastError());
        closesocket(listen_socket);
        WSACleanup();
        exit(1);
    }
    if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR) {
        printf("listen() failed: %d\n", WSAGetLastError());
        closesocket(listen_socket);
        WSACleanup();
        return 1;
    }
    printf("successfully created!");
    buildConnection(&listen_socket);
    return 0;
}

//Thread Processing
DWORD WINAPI ThreadProc(LPVOID lpParam) {
    int clnt_sock = *((int *) lpParam);
    int str_len = 0, i;
    char msg[100];
    char tmp[300];
    char tmpp[5], tmmpp[5];
    char but[10];
    char* ptr;
    int flag = 1;
    int cnt = 0;
    sprintf(tmpp, " %d", clnt_cnt);
    while ((str_len = recv(clnt_sock, msg, sizeof(msg), 0)) > 0) {
        WaitForSingleObject(g_hMutex, INFINITE);
        if (msg[0] == 'h') {
            if(strcmp(tmpp," 1")==0)P1_key=strtol(msg,&ptr,10);
            else  P2_key=strtol(msg,&ptr,10);
            sprintf(tmp, "%s %s", msg, tmpp);
            sprintf(tmmpp, " %d", clnt_cnt);
            strcat(tmp, tmmpp);
            //puts(tmp);
            send(clnt_sock, tmp, sizeof(tmp), 0);
            memset(msg, 0, sizeof(msg));
            memset(tmp, 0, sizeof(tmp));
            memset(tmmpp, 0, sizeof(tmmpp));
        } else {
            //strcat(msg, tmp);
            send_msg(msg, 20, clnt_sock);
            //printf("aaaa");
            memset(msg, '\0', sizeof(msg));
        }
        ReleaseMutex(g_hMutex);
    }
    printf("client exit:%d\n", GetCurrentThreadId());
    clnt_cnt--;
    for (i = 0; i < clnt_cnt; i++) {
        if (clnt_sock == clnt_socks[i]) {
            while (i++ < clnt_cnt - 1)
                clnt_socks[i] = clnt_socks[i + 1];
            break;
        }
    }

    closesocket(clnt_sock);
    WSACleanup();
    exit(1);
    return (DWORD) NULL;
}
DWORD WINAPI CheckThreadProc(LPVOID lpParam)
{
    int clnt_sock = *((int *) lpParam);

}
void buildConnection(SOCKET *listen_socket) {
    while (1) {
        struct sockaddr_in remoteAddr;
        int nAddrLen = sizeof(remoteAddr);
        DWORD dwThreadId;
        //为新客户端套接字分配空间并初始化
        SOCKET *client_socket = malloc(sizeof(SOCKET));
        *client_socket = INVALID_SOCKET;
        *client_socket = accept(*listen_socket, NULL, NULL);
        if (*client_socket == INVALID_SOCKET) {
            printf("accept() failed: %d\n", WSAGetLastError());
            closesocket(*listen_socket);
            WSACleanup();
            exit(1);
        }

        //连接成功建立
        printf("connection established!\n");
        Threads[clnt_cnt] = CreateThread(
                NULL,        // 默认安全属性
                (SIZE_T) NULL,        // 默认堆栈大小
                ThreadProc,    // 线程入口地址（执行线程的函数）
                (void *) client_socket,        // 传给函数的参数
                0,        // 指定线程立即运行
                &dwThreadId);    // 返回线程的ID号
        clnt_socks[clnt_cnt++] = *client_socket;
    }
}

void send_msg(char *msg, int len, int client_sock) {
    int i, iResult;
    for (i = 0; i < clnt_cnt; i++)
        if (clnt_socks[i] != client_sock)iResult = send(clnt_socks[i], msg, len, 0);

}