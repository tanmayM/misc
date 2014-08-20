/* You are given an array of size 2N. Lets call first N elements to be As and last N elements to be Bs. So array is:
A1 A2 A3 A4 .. An B1 B2 B3 B4.. Bn
Your task is to re-arrange elements such that array becomes:
A1 B1 A2 B2 A3 B3 A4 B4.. An Bn
*/

/* Solution: We want to do it 'in-place' i.e. without using variable extra space.
   So, strategy we use here is to put single element in its correct place in one iteration.
   i.e.
   A1 ...
   A1 B1 ...
   A1 B1 A2 ...
   A1 B1 A2 B2 ...
   
   The catch here is that there is no ordering between As and Bs.. they are just unsorted numbers.
   So, each time we put an element in its correct place, we have to maintain the relative order of rest of the elements.
   A1 A2 A3 .. B1 B2 B3 ...            (A1 fixed)
   A1 B1 A2 A3 A4 ... B2 B3 B4 ...     (B1 fixed)
   A1 B1 A2 A3 A4 ... B2 B3 B4 ...     (A2 fixed)
   A1 B1 A2 B2 A3 A4 ... B3 B4 ...     (B2 fixed)
   
   So, basically we 'pluck' a B from its current place, put it in its correct place and slide rest of the elements to its right.
   To achieve this in place, we use a method of toggling the Bs to the left untill it reaches its correct place
 
	Time Complexity : O(n^2)
	Space Complexity : O(1) extra space
 */
 
 #include <iostream>
 #include <vector>
 #include <time.h>
 #include <stdlib.h>
 
 using namespace std;
 #define SIZE 1000000
 
 int merge(vector<int>& arr, int n)
 {
	int steps=0;
 
	for(int i=1, j=n/2;i<n-1; i+=2)
	{
		/* Get A[j] to A[i] */
		int k = j, tmp=arr[j];
		while(k > i)
		{
			arr[k] = arr[k-1];
			k--;
			steps++;
		}		
		arr[i] = tmp;
		j++;
	}
		
	
	return steps;
 }
 
 int main()
 {
	vector<int> arr(SIZE);
	
	srand(time(NULL));
	
	for(int i=0;i<SIZE/2;i++)
	{
		arr[i] = rand()%50;
	}
	
	for(int i=SIZE/2;i<SIZE;i++)
	{
		arr[i] = (rand()%50) - 100;
	}
	
	for(int i=0;i<SIZE;i++)
	{
		cout<<arr[i]<<" ";
	}
	cout<<endl;
	
	int steps = merge(arr, SIZE);
	
	cout<<"After merging::"<<endl;
	for(int i=0;i<SIZE;i++)
	{
		cout<<arr[i]<<" ";
	}
	cout<<endl;
	
	cout<<"Elements="<<SIZE<<", steps="<<steps<<endl;
 }
 
 
