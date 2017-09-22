#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    // 服务器端套接字
	int server_sockfd;
	// 客户端套接字
	int client_sockfd;
	int len;
	// 服务器网络地址结构体
	struct sockaddr_in my_addr;
	// 客户端网络地址结构体
	struct sockaddr_in remote_addr;
	int sin_size;

	// 数据传送的缓冲区
	char buf[BUFSIZ];
	// 数据初始化 清零
	memset(&my_addr, 0, sizeof(my_addr));
	// 设置为IP通信
	my_addr.sin_family = AF_INET;
	// 服务器IP地址 允许连接到所有本地地址上
	my_addr.sin_addr.s_addr = INADDR_ANY;
	// 服务器端口号
	my_addr.sin_port = htons(8000);

	/* 创建服务器端套接字 IPv4协议，面向连接通信，TCP协议 */
	if((server_sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		return 1;
	}

    /* 将套接字绑定到服务器的网络地址上 */
	if (bind(server_sockfd, (struct sockaddr*) &my_addr, sizeof(struct sockaddr)) < 0)
	{
		perror("bind");
		return 1;
	}

	/* 监听连接请求--监听队列长度为 5 */
	listen(server_sockfd, 5);

	sin_size = sizeof(struct sockaddr_in);

	/* 等待客户端连接请求到达 */
	if((client_sockfd = accept(server_sockfd, (struct sockaddr*) &remote_addr, &sin_size)) < 0)
	{
		perror("accept");
		return 1;
	}
	printf("accept client %s\n", inet_ntoa(remote_addr.sin_addr));

	// 发送欢迎信息
	len=send(client_sockfd, "Welcome to my server\n", 21, 0);

	/* 接收客户端的数据并将其发送给客户端 recv返回接收到的字节数，send返回发送的字节数 */
	while((len = recv(client_sockfd, buf, BUFSIZ, 0)) > 0)
	{
		buf[len] = '\0';
		printf("%s\n", buf);
		if(send(client_sockfd, buf, len, 0) < 0)
		{
			perror("write");
			return 1;
		}
	}
	close(client_sockfd);
	close(server_sockfd);
    return 0;
}

------------------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	int client_sockfd;
	int len;
	// 服务器端网络地址结构体
	struct sockaddr_in remote_addr;
	// 数据传送的缓冲区
	char buf[BUFSIZ];
	// 数据初始化--清零
	memset(&remote_addr, 0, sizeof(remote_addr));
	// 设置为IP通信
	remote_addr.sin_family = AF_INET;
	//服务器IP地址
	remote_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//服务器端口号
	remote_addr.sin_port = htons(8000);

	/* 创建客户端套接字 IPv4协议，面向连接通信，TCP协议 */
	if((client_sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		return 1;
	}

	/* 将套接字绑定到服务器的网络地址上 */
	if(connect(client_sockfd, (struct sockaddr*) &remote_addr, sizeof(struct sockaddr)) < 0)
	{
		perror("connect");
		return 1;
	}
	printf("connected to server\n");
	// 接收服务器端信息
	len = recv(client_sockfd, buf, BUFSIZ, 0);
    buf[len] = '\0';
    // 打印服务器端信息
	printf("%s", buf);

	/* 循环的发送接收信息并打印接收信息 recv返回接收到的字节数，send返回发送的字节数 */
	while(1)
	{
		printf("Enter string to send:");
		scanf("%s", buf);
		if(!strcmp(buf, "quit"))
			break;
		len=send(client_sockfd, buf, strlen(buf), 0);
		len=recv(client_sockfd, buf, BUFSIZ, 0);
		buf[len] = '\0';
		printf("received:%s\n", buf);
	}
	// 关闭套接字
	close(client_sockfd);
    return 0;
}


------------------------------------------------------------------------------------------------------------------------

linux中read，write和recv，send

1、recv和send函数提供了和read和write差不多的功能，但是他们提供了第四个参数来控制读写操作。

