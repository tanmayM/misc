#include <iostream>
#include <cmath>

using namespace std;

int main()
{
	short a;
	unsigned short b=0x7fff;
	int size = sizeof(a);
	size = size*8;
	a = pow(2,size-1)-1;
	
	cout <<"a="<<a<<endl;
	

	b = ~0;
	a = b>>1;
	cout <<"again a="<<a<<endl;

	cout <<"highest unsigned value = "<<b<<endl;

	
	
}