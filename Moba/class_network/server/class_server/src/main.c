#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//����windows socket����
#ifdef WIN32   //WIN32
#include <WinSock2.h>
#include <Windows.h>
#pragma comment (lib,"WSOCK32.LIB")
#endif


/*
Select 
����Select������Ȼ��������𣬵ȴ����ǹ���ľ�������¼�����
1��������
2���µ��û�����
3���ر�

1> ׼��һ���������
2> �����ǵ�socket������뵽�����������
3>  ����select�����ȴ������������
4> ������һ��������¼�������ʱ��OS�������ǵ������Select���أ�
5> �����¼�������select;
*/

static int client_fd[4096];
static int socket_count = 0;

#define MAX_BUF_LEN 4096
static unsigned char recv_buf[MAX_BUF_LEN];

int main(int argc, char** argv)
{
	int ret = 0;
	//����һ��windows�汾
#ifdef WIN32
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	ret = WSAStartup(wVersionRequested, &wsaData);
	if (ret != 0)
	{
		printf("WSAStart up failed������\n");
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

	//��������
	ret = listen(s, 1);

	fd_set set;//����һ���������

	while (1)
	{
		// 1> ���һ�¾������
		FD_ZERO(&set);
		FD_SET(s, &set); //������������뵽������

		//�������Ŀͻ��˽��������socket�����뵽������� 
		for(int i = 0;  i< socket_count; i++)
		{
			if(client_fd[i] != INVALID_SOCKET)
			FD_SET(client_fd[i], &set);
		}
		//end

		ret = select(0, &set, NULL, NULL, NULL);
		if (ret < 0)
		{//Error
			printf("select error\n");
			continue;
		}
		else if (ret == 0)
		{//time out
			printf("select timeout\n");
			continue;
		}

		// ���ĸ�������¼�����
		if (FD_ISSET(s, &set))
		{
			//�ͻ��˵�ip + port
			struct sockaddr_in c_address;
			int address_len = sizeof(c_address);
			printf("waiting...!!!\n");
			//cs �Ϳͻ�����Ե�socket
			int cs = accept(s, (struct  sockaddr*)&c_address, &address_len); // ���� ��ס
			printf("new clinet %s: %d\n", inet_ntoa(c_address.sin_addr), ntohs(c_address.sin_port));

			//����ý���Ŀͻ��ˣ�
			client_fd[socket_count] = cs;
			socket_count++;
			//end
			continue;
		}

		//����һ�£�������û�н�������Ŀͻ��εľ��
		for (int i = 0; i < socket_count; i++)
		{
			if (client_fd[i] != INVALID_SOCKET &&FD_ISSET(client_fd[i], &set))
			{
				int len = recv(client_fd[i], recv_buf, MAX_BUF_LEN, 0);
				if (len <= 0)
				{//�ر�socket
					closesocket(client_fd[i]);
					client_fd[i] = INVALID_SOCKET;
				}
				else {//�յ�������
					recv_buf[len] = 0;
					printf("recv��%s\n", recv_buf);
					send(client_fd[i], recv_buf, len, 0);
				}
			}
		}
		//end
	}

failed:
	if (s != INVALID_SOCKET)
	{
		closesocket(s);
		s = INVALID_SOCKET;
	}
	//����������һ��
#ifdef WIN32
	WSACleanup();
#endif

	system("pause");
	return 0;
}