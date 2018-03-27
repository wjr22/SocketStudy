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
 *
 *	结构体定义：("netinet/in.h")
 *	套接字地址:(不同格式地址会强制转换成这个标准套接字地址)
 *  struct sockaddr{
 *		sa_family_t			sa_family;			//address family
 *		char				sa_data[14];		//variable-length address
 *	}
 *************************************************************
 *	常用地址：ipv4:
 *	struct in_addr{
 *		in_addr_t			s_addr;			//IPv4 address
 *	};
 *	struvt sockaddr_in{
 *		sa_family_t			sin_family;			//address family
 *		in_port_t			sin_port;			//port number
 *		struct in_addr		sin_addr;			//IPv4 address	
 *	};
 *	常用地址：IPv6:
 *	struct in6_addr{
 *		uint8_t				s6_addr[16];		//IPv6 address
 *  };
 *	struct sockaddr_in6{
 *		sa_family_t			sin6_family;		//address family
 *		in_port_t			sin6_port;			//port number
 *		uint32_t			sin6_flowinfo;		//traffic class and flow info
 *		struct in6_addr		sin6_addr;			//IPv6 address
 *		uint32_t			sin6_scope_id;		//set of interfaces for scope
 *	};
 * ***********************************************************
 *	struct addrinfo{
 *		int					ai_flags;			//customize behavior
 *		int					ai_family;			//address family
 *		int					ai_socktype;		//socket type
 *		int					ai_protocol;		//protocol
 *		struct	sockaddr   *ai_addr;			//address
 *		char			   *ai_canonname;		//canonical name of host
 *		struct	addrinfo   *ai_next;			
 *	}
 *	****:可选:hints:过滤地址的模板，（用于getaddrinfo传递参数）
 */

 /**
 * 函数定义
 * @param	int domain: 通讯的特性，包括地址格式一般有四个域
 *					AF_INET			:IPv4域
 *					AF_INET6		:IPv6域
 *					AF_UNIX			:UNIX域
 *					AF_UPSPEC		:未定
 *			int type  : 套接字类型，进一步确定通信特征
 *					SOCK_DGRAM		固定长度、无连接、不可靠的报文传递
 *					SOCK_RAW		IP协议的数据报接口
 *					SOCK_SEQPACKET	固定长度、有序、可靠、面向连接的报文传递
 *					SOCK_STREAM		有序、可靠、双向、面向连接的报文传递
 *			int protocol：默认为0，选择默认协议（例如：AF_INET，SOCK_STREAM下协议为TCP）
 * @return int（-1 OR 文件描述符）
 * int socket(int domain,int type,int protocol);
 */
/**
 * @param	const char *restrict host:主机名，可以为节点名或点分格式主机地址
 *			const char *restrict service:服务名，一个服务名或者10进制端口号数串。服务名可以是十进制的端口号，也可以是已定义的服务名称，如ftp、http等
 *			const struct addrinfo *restrict hints：可以是一个空指针，也可以是一个指向某个addrinfo结构的指针，调用者在这个结构中填入关于期望返回的信息类型的暗示。
 *			struct addrinfo **restrict res:用来接收获取的addrinfo信息
 *
 * int getaddrinfo(const char *restrict host, /
 *					const char *restrict service, /
 *					const struct addrinfo *restrict hint,/
 *					struct addrinfo **restrict res)
 *
 *定义函数:const char *gai_strerror( int error );
 *函数说明:
 *     该函数以getaddrinfo返回的非0错误值的名字和含义为他的唯一参数，返回一个指向对应的出错信息串的指针。
 *
 *定义函数: void freeaddrinfo( struct addrinfo *ai );
 *函数说明:
 *     由getaddrinfo返回的所有存储空间都是动态获取的，这些存储空间必须通过调用freeaddrinfo返回给系统。
 */
/**
 * int inet_pton(int family, const char *strptr, void *addrptr);     //将点分十进制的ip地址转化为用于网络传输的数值格式  
 *      返回值：若成功则为1，若输入不是有效的表达式则为0，若出错则为-1  
 *
 * const char * inet_ntop(int family, const void *addrptr, char *strptr, size_t len);     //将数值格式转化为点分十进制的ip地址格式  
 *       返回值：若成功则为指向结构的指针，若出错则为NULL
 */

int
main(int argc,char *argv[]) {
	struct addrinfo *aip,*ailist;
	struct addrinfo hints;
	char buff[1024];
	int err;
	const char *addr;
	void *ptr;

	memset(&hints, 0, sizeof(hints));	//清0
	
	err = getaddrinfo(argv[1], NULL, &hints, &ailist);
	if (err != 0) {
		perror("getaddrinfo failed");
		exit(-1);
	}
	printf("Host:\t%s\n", argv[1]);
	for (aip = ailist; aip != NULL; aip = aip->ai_next) {
		if (aip->ai_family == AF_INET) {
			ptr = &((struct sockaddr_in *)aip->ai_addr)->sin_addr;
			addr = inet_ntop(aip->ai_family, ptr, buff, sizeof(buff));
			printf("address:\t%s\n", buff);
		}
	}
	return 0;
}