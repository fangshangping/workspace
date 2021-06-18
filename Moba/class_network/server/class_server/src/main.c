#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//配置windows socket环境
#ifdef WIN32   //WIN32
#include <WinSock2.h>
#include <Windows.h>
#pragma comment (lib,"WSOCK32.LIB")
#endif

int main(int argc, char** argv)
{
	int ret = 0;
	//配置一下windows版本
#ifdef WIN32
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	ret = WSAStartup(wVersionRequested, &wsaData);
	if (ret != 0)
	{
		printf("WSAStart up failed！！！\n");
		system("pause");
		return -1;
	}
#endif
	int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
	{
		goto failed;
	}

	// ip + port
	struct sockaddr_in sockaddr;
	sockaddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(6080); 
	ret = bind(s, (const struct sockaddr*)&sockaddr, sizeof(sockaddr));
	if (ret != 0)
	{
		goto failed;
	}

	//开启监听
	ret = listen(s, 1);
	while (1)
	{
		//客户端的ip + port
		struct sockaddr_in c_address;
		int address_len = sizeof(c_address);
		//cs 和客户段配对的socket
		printf("waiting...!!!\n"); 
		int cs = accept(s, (struct  sockaddr*)&c_address, &address_len);
		printf("new clinet %s: %d\n", inet_ntoa(c_address.sin_addr), ntohs(c_address.sin_port));

		//接受数据
		char buf[11];
		memset(buf, 0, 11);
		recv(cs, buf, 5, 0);

		//发数据
		send(cs, buf, 5, 0);

		printf("recv： %s\n", buf);
		closesocket(cs);
	}

failed:
	if (s != INVALID_SOCKET)
	{
		closesocket(s);
		s = INVALID_SOCKET;
	}
	//结束的清理一下
#ifdef WIN32
	WSACleanup();
#endif

	system("pause");
	return 0;
}