#ifndef DEFLATE_ENCODER_H
#define DEFLATE_ENCODER_H

#include "lz77_encoder.h"
#include "global_functions.cpp"

class Deflate_encoder
{
	public:
		long* static_tree;
		
		std::string original;
		Deflate_encoder(std::string to_compress);
		std::string encode();
		
		//BTYPE 01
		std::string static_encoding(std::string to_compress);
		unsigned int encode_static_literal(int literal,int* literal_length);
		
		
		//void print_binary_literal(unsigned int literal,int length);
		void print_reversed_binary_literal(int literal,int length);
		
		void add_code_to_string(long code,int length,char* current_to_add,int* current_to_add_bit);
		
		//static tree codes generation functions
			//LITERALS AND LENGTHS
			int get_static_length_code(long length);
			//int get_static_length_extra_bits(long length,int* extra_bits);
			
			//DISTANCES
			int get_static_distance_code(long distance);
			//int get_static_distance_extra_bits(long distance,int* extra_bits);
	protected:
};

#endif
