#include <iostream>

using namespace std;

int logicalRightShift(int a, int n)
{
	
	int flag=0;
	int b = a;
	unsigned int c = ~0;
	int n2 = sizeof(int)*8 - n;
	c <<= n2; 
	c  = ~c;
	
	b = b >> (sizeof(b)*8 - 1);
	b>>=1;
	if( b)
		flag = 1;
	else
		flag = 0;
		
	cout << "Flag="<<flag<<endl;
	a = a>>n;
	
	if(flag)
		a = a & c;
	
	return a;
	
}

int main()
{
	int num, shifts;
	cout <<"Enter number:";
	cin>>num;
	cout<<"Enter shifts:";
	cin>>shifts;
	
	num = logicalRightShift(num, shifts);
	
	cout<<"After shifts: num="<<num<<endl;
	
	
}

