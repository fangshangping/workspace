#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <Windows.h>

static int g_value = 10;
char* ptr = NULL;

HANDLE wait_cond = INVALID_HANDLE_VALUE;

CRITICAL_SECTION lock;

void test_func()
{
	
}

DWORD WINAPI thread_entry(LPVOID lpThreadParameter)
{
	printf("threadid %d\n", GetCurrentThreadId());

	g_value = 9;
	ptr[0] = 10;

	test_func();

	Sleep(5000);

	SetEvent(wait_cond);

	while (1)
	{
		printf("thread call\n");

		EnterCriticalSection(&lock);
		g_value = 200;
		LeaveCriticalSection(&lock);
		Sleep(3000);
	}

	return 0;
}

int main(int argc, char* argv[])
{
	printf("main threadid %d\n", GetCurrentThreadId());

	InitializeCriticalSection(&lock);
	ptr = malloc(100);


	wait_cond = CreateEvent(NULL, FALSE, FALSE, NULL);

	int threadid;
	HANDLE h = CreateThread(NULL,0,thread_entry,NULL,0,&threadid);
	
	test_func();

	WaitForSingleObject(wait_cond, INFINITE);
	while (1)
	{
		printf("main thread\n");
		EnterCriticalSection(&lock);
		g_value = 100;
		LeaveCriticalSection(&lock);
		Sleep(1500);
	}
	return 0;
}