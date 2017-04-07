#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include "constants.h"
#include "queue.h"
#include <math.h>
#include <vector>

#define ASCII_CHARACHTERS_COUNT 256



// Class to build a Huffman tree from a string
class Huffman_Tree
{
	public:
		// class constructor
		Huffman_Tree(std::string _original_string);
		// class destructor
		~Huffman_Tree();
		// encoding function with RFC 1951 guidelies
		static code_d* dynamic_tree_encoding(node_f* root_node,val_f* frequencies,int f_length,bool literals);
		// decoding function with RFC 1951 guidelies
		std::string static_tree_decoding(int* lengths,std::string encoded);
		
		//---FREQUENCE ARRAY RELATED FUNCTIONS---
        //returns an array of char frequencies to be stored in the attribute "frequencies"
        static val_f* get_value_frequencies(std::vector<long> input_vector,int* frequencies_length);
        //bubblesorts the frequence array (better sorting algorithm to be added later)
        static void order_frequence_array(val_f* frequencies,int length);
        //generates the tree
        static node_f* generate_tree(val_f* frequencies,int length);
        
        static int* get_code_length_codes(code_d* codes,bool literals,int* extra_bits);
	private:
		//---ATTRIBUTES---
		//the original string to encode
        std::string original_string;
        
        //---TREE GENERARION RELATED FUNCTIONS---
        //dequeue first element of the queue with the smallest frequence in the first element
        static node_f* get_smallest_node_in_queues(Queue* q1,Queue* q2);
        //standart c++ method to get the integer max value, used only in "get_smallest_node_in_queues" function
        static long max_long_value();
        //now useless debug print, left here if more debugging will be needed (recursive)
        static void print_LVR_tree(node_f* start,std::string prefix); 
        
        //---STRING ENCODING RELATED FUNCTIONS---
        //returns the Huffman prefix of the given charachter (recursive)
        static std::string get_char_value(node_f* start,long to_find);
        //orders the dictionary array for DEFLATE
        static void order_dictionary(val_d dictionary[],int d_length);
        
};

#endif // HUFFMAN_TREE_H
