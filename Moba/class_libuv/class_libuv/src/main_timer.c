#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "uv.h"

#ifdef  WIN32
#include <windows.h>
static unsigned int get_cur_ms()
{
	return GetTickCount();
}
#else
#include <time.h>
#include <limits.h>
static unsigned int get_cur_ms()
{
	time_t t = time(NULL);
	return t;
}
#endif


#include "time_list.h"
static uv_loop_t* event_loop = NULL;
struct timer* t = NULL;
/*
static uv_timer_t timer;

static void on_uv_timer(uv_timer_t* handle) 
{
	printf("timer called\n");
	 
	uv_timer_stop(handle);
}

int main(int argc, char** argv)
{
	event_loop = uv_default_loop();
	uv_timer_init(event_loop, &timer);

	uv_timer_start(&timer, on_uv_timer, 5000, 1000);

	
	uv_run(event_loop, UV_RUN_DEFAULT);
	system("pause");
	return 0;
}
*/

void on_timer_func(void* udata)
{
	static int count = 0;
	char* str = udata;
	printf("%s\n", str);
	count++;
	if (count == 10)
		cancel_timer(t);
}

int main(int argc, char** argv)
{
	event_loop = uv_default_loop();

	 t = schedule(on_timer_func, "HelloWorld!!!", 1000, -1);

	schedule_once(on_timer_func, "called!!!", 1000);

	uv_run(event_loop, UV_RUN_DEFAULT);
	system("pause");
	return 0;
}