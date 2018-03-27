/**
 *	this file is created by wjr in 3/27/2018
 *	it just for Socket Connect experiment.(it's Server-side)
 */

#include <cstdio>
#include<stdlib.h>
#include<string.h>
#include<error.h>

#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<netdb.h>

/**
 *	将文本字符串格式转换成网络字节的二进制地址
 *	int inet_pton(int domain, const char * restrict str,void *restrict addr);
 *	
 *	将网络字节的二进制地址转换成文本字符串格式
 *	const char *inet_ntop(int domain, const char *restrict addr,char *restrict str,socklen_t size);
 *
 *	关联地址和套接字,注意第二个参数类型转换
 *	int bind(int sockfd,const struct sockaddr *addr,socklen_t len);
 */
int main() {
	struct sockaddr_in sock;
	int sockfd,err;
	char buffer[1024];
	const char *addr=INADDR_ANY;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("Create TCP socket failed\n");
		exit(-1);
	}
	else {
		printf("Create TCP socket Success\n");
	}
	sock.sin_family = AF_INET;
	sock.sin_port = htons(65555);
	inet_pton(AF_INET, addr, &(sock.sin_addr.s_addr));
	if (bind(sockfd, (struct sockaddr *)&sock, sizeof(struct sockaddr)) != 0) {
		perror("bind IPv4 address OR port failed");
		exit(-1);
	}
	else {
		printf("bind IPv4:%s Port:65555 Success\n", inet_ntop(AF_INET, &(sock.sin_addr.s_addr), buffer, 40));
	}
	if (listen(sockfd, 10) != 0) {
		perror("Starting Listener Failed");
		exit(-1);
	}
	else {
		printf("Starting Listener Success\n");
	}
}