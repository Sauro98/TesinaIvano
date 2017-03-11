#ifndef DEFLATE_ENCODER_H
#define DEFLATE_ENCODER_H

#include "lz77_encoder.h"

class Deflate_encoder
{
	public:
		std::string original;
		Deflate_encoder(std::string to_compress);
		std::string encode();
	protected:
};

#endif
