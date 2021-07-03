#ifndef __TIME_LIST_H__
#define __TIME_LIST_H__

struct timer;

struct timer* schedule(void(*on_timer)(void* udata),
									void* udata, 
									int after_sec, 
									int repeat_count);

void cancel_timer(struct timer*);

struct timer* schedule_once(void(*on_timer)(void* udata),
												void* udata,
												int after_sec);

#endif