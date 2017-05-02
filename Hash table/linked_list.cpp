#include "linked_list.h"

Linked_list::Linked_list(){
	root = NULL;
	size = 0;
}

void Linked_list::add_item(int to_add){
	if(size == 0){
		root = new node(to_add);
		//println("Added root "<<root->value);
	    size++;
		return;
	}
	node* curr_node = root;
	while(curr_node->next != NULL){
		curr_node = curr_node->next;
	}
	curr_node->next = new node(to_add);
	//println("Added next "<<curr_node->next->value);
	size++;
}

int Linked_list::get_size(){
	return size;
}

node* Linked_list::at(int position){
	if(position >= size)
		return NULL;
	
	node* current_node = root;
	for(int a = 0; a < position; a++)
		current_node = current_node->next;
	return current_node;
	
}

void Linked_list::delete_item(int position){
	if(position >= size)
		return;
	
	if(position == 0){
		root = root->next;
		size--;
		return;
	}
	
	node* current_node = root;
	for(int a = 0; a < position - 1; a++){
		current_node = current_node->next;
	}
	//skip next node
	current_node->next = current_node->next->next;
	
	size--;
}
