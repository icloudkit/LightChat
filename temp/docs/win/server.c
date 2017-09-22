//
// Created by Administrator on 2017/5/27.
//

/*
某局域网内客户端C1先与外网服务器S通信，S记录C1经NAT设备转换后在外网中的ip和port；
然后另一局域网内客户端C2与S通信，S记录C2经NAT设备转换后在外网的ip和port；S将C1的
外网ip和port给C2，C2向其发送数据包；S将C2的外网ip和port给C1，C1向其发送数据包，打
洞完成，两者可以通信。(C1、C2不分先后)

测试假设C1、C2已确定是要与对方通信，实际情况下应该通过C1给S的信息和C2给S的信息，S
判断是否给两者搭桥。（因为C1可能要与C3通信，此时需要等待C3的连接，而不是给C1和
C2搭桥）

编译：gcc UDPServer.c -o UDPServer -lws2_32
*/
#include <Winsock2.h>
#include <stdio.h>

#define DEFAULT_PORT 5050
#define BUFFER_SIZE 100

int main() {

    // server即外网服务器
    int server_port = DEFAULT_PORT;
    WSADATA wsa_data;
    SOCKET server_sock;
    struct sockaddr_in server_addr;

    // 检查协议栈
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        printf("Failed to load Winsock.\n");
        return -1;
    }

    // 建立监听socket
    server_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_sock == INVALID_SOCKET) {
        printf("socket() failed:%d\n", WSAGetLastError());
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_sock, (LPSOCKADDR) &server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("bind() failed:%d\n", WSAGetLastError());
        return -1;
    }

    // 接收来自客户端的连接，source1即先连接到S的客户端C1
    struct sockaddr_in source_addr_1;
    int source_addr_len_1 = sizeof(source_addr_1);
    SOCKET sock_c1 = socket(AF_INET, SOCK_DGRAM, 0);
    char buf_recv_1[BUFFER_SIZE];
    int len;

    len = recvfrom(server_sock, buf_recv_1, sizeof(buf_recv_1), 0, (struct sockaddr *) &source_addr_1, &source_addr_len_1);
    if (len == SOCKET_ERROR) {
        printf("recv() failed:%d\n", WSAGetLastError());
        return -1;
    }

    printf("C1 IP:[%s],PORT:[%d]\n", inet_ntoa(source_addr_1.sin_addr), ntohs(source_addr_1.sin_port));

    // 接收来自客户端的连接，source2即后连接到S的客户端C2
    struct sockaddr_in source_addr_2;
    int sourceAddrLen2 = sizeof(source_addr_2);
    SOCKET sock_c2 = socket(AF_INET, SOCK_DGRAM, 0);
    char buf_recv_2[BUFFER_SIZE];

    len = recvfrom(server_sock, buf_recv_2, sizeof(buf_recv_2), 0, (struct sockaddr *) &source_addr_2, &sourceAddrLen2);
    if (len == SOCKET_ERROR) {
        printf("recv() failed:%d\n", WSAGetLastError());
        return -1;
    }

    printf("C2 IP:[%s],PORT:[%d]\n", inet_ntoa(source_addr_2.sin_addr), ntohs(source_addr_2.sin_port));

    // #向C1发送C2的外网ip和port
    // buf_send_1中存储C2的外网ip和port
    char buf_send_1[BUFFER_SIZE];
    memset(buf_send_1, '\0', sizeof(buf_send_1));
    // C2的ip
    char *ip2 = inet_ntoa(source_addr_2.sin_addr);
    // C2的port
    char port2[10];
    // 10代表10进制
    itoa(ntohs(source_addr_2.sin_port), port2, 10);
    for (int i = 0; i < strlen(ip2); i++) {
        buf_send_1[i] = ip2[i];
    }
    buf_send_1[strlen(ip2)] = '^';
    for (int i = 0; i < strlen(port2); i++) {
        buf_send_1[strlen(ip2) + 1 + i] = port2[i];
    }

    len = sendto(sock_c1, buf_send_1, sizeof(buf_send_1), 0, (struct sockaddr *) &source_addr_1, sizeof(source_addr_1));
    if (len == SOCKET_ERROR) {
        printf("send() failed:%d\n", WSAGetLastError());
        return -1;
    } else if (len == 0) {
        return -1;
    } else {
        printf("send() byte:%d\n", len);
    }

    // 向C2发送C1的外网ip和port
    // bufSend2中存储C1的外网ip和port
    char buf_send_2[BUFFER_SIZE];
    memset(buf_send_2, '\0', sizeof(buf_send_2));
    // C1的ip
    char *ip1 = inet_ntoa(source_addr_1.sin_addr);
    // C1的port
    char port1[10];
    itoa(ntohs(source_addr_1.sin_port), port1, 10);
    for (int i = 0; i < strlen(ip1); i++) {
        buf_send_2[i] = ip1[i];
    }
    buf_send_2[strlen(ip1)] = '^';
    for (int i = 0; i < strlen(port1); i++) {
        buf_send_2[strlen(ip1) + 1 + i] = port1[i];
    }

    len = sendto(sock_c2, buf_send_2, sizeof(buf_send_2), 0, (struct sockaddr *) &source_addr_2, sizeof(source_addr_2));
    if (len == SOCKET_ERROR) {
        printf("send() failed:%d\n", WSAGetLastError());
        return -1;
    } else if (len == 0) {
        return -1;
    } else {
        printf("send() byte:%d\n", len);
    }

    //server的中间人工作已完成，退出即可，剩下的交给C1与C2相互通信
    closesocket(server_sock);
    closesocket(sock_c1);
    closesocket(sock_c2);
    WSACleanup();

    return 0;
}