/**
 *	this file is created by wjr in 3/27/2018
 *	it just for Socket Connect experiment.(it's Server-side)
 */

#include <cstdio>
#include<stdlib.h>
#include<string.h>
#include<error.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<netdb.h>

/**
 *	���ı��ַ�����ʽת���������ֽڵĶ����Ƶ�ַ
 *	int inet_pton(int domain, const char * restrict str,void *restrict addr);
 *	
 *	�������ֽڵĶ����Ƶ�ַת�����ı��ַ�����ʽ
 *	const char *inet_ntop(int domain, const char *restrict addr,char *restrict str,socklen_t size);
 *
 *	������ַ���׽���,ע��ڶ�����������ת��
 *	int bind(int sockfd,const struct sockaddr *addr,socklen_t len);
 *
 *	���ְ󶨵��׽����ϵĵ�ַ
 *	int getsockname(int sockfd,struct sockaddr *restrict addr, socklen_t *restrict alenp);
 *
 *	���ֶԵȷ���ַ�������ӣ�
 *	int getpeername(int sockfd,struct sockaddr *restrict addr, socklen_t *restrict alenp);
 *
 *	�������ӣ�
 *	SOCK_STREAM OR SOCK_SEQPACKET:
 *	�ͻ������ӷ�������������sockfd: int socket(int domain,int type,int protocol)����ֵ(�ļ�������)
 *  int connect(int sockfd, const sockaddr *addr,socklen_t len);
 *	
 *	�����������ͻ�������
 *	int listen(int sockdf,int backlog);
 */

int main() {
	struct sockaddr_in sock;
	struct sockaddr_in client;
	int sockfd,err,new_sockfd,numbytes,i;
	socklen_t structlen;
	char buffer[1024];
	const char *addr=INADDR_ANY;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	structlen = sizeof(struct sockaddr);
	if (sockfd < 0) {
		perror("Create TCP socket failed\n");
		exit(-1);
	}
	else {
		printf("Create TCP socket Success\n");
	}
	sock.sin_family = AF_INET;
	sock.sin_port = htons(8000);
	//inet_pton(AF_INET, addr, &(sock.sin_addr));
	sock.sin_addr.s_addr = INADDR_ANY;
	if (bind(sockfd, (struct sockaddr *)&sock, structlen) != 0) {
		perror("Bind IPv4 address OR port failed");
		exit(-1);
	}
	else {
		printf("Bind IPv4:%s Port:8000 Success\n", inet_ntop(AF_INET, &(sock.sin_addr.s_addr), buffer, 40));
	}
	if (listen(sockfd, 10) != 0) {
		perror("Starting Listener Failed");
		exit(-1);
	}
	else {
		printf("Starting Listener Success\n");
	}
	printf("Ready for Accept,Waitting...\n");
	new_sockfd = accept(sockfd, (struct sockaddr *)&client, &structlen);
	printf("Get Connect!\n");
	if ((numbytes = recv(new_sockfd, buffer, 1024, 0)) == -1) {
		perror("errno!");
		printf("errno!");
		sleep(5000);
		exit(-1);
	}
	send(new_sockfd, "hello world", 12, 0);
	while ((numbytes = recv(new_sockfd, buffer, 1024, 0)) > 0) {
		buffer[numbytes] = '\0';
		printf("%s\n", buffer);
		if (send(new_sockfd, buffer, numbytes, 0) < 0) {
			perror("write");
			return 1;
		}
	}
	close(new_sockfd);
	close(sockfd);
	return 0;
}