
/**
 *	this file is made by wjr in 3/27/2018
 *	description:
 *		HOW TO USE: GetAddressIInfo $(hostname)
 *			{
 *				.hostname{
 *					description:"������������Ϊ�ڵ������ָ�ʽ������ַ"
 *					eg.:www.baidu.com(OR baidu.com)
 *				}
 *			}
 *		WHAT IT IS: it can print $(hostname)'s protocol family��address
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
 *	�ṹ�嶨�壺("netinet/in.h")
 *	�׽��ֵ�ַ:(��ͬ��ʽ��ַ��ǿ��ת���������׼�׽��ֵ�ַ)
 *  struct sockaddr{
 *		sa_family_t			sa_family;			//address family
 *		char				sa_data[14];		//variable-length address
 *	}
 *************************************************************
 *	���õ�ַ��ipv4:
 *	struct in_addr{
 *		in_addr_t			s_addr;			//IPv4 address
 *	};
 *	struvt sockaddr_in{
 *		sa_family_t			sin_family;			//address family
 *		in_port_t			sin_port;			//port number
 *		struct in_addr		sin_addr;			//IPv4 address	
 *	};
 *	���õ�ַ��IPv6:
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
 *	****:��ѡ:hints:���˵�ַ��ģ�壬������getaddrinfo���ݲ�����
 */

 
/**
 * @param	const char *restrict host:������������Ϊ�ڵ������ָ�ʽ������ַ
 *			const char *restrict service:��������һ������������10���ƶ˿ں�������������������ʮ���ƵĶ˿ںţ�Ҳ�������Ѷ���ķ������ƣ���ftp��http��
 *			const struct addrinfo *restrict hints��������һ����ָ�룬Ҳ������һ��ָ��ĳ��addrinfo�ṹ��ָ�룬������������ṹ����������������ص���Ϣ���͵İ�ʾ��
 *			struct addrinfo **restrict res:�������ջ�ȡ��addrinfo��Ϣ
 *
 * int getaddrinfo(const char *restrict host, /
 *					const char *restrict service, /
 *					const struct addrinfo *restrict hint,/
 *					struct addrinfo **restrict res)
 *
 *���庯��:const char *gai_strerror( int error );
 *����˵��:
 *     �ú�����getaddrinfo���صķ�0����ֵ�����ֺͺ���Ϊ����Ψһ����������һ��ָ���Ӧ�ĳ�����Ϣ����ָ�롣
 *
 *���庯��: void freeaddrinfo( struct addrinfo *ai );
 *����˵��:
 *     ��getaddrinfo���ص����д洢�ռ䶼�Ƕ�̬��ȡ�ģ���Щ�洢�ռ����ͨ������freeaddrinfo���ظ�ϵͳ��
 */
/**
 * int inet_pton(int family, const char *strptr, void *addrptr);     //�����ʮ���Ƶ�ip��ַת��Ϊ�������紫�����ֵ��ʽ  
 *      ����ֵ�����ɹ���Ϊ1�������벻����Ч�ı��ʽ��Ϊ0����������Ϊ-1  
 *
 * const char * inet_ntop(int family, const void *addrptr, char *strptr, size_t len);     //����ֵ��ʽת��Ϊ���ʮ���Ƶ�ip��ַ��ʽ  
 *       ����ֵ�����ɹ���Ϊָ��ṹ��ָ�룬��������ΪNULL
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

	memset(&hints, 0, sizeof(hints));					//��0
		
	err = getaddrinfo(argv[1], NULL, &hints, &ailist);	//����hostname��ȡip��ַ��
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
			ptr = &((struct sockaddr_in *)aip->ai_addr)->sin_addr;	//��ȡ��ַָ�루v4��
			break;
		case AF_INET6:
			ptr = &((struct sockaddr_in6 *)aip->ai_addr)->sin6_addr;//��v6��
			break;
		}
		addr = inet_ntop(aip->ai_family, ptr, buff, sizeof(buff));	//ת����ַ��ʽ
		printf("address:\t%s\n", buff);
	}
	return 0;
}
*/