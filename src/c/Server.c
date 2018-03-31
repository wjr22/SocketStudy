/**
 *	this file is created by wjr in 3/27/2018
 *	it just for Socket Connect experiment.(it's Server-side)
 */

#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<error.h>
#include<errno.h>
#include<unistd.h>

#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<pthread.h>
/**
 *	将文本字符串格式转换成网络字节的二进制地址
 *	int inet_pton(int domain, const char * restrict str,void *restrict addr);
 *	
 *	将网络字节的二进制地址转换成文本字符串格式
 *	const char *inet_ntop(int domain, const char *restrict addr,char *restrict str,socklen_t size);
 *
 *	关联地址和套接字,注意第二个参数类型转换
 *	int bind(int sockfd,const struct sockaddr *addr,socklen_t len);
 *
 *	发现绑定到套接字上的地址
 *	int getsockname(int sockfd,struct sockaddr *restrict addr, socklen_t *restrict alenp);
 *
 *	发现对等方地址（已连接）
 *	int getpeername(int sockfd,struct sockaddr *restrict addr, socklen_t *restrict alenp);
 *
 *  设置套接字选项
 *	int setsockopt(int sockfd,int level,int option, const void *val, socklen_t len);
 *	
 *	建立连接：
 *	SOCK_STREAM OR SOCK_SEQPACKET:
 *	客户端连接服务器：参数：sockfd: int socket(int domain,int type,int protocol)返回值(文件描述符)
 *  int connect(int sockfd, const sockaddr *addr,socklen_t len);
 *	
 *	服务器侦听客户端请求
 *	int listen(int sockdf,int backlog);
 */
struct Socket {
	int					sockfd;
	struct sockaddr		*Clent;
	socklen_t			socklen;
};

int initserver(int type,struct sockaddr *hints,socklen_t len,int qlen);
void *
recv_thread(void *arg) {
	struct sockaddr_in clent;
	int sockfd_clent;
	int flag = 0;
	char buf[1024] = { 0 };
	struct Socket *Clent=(struct Socket *)arg;
	sockfd_clent=Clent->sockfd;
        printf("this is in pthread\n");	
	flag = recv(sockfd_clent, buf, sizeof(buf), 0);
	if (flag == 0) {
		printf("Close!\n");
		return NULL;
	}
	printf("from %s:%s", inet_ntoa(clent.sin_addr), buf);
	return NULL;
}


int main() {
	struct sockaddr_in sock;
	struct Socket Clent_socket;
	struct sockaddr_in clent;
	int sockfd,err,sockfd_clent;
	
	char addr[20];	//Server IP
	socklen_t structlen = sizeof(struct sockaddr);

	
	sock.sin_family = AF_INET;
	sock.sin_port = htons(8000);
	//inet_pton(AF_INET, addr, (void *)&(sock.sin_addr));	it didn't need
	sock.sin_addr.s_addr = INADDR_ANY;

	sockfd = initserver(SOCK_STREAM, (struct sockaddr*)&sock, structlen, 10);

	Clent_socket.sockfd = sockfd;
	Clent_socket.socklen = structlen;

        printf("Server sockfd is %d\n",sockfd);
	printf("Ready for Accept,Waitting...\n");
	
	while (1) {
		static int i = 0;
		i++;
		if ((sockfd_clent = accept(sockfd, (struct sockaddr *)&clent,&structlen))<0) {
                        printf("Accept Failed!\n");
			exit(-1);
		}
		Clent_socket.Clent=(struct sockaddr*)&clent;
		printf("Get Connect!\n");
		pthread_t pthread_recv;
		if (pthread_create(&pthread_recv, NULL, recv_thread,&Clent_socket) != 0) {
			perror("Can't Create New pthread");
		}
		printf("this is %d pthread\n", i);
	}
	close(sockfd);
	return 0;
}

/*
 *	允许地址复用的服务端初始化套接字函数
 *  hints : include address、family and so on
 *  len   : sizeof hints
 *	qlen  :	backlog
 *	return : socket file description(sockfd)
 */
int
initserver(int type, struct sockaddr *hints,socklen_t len,int qlen) {
	int sockfd, err, reuse = 1;
        socklen_t sizeofint=sizeof(int);
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0) {
		printf("Create TCP socket failed\n");
		return -1;
	}
	else {
		printf("Create TCP socket Success!\n");
	}
	/* 重用bind中的地址 */
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeofint) < 0) {
                printf("setsockopt failed\n");
		goto errout;
	}

	if (bind(sockfd, hints, len) != 0) {
                printf("bind failed\n");
		goto errout;
	}
	if (listen(sockfd, qlen) != 0) {
                printf("listen failed\n");
		goto errout;
	}
	return sockfd;
errout:
	err = errno;
	close(sockfd);
        printf("errno :%d\n",errno);
	errno = err;
	return -1;
}
