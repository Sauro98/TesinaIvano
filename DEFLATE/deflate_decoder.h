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
		
		std::string decode();
		std::string static_decoding(std::string to_decode);
		
		int get_next_static_value(std::string feed,long*current_char,int* current_bit);
		unsigned int read_bits(std::string feed,long* current_char,int* current_bit,int bits_count);
		unsigned int read_bits(std::string feed,long* current_char,int* current_bit,int bits_count,bool reversed);
		int get_length_from_code(int code);
		int get_distance_from_code(int code);
	protected:
};

#endif
