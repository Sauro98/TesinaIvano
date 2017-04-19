#include "deflate_encoder.h"

std::ostringstream output_builder;
std::ostringstream debug_builder;

const int Deflate_encoder::hclen_reference[] = {16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15};

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
	
	// loop through the text to compress and populate the vectors for literals/lenghts and distancies with the values found
	
	#if DEBUG > 0
		println("START ENCODING");
	#endif
	while(lz77_encoder.hasMore()){
		int result_length = 0;
		long* result = lz77_encoder.getNext(&result_length);
		
		if(result_length == 1){
			//literal
			literals_vector.push_back(result[0]);
			println("\tLiteral");
			println("\t\t"<<(int) result[0]<<" "<<(char)result[0]<<" - ");
		}else{
			//length
			literals_vector.push_back(get_static_length_code(result[0]));
			distances_vector.push_back(get_static_distance_code(result[1]));
			println("\tLength/Distance");
			print("\t\t"<<result[0]);
			print("," << result[1] << "\n");
		}
	}
	
	//end code
	literals_vector.push_back(256);
	
	int l_length = 0;
	int d_length = 0;
	
	//gat the frequence for eache value of eache alphabet
	val_f* l_frequencies = Huffman_Tree::get_value_frequencies(literals_vector,&l_length);	
	val_f* d_frequencies = Huffman_Tree::get_value_frequencies(distances_vector,&d_length);
	
	//generate the tree based on the frequencies
	node_f* l_root = Huffman_Tree::generate_tree(l_frequencies,l_length);
	node_f* d_root = Huffman_Tree::generate_tree(d_frequencies,d_length);
	
	//get the huffman codes for eache value in each alphabet
	code_d* literal_codes = Huffman_Tree::dynamic_tree_encoding(l_root,l_frequencies,l_length,AT_LITERALS);
	code_d* distance_codes = Huffman_Tree::dynamic_tree_encoding(d_root,d_frequencies,d_length,AT_DISTANCIES);
	
	//arrays that will hold the extra bits for each value in each alphabet 
	int* l_extra_bits = new int[DYNAMIC_LITERALS_ALPHABET_LENGTH];
	int* d_extra_bits = new int[DYNAMIC_DISTANCES_ALPHABET_LENGTH];
	
	//get the code lengths used in the alphabets as in RFC 1951 hapter 3.2.7
	int l_code_lengths_length = 0;
	int d_code_lengths_length = 0;
	int* l_code_lengths = Huffman_Tree::get_code_length_codes(literal_codes,true,l_extra_bits,&l_code_lengths_length);
	int* d_code_lengths = Huffman_Tree::get_code_length_codes(distance_codes,false,d_extra_bits,&d_code_lengths_length);
	
	if(d_code_lengths_length == 0){
		d_code_lengths = new int[1];
		d_code_lengths[0] = 0;
		d_code_lengths_length = 1;
	}
	
	//now i have to combine the two code lengths arrays and generate the frequencies array
	//then generate the tree and get the codes that should be three bits each
	
	std::vector<long> tot_code_lengths;
	tot_code_lengths.reserve(l_code_lengths_length + d_code_lengths_length);
	
	for(int l_i= 0; l_i < l_code_lengths_length; l_i++){
		tot_code_lengths.push_back(l_code_lengths[l_i]);
	}
	for(int d_i= 0; d_i < d_code_lengths_length; d_i++){
		tot_code_lengths.push_back(d_code_lengths[d_i]);
	}
	
	//now the two arrays are combined and i need to get the frequency array out of it
	int code_lengths_length = 0;
	val_f* c_frequencies = Huffman_Tree::get_value_frequencies(tot_code_lengths,&code_lengths_length);
	
	//generate the code tree based on the frequencies
	
	node_f* c_root = Huffman_Tree::generate_tree(c_frequencies,code_lengths_length);
	
	//get the huffman codes for each value in the code lengths alphabet
	
	code_d* codes_codes = Huffman_Tree::dynamic_tree_encoding(c_root,c_frequencies,code_lengths_length,AT_CODES);
	
	//start generating block
	
	int HLIT = 0;
	for(int last_used_code_index = DYNAMIC_LITERALS_ALPHABET_LENGTH - 1;last_used_code_index > 256;last_used_code_index--){
		if(literal_codes[last_used_code_index].code_length != 0){
			HLIT = last_used_code_index + 1;
			break;
		}
	}
	
	int HDIST = 0;
	for(int last_used_code_index = DYNAMIC_DISTANCES_ALPHABET_LENGTH - 1;last_used_code_index > 1;last_used_code_index--){
		//println("DCL: ["<<last_used_code_index<<"] "<<distance_codes[last_used_code_index].code_length );
		if(distance_codes[last_used_code_index].code_length != 0){
			HDIST = last_used_code_index + 1;
			break;
		}
	}
	
	int HCLEN = 0;
	for(int last_used_code_index = DYNAMIC_CODES_ALPHABET_LENGTH - 1;last_used_code_index > 4;last_used_code_index--){
		//println("CCL: ["<<hclen_reference[last_used_code_index]<<"] "<<codes_codes[hclen_reference[last_used_code_index]].code_length );
		if(codes_codes[hclen_reference[last_used_code_index]].code_length != 0){
			HCLEN = last_used_code_index + 1;
			break;
		}
	}
	
	//println("HLIT "<<HLIT<<" "<<HLIT-257);
	//println("HDIST "<<HDIST<<" "<<HDIST-1);
	//println("HCLEN "<<HCLEN<<" "<<HCLEN-4);
	//HLIT
	//DYNAMIC_LITERALS_ALPHABET_LENGTH - 257
	add_code_to_string(HLIT - 257,5,true);
	//HDIST
	//DYNAMIC_DISTANCES_ALPHABET_LENGTH - 1
	add_code_to_string(HDIST - 1,5,true);
	//HCLEN
	//DYNAMIC_CODES_ALPHABET_LENGTH - 4
	add_code_to_string(HCLEN - 4,4,true);
	
	//HC_CONTENT
	for(int c_i = 0;c_i < HCLEN;c_i++){
		add_code_to_string(codes_codes[hclen_reference[c_i]].code_length,3,true);	
	}
	
	//HL_CONTENT
	int l_extra_bits_index = 0;
	int hlit_counter = 0;
	for(int l_i = 0;l_i < l_code_lengths_length; l_i++){
		code_d curr_code = codes_codes[l_code_lengths[l_i]];
		add_code_to_string(curr_code.value,curr_code.code_length);
		int extra_bits_count = 0;
		
		if(curr_code.symbol == 16){
			extra_bits_count = 2;
			hlit_counter += 3 + l_extra_bits[l_extra_bits_index];
		}else if(curr_code.symbol == 17){
			extra_bits_count = 3;
			hlit_counter += 3 + l_extra_bits[l_extra_bits_index];
		}else if(curr_code.symbol == 18){
			extra_bits_count = 7;
			hlit_counter += 11 + l_extra_bits[l_extra_bits_index];
		}else
			hlit_counter ++;
			
		add_code_to_string(l_extra_bits[l_extra_bits_index],extra_bits_count,true);
		
		if(extra_bits_count > 0)
			l_extra_bits_index++;
			
		println("HLIT COUNTER "<<hlit_counter);
		
		if(hlit_counter >= HLIT)
			break;
			
	}
	
	//HDIST_CONTENT
	int d_extra_bits_index = 0;
	int hdist_counter = 0;
	for(int d_i = 0;d_i < d_code_lengths_length; d_i++){
		code_d curr_code = codes_codes[d_code_lengths[d_i]];
		add_code_to_string(curr_code.value,curr_code.code_length);
		int extra_bits_count = 0;
		println("ccS"<<curr_code.symbol);
		if(curr_code.symbol == 16){
			extra_bits_count = 2;
			hdist_counter += 3 + d_extra_bits[d_extra_bits_index];
		}else if(curr_code.symbol == 17){
			extra_bits_count = 3;
			hdist_counter += 3 + d_extra_bits[d_extra_bits_index];
		}else if(curr_code.symbol == 18){
			extra_bits_count = 7;
			hdist_counter += 11 + d_extra_bits[d_extra_bits_index];
		}else
			hdist_counter ++;
		add_code_to_string(d_extra_bits[d_extra_bits_index],extra_bits_count,true);
		println("eb "<<d_extra_bits[d_extra_bits_index]);
		if(extra_bits_count > 0)
			d_extra_bits_index++;
			
		println("HDIST COUNTER "<<hdist_counter);
		
		if(hdist_counter >= HDIST)
			break;	
	}
	
	lz77_encoder.reset();
	println("RESET");
	while(lz77_encoder.hasMore()){
		int result_length = 0;
		long* result = lz77_encoder.getNext(&result_length);
		
		if(result_length == 1){
			//literal
			int literal = result[0];
			int encoded = literal_codes[literal].value;
			int encoded_length = literal_codes[literal].code_length;
			add_code_to_string(encoded,encoded_length);
			
			println("\tLiteral");
			println("\t\t"<<(int) result[0]<<" "<<(char)result[0]<<" - ");
		}else{
			//length
				int length = result[0];
				//length value in the deflate static alphabet (like for 3 the value is 257)
				int static_length_value = get_static_length_code(length);
				int huffman_length_code = literal_codes[static_length_value].value;
				int huffman_length_code_length = literal_codes[static_length_value].code_length;
				add_code_to_string(huffman_length_code,huffman_length_code_length);
				//extra bits
				int length_extra_bits_count = 0;
				int length_extra_bits = get_static_length_extra_bits(length,&length_extra_bits_count);
				add_code_to_string(length_extra_bits,length_extra_bits_count,true);
			//distance
				int distance = result[1];
				int static_distance_value = get_static_distance_code(distance);
				int huffman_distance_code = distance_codes[static_distance_value].value;
				int huffman_distance_code_length = distance_codes[static_distance_value].code_length;
				add_code_to_string(huffman_distance_code,huffman_distance_code_length);
				//extra bits
				int distance_extra_bits_count = 0;
				int distance_extra_bits = get_static_distance_extra_bits(distance,&distance_extra_bits_count);
				add_code_to_string(distance_extra_bits,distance_extra_bits_count,true);
			
			println("\tLength/Distance");
			print("\t\t"<<length<<" "<<static_length_value);
			print(","<<distance<< "\n");
		}
	}
	
	int encoded_end = literal_codes[256].value;
	int encoded_end_length = literal_codes[256].code_length;
	add_code_to_string(encoded_end,encoded_end_length);
	
	//if last bit inserted was not the last bit in the byte insert the full last byte
	if(current_to_add_bit != 0)
		output_builder << current_to_add;
	
	return output_builder.str();
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
    println("Length: "<<length);
    
    add_code_to_string(length,16,true);
    
    int16_t mask = 65535;
    int16_t nlength = (length) ^ mask;
    println("NLength: "<<nlength);
    add_code_to_string(nlength,16,true);
    
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
			
			add_code_to_string(extra_bits_value,extra_bits,true);
			
			unsigned int encoded_distance = get_static_distance_code(distance);
			
			#if DEBUG > 0
				//print("\t\t"<<distance<<" - ");
				debug_builder << "," << distance << ")\n";
			#endif
			
			
			add_code_to_string(encoded_distance,5);
			
			
			extra_bits_value = get_static_distance_extra_bits(distance,&extra_bits);
			
			add_code_to_string(extra_bits_value,extra_bits,true);
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
	add_code_to_string(code,length,false);
}

void Deflate_encoder::add_code_to_string(long code,int length,bool reversed){
	if(length > 0){
		int current_encoded_bit = 0;
		long mask = get_mask_for_length(length);
		while(current_encoded_bit < length){
			if(current_to_add_bit == 8){
				output_builder << current_to_add;
				current_to_add = 0;
				current_to_add_bit = 0;
			}
			int bit_to_add;
			if(!reversed)
				bit_to_add = ((code << current_encoded_bit) & mask) >> (length - 1);
			else
				bit_to_add = ((code << length - 1 - current_encoded_bit) & mask) >> (length - 1);
			#if DEBUG > 0
				//print(bit_to_add);
			#endif
			
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
