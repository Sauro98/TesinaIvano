#include "encoder.h"

Encoder::Encoder(ulong key)
{
	k_manager = Key_manager(key);
}

ulong Encoder::encode(ulong to_encode) {
    ulong encoded = 0;
    ulong permuted = Permutations::initial_permutation(to_encode) ;
    uint left_block = (permuted >> 32);
    uint right_block = ((permuted << 32) >> 32);
    for (int cycle = 0;cycle < 16; cycle++) {
        uint temp = right_block;
        right_block = feistel(right_block,cycle,false);
        right_block = left_block ^ right_block;
        left_block = temp;
    }
    //si mette prima rightblock perchè nell'ultimo passaggio non si invertono
    ulong joined = right_block;
	joined <<= 32;
	joined |= left_block;
    encoded = Permutations::final_permutation(joined);
    return encoded;
}

ulong Encoder::decode(ulong to_decode) {
    ulong permuted = Permutations::initial_permutation(to_decode);
    uint left_block = (uint)(permuted >> 32);
    uint right_block = (uint)((permuted << 32) >> 32);
    for (int cycle = 0; cycle < 16; cycle++) {
        uint temp = right_block;
        right_block = feistel(right_block, cycle, true);
        right_block = left_block ^ right_block;
		left_block = temp;
    }

    ulong joined = right_block;
    joined <<= 32;
	joined = joined | left_block;

    ulong decoded_text = Permutations::final_permutation(joined);
	return decoded_text;
            
}

uint Encoder::feistel(uint original,int cycle,bool decoding) {
    uint modified = 0;
    ulong expanded = Permutations::expansion_permutation(original);
    ulong subkey = (decoding)? k_manager.get_subkey(15-cycle) : k_manager.get_subkey(cycle);
    ulong miscelated = expanded ^ subkey;
    uchar blocks[8];

    blocks[0] = (uchar)((miscelated & mask_first_six_bits) >> 42);
    blocks[1] = (uchar)((miscelated & mask_second_six_bits) >> 36);
    blocks[2] = (uchar)((miscelated & mask_third_six_bits) >> 30);
    blocks[3] = (uchar)((miscelated & mask_fourth_six_bits) >> 24);
    blocks[4] = (uchar)((miscelated & mask_fifth_six_bits) >> 18);
    blocks[5] = (uchar)((miscelated & mask_sixth_six_bits) >> 12);
    blocks[6] = (uchar)((miscelated & mask_seventh_six_bits) >> 6);
    blocks[7] = (uchar)(miscelated & mask_eighth_six_bits);

    blocks[0] = Secure_boxes::secure_box_1(blocks[0]);
    blocks[1] = Secure_boxes::secure_box_2(blocks[1]);
    blocks[2] = Secure_boxes::secure_box_3(blocks[2]);
    blocks[3] = Secure_boxes::secure_box_4(blocks[3]);
    blocks[4] = Secure_boxes::secure_box_5(blocks[4]);
    blocks[5] = Secure_boxes::secure_box_6(blocks[5]);
    blocks[6] = Secure_boxes::secure_box_7(blocks[6]);
    blocks[7] = Secure_boxes::secure_box_8(blocks[7]);

    modified = modified | (uint)(blocks[0] << 28);
    modified = modified | (uint)(blocks[1] << 24);
    modified = modified | (uint)(blocks[2] << 20);
    modified = modified | (uint)(blocks[3] << 16);
    modified = modified | (uint)(blocks[4] << 12);
    modified = modified | (uint)(blocks[5] << 8);
    modified = modified | (uint)(blocks[6] << 4);
    modified = modified | (uint)(blocks[7]);

    modified = Permutations::fiestel_permutation(modified);

    return modified;
}
