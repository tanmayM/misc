#include <iostream>
#include <stdlib.h>

using namespace std;

//class cIter;

class c
{
    int arr[10];
    //friend class cIter;
    public:
    c(int ar[], int sz)
    {
        if(sz>10)
        {
            cout<<"can't create class c object. size greater than max"<<endl;
            exit(1);
        }
        for(int i=0;i<sz;i++)
        {
            arr[i] = ar[i];
        }
    }
    class cIter
    {
        c &cRef;
        int index;
        public:
        cIter(c &cObj, int i):cRef(cObj), index(i)
        {}

        void operator ++()
        {
            index++;
        }

        void operator ++(int)
        {
                    index++;
        }
        void operator --()
        {
            index--;
        }

        int operator *()
        {
            return cRef.arr[index];
        }

        bool operator ==(cIter obj)
        {
            if (this->index == obj.index)
                return true;
            else
                return false;
        }
        bool operator !=(cIter obj)
        {
            if (this->index != obj.index)
                return true;
            else
                return false;
        }

    };

    cIter begin();
    cIter end();
};

c::cIter c:: begin()
{
    return cIter(*this, 0);
}


c::cIter c:: end()
{
    return cIter(*this, 10);
}


int main()
{
    int ar[10] = {0,1,2,3,4};
    c cObj(ar,5);

    for(c::cIter it = cObj.begin(); it != cObj.end();it++)
    {
        cout<<*it<<endl;
    }
    return 0;
}
