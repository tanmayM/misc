/* Given an array of size N which contains elements in the range (0 – M-1) with no duplicates where M>N,
   Find the smallest missing positive integer.
   
	Known bug: Implementation doesn't check for duplicates while generating numbers using rand(). Could lead to infinite
	loop.
	
	Author: Tanmay
*/

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

using namespace std;
#define VECTOR_SIZE 50

int gSteps=0;
void putInPlace(vector<int> &arr, int size, int i)
{
	int tmp, tmp2;
	
	gSteps++;
	cout<<__func__<<":"<<i<<","<<arr[i]<<endl;
	
	if(arr[i] > size || arr[i] < 1)
	{
		arr[i] = 0;
		return;
	}
	
	if(arr[i] == i + 1)
		return;	/* The element is in its right place */
	
	tmp = arr[arr[i]-1];
	arr[arr[i]-1] = arr[i];
	arr[i] = 0;
	
	while(1)
	{
		cout<<"tmp="<<tmp<<endl;
		if(tmp > size || tmp < 1)
			break;
		
		tmp2 = arr[tmp-1];
		arr[tmp-1] = tmp;
		tmp = tmp2;
		gSteps++;
	}
}

int main()
{
	vector <int> a(VECTOR_SIZE);
	string str;

	srand(time(NULL));
	for(int i=0;i<VECTOR_SIZE;i++)
	{
		a[i] = rand()%1000;
		//a[i] = VECTOR_SIZE-i;
	}
	//a[30] = 1002;
	for(int i=0;i<VECTOR_SIZE;i++)
	{
		cout<<a[i]<<"\t";
	}
	cout<<endl<<"continue?:";
	cin>>str;
	
	if(str == "no")
		return 0;
	
	for(int i=0;i<VECTOR_SIZE;i++)
	{
		putInPlace(a, VECTOR_SIZE, i);
	}
	
	for(int i=0;i<VECTOR_SIZE;i++)
	{
		cout<<a[i]<<"\t";
	}
	
	int i=0;
	for(i=0;i<VECTOR_SIZE;i++)
	{
		if(0 == a[i])
			break;
	}
	
	cout<<endl<<"smallest missing +ve integer="<<i+1<<", found in "<<gSteps<<" steps!"<<endl;
}