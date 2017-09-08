#include "hashtable.h"

HashTable::HashTable(long _size){

	size = _size;
	if(size > 0){	
		items = new item[size];
	}else{
		items = new item[1];
	}
	is_clean = true;
}

HashTable::HashTable(){
}

HashTable::~HashTable()
{
	//delete[] items;
}

int HashTable::do_hash(unsigned char* key){
    unsigned int hash = 0;
    int c;
    while (c = *key++)
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash % size;
    
}
//strcmp(items[hash].key,key) == 0
bool HashTable::insert_item(char* key, int value){
	is_clean = false;
	int hash = do_hash((unsigned char*)key);
	int curr_index = 0;
	bool can_run = true;
	while(can_run){
		if(!items[hash].is_set || (char_to_uint(items[hash].key[0]) == char_to_uint(key[0]) && char_to_uint(items[hash].key[1]) == char_to_uint(key[1]) && char_to_uint(items[hash].key[2]) == char_to_uint(key[2]))){
			strncpy(items[hash].key, key, HASHTABLE_SUBSTRING_LENGTH);
			items[hash].value.add_item(value);
			items[hash].set();
			return true;
		}
		curr_index++;
		hash++;
		hash %= size;
		if(can_run)
			can_run = curr_index < size;
	}
	return false;
}

/*int* HashTable::search(char* key,int* resp_size,int curr_i){
	//println("Searching for key "<<key);
	int hash = do_hash((unsigned char*)key);
	//println("HASH DONE");
	int curr_index = 0;
	while(curr_index < size){
		//println("curr_index "<<curr_index);
		if(items[hash].is_set && (char_to_uint(items[hash].key[0]) == char_to_uint(key[0]) && char_to_uint(items[hash].key[1]) == char_to_uint(key[1]) && char_to_uint(items[hash].key[2]) == char_to_uint(key[2]))){
			return items[hash].value.get_values(curr_i, resp_size);
		}
		curr_index++;
		hash++;
		hash %= size;
	}
	*resp_size = 0;
	return new int[0];
}*/

void HashTable::print_table(){
	/*println("CONTENT[");
	for(int a = 0; a < size; a++){
		if(items[a].key){
			print("Element["<<a<<"] key "<<items[a].key<<" value ");
			for(int i = 0; i < items[a].value.get_size();i++){
				print(","<<items[a].value.at(i));
			}
			println("");
		}
	}
	println("]");*/
}

void HashTable::clear(){
	if (!is_clean) {
		for (int a = 0; a < size; a++) {
			items[a]= item();
		}
		is_clean = true;
	}
}

void HashTable::set_size(int _size){
	size = _size;
}



