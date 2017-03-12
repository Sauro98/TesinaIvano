#ifndef DEFLATE_ENCODER_H
#define DEFLATE_ENCODER_H

#include "lz77_encoder.h"

#define STATIC_7_BITS_CODES 48
#define STATIC_8_BITS_CODES 400
#define  MAX_BITS  10


class Deflate_encoder
{
	public:
		std::string original;
		Deflate_encoder(std::string to_compress);
		std::string encode();
		
		//BTYPE 01
		std::string static_encoding(std::string to_compress);
		unsigned int encode_static_literal(int literal,int* literal_length);
		int* construct_static_tree();
		
		
		void print_binary_literal(int literal,int length);
		void print_reversed_binary_literal(int literal,int length);
	protected:
};

#endif
