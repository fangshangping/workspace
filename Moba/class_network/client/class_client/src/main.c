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
	
	//发送链接请求到我们的监听socket
	ret = connect(s, (const struct sockaddr*)&sockaddr, sizeof(sockaddr));
	
	if (ret != 0)
	{
		goto failed;
	}

	//链接成功，s与服务器对应的socket就会建立链接
	//客户端在连接的时候也需要 一个ip + port
	char buf[11];
	memset(buf, 0, 11);
	send(s, "Hello", 5, 0);
	recv(s, buf, 5, 0);
	printf("recv: %s\n", buf);

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