#include "deflate_encoder.h"

Deflate_encoder::Deflate_encoder(std::string to_compress){
	original = to_compress;
	println(original);
	#if DEBUG > 0
		println("START CONSTRUCTION OF STATIC TREES");
	#endif
	static_tree = construct_static_literal_and_lengths_tree();
	#if DEBUG > 0
		println("END");
	#endif
}

std::string Deflate_encoder::encode(){
	return static_encoding(original);
}


std::string Deflate_encoder::static_encoding(std::string to_compress){
	
	LZ77_Encoder lz77_encoder = LZ77_Encoder(to_compress);
	
	std::string compressed = "";
	
	unsigned char current_to_add = 3;//011
	int* current_bit = new int;
	*current_bit = 3;
	#if DEBUG > 0
		println("START ENCODING");
	#endif
	
	while(lz77_encoder.hasMore()){
		int result_length = 0;
		long* result = lz77_encoder.getNext(&result_length);
		
		if(result_length == 1){
			#if DEBUG > 0
				println("\tLiteral");
			#endif
			char literal = (char) result[0];
			int literal_length = 0;
			unsigned long encoded_literal = encode_static_literal(literal,&literal_length);
			
			#if DEBUG > 0
				print("\t\t"<<(int) literal<<" "<<literal<<" "<<encoded_literal<<" - ");
				print_binary_literal(encoded_literal,literal_length);
			#endif
			
			add_code_to_string(encoded_literal,literal_length,&current_to_add,current_bit,&compressed);
		}else{
			#if DEBUG > 0
				println("\tLength/Distance");
			#endif
			int length = result[0];
			int distance = result[1];
			int literal_length = 0;
			
			unsigned int encoded_length = encode_static_literal(get_static_length_code(length),&literal_length);
			#if DEBUG > 0
				print("\t\t"<<length<<" - ");
				print_binary_literal(encoded_length,literal_length);
			#endif
			int extra_bits = 0;
			int extra_bits_value = get_static_length_extra_bits(length,&extra_bits);
			#if DEBUG > 0
				print("\t\teb: ");
				print_binary_literal(extra_bits_value,extra_bits);
			#endif
			
			add_code_to_string(encoded_length,literal_length,&current_to_add,current_bit,&compressed);
			add_code_to_string(extra_bits_value,extra_bits,&current_to_add,current_bit,&compressed);
			
			unsigned int encoded_distance = get_static_distance_code(distance);
			#if DEBUG > 0
				print("\t\t"<<distance<<" - ");
				print_binary_literal(encoded_distance,5);
			#endif
			extra_bits = 0;
			extra_bits_value = get_static_distance_extra_bits(distance,&extra_bits);
			#if DEBUG > 0
				print("\t\teb: ");
				print_binary_literal(extra_bits_value,extra_bits);
			#endif
			
			add_code_to_string(encoded_distance,5,&current_to_add,current_bit,&compressed);
			add_code_to_string(extra_bits_value,extra_bits,&current_to_add,current_bit,&compressed);
		}
	}
	
	int end_code = 256;
	int length = 0;
	int encoded_end = encode_static_literal(end_code,&length);
	add_code_to_string(encoded_end,length,&current_to_add,current_bit,&compressed);
	
	if(*current_bit != 0)
		compressed += current_to_add;
	
	
	//DELETE THIS!!!!!
	//compressed[0] = 99;
	//compressed[1] = 171;
	
	#if DEBUG > 0
		println("END");
	#endif
	
	println("COMPRESSED length: "<<compressed.length());
	#if DEBUG > 0
	for(int i = 0;i < compressed.length();i++){
		print(compressed.at(i)<<" ");
		print_binary_literal(compressed.at(i),8);
	}
	#endif
	return compressed;
}

unsigned int Deflate_encoder::encode_static_literal(int literal,int* literal_length){
	*literal_length = get_static_literal_length(literal);
	int mask = get_mask_for_length(*literal_length);
	int literal_code = static_tree[literal];
	
	#if DEBUG > 1
		print("\tLITERAL : "<<literal_code<<" BINARY: ");
		print_binary_literal(literal_code,*literal_length);
	#endif
	
	//FACCIO IL ROVESCIAMENTO QUANDO LO SCRIVO NEI BIT,
	
	/*int reversed = 0;
	for(int a = 0;a < *literal_length;a++){
			int left_shifted = literal_code <<  (*literal_length - 1 - a);
			int masked = left_shifted & mask;
			int current = (masked ) >> (*literal_length - 1);
			reversed |= (current << (*literal_length - 1 - a)); 
	}
	
	#if DEBUG > 1
		print("\tREVERSED : "<<reversed<<" BINARY: ");
		print_binary_literal(reversed,*literal_length);
	#endif*/
	return literal_code;
}

void Deflate_encoder::print_reversed_binary_literal(int literal,int length){
	for(int a = 0;a < length;a++){
		int current = (unsigned char)(literal <<  (length - 1 - a)) >> (length - 1);
		print(current);
	}
	println("");
}

