#include "deflate_encoder.h"


std::ostringstream output_builder;
std::ostringstream debug_builder;



Deflate_encoder::Deflate_encoder(){
	static_tree = construct_static_literal_and_lengths_tree();
	current_to_add = 0;
	current_to_add_bit = 0;
}

Deflate_encoder::~Deflate_encoder() {
	delete[] static_tree;
}

std::string Deflate_encoder::encode(char* to_compress, int length){
	//non è il massimo come scelta dei blocchi ma per questa implementazione dell'algoritmo deflate è più che sufficiente
    do_encoding(to_compress, length,DYNAMIC);
    return output_builder.str();
}

void Deflate_encoder::do_encoding(char* to_compress,int length,int dec_type){
    
	switch(dec_type){
		case STATIC:
			static_encoding(to_compress,length);
			break;
		case DYNAMIC:
			dynamic_encoding(to_compress,length);
			break;
		case NON_COMPRESSED:
			non_compressed_encoding(to_compress,length);
			break;
		default:
			println("CRITICAL ERROR");
	}
}

void Deflate_encoder::dynamic_encoding(char* to_compress,int size){

	LZ77_Encoder lz77_encoder = LZ77_Encoder(to_compress, size);

	while (lz77_encoder.hasMore()) {

		lz77_r results[MAX_SYMBOLS];
		int results_index = 0;

		val_f litlen_f[DYNAMIC_LITERALS_ALPHABET_LENGTH];
		for (int a = 0; a < DYNAMIC_LITERALS_ALPHABET_LENGTH; a++)
			litlen_f[a] = val_f(a, 0);
		val_f dist_f[DYNAMIC_DISTANCES_ALPHABET_LENGTH];
		for (int a = 0; a < DYNAMIC_DISTANCES_ALPHABET_LENGTH; a++)
			dist_f[a] = val_f(a, 0);

		// loop through the text to compress and populate the vectors for literals/lenghts and distancies with the values found

		while (lz77_encoder.hasMore() && results_index < MAX_SYMBOLS) {
			lz77_r result = lz77_encoder.getNext();
			results[results_index] = result;
			results_index++;
			if (!result.is_pair) {
				//literal
				litlen_f[result.literal_length].frequence++;
			} else {
				//length
				litlen_f[get_static_length_code(result.literal_length)].frequence++;
				dist_f[get_static_distance_code(result.distance)].frequence++;
			}
		}
		//end code
		litlen_f[256].frequence++;


		if (lz77_encoder.hasMore())
			add_code_to_string(0, 1);//1
		else
			add_code_to_string(1, 1);//0

		add_code_to_string(1, 2);//10

		int l_length = 0;
		int d_length = 0;

		SORT_ARRAY(litlen_f,litlen_f + DYNAMIC_LITERALS_ALPHABET_LENGTH);
		SORT_ARRAY(dist_f, dist_f + DYNAMIC_DISTANCES_ALPHABET_LENGTH);

		code_d* l_codes_raw = Huffman_Tree::generate_tree(litlen_f, DYNAMIC_LITERALS_ALPHABET_LENGTH, &l_length,15);
		code_d* d_codes_raw = Huffman_Tree::generate_tree(dist_f, DYNAMIC_DISTANCES_ALPHABET_LENGTH, &d_length,15);


		//get the huffman codes for eache value in each alphabet
		code_d* literal_codes = Huffman_Tree::dynamic_tree_encoding(l_codes_raw, l_length, AT_LITERALS);
		code_d* distance_codes = Huffman_Tree::dynamic_tree_encoding(d_codes_raw, d_length, AT_DISTANCIES);

		delete[] l_codes_raw;
		delete[] d_codes_raw;

		//arrays that will hold the extra bits for each value in each alphabet 
		int l_extra_bits[DYNAMIC_LITERALS_ALPHABET_LENGTH];
		int d_extra_bits[DYNAMIC_DISTANCES_ALPHABET_LENGTH];

		//get the code lengths used in the alphabets as in RFC 1951 hapter 3.2.7
		int l_code_lengths_length = 0;
		int d_code_lengths_length = 0;
		int* l_code_lengths = Huffman_Tree::get_code_length_codes(literal_codes, true, l_extra_bits, &l_code_lengths_length);
		int* d_code_lengths = Huffman_Tree::get_code_length_codes(distance_codes, false, d_extra_bits, &d_code_lengths_length);

		bool do_hdist = true;
		if (d_code_lengths_length == 0) {
			d_code_lengths = new int[1];
			d_code_lengths[0] = 0;
			d_code_lengths_length = 1;
			do_hdist = false;
		}

		val_f c_frequencies[DYNAMIC_CODES_ALPHABET_LENGTH];
		for (int a = 0; a < DYNAMIC_CODES_ALPHABET_LENGTH; a++)
			c_frequencies[a] = val_f(a, 0);

		for (int l_i = 0; l_i < l_code_lengths_length; l_i++) {
			c_frequencies[l_code_lengths[l_i]].frequence++;
		}
		for (int d_i = 0; d_i < d_code_lengths_length; d_i++) {
			c_frequencies[d_code_lengths[d_i]].frequence++;
		}

		//now the two arrays are combined and i need to get the frequency array out of it
		int code_lengths_length = 0;
		SORT_ARRAY(c_frequencies, c_frequencies + DYNAMIC_CODES_ALPHABET_LENGTH);

		//generate the code tree based on the frequencies
		code_d* c_raw = Huffman_Tree::generate_tree(c_frequencies, DYNAMIC_CODES_ALPHABET_LENGTH,&code_lengths_length,7);


		//get the huffman codes for each value in the code lengths alphabet
		code_d* codes_codes = Huffman_Tree::dynamic_tree_encoding(c_raw, code_lengths_length, AT_CODES);

		delete[] c_raw;
		//start generation of block

		int HLIT = 0;
		for (int last_used_code_index = DYNAMIC_LITERALS_ALPHABET_LENGTH - 1; last_used_code_index > 256; last_used_code_index--) {
			if (literal_codes[last_used_code_index].code_length != 0) {
				HLIT = last_used_code_index + 1;
				break;
			}
		}
		if (HLIT == 0)
			HLIT = 257;

		int HDIST = (do_hdist) ? 0 : 1;
		if (do_hdist) {
			for (int last_used_code_index = DYNAMIC_DISTANCES_ALPHABET_LENGTH - 1; last_used_code_index > 1; last_used_code_index--) {
				if (distance_codes[last_used_code_index].code_length != 0) {
					HDIST = last_used_code_index + 1;
					break;
				}
			}
		}

		int HCLEN = 0;
		for (int last_used_code_index = DYNAMIC_CODES_ALPHABET_LENGTH - 1; last_used_code_index > 4; last_used_code_index--) {
			if (codes_codes[hclen_reference[last_used_code_index]].code_length != 0) {
				HCLEN = last_used_code_index + 1;
				break;
			}
		}
		//HLIT
		add_code_to_string(HLIT - 257, 5, true);
		//HDIST
		add_code_to_string(HDIST - 1, 5, true);
		//HCLEN
		add_code_to_string(HCLEN - 4, 4, true);

		//HC_CONTENT
		for (int c_i = 0; c_i < HCLEN; c_i++) {
			add_code_to_string(codes_codes[hclen_reference[c_i]].code_length, 3, true);
		}
		add_dynamic_alphabet_content(l_code_lengths_length, codes_codes, l_extra_bits, l_code_lengths, HLIT);

		delete[] l_code_lengths;

		//HDIST_CONTENT
		add_dynamic_alphabet_content(d_code_lengths_length, codes_codes, d_extra_bits, d_code_lengths, HDIST);

		delete[] d_code_lengths;
		delete[] codes_codes;

		for (int i = 0; i < results_index; i++) {
			lz77_r result = results[i];
			if (!result.is_pair) {
				//literal
				int literal = result.literal_length;
				add_code_to_string(literal_codes[literal].value, literal_codes[literal].code_length);

			} else {
				//length
				int length = result.literal_length;
				add_length_distance_to_dynamic_content(length, true, literal_codes);
				//distance
				int distance = result.distance;
				add_length_distance_to_dynamic_content(distance, false, distance_codes);
			}
		}

		delete[] distance_codes;

		int encoded_end = literal_codes[256].value;
		int encoded_end_length = literal_codes[256].code_length;
		add_code_to_string(encoded_end, encoded_end_length);


		delete[] literal_codes;

		lz77_encoder.print_perc();
	}
	
	//if last bit inserted was not the last bit in the byte insert the full last byte
	if(current_to_add_bit != 0)
		output_builder << current_to_add;
		
	println("");

}


