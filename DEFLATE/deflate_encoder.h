#ifndef DEFLATE_ENCODER_H
#define DEFLATE_ENCODER_H

#include "lz77_encoder.h"

#define STATIC_7_BITS_CODES 48
#define STATIC_8_BITS_CODES 400
#define MAX_BITS  10

#define MASK_1_BIT 1U
#define MASK_2_BITS 3U
#define MASK_3_BITS 7U
#define MASK_4_BITS 15U
#define MASK_5_BITS 31U
#define MASK_6_BITS 63U
#define MASK_7_BITS 127U
#define MASK_8_BITS 255U
#define MASK_9_BITS 511U


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
		long get_mask_for_length(int length);
		
		
		void print_binary_literal(unsigned int literal,int length);
		void print_reversed_binary_literal(int literal,int length);
		
		void add_code_to_string(long code,int length,unsigned char* current_to_add,int* current_to_add_bit,std::string* compressed);
		
		//static tree codes generation functions
			//LITERALS AND LENGTHS
			long* construct_static_leteral_and_lengths_tree();
			int get_static_literal_length(int literal);
			int get_static_length_code(long length);
			int get_static_length_extra_bits(long length,int* extra_bits);
			
			//DISTANCES
			int get_static_distance_code(long distance);
			int get_static_distance_extra_bits(long distance,int* extra_bits);
	protected:
};

#endif
