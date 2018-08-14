#ifndef DES_CHIPHER_H
#define DES_CHIPHER_H

#include <stdlib.h>
#include "encoder.h"
#include <time.h>
#include "mingw-threads/mingw.thread.h"

#define MODE_DES 0
#define MODE_3DES 1
//#define chipher_debug_mode

#define get_ulong_from_char(orig, index) ((orig) & (ulong) 0xFF) << (((ulong)7 - (index)) * (ulong) 8)
#define get_char(orig, index) (((orig) >> (ulong) (56 - ((ulong) 8 * (index)))) & (ulong) 0xFF);
#define ulong_from_str(orig, modified) \
    for(int a = 0; a < 8; a++){ \
        modified |= ((ulong)(orig[a]) << ((7-a)*8)); \
    }

class DES_Cipher {
public:
    DES_Cipher(std::string _key, int _mode);

    std::string crypt(const std::string& original) const;

    std::string decrypt(const std::string& original) const;

    static std::string get_string_from_ulong(ulong original);

    static ulong get_ulong_from_string(const char *original, size_t offset);

private:
    int mode;
    std::string key;

    std::string exec_crypt(const std::string& original, bool crypt) const;
    static void crypt_string(const char* str, size_t length,bool crypt, std::string key, std::string* toReturn);


};

#endif
