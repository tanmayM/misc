#include <iostream>
#include<stdio.h>
using namespace std;
int allocate( int **y)
{
	int *x;
	
	x = new int(100);
	*y = x;
	
	return 0;
}

int main()
{
	int * y[10];
	
	for(int i=0;i<10;i++)
	{
	printf("Before &y[%d]=%d, y[%d]=%d\n", i, &y[i],  i, y[i]);
	allocate(&y[i]);
	y[i][0] = 10;
	printf("After &y[%d]=%d, y[%d]=%d\n", i, &y[i],  i, y[i]);
	}	

	for(int i=0;i<10;i++)
	{
	cout<<"y[0]="<<y[i][0]<<endl;
	}	
}