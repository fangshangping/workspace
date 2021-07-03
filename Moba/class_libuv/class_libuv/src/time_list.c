#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "uv.h"
#include "time_list.h"


#define my_malloc malloc
#define my_free free
struct timer 
{
	uv_timer_t uv_timer;
	void(*on_timer)(void* udata);
	void* udata;
	int repeat_count;
};

struct timer* alloc_timer(void(*on_timer)(void* udata), void* udata, int repeat_count)
{
	struct timer* t = my_malloc(sizeof(struct timer));
	memset(t, 0, sizeof(struct timer));

	t->on_timer = on_timer;
	t->udata = udata;
	t->repeat_count = repeat_count;

	uv_timer_init(uv_default_loop(), &t->uv_timer);

	return t;
}

void free_timer(struct timer* t)
{
	my_free(t);
}

void on_uv_timer(uv_timer_t* handle)
{
	struct timer* t = handle->data;
	if (t->repeat_count < 0)
	{
		t->on_timer(t->udata);
	}
	else 
	{
		t->repeat_count--;
		t->on_timer(t->udata);
		if (t->repeat_count == 0)
		{
			uv_timer_stop(&t->uv_timer);
			free(t);
		}
	}
}

struct timer* schedule(void(*on_timer)(void* udata),
	void* udata,
	int after_sec,
	int repeat_count) 
{
	struct timer* t = alloc_timer(on_timer, udata, repeat_count);

	t->uv_timer.data = t;
	uv_timer_start(&t->uv_timer, on_uv_timer, after_sec, after_sec);

	return t;
}


void cancel_timer(struct timer* t)
{
	if (t->repeat_count == 0)
		return;
	uv_timer_stop(&t->uv_timer);
	free(t);
}

struct timer* schedule_once(void(*on_timer)(void* udata),
	void* udata,
	int after_sec)
{
	return schedule(on_timer, udata, after_sec, 1); 
}