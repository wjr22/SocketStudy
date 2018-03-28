
/**
 *	this file is made by wjr in 3/27/2018
 *	description:
 *		HOW TO USE: GetAddressIInfo $(hostname)
 *			{
 *				.hostname{
 *					description:"主机名，可以为节点名或点分格式主机地址"
 *					eg.:www.baidu.com(OR baidu.com)
 *				}
 *			}
 *		WHAT IT IS: it can print $(hostname)'s protocol family、address
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


void
print_family(struct addrinfo *aip) {
	printf(" Family:\t\t");
	switch (aip->ai_family) {
	case AF_INET:
		printf("ipv4\n");
		break;
	case AF_INET6:
		printf("ipv6\n");
		break;
	case AF_UNSPEC:
		printf("unspecified\n");
		break;
	case AF_UNIX:
		printf("unix\n");
		break;
	default:
		printf("Unknown\n");
		break;
	}
}
void
print_type(struct addrinfo *aip) {
	printf(" type:\t\t");
	switch (aip->ai_socktype) {
	case SOCK_STREAM:
		printf("stream\n");
		break;
	case SOCK_DGRAM:
		printf("datagram\n");
		break;
	case SOCK_RAW:
		printf("raw\n");
		break;
	case SOCK_SEQPACKET:
		printf("seqpacket\n");
		break;
	default:
		printf("Unknown (%d)\n", aip->ai_socktype);
		break;
	}
}
void
print_protocol(struct addrinfo *aip) {
	printf(" protocol:\t");
	switch (aip->ai_protocol) {
	case IPPROTO_TCP:
		printf("TCP\n");
		break;
	case IPPROTO_UDP:
		printf("UDP\n");
		break;
	case IPPROTO_RAW:
		printf("RAW\n");
		break;
	default:
		printf("Unknow\n");
		break;
	}
}

/*
int
main(int argc,char *argv[]) {
	struct addrinfo *aip,*ailist;
	struct addrinfo hints;
	char buff[1024];
	int err;
	const char *addr;
	void *ptr;

	memset(&hints, 0, sizeof(hints));					//清0
		
	err = getaddrinfo(argv[1], NULL, &hints, &ailist);	//根据hostname获取ip地址族
	if (err != 0) {
		perror("getaddrinfo failed");
		exit(-1);
	}
	printf("Host:\t%s\n", argv[1]);
	for (aip = ailist; aip != NULL; aip = aip->ai_next) {
		print_family(aip);
		print_type(aip);
		print_protocol(aip);
		switch (aip->ai_family) {
		case AF_INET:
			ptr = &((struct sockaddr_in *)aip->ai_addr)->sin_addr;	//获取地址指针（v4）
			break;
		case AF_INET6:
			ptr = &((struct sockaddr_in6 *)aip->ai_addr)->sin6_addr;//（v6）
			break;
		}
		addr = inet_ntop(aip->ai_family, ptr, buff, sizeof(buff));	//转换地址格式
		printf("address:\t%s\n", buff);
	}
	return 0;
}
*/