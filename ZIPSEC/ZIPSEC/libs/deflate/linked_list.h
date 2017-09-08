#ifndef LINKED_LIST_H
#define LINKED_LIST_H


#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "constants.h"
#include <array>

typedef struct node{
	int value;
	node* next;
	
	node(int _value){
		value = _value;
		next = NULL;
	}
	~node(){
		free(next);
	}
	
	
	
}node;


class Linked_list
{
	public:
		Linked_list();
		Linked_list(int value);
		~Linked_list();
		void add_item(int to_add);
		int get_size();
		int get_size(int curr_i);
		int at(int position);
		void reset_search();
		void get_values(int curr_i, int * subs_positions, int * resp_size);
	protected:
		node* root;
		node* last_added;
		node* last_searched;
		int last_searched_index;
		int size;
		
};


typedef struct item{
	char key[HASHTABLE_SUBSTRING_LENGTH];
	Linked_list value;
	bool is_set;
	
	item(){
		//key = NULL;
		is_set = false;
		value = Linked_list();
	}

	void set() {
		is_set = true;
	}
	~item(){
		//free(key);
	}
	
}item;



#endif
