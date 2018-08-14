//
// Created by Ivano on 11/08/2018.
//

#include <gtest/gtest.h>
#include <libs/des/permutations.h>
#include "des_chipher.h"

TEST(DESCipher, getChar) {
    ulong testlong = (ulong) 70 | (ulong) 71 << (ulong) 8
                     | (ulong) 72 << (ulong) 16
                     | (ulong) 73 << (ulong) 24
                     | (ulong) 74 << (ulong) 32
                     | (ulong) 75 << (ulong) 40
                     | (ulong) 76 << (ulong) 48
                     | (ulong) 77 << (ulong) 56;

    auto c = (char) get_char(testlong, 7);
    ASSERT_EQ(c, 'F');
    c = (char) get_char(testlong, 6);
    ASSERT_EQ(c, 'G');
    c = (char) get_char(testlong, 5);
    ASSERT_EQ(c, 'H');
    c = (char) get_char(testlong, 4);
    ASSERT_EQ(c, 'I');
    c = (char) get_char(testlong, 3);
    ASSERT_EQ(c, 'J');
    c = (char) get_char(testlong, 2);
    ASSERT_EQ(c, 'K');
    c = (char) get_char(testlong, 1);
    ASSERT_EQ(c, 'L');
    c = (char) get_char(testlong, 0);
    ASSERT_EQ(c, 'M');

}

TEST(DES_Cipher, getUlongFromChar) {
    ulong test = get_ulong_from_char((ulong) 'F', 7);
    test |= get_ulong_from_char((ulong) 'G', 6);
    test |= get_ulong_from_char((ulong) 'H', 5);
    test |= get_ulong_from_char((ulong) 'I', 4);
    test |= get_ulong_from_char((ulong) 'J', 3);
    test |= get_ulong_from_char((ulong) 'K', 2);
    test |= get_ulong_from_char((ulong) 'L', 1);
    test |= get_ulong_from_char((ulong) 'M', 0);
    ulong expected = ((ulong) 70) + ((ulong) 71 << (ulong) 8) |
                     ((ulong) 72 << (ulong) 16) |
                     ((ulong) 73 << (ulong) 24) |
                     ((ulong) 74 << (ulong) 32) |
                     ((ulong) 75 << (ulong) 40) |
                     ((ulong) 76 << (ulong) 48) |
                     ((ulong) 77 << (ulong) 56);
    ASSERT_EQ(test, expected);
}

TEST(DES_Cipher, getStringFromUlong) {
    ulong test = ((ulong) 70) + ((ulong) 71 << (ulong) 8) |
                 ((ulong) 72 << (ulong) 16) |
                 ((ulong) 73 << (ulong) 24) |
                 ((ulong) 74 << (ulong) 32) |
                 ((ulong) 75 << (ulong) 40) |
                 ((ulong) 76 << (ulong) 48) |
                 ((ulong) 77 << (ulong) 56);
    std::string res = DES_Cipher::get_string_from_ulong(test);
    ASSERT_STREQ(res.c_str(), "MLKJIHGF");
}

TEST(DES_Cipher, getUlongFromString) {
    ulong expected = ((ulong) 70) + ((ulong) 71 << (ulong) 8) |
                     ((ulong) 72 << (ulong) 16) |
                     ((ulong) 73 << (ulong) 24) |
                     ((ulong) 74 << (ulong) 32) |
                     ((ulong) 75 << (ulong) 40) |
                     ((ulong) 76 << (ulong) 48) |
                     ((ulong) 77 << (ulong) 56);
    std::string test = "MLKJIHGF";
    ulong res = DES_Cipher::get_ulong_from_string(test.c_str(), 0);
    /*ulong res = 0;

    ulong_from_str(test.c_str(), res)*/
    ASSERT_EQ(res, expected);
}

TEST(DES_Cipher, crypt) {
    std::string toCrypt = "MLKJIHGF";
    std::string key = "12345678";
    DES_Cipher cipher(key, MODE_DES);
    std::string crypted = cipher.crypt(toCrypt);
    std::string expected;
    expected += (char) 0xBA;
    expected += (char) 0x3A;
    expected += (char) 0x7B;
    expected += (char) 0xA6;
    expected += (char) 0xD6;
    expected += (char) 0xEE;
    expected += (char) 0x18;
    expected += (char) 0x49;
    ASSERT_STREQ(crypted.c_str(), expected.c_str());

    toCrypt = "MLKJIHGFMLKJIHGF";
    crypted = cipher.crypt(toCrypt);
    expected += (char) 0xBA;
    expected += (char) 0x3A;
    expected += (char) 0x7B;
    expected += (char) 0xA6;
    expected += (char) 0xD6;
    expected += (char) 0xEE;
    expected += (char) 0x18;
    expected += (char) 0x49;
    ASSERT_STREQ(crypted.c_str(), expected.c_str());
    toCrypt += "M";
    crypted = cipher.crypt(toCrypt);
    for(int a = 0; a < crypted.length();a++)
        std::printf("%X \n", crypted[a]);
    expected += (char) 0xD2;
    expected += (char) 0xE2;
    expected += (char) 0x35;
    expected += (char) 0xDD;
    expected += (char) 0xA7;
    expected += (char) 0x05;
    expected += (char) 0x15;
    expected += (char) 0x2;

}

TEST(DES_Cipher, decrypt){
    std::string toDecrypt;
    toDecrypt += (char) 0xBA;
    toDecrypt += (char) 0x3A;
    toDecrypt += (char) 0x7B;
    toDecrypt += (char) 0xA6;
    toDecrypt += (char) 0xD6;
    toDecrypt += (char) 0xEE;
    toDecrypt += (char) 0x18;
    toDecrypt += (char) 0x49;
    std::string key = "12345678";
    DES_Cipher cipher(key, MODE_DES);
    std::string expected = "MLKJIHGF";
    std::string decrypted = cipher.decrypt(toDecrypt);
    ASSERT_STREQ(decrypted.c_str(), expected.c_str());

    toDecrypt += (char) 0xBA;
    toDecrypt += (char) 0x3A;
    toDecrypt += (char) 0x7B;
    toDecrypt += (char) 0xA6;
    toDecrypt += (char) 0xD6;
    toDecrypt += (char) 0xEE;
    toDecrypt += (char) 0x18;
    toDecrypt += (char) 0x49;
    expected += "MLKJIHGF";
    decrypted = cipher.decrypt(toDecrypt);
    ASSERT_STREQ(decrypted.c_str(), expected.c_str());

}