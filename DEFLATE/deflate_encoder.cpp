#include "deflate_encoder.h"

Deflate_encoder::Deflate_encoder(std::string to_compress){
	original = to_compress;
}

std::string Deflate_encoder::encode(){
	int length = 0;
	LZ77_Encoder encoder = LZ77_Encoder();
	
	//TODO: eliminate last charachter from LZ77
	//TODO: implement static DEFLATE
	
	
	lz77_r* result = encoder.encode(original,&length);
	for(int index = 0;index < length;index++){
		int t_length = result[index].l;
		int t_origin = result[index].o;
		char next = result[index].a;
		print("<"<<t_length<<","<<t_origin<<">"<<next);
	}
	println("");
	return "";
}