void Deflate_encoder::add_code_to_string(long code,int length,unsigned char* current_to_add,int* current_to_add_bit,std::string* compressed){
	int current_encoded_bit = 0;
	long mask = get_mask_for_length(length);
	while(current_encoded_bit < length){
		if(*current_to_add_bit == 8){
			(*compressed) = (*compressed) + (char)(*current_to_add);
			*current_to_add = 0;
			*current_to_add_bit = 0;
		}
		int bit_to_add = ((code << current_encoded_bit) & mask) >> (length - 1);
		//TODO::CONTROLLA
		*current_to_add |= (bit_to_add << (/*7 -*/ *current_to_add_bit));
		current_encoded_bit++;
		(*current_to_add_bit)++;
	}
	return;
}

int Deflate_encoder::get_static_length_code(long length){
	if(length == 3)
		return 257;
	if(length == 4)
		return 258;
	if(length == 5)
		return 259;
	if(length == 6)
		return 260;
	if(length == 7)
		return 261;
	if(length == 8)
		return 262;
	if(length == 9)
		return 263;
	if(length == 10)
		return 264;
	if(length >= 11 && length <= 12)
		return 265;
	if(length >= 13 && length <= 14)
		return 266;
	if(length >= 15 && length <= 16)
		return 267;
	if(length >= 17 && length <= 18)
		return 268;
	if(length >= 19 && length <= 22)
		return 269;
	if(length >= 23 && length <= 26)
		return 270;
	if(length >= 27 && length <= 30)
		return 271;
	if(length >= 31 && length <= 34)
		return 272;
	if(length >= 35 && length <= 42)
		return 273;
	if(length >= 43 && length <= 50)
		return 274;
	if(length >= 51 && length <= 58)
		return 275;
	if(length >= 59 && length <= 66)
		return 276;
	if(length >= 67 && length <= 82)
		return 277;
	if(length >= 83 && length <= 98)
		return 278;
	if(length >= 99 && length <= 114)
		return 279;
	if(length >= 115 && length <= 130)
		return 280;
	if(length >= 131 && length <= 162)
		return 281;
	if(length >= 163 && length <= 194)
		return 282;
	if(length >= 195 && length <= 226)
		return 283;
	if(length >= 227 && length <= 257)
		return 284;
	if(length == 258)
		return 285;
}
int Deflate_encoder::get_static_length_extra_bits(long length,int* extra_bits){
	if(length == 3){
		*extra_bits = 0;
		return 0;
	}
	if(length == 4){
		*extra_bits = 0;
		return 0;
	}
	if(length == 5){
		*extra_bits = 0;
		return 0;
	}
	if(length == 6){
		*extra_bits = 0;
		return 0;
	}
	if(length == 7){
		*extra_bits = 0;
		return 0;
	}
	if(length == 8){
		*extra_bits = 0;
		return 0;
	}
	if(length == 9){
		*extra_bits = 0;
		return 0;
	}
	if(length == 10){
		*extra_bits = 0;
		return 0;
	}
	if(length >= 11 && length <= 12){
		*extra_bits = 1;
		return length - 11;
	}
	if(length >= 13 && length <= 14){
		*extra_bits = 1;
		return length - 13;
	}
	if(length >= 15 && length <= 16){
		*extra_bits = 1;
		return length - 15;
	}
	if(length >= 17 && length <= 18){
		*extra_bits = 1;
		return length - 17;
	}
	if(length >= 19 && length <= 22){
		*extra_bits = 2;
		return length - 19;
	}
	if(length >= 23 && length <= 26){
		*extra_bits = 2;
		return length - 23;
	}
	if(length >= 27 && length <= 30){
		*extra_bits = 2;
		return length - 27;
	}
	if(length >= 31 && length <= 34){
		*extra_bits = 2;
		return length - 31;
	}
	if(length >= 35 && length <= 42){
		*extra_bits = 3;
		return length - 35;
	}
	if(length >= 43 && length <= 50){
		*extra_bits = 3;
		return length - 43;
	}
	if(length >= 51 && length <= 58){
		*extra_bits = 3;
		return length - 51;
	}
	if(length >= 59 && length <= 66){
		*extra_bits = 3;
		return length - 59;
	}
	if(length >= 67 && length <= 82){
		*extra_bits = 4;
		return length - 67;
	}
	if(length >= 83 && length <= 98){
		*extra_bits = 4;
		return length - 83;
	}
	if(length >= 99 && length <= 114){
		*extra_bits = 4;
		return length - 99;
	}
	if(length >= 115 && length <= 130){
		*extra_bits = 4;
		return length - 115;
	}
	if(length >= 131 && length <= 162){
		*extra_bits = 5;
		return length - 131;
	}
	if(length >= 163 && length <= 194){
		*extra_bits = 5;
		return length - 163;
	}
	if(length >= 195 && length <= 226){
		*extra_bits = 5;
		return length - 195;
	}
	if(length >= 227 && length <= 257){
		*extra_bits = 5;
		return length - 227;
	}
	if(length == 258){
		*extra_bits = 0;
		return 0;
	}
}

