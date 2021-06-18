#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "my_math.h"

int g_a = 7;

int math_sum(int begin,int end)
{
	int sum = 0;
	for (int i = begin; i <= end; i++)
	{
		sum += i;
	}
	return sum;
}

int math_abs(int value)
{
	return (value > 0 ? value : -value);
}