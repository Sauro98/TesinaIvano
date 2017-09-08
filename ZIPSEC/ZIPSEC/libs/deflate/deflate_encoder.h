#ifndef DEFLATE_ENCODER_H
#define DEFLATE_ENCODER_H

#include <vector>

#include "lz77_encoder.h"
#include "global_functions.cpp"
#include "huffman_tree.h"


class Deflate_encoder
{
	public:
		Deflate_encoder();
		~Deflate_encoder();
		std::string encode(char* to_compress, int length);
		
		
	protected:
		//current output char
		char current_to_add;
		//current output bit
		int current_to_add_bit;
		//static tree codes
		int* static_tree;


		void do_encoding(char* to_compress, int length,int dec_type);

		//BTYPE 01
		void static_encoding(char* to_compress, int length);
		unsigned int encode_static_literal(int literal, int* literal_length);

		//BTYPE00
		void non_compressed_encoding(char* to_compress, int length);

		// BTYPE 10
		void dynamic_encoding(char* to_compress, int length);

		void add_dynamic_alphabet_content(int code_lengths, code_d* codes, int* extra_bits_array, int* code_lengths_array, int HLIMIT);
		void add_length_distance_to_dynamic_content(int value, bool length, code_d* codes);

		void print_reversed_binary_literal(int literal, int length);

		void add_code_to_string(int code, int length);
		void add_code_to_string(int code, int length, bool reversed);

		//static tree codes generation functions
		//LITERALS AND LENGTHS
		int get_static_length_code(int length);

		//DISTANCES
		int get_static_distance_code(int distance);
};

#endif