void Deflate_encoder::add_length_distance_to_dynamic_content(int value, bool length, code_d* codes) {
	int static_value = (length) ? get_static_length_code(value) : get_static_distance_code(value);
	int huffman_code = codes[static_value].value;
	int huffman_code_length = codes[static_value].code_length;
	add_code_to_string(huffman_code, huffman_code_length);
	//extra bits
	int extra_bits_count = 0;
	int extra_bits = (length) ? get_static_length_extra_bits(value, &extra_bits_count) : get_static_distance_extra_bits(value, &extra_bits_count);
	add_code_to_string(extra_bits, extra_bits_count, true);
}

//Il metodo di salvataggio dei blocchi non viene applicato in questa implementazione dell'algoritmo deflate ma è presente (funzionante) per completezza 
void Deflate_encoder::non_compressed_encoding(char* to_compress,int size){
	add_code_to_string(1, 1);
	add_code_to_string(0,2);//00
    
    //skip all remaining bits, as RFC1951
    if(current_to_add_bit != 0){
        output_builder << current_to_add;
        current_to_add = 0;
        current_to_add_bit = 0;
    }
    
    //ADD HEADER
    uint16_t length = size;
    
    add_code_to_string(length,16,true);
    
    uint16_t mask = 65535;
    uint16_t nlength = (length) ^ mask;
    add_code_to_string(nlength,16,true);
    
    if(current_to_add_bit != 0){
        output_builder << current_to_add;
        current_to_add = 0;
        current_to_add_bit = 0;
    }

    //ADD DATA
    
    for(int d_i = 0;d_i < length;d_i++){
            output_builder << to_compress[d_i];
    }
	
}