int recv(int sockfd,void *buf,int len,int flags)
int send(int sockfd,void *buf,int len,int flags)

前面的三个参数和read,write相同，第四个参数能够是0或是以下的组合：
| MSG_DONTROUTE | 不查找路由表                   |
| MSG_OOB       | 接受或发送带外数据              |
| MSG_PEEK      | 查看数据,并不从系统缓冲区移走数据 |
| MSG_WAITALL   | 等待任何数据                   |

如果flags为0，则和read,write一样的操作。

2、在unix中，所有的设备都可以看成是一个文件，所以我们可以用read来读取socket数据。

// 发送欢迎信息
len = send(client_sockfd, "Welcome to my server\n", 21, 0);

// 接收客户端的数据并将其发送给客户端 recv返回接收到的字节数，send返回发送的字节数
while((len = recv(client_sockfd, buf, BUFSIZ, 0)) > 0) {
    buf[len] = '\0';
    printf("%s\n", buf);
    if(send(client_sockfd, buf, len, 0) < 0) {
        perror("write");
        return 1;
    }
}
------------------------------------------------------------------------------------------------------------------------
// 预处理命令

/* 条件编译，#if、#else、#elif和#endif指令 */

/*
第一种形式：
#if defined(或者是ifdef)<标识符>
<程序段>
[
 #else
 <程序段>
]
#endif
*/

/*
第二种形式：
#if !defined(或者是ifndef)<标识符>
<程序段>
[
#else
<程序段>
]
#endif
*/

/*
第三种形式：
#ifdef <标识符>
    [
    #elif
    <程序段>
    ]
    [
    #elif
    <程序段>
    ]
#else
<程序段>
#endif
*/

#if WINDOWS

#endif

/* if not define 的简写，先测试x是否被宏定义过 */
#ifndef X
    // 宏定义
    #define X 10;
#endif

/* #undef 就是取消一个宏的定义，之后这个宏所定义无效；但是可以重新使用#define进行定义 */
#undef TYPE_BASIC_DEF

#ifndef CONST_NAME
#error Sorry, an error has occurred!
#endif

/* #import不会引起交叉编译的问题，如果用#include引入头文件，会导致交叉编译 */
/* #include <文件名> 标准方式 #include "文件名" 用户方式 */


/*
// #pragma para 设定编译器的状态或者是指示编译器完成一些特定的动作
// #pragma message("消息文本") 在编译输出窗口中将消息文本打印出来

#ifdef _X86
#pragma message("X86 macro activated!");
#endif

// #pragma code_seg([[{ push|pop},][identifier,]]["segment-name"[,"segment-class"])
// 将函数存放在.my_data1节中 #pragma code_seg(".my_data1")

// #pragma once 保证头文件被编译一次
// #pragma hdrstop 预编译头文件到此为止，后面的头文件不进行预编译
// #pragma warning 允许有选择性的修改编译器的警告消息的行为

// #pragma comment 格式 #pragma comment("comment-type"[,commentstring]) 将一个注释记录放入一个对象文件或可执行文件中
// comment-type 注释类型 (compiler,exestr,dumpbin,lib,linker)

// #progma pack(n) 指定结构体对齐方式 #pragma pack(4) 设定为4字节对齐
*/

/*
// #line 强制编译器按指定的行号，开始对源程序的代码重新编号，在调试的时候，可以按此规定输出错误代码的准确位置
// #line constant "filename" 使得其后的源代码从指定的行号constant重新开始编号，并将当前文件的名命名为filename
*/

// 运算符#和## #实现文本替换 ##的作用是串连接

/*
预定义常量
__LINE__：当前源代码的行号，为整型常量
__FILE__：当前编译程序文件的名称，为字符串
__DATE__：编译程序文件日期，为字符串（”MM DD YYYY＂形式，如” Jan 19 1993”）
__TIME__：编译程序文件时间，为字符串（＂hh:mm:ss＂形式，如”08:30:23”）
__STDC__：ANSI C标志，整型常量1,说明此程序兼容ANSI C标准。
*/