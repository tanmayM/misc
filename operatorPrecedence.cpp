#include <iostream>
#include <stdio.h>
using namespace std;


int main(){  
  int x, y, z;  
  x = y = z = -1;  
  y = ++x && ++y && ++z;  
  printf("x = %d, y = %d, z = %d", x, y, z);  
  return 0;  
} 