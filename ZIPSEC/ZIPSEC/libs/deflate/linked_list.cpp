#include "linked_list.h"

Linked_list::Linked_list(){
	root = NULL;
	size = 0;
	last_searched = NULL;
	last_searched_index = 0;
}

Linked_list::Linked_list(int value) {
	root = new node(value);
	last_added = root;
	size = 1;
	last_searched = NULL;
	last_searched_index = 0;
}


Linked_list::~Linked_list(){
	free(root);
}

void Linked_list::add_item(int to_add){
	if(root == NULL){
		root = new node(to_add);
		last_added = root;
	    size++;
		return;
	}
	last_added->next = new node(to_add);
	last_added = last_added->next;
	size++;
}

int Linked_list::get_size(){
	return size;
}

int Linked_list::get_size(int curr_i){
	node* current = root;
	int curr_size = 0;
	while(current != NULL && current->value < curr_i){
		current = current->next;
		curr_size++;
	}
	return curr_size;
}

int Linked_list::at(int position){
	if(position >= size)
		return -1;
	
	last_searched = (last_searched == NULL)?root:last_searched;
	int a;
	for(a = last_searched_index; a < position; a++){
		last_searched = last_searched->next;
	}
	last_searched_index = a;
	return last_searched->value;
	last_searched = NULL;
	
}


void Linked_list::reset_search(){
	last_searched = NULL;
	last_searched_index = 0;
}


void Linked_list::get_values(int curr_i,int* subs_positions,int* resp_size) {

	//there are a maximum of  SEARCH_BUFFER_LENGTH/3 occurrences of a substring of length 3 in a buffer of size SEARCH_BUFFER_LENGTH
	
	int final_size = 0;

	node* curr = root;

	while (curr != NULL) {
		int val = curr->value;
		//too forward
		if (val > (curr_i - 1))
			break;
		//just rigth
		if (val >  curr_i - SEARCH_BUFFER_LENGTH) {
			subs_positions[final_size] = val;
			final_size++;
			curr = curr->next;
		} else {
			//too old
			node* old = root;
			root = root->next;  //the deleted node will always be the root, because the elements are ordered
			curr = curr->next; 
			old->next = NULL;   //avoid destroying the entire linked list when the destructor is called
			delete old;
		}
		 
	}

	*resp_size = final_size;
}

