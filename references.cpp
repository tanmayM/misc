#include <iostream>

using namespace std;

class aClass
{
		public:

	int a,b,c;
	void func() {}
};

void theChanger( aClass& s)
{
cout<<s.a<<s.b<<s.c<<endl;
int p;
int *p2;

p = (int)&(s.a);
p2 = (int *)p;
*p2 = 50;
cout<<s.a<<s.b<<s.c<<endl;

}

int main()
{
	aClass o;
	o.a=o.b=o.c=100;
	
	theChanger(o);
	
}
