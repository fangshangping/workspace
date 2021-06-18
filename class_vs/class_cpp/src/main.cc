#include <stdio.h>
#include <stdlib.h>
#include <string.h>



class student 
{
	char* name;
protected:
	int age;

public:
	int sex;
};
typedef student ss;

class A : student
{
	void set_age(int a) {
		age = a;
	}

public :
	void operator= (int sex)
	{
		this->sex = sex;
	}
};

template <class T>
void swap_value(T a,T b)
{

}

int main(int argc, char** argv)
{
	printf("Hello World!!!\n");
	
	system("pause");

	/*student s;
	s.sex;*/
	ss s;
	s.sex = 'c';
	
	A a;
	a = 4;

	//char* c_ptr = "helloworld!";

	return 0;
}