#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib")

int main(int argc, char** argv)
{
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);

	SOCKET svr = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6080);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int len = sizeof(addr);
	bind(svr, (SOCKADDR*)&addr, len);

	// ÊÕÊý¾Ý
	char buf[128];
	SOCKADDR_IN client;
	len = sizeof(client);
	int recv_len = recvfrom(svr, buf, 128, 0, (SOCKADDR*)&client, &len);
	printf("recv_len = %d\n", recv_len);
	if (recv_len > 0)
	{
		buf[recv_len] = 0;
		printf("recv: %s\n", buf);
	}

	int sended = sendto(svr, buf, recv_len, 0, (const SOCKADDR*)&client, len);
	printf("sended = %d\n", sended);

	WSACleanup();
	system("pause");
	return 0;
}