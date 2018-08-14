#include "encoder.h"

Encoder::Encoder(ulong key) {
    k_manager = Key_manager(key);
}

ulong Encoder::encode(ulong to_encode, bool rev) {
    ulong permuted = 0;
    initial_permutation(to_encode, permuted);
    auto left_block = (uint) (permuted >> (ushort) 32);
    ulong right_block = permuted & 0xFFFFFFFF;
    for (int cycle = 0; cycle < 16; cycle++) {
        auto temp = (uint) right_block;
        ulong expanded = 0, modified = 0;
        expansion_permutation(right_block, expanded);
        expanded ^= k_manager.get_subkey(rev ? 15 - cycle : cycle);
        for (uchar a = 0; a < 8; a++)
            modified |= SECURE_WITH_BOX(get_sixs_bits(expanded, a), a) << (ushort) (28 - 4 * a);
        expanded = 0;
        fiestel_permutation(modified, expanded);
        right_block = left_block ^ expanded;
        left_block = temp;
    }
    //si mette prima rightblock perchè nell'ultimo passaggio non si invertono
    ulong joined = ((ulong) right_block << (ushort) 32) | left_block;
    ulong encoded = 0;
    final_permutation(joined, encoded);
    return encoded;
}

Encoder::Encoder() {}
