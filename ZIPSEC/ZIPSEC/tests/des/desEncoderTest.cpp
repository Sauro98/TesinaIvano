//
// Created by Ivano on 11/08/2018.
//

#include <gtest/gtest.h>
#include <libs/des/permutations.h>
#include <libs/des/encoder.h>
#include <libs/des/des_chipher.h>

TEST(Encoder, encode) {
    ulong toEncode = ((ulong) 70) + ((ulong) 71 << (ulong) 8) |
                     ((ulong) 72 << (ulong) 16) |
                     ((ulong) 73 << (ulong) 24) |
                     ((ulong) 74 << (ulong) 32) |
                     ((ulong) 75 << (ulong) 40) |
                     ((ulong) 76 << (ulong) 48) |
                     ((ulong) 77 << (ulong) 56);

    Encoder encoder(DES_Cipher::get_ulong_from_string("12345678", 0));
    ulong encoded = encoder.encode(toEncode,false);
    ulong expected = 13419173996204529737ULL;
    ASSERT_EQ(encoded, expected);
}

TEST(Encoder, decode) {
    ulong expected = ((ulong) 70) + ((ulong) 71 << (ulong) 8) |
                     ((ulong) 72 << (ulong) 16) |
                     ((ulong) 73 << (ulong) 24) |
                     ((ulong) 74 << (ulong) 32) |
                     ((ulong) 75 << (ulong) 40) |
                     ((ulong) 76 << (ulong) 48) |
                     ((ulong) 77 << (ulong) 56);
    ulong toDecode = 13419173996204529737ULL;
    Encoder encoder(DES_Cipher::get_ulong_from_string("12345678", 0));
    ulong decoded = encoder.encode(toDecode,true);
    ASSERT_EQ(decoded, expected);
}
