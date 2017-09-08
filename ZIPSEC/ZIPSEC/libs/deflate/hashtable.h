#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "constants.h"
#include "linked_list.h"
#include "global_functions.cpp"

class HashTable
{
	public:
		HashTable(long _size);
		HashTable();
		~HashTable();
		bool insert_item(char* key,int value);
		int* search(char* key,int* resp_size,int curr_i);
		void print_table();
		void clear();
		void set_size(int _size);
	protected:
		long size;
	    item* items;	
	    int do_hash(unsigned char* key);
		bool is_clean;
};

#endif
