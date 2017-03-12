#include "deflate_encoder.h"

Deflate_encoder::Deflate_encoder(std::string to_compress){
	original = to_compress;
}

std::string Deflate_encoder::encode(){
	static_encoding(original);
	return "";
}


std::string Deflate_encoder::static_encoding(std::string to_compress){
	int length = 0;
	LZ77_Encoder encoder = LZ77_Encoder();
	
	//TODO: eliminate last charachter from LZ77
	//TODO: implement static DEFLATE
	
	
	lz77_r* result = encoder.encode(original,&length);
	for(int index = 0;index < length;index++){
		int t_length = result[index].l;
		int t_origin = result[index].o;
		if(t_length == 0 && t_origin == 0){
			int literal_length = 0;
			encode_static_literal(result[index].a,&literal_length);
		}else{
			char next = result[index].a;
			print("<"<<t_length<<","<<t_origin<<">"<<next);
		}
		
	}
	println("");
	return "";
}

unsigned int Deflate_encoder::encode_static_literal(int literal,int* literal_length){
	unsigned int reversed = 0;
	if(literal <= 143){
		*literal_length = 8;
		literal += STATIC_7_BITS_CODES;
		println(literal);
		//print_binary_literal(literal,*literal_length);
		for(int a = 0;a < *literal_length;a++){
			int current = (unsigned char)(literal <<  (*literal_length - 1 - a)) >> (*literal_length - 1);
			reversed |= (current << (*literal_length - 1 - a)); 
		}
		println(reversed);	
		print_reversed_binary_literal(literal,*literal_length);
	}else{
		*literal_length = 9;
		literal += STATIC_8_BITS_CODES;
		println(literal);
		//print_binary_literal(literal,*literal_length);
		for(int a = 0;a < *literal_length;a++){
			int current = ((literal <<  (*literal_length - 1 - a)) && 511) >> (*literal_length - 1);
			reversed |= (current << (*literal_length - 1 - a)); 
		}
		println(reversed);	
		print_reversed_binary_literal(literal,*literal_length);
	}
	return 0;
}

void Deflate_encoder::print_binary_literal(int literal,int length){
	for(int a = 0;a < length;a++){
		int current = (unsigned char)(literal <<  a) >> (length - 1);
		print(current);
	}
	println("");
}

void Deflate_encoder::print_reversed_binary_literal(int literal,int length){
	for(int a = 0;a < length;a++){
		int current = (unsigned char)(literal <<  (length - 1 - a)) >> (length - 1);
		print(current);
	}
	println("");
}

int* Deflate_encoder::construct_static_tree(){
	int lengths[288];
	for(int i = 0;i<=143;i++){
		lengths[i] = 8;
	}
	for(int i = 144;i <= 255;i++){
		lengths[i] = 9;
	}
	for(int i = 256;i <= 279;i++){
		lengths[i] = 7;
	}
	for(int i = 280;i <= 287;i++){
		lengths[i] = 8; 
	}
	
	int bl_count[MAX_BITS];
	for(int i = 0;i < MAX_BITS;i++){
		bl_count[i] = 0;
	}
	for(int i = 0;i<= 287;i++){
		bl_count[lengths[i]] ++;
	}
	
	code = 0;
    bl_count[0] = 0;
    int next_code[MAX_BITS];
    for (bits = 1; bits <= MAX_BITS; bits++) {
    	code = (code + bl_count[bits-1]) << 1;
    	next_code[bits] = code;
    }
    //TODO: loop through all values and find the code for them. return that array of length 288 and not next code
    return next_code;
}
