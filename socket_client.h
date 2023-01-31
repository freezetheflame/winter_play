//
// Created by shenjiawei on 2023/1/19.
//

#ifndef WINTER_PLAY_SOCKET_CLIENT_H
#define WINTER_PLAY_SOCKET_CLIENT_H

#include<stdio.h>
#include <string.h>
#include <stdbool.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

static int num = 1;
HANDLE g_hMutex = NULL;
typedef struct NODE {
    char buf[1024];
    struct NODE *next;
} node;
node *request_queue;
SOCKET server_socket;
int cnt;

node *getLastElement(node *ptr) {
    node *p = ptr;
    while (p->next)p = p->next;
    return p;
}

node *getHead(node *ptr) {
    return ptr->next;
}

void removeHead(node *ptr) {
    ptr->next = ptr->next->next;
}

bool isEmpty(node *ptr) {
    return ptr->next == NULL;
}

void maintainContact() {
    while (1) {
        WaitForSingleObject(g_hMutex, INFINITE);
        if (!isEmpty(request_queue)) {
            node *head = getHead(request_queue);
            int iResult = send(server_socket, head->buf, strlen(head->buf), 0);
            removeHead(request_queue);
            if (iResult == SOCKET_ERROR) {
                printf("send failed: %d\n", WSAGetLastError());
                closesocket(server_socket);
                WSACleanup();
                exit(1);
            }
            node *ans = malloc(sizeof(node));
            memset(ans->buf, '\0', sizeof(ans->buf));
            ans->next = NULL;
            recv(server_socket, ans->buf, 20, 0);
            sscanf(ans->buf, "%d%d%d", &HisKey, &client_num, &num);
           // if (HisKey != 0)printf("The key is %s\n", ans->buf);
            free(ans);
        } else {
            if (num != 2) {
                WaitForSingleObject(g_hMutex, INFINITE);
                char tmp[10];
                send(server_socket, "hello", 5, 0);
                node *ans = malloc(sizeof(node));
                memset(ans->buf, '\0', sizeof(ans->buf));
                ans->next = NULL;
                recv(server_socket, ans->buf, sizeof(ans->buf), 0);
                sscanf(ans->buf, "%s%d%d", tmp, &client_num, &num);
                free(ans);
                //printf("(%s,%d,%d)\n", tmp, client_num, num);
                ReleaseMutex(g_hMutex);
            }
            if (num == 2) {
                ReadyNot = false;
               // printf("I'm done");
            }

        }
        ReleaseMutex(g_hMutex);
    }
}

void StartUpSocket() {
    printf("This is the client.\n");
    WSADATA wsadata;
    int iResult;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        exit(1);
    }
    struct addrinfo *result, *ptr, hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    iResult = getaddrinfo("127.0.0.1", "8080", &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        exit(1);
    }
    server_socket = INVALID_SOCKET;
    ptr = result;
    server_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (server_socket == INVALID_SOCKET) {
        printf("socket() failed: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(server_socket);
        WSACleanup();
        exit(1);
    }
    if (connect(server_socket, ptr->ai_addr, (int) ptr->ai_addrlen) ==
        SOCKET_ERROR) {
        printf("connect() failed.\n");
        system("pause");
        freeaddrinfo(result);
        closesocket(server_socket);
        WSACleanup();
        exit(1);
    }
    freeaddrinfo(result);
    printf("connection established!\n");
    char ss[20];
}

#endif //WINTER_PLAY_SOCKET_CLIENT_H
