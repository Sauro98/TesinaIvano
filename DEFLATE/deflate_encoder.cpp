#include "deflate_encoder.h"

std::ostringstream output_builder;
std::ostringstream debug_builder;

Deflate_encoder::Deflate_encoder(std::string to_compress){
	original = to_compress;
	#if DEBUG > 0
		//println(original);
		println("START CONSTRUCTION OF STATIC TREES");
	#endif
	static_tree = construct_static_literal_and_lengths_tree();
	#if DEBUG > 0
		println("END");
	#endif
	current_to_add = 0;
	current_to_add_bit = 0;
}

std::string Deflate_encoder::encode(){
    add_code_to_string(1,1);//1
    return dynamic_encoding(original);
    //return non_compressed_encoding(original);
	//return static_encoding(original);
}

std::string Deflate_encoder::dynamic_encoding(std::string to_compress){
	add_code_to_string(1,2);//10
	
	std::vector<long> literals_vector;
	literals_vector.reserve(to_compress.size());
	std::vector<long> distances_vector;
	
	LZ77_Encoder lz77_encoder = LZ77_Encoder(to_compress);
	
	std::string compressed = "";
	
	#if DEBUG > 0
		println("START ENCODING");
	#endif
	while(lz77_encoder.hasMore()){
		int result_length = 0;
		long* result = lz77_encoder.getNext(&result_length);
		
		if(result_length == 1){
			//literal
			literals_vector.push_back(result[0]);
		}else{
			//length
			literals_vector.push_back(get_static_length_code(result[0]));
			distances_vector.push_back(get_static_distance_code(result[1]));
		}
	}
	int l_length = 0;
	int d_length = 0;
	
	val_f* l_frequencies = Huffman_Tree::get_value_frequencies(literals_vector,&l_length);	
	val_f* d_frequencies = Huffman_Tree::get_value_frequencies(distances_vector,&d_length);
	
	for(int i = 0;i<l_length;i++){
		println("Literal "<<l_frequencies[i].value<<" Frequence "<<l_frequencies[i].frequence);
	}
	for(int i = 0;i<d_length;i++){
		println("Distance "<<d_frequencies[i].value<<" Frequence "<<d_frequencies[i].frequence);
	}
	
	node_f* l_root = Huffman_Tree::generate_tree(l_frequencies,l_length);
	node_f* d_root = Huffman_Tree::generate_tree(d_frequencies,d_length);
	
	return "gfhj";
}

std::string Deflate_encoder::non_compressed_encoding(std::string to_compress){
    add_code_to_string(0,2);//00
    
    //skip all remaining bits, as RFC1951
    if(current_to_add_bit != 0){
        output_builder << current_to_add;
        current_to_add = 0;
        current_to_add_bit = 0;
    }
    
    //ADD HEADER
    int16_t length = to_compress.length();
    uint16_t reversed_l = 0;
    for(int eb_i = 0; eb_i < 16;eb_i++){
				int t_a = (length >> eb_i);
				t_a &= 1;
				reversed_l |= (t_a << (16 - 1 - eb_i));
    }
    println("Length: "<<length);
    
    add_code_to_string(reversed_l,16);
    
    int16_t mask = 65535;
    int16_t nlength = (reversed_l) ^ mask;
    println("NLength: "<<nlength);
    add_code_to_string(nlength,16);
    
    if(current_to_add_bit != 0){
        output_builder << current_to_add;
        current_to_add = 0;
        current_to_add_bit = 0;
    }

    //ADD DATA
    
    for(int d_i = 0;d_i < length;d_i++){
            output_builder << to_compress.at(d_i);
    }
	
	#if DEBUG > 0
		println("END");
	#endif
    
    return output_builder.str();
}

