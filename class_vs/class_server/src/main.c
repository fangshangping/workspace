#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef WIN32   //WIN32
#include <WinSock2.h>
#include <MSWSock.h>
#include <Windows.h>

#pragma comment (lib,"WSOCK32.LIB")
#pragma comment (lib,"ws2_32.lib")
#pragma comment (lib,"odbc32.lib")
#pragma comment (lib,"odbccp32.lib")
#endif


enum {
	IOCP_ACCPET = 0, 
	IOCP_RECV,
	IOCP_WRITE,
};
#define MAX_RECV_SIZE 8192
struct io_package 
{
	WSAOVERLAPPED overlapped;
	int opt;
	int accpet_sock;
	WSABUF wsabuffer;
	char pkg[MAX_RECV_SIZE];
};

static void post_accept(SOCKET l_sock)
{
	struct io_package* pkg = malloc(sizeof(struct io_package));
	memset(pkg, 0, sizeof(struct io_package));

	pkg->wsabuffer.buf = pkg->pkg;
	pkg->wsabuffer.len = MAX_RECV_SIZE - 1;
	pkg->opt = IOCP_ACCPET;

	DWORD dwBytes = 0;
	SOCKET client = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	int addr_size = sizeof(struct sockaddr_in) + 16;
	pkg->accpet_sock = client;

	AcceptEx(l_sock, client, pkg->wsabuffer.buf, 0,
		addr_size, addr_size, &dwBytes, &pkg->overlapped);
}

static void post_recv(SOCKET client_fd)
{
	struct io_package* io_data = malloc(sizeof(struct io_package));
	memset(io_data, 0, sizeof(struct io_package));

	io_data->wsabuffer.buf = io_data->pkg;
	io_data->wsabuffer.len = MAX_RECV_SIZE - 1;
	io_data->opt = IOCP_RECV;
	io_data->accpet_sock = client_fd;

	DWORD dwRecv = 0;
	DWORD dwFlags = 0;
	int ret = WSARecv(client_fd, &(io_data->wsabuffer),
		1, &dwRecv, &dwFlags,
		&(io_data->overlapped), NULL);
}

int main(int argc, char* argv[])
{

#ifdef WIN32
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);
#endif

	//创建
	HANDLE iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (iocp == INVALID_HANDLE_VALUE)
	{
		goto failed;
	}

	SOCKET l_sock = INVALID_SOCKET;
	l_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (l_sock == INVALID_SOCKET)
	{
		goto failed;
	}

	struct sockaddr_in s_address;
	memset(&s_address, 0, sizeof(s_address));
	s_address.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	s_address.sin_family = AF_INET;
	s_address.sin_port = htons(6080);

	if (bind(l_sock, (const struct sockaddr*)&s_address, sizeof(s_address)) != 0)
	{
		goto failed;
	}

	if (listen(l_sock, SOMAXCONN) != 0)
	{
		goto failed;
	}

	CreateIoCompletionPort((HANDLE)l_sock, iocp, (DWORD)0, 0); 
	post_accept(l_sock);

	while (1)
	{
		DWORD dwTrans;
		DWORD udata;
		struct io_package* io_data;

		int ret = GetQueuedCompletionStatus(iocp, &dwTrans, &udata, (LPOVERLAPPED*)&io_data, WSA_INFINITE);

		if (ret == 0)
		{//意外

			if (io_data)
			{
				if (io_data->accpet_sock == IOCP_RECV)
				{
					closesocket(io_data->accpet_sock);
					free(io_data);
				}
				else if (io_data->accpet_sock == IOCP_ACCPET)
				{
					free(io_data);
					post_accept(l_sock);
				}
			}
			continue;
		}

		if (dwTrans == 0 && io_data->opt == IOCP_RECV)
		{//关闭
			closesocket(io_data->accpet_sock);
			free(io_data);
			continue;
		}

		switch (io_data->opt)		
		{
		case IOCP_ACCPET:
		{
			int client_fd = io_data->accpet_sock;
			int addr_size = sizeof(struct sockaddr_in) + 16;
			struct sockaddr_in* l_addr = NULL;
			int l_len = sizeof(struct sockaddr_in);

			struct sockaddr_in* r_addr = NULL;
			int r_len = sizeof(struct sockaddr_in);

			GetAcceptExSockaddrs(io_data->wsabuffer.buf, 0, addr_size, addr_size,
				(struct sockaddr**)&l_addr,&l_len,
				(struct sockaddr**)&r_addr, &r_len);

			CreateIoCompletionPort((HANDLE)client_fd, iocp, (DWORD)client_fd, 0); 
			post_recv(client_fd);

			free(io_data);
			post_accept(l_sock);
		}
		break;
		case IOCP_RECV:
		{
			io_data->pkg[dwTrans] = 0;

			printf("IOCP recv %d %s\n", dwTrans, io_data->pkg);
			send(io_data->accpet_sock, io_data->pkg, dwTrans, 0);
			DWORD dwRecv = 0;
			DWORD dwFlags = 0;
			int ret = WSARecv(io_data->accpet_sock, &(io_data->wsabuffer),
				1, &dwRecv, &dwFlags,
				&(io_data->overlapped), NULL);
		}
		break;
		}
	}

failed:
	if (iocp != INVALID_HANDLE_VALUE)
	{
		CloseHandle(iocp);
	}

	if (l_sock != INVALID_SOCKET)
	{
		closesocket(l_sock);
	}

#ifdef WIN32
	WSACleanup();
#endif


	return 0;
}