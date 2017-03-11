#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include "constants.h"
#include "queue.h"
#include <math.h>

//values for debug
//0 NO DEBUG
//1 NORMAL DEBUG
//2 DEEP DEBUG
#define DEBUG 1
#define TEST

#define ASCII_CHARACHTERS_COUNT 256



// Class to build a Huffman tree from a string
class Huffman_Tree
{
	public:
		// class constructor
		Huffman_Tree(std::string _original_string);
		// class destructor
		~Huffman_Tree();
		// encoding function
		std::string encode();
		// encoding function with RFC 1951 standard static tree
		std::string static_tree_encoding(int* lengths);
		// decding functions
		std::string decode(std::string encoded);
		// decoding function with RFC 1951 standard static tree
		std::string static_tree_decoding(int* lengths,std::string encoded);
	private:
		//---ATTRIBUTES---
		//the original string to encode
        std::string original_string;
        //length of the char frequencies array for loops iterations
        int frequencies_array_length;
        //array with each charachter in the original string and its frequence in the string
        char_f* frequencies;
        //the tree node from wich access the rest of the tree
        node_f* root_node;
        
        
        //---FREQUENCE ARRAY RELATED FUNCTIONS---
        //returns an array of char frequencies to be stored in the attribute "frequencies"
        char_f* get_char_frequencies();
        //bubblesorts the frequence array (better sorting algorithm to be added later)
        void order_frequence_array();
        
        //---TREE GENERARION RELATED FUNCTIONS---
        //dequeue first element of the queue with the smallest frequence in the first element
        node_f* get_smallest_node_in_queues(Queue* q1,Queue* q2);
        void generate_tree();
        //standart c++ method to get the integer max value, used only in "get_smallest_node_in_queues" function
        int max_int_value();
        //now useless debug print, left here if more debugging will be needed (recursive)
        void print_LVR_tree(node_f* start,std::string prefix); 
        
        //---STRING ENCODING RELATED FUNCTIONS---
        //returns the Huffman prefix of the given charachter (recursive)
        std::string get_char_value(node_f* start,char to_find);
        //actually encodes the original string
        std::string translate_original_in_encoded();
        //orders the dictionary array for DEFLATE
        void order_dictionary(char_d dictionary[]);
        
        //---STRING DECODING RELATED FUNCTIONS---
        std::string transform_encoded_string_to_binary(std::string encoded);
};

#endif // HUFFMAN_TREE_H