std::string Deflate_encoder::static_encoding(std::string to_compress){
	
	LZ77_Encoder lz77_encoder = LZ77_Encoder(to_compress);
	
	std::string compressed = "";
	
	add_code_to_string(2,2);//10
	
	#if DEBUG > 0
		println("START ENCODING");
	#endif
	while(lz77_encoder.hasMore()){
		int result_length = 0;
		long* result = lz77_encoder.getNext(&result_length);
		
		if(result_length == 1){
			#if DEBUG > 0
				//println("\tLiteral");
			#endif
			char literal = (char) result[0];
			int literal_length = 0;
			unsigned long encoded_literal = encode_static_literal(literal,&literal_length);
			
			#if DEBUG > 0
				//print("\t\t"<<(int) literal<<" "<<literal<<" "<<encoded_literal<<" - ");
				debug_builder << literal;
			#endif
			add_code_to_string(encoded_literal,literal_length);
		}else{
			#if DEBUG > 0
				//println("\tLength/Distance");
			#endif
			int length = result[0];
			int distance = result[1];
			
			int literal_length = 0;
			
			unsigned int encoded_length = encode_static_literal(get_static_length_code(length),&literal_length);
			#if DEBUG > 0
				//print("\t\t"<<length<<" - ");
				debug_builder << "\n(" << length;
			#endif
			
			
			add_code_to_string(encoded_length,literal_length);
			
			int extra_bits = 0;
			int extra_bits_value = get_static_length_extra_bits(length,&extra_bits);
			
			
			
			//TEST
			int ebv_cl = extra_bits_value;
			extra_bits_value = 0;
			
			for(int eb_i = 0; eb_i < extra_bits;eb_i ++){
				int t_a = (ebv_cl >> eb_i);
				t_a &= 1;
				extra_bits_value |= (t_a << (extra_bits - 1 - eb_i));
			}
			
			//
			
			#if DEBUG > 0
				//print("\t\teb: ");
			#endif
			
			
			add_code_to_string(extra_bits_value,extra_bits);
			
			unsigned int encoded_distance = get_static_distance_code(distance);
			
			#if DEBUG > 0
				//print("\t\t"<<distance<<" - ");
				debug_builder << "," << distance << ")\n";
			#endif
			
			
			add_code_to_string(encoded_distance,5);
			
			
			extra_bits_value = get_static_distance_extra_bits(distance,&extra_bits);
			
			//TEST
			
			int ebv_c = extra_bits_value;
			extra_bits_value = 0;
			
			for(int eb_i = 0; eb_i < extra_bits;eb_i ++){
				int t_a = (ebv_c >> eb_i);
				t_a &= 1;
				extra_bits_value |= (t_a << (extra_bits - 1 - eb_i));
			}
			
			#if DEBUG > 0
				//print("\t\teb: ");
			#endif
			
			add_code_to_string(extra_bits_value,extra_bits);
		}
	}
	
	int end_code = 256;
	int length = 0;
	int encoded_end = encode_static_literal(end_code,&length);
	add_code_to_string(encoded_end,length);
	
	if(current_to_add_bit != 0)
		output_builder << current_to_add;
	
	#if DEBUG > 0
		println("END");
	#endif
	
	std::ofstream myfile;
    myfile.open ("compressed_debug.txt");
    myfile << debug_builder.str();
    myfile.close();
	
	compressed = output_builder.str();
	
	println("COMPRESSED length: "<<compressed.length());
	#if DEBUG > 1
	for(int i = 0;i < compressed.length();i++){
		println(compressed.at(i));
	}
	#endif
	return compressed;
}

unsigned int Deflate_encoder::encode_static_literal(int literal,int* literal_length){
	*literal_length = get_static_literal_length(literal);
	int literal_code = static_tree[literal];
	#if DEBUG > 1
		print("\tLITERAL : "<<literal_code<<" BINARY: ");
		print_binary_literal(literal_code,*literal_length);
	#endif
	return literal_code;
}

void Deflate_encoder::print_reversed_binary_literal(int literal,int length){
	for(int a = 0;a < length;a++){
		int current = (unsigned char)(literal <<  (length - 1 - a)) >> (length - 1);
		print(current);
	}
	println("");
}

void Deflate_encoder::add_code_to_string(long code,int length){
	if(length > 0){
		int current_encoded_bit = 0;
		long mask = get_mask_for_length(length);
		while(current_encoded_bit < length){
			if(current_to_add_bit == 8){
				output_builder << current_to_add;
				current_to_add = 0;
				current_to_add_bit = 0;
			}
			int bit_to_add = ((code << current_encoded_bit) & mask) >> (length - 1);
			
			#if DEBUG > 0
				//print(bit_to_add);
			#endif
			
			//TODO::CONTROLLA
			current_to_add |= (bit_to_add << (/*7 -*/ current_to_add_bit));
			current_encoded_bit++;
			current_to_add_bit++;
		}
		#if DEBUG > 0
			//println("");
		#endif
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
