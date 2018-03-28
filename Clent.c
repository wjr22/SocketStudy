//网络编程--客户端
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/*
强调：当客户端连接服务器时，服务器会产生一个新的文件描述符（套接字）与客户端交互，这个新的套接字不是服务器端的监听套接字
套接字是全双工的，在一个网络管道中的两端，每端都可以进行读写操作。
*/

typedef struct _recvmodel
{
	int st;
	struct sockaddr_in * addr;
} RecvModel;

//send message
void * send_thread(void *arg)
{
	if (arg == NULL)
	{
		printf("param is not allow NULL!\n");
		return NULL;
	}
	int st = *(int *)arg;
	char buf[1024] = { 0 };
	while (1)
	{
		read(STDIN_FILENO, buf, sizeof(buf));
		if (send(st, buf, strlen(buf), 0) == -1)
		{
			printf("send failed ! error message %s\n", strerror(errno));
			return NULL;
		}
		memset(buf, 0, sizeof(buf));
	}
	return NULL;
}

//recv message
void * recv_thread(void * arg)
{
	if (arg == NULL)
	{
		printf("param is not allow NULL!\n");
		return NULL;
	}
	RecvModel * model = (RecvModel *)arg;
	int flag = 0;
	char buf[1024] = { 0 };
	while (1)
	{
		flag = recv(model->st, buf, sizeof(buf), 0);
		if (flag == 0)
		{
			printf("对方已经关闭连接！\n");
			return NULL;
		}
		else if (flag == -1)
		{
			printf("recv failed ! error message : %s\n", strerror(errno));
			return NULL;
		}
		printf("from %s:%s", inet_ntoa(model->addr->sin_addr), buf);
		memset(buf, 0, sizeof(buf));
	}
	return NULL;
}

int main(int arg, char *args[])
{
	//打开socket
	int st = socket(AF_INET, SOCK_STREAM, 0);
	if (st == -1)
	{
		printf("open socket failed! error message:%s\n", strerror(errno));
		return -1;
	}
	//定义IP地址结构
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	//设置TCP/IP连接
	addr.sin_family = AF_INET;
	//设置端口号
	addr.sin_port = htons(8080);
	//设置允许连接地址
	addr.sin_addr.s_addr = inet_addr("192.168.1.102");
	//connect server
	int numx = connect(st, (struct sockaddr *) &addr, sizeof(addr));
	if (numx == -1)
	{
		printf("connect server failed ! error message :%s\n", strerror(errno));
		goto END;
	}

	RecvModel model;
	model.st = st;
	model.addr = &addr;
	//开启多线程--线程1接收消息，线程2发送消息
	pthread_t thr1, thr2;
	if (pthread_create(&thr1, NULL, send_thread, &st) != 0)
	{
		printf("create thread failed ! \n");
		goto END;
	}
	if (pthread_create(&thr2, NULL, recv_thread, &model) != 0)
	{
		printf("create thread failed ! \n");
		goto END;
	}
	pthread_join(thr1, NULL);
	pthread_join(thr2, NULL);
END: close(st);
	return 0;
}