
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



#include "my_math.h"

//#define MAX 342626

#define add(a,b)  a+b

static int test_goto();

#define to_string(s) #s




int main(int argc,char** argv) {
	
	printf("==================================\n");
	printf("argc = %d\n", argc);
	for (int i = 0; i < argc; i++)
	{
		printf("argv[%d] = %s\n", i, argv[i]);
	}
	printf("==================================\n");

	printf("Hello World!\n");

	//printf(to_string(�����һ����ɵ�ƣ�����\n));
	//mmm();

	char* c_ptr = "helloworld!";

	//c_ptr[1] = 'a';

	printf("%s\n", c_ptr);
	/*int a = 10;
	printf("a�ĵ�ַ%x\n", &a);
	printf("a = %d\n", a);


	printf("a = %d\n", a);
	*/

	/*char* c = NULL;
	printf("mem is %d\n", sizeof(char*));

	int* int_ptr = NULL;
	int_ptr = 789;

	printf("int_ptr = %d\n", int_ptr);*/

	/*int b[] = { 3,4,5,6,7 };

	int* p = b;

	printf("0x%p,	0x%p,	0x%p\n", &b[1], p + 1, b + 1);

	float f_value = 3.1415926f;

	printf("f_value = %f \n", f_value);
	printf("f_value = %f	%d\n", f_value, f_value);*/

	

	//int* ptr = (int*)malloc(sizeof(int) * 100);

	//for (size_t i = 0; i < 100; i++)
	//{
	//	ptr[i] = i + 1;
	//}

	///*free(ptr);*/

	//int* ptr2 = realloc(ptr, sizeof(int) * 200);

	//for (int i = 0; i < 10; i++)
	//{
	//	printf("%d	", ptr2[i]);
	//}
	//printf("\n");
	//free(ptr2);

	//printf("%d\n", add(5, 6));

	//int sum = math_sum(3, 4);

	//printf("sum = %d \n", sum);

	//int g_a = 20;
	////g_a = 10;
	//printf("%d\n", g_a);

	//printf("%c\n", 0);
//��������

	//�����ַ��� �Ƕ����ڴ������  -->ֻ���� �����޸ĵ�

	//strdup   malloc   free

	//malloc

#if 0

#endif

#ifdef MAX
	printf("Hello World!	\n");
#endif

#ifndef MAX
#endif

	struct FILE* f = NULL;
	fopen_s(&f, "hello.txt", "r");
	
	if (f == NULL)
	{
		printf("���ʼ���ʧ�ܣ�����\n");
	}
	else
		printf("���ļ��ɹ�������\n");

	//fseek

	//ftell  ���ص�ǰ�ļ���λ��

	//

	
	if (f)
	{
		fclose(f);
	}

	f = fopen("write.txt","wb");
	char* write_data = "helloworld!!!from wirte!!!";
	fwrite(write_data, 1, strlen(write_data), f);
	fclose(f);

	fprintf(stdout, "nihaoa\n");

	//const ����* ����   ����ͨ���������ı�����
	//����* const ����   ���ܸı����

	int sum = test_goto();
	printf("sum = %d\n", sum);

	system("pause");
	return 0;
}


static int test_goto()
{
	int sum = 0;
	int i = 0;
again:
	sum += i;
	i++;
	if (i < 100)
		goto again;

	return sum;
}