#include <iostream>
#include <stdio.h>
#include <tr1/functional>
#include <string>

#define LOGE(...) {printf(__VA_ARGS__);}
#define DEFAULT_SIZE 5
using namespace std;

typedef struct Htable
{
	std::string key;
	int val;
	bool valid;
	unsigned int next;
}Htable;

class hashmap
{
	public:
		hashmap();
		~hashmap();
		int add(char * key, int val);
		int get(char * key, int * val);
		int del(char * key);
		void printMap();	
	private:
		int findSlot(unsigned int *hash, string key, unsigned int * lastInChain);
		Htable htable[DEFAULT_SIZE];
		int size;
		std::tr1::hash<std::string> strHash;
};

hashmap :: hashmap()
{
	size = DEFAULT_SIZE;
	for (int i=0;i<size;i++)
	{
		htable[i].val = 0;
		htable[i].valid = false;
	}
}

hashmap :: ~hashmap()
{
}

int hashmap :: findSlot(unsigned int *hash, string key, unsigned int * lastInChain)
{	
	int i=0;
	unsigned int pos = *hash;
	
	for ( i=0; i< size;i++)
	{
		if(htable[pos].next !=-1)
		{
			break;			
		}
		pos = htable[pos].next;
	}
	
	if(i == size)
	{
		// log it
		return -1;
	}
	
	for(i=0;i<size;i++)
	{
		if(false == htable[i].valid)
			break;
	}
	
	if(i == size)
	{
		//log it
		return -1;

	}
	*lastInChain = pos;
	*hash = i;
	
	return 0;
}

int hashmap :: add(char * key, int v)
{
	unsigned int pos, lastInChain;
	int ret=0;
	bool collision = false;

	if(NULL == key)
		return -2;
	
	std::string str(key);
	pos = (unsigned int)strHash(str);
	
	cout<<"pos1="<<pos<<endl;
	pos = pos%size;
	if(true == htable[pos].valid)
	{
		collision = true;
		if(str == htable[pos].key)
		{
			//log it : Key already present
			cout<<"Key "<< str << " already present" <<endl;
			return 0;
		}
		//LOGE("%s: hash: %d occupied. cant add new element with key %s\n", __FUNCTION__, pos, key);
		/* There is collision. Find next slot */
		ret = findSlot(&pos, str, &lastInChain);
		if(-1 == ret)
		{
		// log it
		return -1;	
		}
	}
	
	htable[pos].val= v;
	htable[pos].key= str;
	htable[pos].valid = true;
	if(collision == true)
		htable[lastInChain].next = pos;
	
	cout<<"add success: {"<<key<<", "<<v<<"} added at pos:"<<pos<<endl;
	return 0;
}

int hashmap :: del(char * key)
{
	unsigned int pos;
	std::string str(key);
	pos = strHash(str);
	pos = pos%size;

	if(false == htable[pos].valid)
	{
		LOGE("%s: Key not present\n", __FUNCTION__);
		return -1;
	}
	htable[pos].valid = false;

	return 0;
}

int hashmap :: get(char * key, int * val)
{
	unsigned int pos;
	std::string str(key);
	pos = strHash(str);
	pos = pos%size;
	if(htable[pos].valid)
	{
		*val = htable[pos].val;
		return 0;
	}
	
	LOGE("%s: Key not present\n", __FUNCTION__);
	return -1;
}

void hashmap :: printMap()
{
	int i=0;
	for(i=0;i<size;i++)
	{
		if(htable[i].valid)
		{
			cout << i << ":: key:" << htable[i].key << ", value:" << htable[i].val << endl;
		}
	}
}

int main()
{
	hashmap m;
	char str[50];
	char * c = str;
		
	while(1)
	{
	cout<<"Enter a string:";
	cin>>str;
	if(!strcmp(str, "exit"))
		break;
	m.add(c, 1);
	}
	
	m.printMap();
}
