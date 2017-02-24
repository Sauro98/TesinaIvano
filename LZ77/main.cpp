#include <cstdlib>
#include <iostream>
#include "lz77_encoder.h"
int main(int argc, char *argv[])
{
    LZ77_Encoder encoder = LZ77_Encoder();
    int encoded_length;
    lz77_r* encoded = encoder.encode("Ok, questo e' un test per verificare il funzionamento di LZ77",&encoded_length);
    println("ENCODED DONE");
    std::string s = encoder.decode(encoded,encoded_length);
    system("PAUSE");
    return 0;
}
