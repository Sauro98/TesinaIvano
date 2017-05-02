#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "linked_list.h"



typedef struct item{
	char* key;
	Linked_list value;
	
	item(){
		key = NULL;
		value = Linked_list();
	}
	
}item;


#endif

