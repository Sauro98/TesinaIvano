#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdlib.h>
#include <iostream>
#include <string.h>

#define print(x) std::cout<<x;
#define println(x) std::cout<<x<<std::endl;

typedef struct item{
	char* key;
	int value;
	
	item(char* _key,int _value){
		key = _key;
		value = _value;
	}
	
	item(){
		key = NULL;
	}
	
}item;


#endif

