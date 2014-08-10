#include <iostream>

using namespace std;

/* An Iterator works on an independent different class (or basic data type).
   The idea of iterator is to provide the functionality of iterating over an array (any list generically) of 
   objects of that class so that loops can be conveniently written to loop over the array of user defined 
   types. 
   For basic data types or even classes, I don't see any advantage of iterators over array indices. However,
   in case of advance data structures provided by STL or other libraries, the subscript operator may not be
   defined and the collection itself may not be sequential. Thirdly, we may want to iterate over a collection
   in a different order than sequential.
   */
class CharArrIter
{
public:
	CharArrIter(char * c):arr(c) {}
	CharArrIter(const CharArrIter &citer):arr(citer.arr){}
	CharArrIter& operator++() { ++arr; return *this;}
	CharArrIter operator++(int) {CharArrIter tmp(*this); operator++(); return tmp;}
	char operator*() { return *arr;}
	bool operator!=(CharArrIter citer) {return this->arr != citer.arr;}
private:
	char * arr;	
};

int main()
{
	char arr[10] = "tanmay";
	
	CharArrIter itrStart(arr);
	CharArrIter itrEnd(arr+6);
	
	for(;itrStart != itrEnd; itrStart++)
	{
		cout<<*itrStart<<endl;
	}
}