void Deflate_encoder::static_encoding(char* to_compress,int size){
	
	LZ77_Encoder lz77_encoder = LZ77_Encoder(to_compress,size);
	while (lz77_encoder.hasMore()) {
		int curr_symbols = 0;
		lz77_r results[MAX_SYMBOLS];
		while (lz77_encoder.hasMore() && curr_symbols < MAX_SYMBOLS) {
			results[curr_symbols] = lz77_encoder.getNext();
			curr_symbols++;

		}

		if (lz77_encoder.hasMore())
			add_code_to_string(0, 1);
		else
			add_code_to_string(1, 1);


		add_code_to_string(2, 2);//10

		for(int a = 0; a < curr_symbols; a++){
			lz77_r result = results[a];
			if (!result.is_pair) {
				int literal = result.literal_length;
				int literal_length = 0;
				unsigned int encoded_literal = encode_static_literal(literal, &literal_length);
				add_code_to_string(encoded_literal, literal_length);
			} else {

				int length = result.literal_length;
				int distance = result.distance;

				int literal_length = 0;

				unsigned int encoded_length = encode_static_literal(get_static_length_code(length), &literal_length);

				add_code_to_string(encoded_length, literal_length);

				int extra_bits = 0;
				int extra_bits_value = get_static_length_extra_bits(length, &extra_bits);

				add_code_to_string(extra_bits_value, extra_bits, true);

				unsigned int encoded_distance = get_static_distance_code(distance);

				add_code_to_string(encoded_distance, 5);
				extra_bits_value = get_static_distance_extra_bits(distance, &extra_bits);
				add_code_to_string(extra_bits_value, extra_bits, true);
			}
		}


		int end_code = 256;
		int length = 0;
		int encoded_end = encode_static_literal(end_code, &length);
		add_code_to_string(encoded_end, length);


		lz77_encoder.print_perc();

	}


	if(current_to_add_bit != 0)
		output_builder << current_to_add;

	println("");
}



void Deflate_encoder::add_dynamic_alphabet_content(int code_lengths_length,code_d* codes,int* extra_bits_array,int* code_lengths_array,int HLIMIT){
	int extra_bits_index = 0;
	int h_counter = 0;
//	print("CONTENT [");
	for(int l_i = 0;l_i < code_lengths_length; l_i++){
		code_d curr_code = codes[code_lengths_array[l_i]];
		add_code_to_string(curr_code.value,curr_code.code_length);
		int extra_bits_count = 0;
		
		if(curr_code.symbol == 16){
			extra_bits_count = 2;
			h_counter += 3 + extra_bits_array[extra_bits_index];
		}else if(curr_code.symbol == 17){
			extra_bits_count = 3;
			h_counter += 3 + extra_bits_array[extra_bits_index];
		}else if(curr_code.symbol == 18){
			extra_bits_count = 7;
			h_counter += 11 + extra_bits_array[extra_bits_index];
		}else
			h_counter ++;
			
		add_code_to_string(extra_bits_array[extra_bits_index],extra_bits_count,true);
		
		if(extra_bits_count > 0)
			extra_bits_index++;
		
		if(h_counter >= HLIMIT){
			if(h_counter > HLIMIT){
				println("Error too long: "<<h_counter<<"/"<<HLIMIT<<")");
			}
			break;
		}
			
	}
	return;
}

unsigned int Deflate_encoder::encode_static_literal(int literal,int* literal_length){
	literal = (literal < 0)?char_to_uint(literal):literal;
	*literal_length = get_static_literal_length(literal);
	int literal_code = static_tree[literal];
	return literal_code;
}


void Deflate_encoder::add_code_to_string(int code,int length){
	add_code_to_string(code,length,false);
}

void Deflate_encoder::add_code_to_string(int code,int length,bool reversed){
	if(length > 0){
		int current_encoded_bit = 0;
		while(current_encoded_bit < length){
			if(current_to_add_bit == 8){
				output_builder << current_to_add;
				current_to_add = 0;
				current_to_add_bit = 0;
			}
			int bit_to_add;
			if(!reversed){
				
			    bit_to_add = (code >> (length - 1 - current_encoded_bit)) & 1;
			}
			else{
				bit_to_add = (code >> current_encoded_bit) & 1;
			}
			
			current_to_add |= (bit_to_add << (current_to_add_bit));
			current_encoded_bit++;
			current_to_add_bit++;
		}
	}
	return;
}

int Deflate_encoder::get_static_length_code(int length){
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
	println("Invalid lenght code " << length);
	return -1;
}

int Deflate_encoder::get_static_distance_code(int distance){
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
	println("Invalid distance code " << distance);
	return -1;
}
