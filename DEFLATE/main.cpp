#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include "deflate_encoder.h"
#include "deflate_decoder.h"
int main(int argc, char** argv) {
	
	//////////////////////////////////
	//TODO: Decodifica dinamica
	
	
	//LETTURA FILE
	
	/*std::ifstream input( "deflate_decoder.cpp", std::ios::binary );
    // copies all data into buffer
    std::vector<char> buffer((
            std::istreambuf_iterator<char>(input)), 
            (std::istreambuf_iterator<char>()));
	
	std::string orig(buffer.begin(),buffer.end());
	*/
	
	/////////////////////////////////////////////
	std::string orig = "Deflate inflate on a plate";
	Deflate_encoder encoder = Deflate_encoder(orig);
	std::string compressed = encoder.encode();
	
	//println("Compressed: \n "<<compressed<<"\n");
	
	std::ofstream cf;
    cf.open ("compressed_text.txt", std::ios::binary );
    cf << compressed;
    cf.close();
	
	Deflate_decoder decoder = Deflate_decoder(compressed);
	std::string decoded = decoder.decode();
	
	//println("Decoded: \n"<<decoded);
	std::ofstream myfile;
    myfile.open ("decompressed_text.txt", std::ios::binary );
    myfile << decoded;
    myfile.close();
    
	if(orig == decoded){
		println("SUCCESS");
	}else{
		println("FAILURE");
	}
	
	system("Pause");
	//system("infgen");
	system("infgen -r -d compressed_text.txt "/*> infgen_debug.txt*/);
	system("Pause");
}
