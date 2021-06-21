#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib")

int main(int argc, char** argv)
{
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);

	SOCKET client = socket(AF_INET, SOCK_DGRAM, 0);
	

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6080);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int len = sizeof(addr);
	
	int send_len = sendto(client, "Hello", 5, 0, (const SOCKADDR*)&addr, len);
	printf("send_len = %d\n", send_len);

	char buf[128];
	struct sockaddr_in sender_addr;
	len = sizeof(sender_addr);
	int recv_len = recvfrom(client, buf, 128, 0, (SOCKADDR*)&sender_addr, &len);
	printf("recv_len = %d\n", recv_len);
	if (recv_len > 0)
	{
		buf[recv_len] = 0;
		printf("recv: %s\n", buf);
	}
	

	WSACleanup();
	system("pause");
	return 0;
}