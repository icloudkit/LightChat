#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <strings.h>


#define REMOTEPORT 4567
#define REMOTEIP "127.0.0.1"

int mains(int argc, char *argv[]) {
    int s, len;
    struct sockaddr_in addr;
    int addr_len;
    char msg[256];
    int i = 0;

    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("error");
        exit(1);
    } else {
        printf("socket created .\n");
        printf("socked id: %d \n", s);
        printf("remote ip: %s \n", REMOTEIP);
        printf("remote port: %d \n", REMOTEPORT);
    }

    len = sizeof(struct sockaddr_in);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(REMOTEPORT);
    addr.sin_addr.s_addr = inet_addr(REMOTEIP);

    while (1) {
        bzero(msg, sizeof(msg));
        len = read(STDIN_FILENO, msg, sizeof(msg));
        sendto(s, msg, len, 0, &addr, addr_len);
        printf("\nInput message: %s \n", msg);
        /* 这是接收到的信息。 */
        len = recvfrom(s, msg, sizeof(msg), 0, &addr, &addr_len);
        printf("%d :", i);
        i++;
        /* 这是服务器返回的信息。 */
        printf("Received message: %s \n", msg);
    }
}