#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include "lz77_encoder.h"
int main(int argc, char *argv[])
{
	std::ifstream input( "gym.blend", std::ios::binary );
    // copies all data into buffer
    std::vector<char> buffer((
            std::istreambuf_iterator<char>(input)), 
            (std::istreambuf_iterator<char>()));
	
	std::string orig(buffer.begin(),buffer.end());
	input.close();
    LZ77_Encoder encoder = LZ77_Encoder();
    int encoded_length;
    //std::string orig = "Deflate and inflate until it is late and eat on a plate";
    lz77_r* encoded = encoder.encode(orig,&encoded_length);
    println("ENCODED DONE");
    std::string s = encoder.decode(encoded,encoded_length);
    if(orig == s)
    	println("SUCCESS");
    else
    	println("FAILURE");
	//system("PAUSE");
    return 0;
}
