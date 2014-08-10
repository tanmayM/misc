#include <iostream>

using namespace std;

int main()
{
	int a, b=0;
	
	a = (2,3,b=4),b=6;
	
	cout<<"a="<<a<<endl;
	cout<<"b="<<b<<endl;

	a=0;
	b=100;
	cout <<"-----------------------------------"<<endl;
	a = b || (b=10);
	cout<<"a="<<a<<endl;
	cout<<"b="<<b<<endl;

	cout <<"-----------------------------------"<<endl;
	a = b && (b=10);
	cout<<"a="<<a<<endl;
	cout<<"b="<<b<<endl;
	
	cout <<"-----------------------------------"<<endl;
	int result, c;
	
	result=0;
	a=0;
	b=25;
	c=50;
	a && (result = b, 1) || (result = c, 0);
	cout<<"a="<<a<<endl;
	cout<<"b="<<b<<endl;
	cout<<"c="<<c<<endl;
	cout<<"result="<<result<<endl;
	
	
}