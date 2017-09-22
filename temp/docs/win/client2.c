//
// Created by Administrator on 2017/5/27.
//

/*
客户端C2，连接到外网服务器S，并从S的返回信息中得到它想要连接的C1的外网ip和port，然后
C2给C1发送数据包进行连接。
*/
#include<winsock2.h>
#include<stdio.h>

#define PORT 8888
#define BUFFER_SIZE 100

// 调用方式:UDPClient2 10.2.2.2 5050 (外网服务器S的ip和port)
int main(int argc, char *argv[]) {

    WSADATA wsa_data;
    struct sockaddr_in server_addr;
    struct sockaddr_in this_addr;

    this_addr.sin_family = AF_INET;
    this_addr.sin_port = htons(PORT);
    this_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (argc < 3) {
        printf("Usage: client1[server IP address , server Port]\n");
        return -1;
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        printf("Failed to load Winsock.\n");
        return -1;
    }

    // 初始化服务器S信息
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    // 建立与服务器通信的socket和与客户端通信的socket，注意必须绑定到相同端口。
    SOCKET sock_serv = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_serv == INVALID_SOCKET) {
        printf("socket() failed:%d\n", WSAGetLastError());
        return -1;
    }
    if (bind(sock_serv, (LPSOCKADDR) &this_addr, sizeof(this_addr)) == SOCKET_ERROR) {
        printf("bind() failed:%d\n", WSAGetLastError());
        return -1;
    }
    SOCKET sock_cli = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_cli == INVALID_SOCKET) {
        printf("socket() failed:%d\n", WSAGetLastError());
        return -1;
    }

    char buf_send[] = "I am C2";
    char buf_recv[BUFFER_SIZE];
    memset(buf_recv, '\0', sizeof(buf_recv));
    // 暂存接受数据包的来源，在recvfrom中使用
    struct sockaddr_in source_addr;
    // 在recvfrom中使用
    int source_addr_len = sizeof(source_addr);
    // C1的地址信息
    struct sockaddr_in opposite_side_addr;

    int len;

    // C2给S发送数据包
    len = sendto(sock_serv, buf_send, sizeof(buf_send), 0, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (len == SOCKET_ERROR) {
        printf("sendto() failed:%d\n", WSAGetLastError());
        return -1;
    }

    // C2从S返回的数据包中得到C1的外网ip和port
    len = recvfrom(sock_serv, buf_recv, sizeof(buf_recv), 0, (struct sockaddr *) &source_addr, &source_addr_len);
    if (len == SOCKET_ERROR) {
        printf("recvfrom() failed:%d\n", WSAGetLastError());
        return -1;
    }

    // 下面的处理是由于测试环境（本机+两台NAT联网的虚拟机）原因，若在真实环境中不需要这段处理。
    /*
      关闭与服务器通信的socket，并把与C1通信的socket绑定到相同的端口；真实环境中，路由器的NAT会将客户端
      对外的访问从路由器的外网ip某固定端口发送出去，并在此端口接收
    */
    closesocket(sock_serv);
    if (bind(sock_cli, (LPSOCKADDR) &this_addr, sizeof(this_addr)) == SOCKET_ERROR) {
        printf("bind() failed:%d\n", WSAGetLastError());
        return -1;
    }

    char ip[20];
    char port[10];
    int i;
    for (i = 0; i < strlen(buf_recv); i++)
        if (buf_recv[i] != '^')
            ip[i] = buf_recv[i];
        else break;
    ip[i] = '\0';
    int j;
    for (j = i + 1; j < strlen(buf_recv); j++)
        port[j - i - 1] = buf_recv[j];
    port[j - i - 1] = '\0';

    opposite_side_addr.sin_family = AF_INET;
    opposite_side_addr.sin_port = htons(atoi(port));
    opposite_side_addr.sin_addr.s_addr = inet_addr(ip);

    // 下面的处理是由于测试环境（本机+两台NAT联网的虚拟机）原因，若在真实环境中不需要这段处理。
    /*
      此处由于是在本机，ip为127.0.0.1，但是如果虚拟机连接此ip的话，是与虚拟机本机通信，而不是
      真实的本机，真实本机即此实验中充当NAT的设备，ip为10.0.2.2。
    */
    opposite_side_addr.sin_addr.s_addr = inet_addr("10.0.2.2");

    // 设置sock_cli为非阻塞
    unsigned long ul = 1;
    ioctlsocket(sock_cli, FIONBIO, (unsigned long *) &ul);

    //C2向C1不停地发出数据包，得到C1的回应，与C1建立连接
    while (1) {
        Sleep(1000);
        //C2向C1发送数据包
        len = sendto(sock_cli, buf_send, sizeof(buf_send), 0, (struct sockaddr *) &opposite_side_addr,
                     sizeof(opposite_side_addr));
        if (len == SOCKET_ERROR) {
            printf("while sending package to C1 , sendto() failed:%d\n", WSAGetLastError());
            return -1;
        } else {
            printf("successfully send package to C1\n");
        }

        // C2接收C1返回的数据包，说明C1到C2打洞成功，C1可以直接与C2通信了
        len = recvfrom(sock_cli, buf_recv, sizeof(buf_recv), 0, (struct sockaddr *) &source_addr, &source_addr_len);
        if (len == WSAEWOULDBLOCK) {
            continue;//未收到回应
        } else {
            printf("C1 IP:[%s],PORT:[%d]\n", inet_ntoa(source_addr.sin_addr), ntohs(source_addr.sin_port));
            printf("C1 says:%s\n", buf_recv);

        }
    }

    closesocket(sock_cli);

}