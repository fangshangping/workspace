#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//配置windows socket环境
#ifdef WIN32   //WIN32
#include <WinSock2.h>
#include <Windows.h>
#pragma comment (lib,"WSOCK32.LIB")
#endif


/*
Select 
调用Select函数，然后任务挂起，等待我们管理的句柄，有事件发生
1：读数据
2：新的用户进来
3：关闭

1> 准备一个句柄集合
2> 讲我们的socket句柄加入到我们这个集合
3>  调用select函数等待在这个集合上
4> 当其中一个句柄有事件发生的时候，OS唤醒我们的任务从Select返回：
5> 处理事件，继续select;
*/

static int client_fd[4096];
static int socket_count = 0;

#define MAX_BUF_LEN 4096
static unsigned char recv_buf[MAX_BUF_LEN];

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

	fd_set set;//定义一个句柄集合

	while (1)
	{
		// 1> 清空一下句柄集合
		FD_ZERO(&set);
		FD_SET(s, &set); //将监听句柄加入到集合里

		//把其它的客户端接入进来的socket，进入到句柄集合 
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

		// 是哪个句柄有事件发生
		if (FD_ISSET(s, &set))
		{
			//客户端的ip + port
			struct sockaddr_in c_address;
			int address_len = sizeof(c_address);
			printf("waiting...!!!\n");
			//cs 和客户段配对的socket
			int cs = accept(s, (struct  sockaddr*)&c_address, &address_len); // 堵塞 卡住
			printf("new clinet %s: %d\n", inet_ntoa(c_address.sin_addr), ntohs(c_address.sin_port));

			//保存好接入的客户端；
			client_fd[socket_count] = cs;
			socket_count++;
			//end
			continue;
		}

		//遍历一下，看看有没有接入进来的客户段的句柄
		for (int i = 0; i < socket_count; i++)
		{
			if (client_fd[i] != INVALID_SOCKET &&FD_ISSET(client_fd[i], &set))
			{
				int len = recv(client_fd[i], recv_buf, MAX_BUF_LEN, 0);
				if (len <= 0)
				{//关闭socket
					closesocket(client_fd[i]);
					client_fd[i] = INVALID_SOCKET;
				}
				else {//收到了数据
					recv_buf[len] = 0;
					printf("recv：%s\n", recv_buf);
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
	//结束的清理一下
#ifdef WIN32
	WSACleanup();
#endif

	system("pause");
	return 0;
}