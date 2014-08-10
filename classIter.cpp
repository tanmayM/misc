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
   
 class myClass
 {
	public:
		myClass() {x=0; y=0;}
		void set(int x, int y){this->x = x; this->y = y;}
		friend ostream& operator<<(ostream& in, myClass c);
	private:
		int x;
		double y;
 };
 
 ostream& operator<<(ostream& in, myClass c)
 {
	in<<c.x<<","<<c.y<<endl;
	return in;
 }
   
class myClassArrIter
{
public:
	myClassArrIter(myClass * c):arr(c) {}
	myClassArrIter(const myClassArrIter &citer):arr(citer.arr){}
	myClassArrIter& operator++() { ++arr; return *this;}
	myClassArrIter operator++(int) {myClassArrIter tmp(*this); operator++(); return tmp;}
	myClass operator*() { return *arr;}
	bool operator!=(myClassArrIter citer) {return this->arr != citer.arr;}
	
private:
	myClass * arr;	
};

int main()
{
	myClass arr[10];
	
	for(int i=0;i<10;i++)
	{
		arr[i].set(i,i);
	}
	
	myClassArrIter itrStart(arr);
	myClassArrIter itrEnd(arr+10);
	
	for(;itrStart != itrEnd; itrStart++)
	{
		cout<<*itrStart<<endl;
	}
}