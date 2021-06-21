#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <uv.h>


void on_udp_send_end(uv_udp_send_t* req, int status)
{
	if (status == 0)
	{
		printf("send success\n");
	}
	free(req);
}

void uv_alloc_buf(uv_handle_t* handle,
	size_t suggested_size,
	uv_buf_t* buf) 
{
	if (handle->data != NULL)
	{
		free(handle->data);
		handle->data = NULL;
	}
	buf->base = malloc(suggested_size + 1);
	buf->len = suggested_size + 1;
	handle->data = buf->base;
}

void affter_uv_udp_recv(uv_udp_t* handle,
	ssize_t nread,
	const uv_buf_t* buf,
	const struct sockaddr* addr,
	unsigned flags)
{
	char ip_addr[128];
	uv_ip4_name(addr, ip_addr, 128);
	int port = ntohs(((SOCKADDR_IN*)addr)->sin_port);
	printf("ip: %s:%d nread = %d\n", ip_addr, port, nread);

	char* str_buf = handle->data;
	str_buf[nread] = 0;
	printf("recv: %s\n", str_buf);

	uv_buf_t w_buf;
	w_buf = uv_buf_init("PING", 4);
	uv_udp_send_t* req = malloc(sizeof(uv_udp_send_t));
	uv_udp_send(req, handle, &w_buf, 1, addr, on_udp_send_end);
}

int main(int argc, char** argv)
{
	uv_loop_t* event_loop = uv_default_loop();
	uv_udp_t server;
	memset(&server, 0, sizeof(uv_udp_t));

	uv_udp_init(event_loop, &server);

	struct sockaddr_in addr;
	uv_ip4_addr("0.0.0.0", 6080, &addr);
	uv_udp_bind(&server, &addr, 0);

	uv_udp_recv_start(&server, uv_alloc_buf, affter_uv_udp_recv);

	uv_run(event_loop, UV_RUN_DEFAULT);
	system("pause");
	return 0;
}