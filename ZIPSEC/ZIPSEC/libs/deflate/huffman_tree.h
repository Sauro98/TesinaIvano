#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include "constants.h"
#include "queue.h"
#include <math.h>
#include "global_functions.cpp"
#define ASCII_charACHTERS_COUNT 256



// Class to build a Huffman tree from a string
class Huffman_Tree
{
	public:
		// class constructor
		Huffman_Tree(std::string _original_string);
		// class destructor
		~Huffman_Tree();
		// encoding function with RFC 1951 guidelies
		static code_d* dynamic_tree_encoding(code_d* raw_codes,int codes_length,int alphabet_type);

		//static void dynamic_tree_encoding(code_d * raw_codes, code_d * deflate_dictionary, int codes_length, int alphabet_type);
		
		//---FREQUENCE ARRAY RELATED FUNCTIONS---

		static code_d * generate_tree(val_f* frequencies, int length, int * out_length,int MAX);
        
        static int* get_code_length_codes(code_d* codes,bool literals,int* extra_bits,int* codes_array_length);

	private:
		//---ATTRIBUTES---
		//the original string to encode
        std::string original_string;

        //---TREE GENERARION RELATED FUNCTIONS---
        //dequeue first element of the queue with the smallest frequence in the first element
        static node_f* get_smallest_node_in_queues(Queue* q1,Queue* q2);
        //standart c++ method to get the integer max value, used only in "get_smallest_node_in_queues" function
        static int max_int_value();
        

        
};

#endif // HUFFMAN_TREE_H