int Deflate_encoder::get_static_distance_code(long distance){
	if(distance == 1)
		return 0;
	if(distance == 2)
		return 1;
	if(distance == 3)
		return 2;
	if(distance == 4)
		return 3;
	if(distance >= 5 && distance <= 6)
		return 4;
	if(distance >= 7 && distance <= 8)
		return 5;
	if(distance >= 9 && distance <= 12)
		return 6;
	if(distance >= 13 && distance <= 16)
		return 7;
	if(distance >= 17 && distance <= 24)
		return 8;
	if(distance >= 25 && distance <= 32)
		return 9;
	if(distance >= 33 && distance <= 48)
		return 10;
	if(distance >= 49 && distance <= 64)
		return 11;
	if(distance >= 65 && distance <= 96)
		return 12;
	if(distance >= 97 && distance <= 128)
		return 13;
	if(distance >= 129 && distance <= 192)
		return 14;
	if(distance >= 193 && distance <= 256)
		return 15;
	if(distance >= 257 && distance <= 384)
		return 16;
	if(distance >= 385 && distance <= 512)
		return 17;
	if(distance >= 513 && distance <= 768)
		return 18;
	if(distance >= 769 && distance <= 1024)
		return 19;
	if(distance >= 1025 && distance <= 1536)
		return 20;
	if(distance >= 1537 && distance <= 2048)
		return 21;
	if(distance >= 2049 && distance <= 3072)
		return 22;
	if(distance >= 3073 && distance <= 4096)
		return 23;
	if(distance >= 4097 && distance <= 6144)
		return 24;
	if(distance >= 6145 && distance <= 8192)
		return 25;
	if(distance >= 8193 && distance <= 12288)
		return 26;
	if(distance >= 12289 && distance <= 16384)
		return 27;
	if(distance >= 16385 && distance <= 24576)
		return 28;
	if(distance >= 24577 && distance <= 32768)
		return 29;
}

int Deflate_encoder::get_static_distance_extra_bits(long distance,int* extra_bits){
	if(distance == 1){
		*extra_bits = 0;
		return 0;
	}
	if(distance == 2){
		*extra_bits = 0;
		return 0;
	}
	if(distance == 3){
		*extra_bits = 0;
		return 0;
	}
	if(distance == 4){
		*extra_bits = 0;
		return 0;
	}
	if(distance >= 5 && distance <= 6){
		*extra_bits = 1;
		return distance - 5;
	}
	if(distance >= 7 && distance <= 8){
		*extra_bits = 1;
		return distance - 7;
	}
	if(distance >= 9 && distance <= 12){
		*extra_bits = 2;
		return distance - 9;
	}
	if(distance >= 13 && distance <= 16){
		*extra_bits = 2;
		return distance - 13;
	}
	if(distance >= 17 && distance <= 24){
		*extra_bits = 3;
		return distance - 17;
	}
	if(distance >= 25 && distance <= 32){
		*extra_bits = 3;
		return distance - 25;
	}
	if(distance >= 33 && distance <= 48){
		*extra_bits = 4;
		return distance - 33;
	}
	if(distance >= 49 && distance <= 64){
		*extra_bits = 4;
		return distance - 49;
	}
	if(distance >= 65 && distance <= 96){
		*extra_bits = 5;
		return distance - 65;
	}
	if(distance >= 97 && distance <= 128){
		*extra_bits = 5;
		return distance - 97;
	}
	if(distance >= 129 && distance <= 192){
		*extra_bits = 6;
		return distance - 129;
	}
	if(distance >= 193 && distance <= 256){
		*extra_bits = 6;
		return distance - 193;
	}
	if(distance >= 257 && distance <= 384){
		*extra_bits = 7;
		return distance - 257;
	}
	if(distance >= 385 && distance <= 512){
		*extra_bits = 7;
		return distance - 385;
	}
	if(distance >= 513 && distance <= 768){
		*extra_bits = 8;
		return distance - 513;
	}
	if(distance >= 769 && distance <= 1024){
		*extra_bits = 8;
		return distance - 769;
	}
	if(distance >= 1025 && distance <= 1536){
		*extra_bits = 9;
		return distance - 1025;
	}
	if(distance >= 1537 && distance <= 2048){
		*extra_bits = 9;
		return distance - 1537;
	}
	if(distance >= 2049 && distance <= 3072){
		*extra_bits = 10;
		return distance - 2049;
	}
	if(distance >= 3073 && distance <= 4096){
		*extra_bits = 10;
		return distance - 3073;
	}
	if(distance >= 4097 && distance <= 6144){
		*extra_bits = 11;
		return distance - 4097;
	}
	if(distance >= 6145 && distance <= 8192){
		*extra_bits = 11;
		return distance - 6145;
	}
	if(distance >= 8193 && distance <= 12288){
		*extra_bits = 12;
		return distance - 8193;
	}
	if(distance >= 12289 && distance <= 16384){
		*extra_bits = 12;
		return distance - 12289;
	}
	if(distance >= 16385 && distance <= 24576){
		*extra_bits = 13;
		return distance - 16385;
	}
	if(distance >= 24577 && distance <= 32768){
		*extra_bits = 13;
		return distance - 24577;
	}
}
