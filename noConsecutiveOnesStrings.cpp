/* Problem: Given a positive integer N, count all possible distinct binary strings of length N such that there are no consecutive 1′s.
*/

/* Solution: We can use the principle of dynamic programming here. Store the maximum strings that can be created with x bits and
   use that to find the strings that can be created with x+1 bits.
   
   We don't want consecutive 1s. So we differentiate with strings starting with 0 and strings starting with 1. So we keep
   2 counts (withZero, withOne). When we calculate for x+1 bits, we also calculate for (x+1)th bit being 
   0 and 1. 
   
   For x+1 bits, a string starting with 0 can work with ALL the strings that can be created with x bits (withZero+withOne).
   For strings starting with 1, we can only combine with withZero. We store these 2 values for x+1 bits and
   use it to calculate for x+2 bits.
   
   Time Complexity: O(N)
   Space complexity : constant
*/

#include <iostream>

using namespace std;

int main()
{
	int withZero, withOne, noOfBits, tmp;
	
	
	while(1)
	{
	withZero=1;
	withOne=1;

	cout << "Enter the number of digits::";
	cin >> noOfBits;
	if(0==noOfBits)
		break;
	
	for(int i=2; i<=noOfBits; i++)
	{
		tmp = withZero + withOne;
		withOne = withZero;
		withZero = tmp;
		//cout << "i=" << i << ", wi
	}
	
	cout << "Max no of strings without consecutive 1s with " << noOfBits << " bits = " << withZero + withOne << endl;
	}
	return 0;
}



