#ifndef DEFLATE_DECODER_H
#define DEFLATE_DECODER_H

#include "constants.h"
#include "global_functions.cpp"

class Deflate_decoder
{
	public:
		std::string original;
		s_node_f* static_root;
		
		Deflate_decoder(std::string _original);
		std::string decode(int * compressed_size);
		
		int get_next_dynamic_value(node_f* tree_root);
		node_f* generate_tree_from_array(int* codes,int* lengths,int codes_length,bool is_hclen);
		int get_next_static_value();
		int read_bits(int bits_count);
		int read_bits(int bits_count,bool reversed);
		int get_length_from_code(int code);
		int get_distance_from_code(int code);
		int get_alphabet_code_lengths(int* code_lengths,int size,node_f* tree_root);
	protected:
};

#endif
