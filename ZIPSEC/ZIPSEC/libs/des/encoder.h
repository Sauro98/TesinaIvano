#ifndef ENCODER_H
#define ENCODER_H

#include "permutations.h"
#include "key_manager.h"
#include "secure_boxes.h"

#define mask_first_six_bits   0xFC0000000000ULL
#define mask_second_six_bits  0x3F000000000ULL
#define mask_third_six_bits   0xFC0000000ULL
#define mask_fourth_six_bits  0x3F000000
#define mask_fifth_six_bits   0xFC0000
#define mask_sixth_six_bits   0x3F000
#define mask_seventh_six_bits 0xFC0
#define mask_eighth_six_bits  0x3F

class Encoder
{
	public:
		Encoder(ulong key);
		ulong encode(ulong to_encode);
		ulong decode(ulong to_decode);
		uint feistel(uint original,int cycle,bool decoding);
	
	private:
		Key_manager k_manager;
};

#endif
