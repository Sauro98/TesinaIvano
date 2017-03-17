#include "deflate_decoder.h"

Deflate_decoder::Deflate_decoder(std::string _original){
	original = _original;
	long* static_codes = construct_static_literal_and_lengths_tree();
	/*
		EXPLANATION OF THE NEXT FOR LOOP
		
		for every char that has a prefix 
			loop through prefix bits and if
				- the bit is 0 then we move to the left branch of the current node (and create it if necessary)
				- the bit is 1 then we move to the right branch of the current node (and create it if necessary)
				if the end of the prefix is reached then insert the symbol that the current prefix represents in the current node
				move on to the next prefix
			at the end of the loop we are left with a tree that will allow us to loop through the encoded string bits and decode it
			
	*/
	
	static_root = new node_f((Huffman_node*)NULL);
    
    for(int c_i = 0;c_i < 288;c_i++){
    	long code = static_codes[c_i];
    	int code_length = get_static_literal_length(c_i);
    	int mask = get_mask_for_length(code_length);
    	//if(c_i == 68 ||c_i == 277 || c_i == 101)
    	//	println("CHAR "<<(char)c_i<<" CODE "<<code <<" LEN "<<code_length);
    		
    	node_f* current_node = static_root;
    	for(int a = 0;a < code_length; a++){
    		int curr = ((code <<  (a )) & mask ) >> (code_length - 1);
    		//if(c_i == 68 ||c_i == 277 || c_i == 101)
    		//	print(curr);
    		if(curr == 0){
    			if(current_node->left_child == NULL)
    				current_node->left_child = new node_f(current_node);
    			current_node = current_node->left_child;
    		}else{
    			if(current_node->right_child == NULL)
    				current_node->right_child = new node_f(current_node);
    			current_node = current_node->right_child;
    		}
    		if(a == code_length - 1)
    			current_node->value = c_i;
    	}
    	//if(c_i == 68 ||c_i == 277 || c_i == 101)
    		//	println("");
    }
}

std::string Deflate_decoder::decode(){
	long current_byte = 0;
	int current_bit = 0;
	for(int a = 0; a < original.length();a++){
            for(int b = 0; b < 8;b++){
                    print(read_bits(original,&current_byte,&current_bit,1));
            }
            println("");
    }
    current_byte = 0;
	current_bit = 0;
	int bfinal = 0;
	do{
        bfinal = read_bits(original,&current_byte,&current_bit,1);
		int btype = read_bits(original,&current_byte,&current_bit,2);
		/*//bits are in reversed order
		if(btype == 1)
		         btype = 2;
        else if (btype == 2)
                 btype = 1;*/
		#if DEBUG > 0
			println("BTYPE: "<<btype);
			println("BFINAL: "<<bfinal);
		#endif
		if(btype == BTYPE00){
			
		}else if(btype == BTYPE10){
			
		}else if(btype == BTYPE01){
			println("STATIC DECODING");
			int decoded_value = 0;
			do{
				decoded_value = get_next_static_value(original,&current_byte,&current_bit);
				if(decoded_value < 256){
					println("LITERAL - "<<(char)decoded_value);
				}else if(decoded_value > 256){
					println("LEN-DIS - not implemented");
					int length = get_length_from_code(decoded_value);
					int e_b = 0;
					get_static_length_extra_bits(length,&e_b);
					int extra_bits = read_bits(original,&current_byte,&current_bit,e_b);
					length += extra_bits;
					println("LENGTH - "<<length);
					
					//TODO (#1#): NOW GET DISTANCE
		
				}
			}while(decoded_value != END_CODE);
			
		}else{
			println("CRITICAL ERROR");
		}
	}while(bfinal != BFINAL);
	return "";
}

int Deflate_decoder::get_next_static_value(std::string feed,long*current_char,int* current_bit){
	node_f* curr_node = static_root;
	while(curr_node->value == -1){
		int movement = read_bits(feed,current_char,current_bit,1);
		print(movement);
		if(movement == 0)
			curr_node = curr_node->left_child;
		else
			curr_node = curr_node->right_child;
	}
	println("");
	int value = curr_node->value;
	//println("value "<<value<<" "<<(char)value);
	return value;
}

int Deflate_decoder::get_length_from_code(int code){
	switch(code){
		case 257:
			return 3;
		case 258:
			return 4;
		case 259:
			return 5;
		case 260:
			return 6;
		case 261:
			return 7;
		case 262:
			return 8;
		case 263:
			return 9;
		case 264:
			return 10;
		case 265:
			return 11;
		case 266:
			return 13;
		case 267:
			return 15;
		case 268:
			return 17;
		case 269:
			return 19;
		case 270:
			return 23;
		case 271:
			return 27;
		case 272:
			return 31;
		case 273:
			return 35;
		case 274:
			return 43;
		case 275:
			return 51;
		case 276:
			return 59;
		case 277:
			return 67;
		case 278:
			return 83;
		case 279:
			return 99;
		case 280:
			return 115;
		case 281:
			return 131;
		case 282:
			return 163;
		case 283:
			return 195;
		case 284:
			return 227;
		case 285:
			return 258;	
	}
	return 0;
}

unsigned int Deflate_decoder::read_bits(std::string feed,long* current_char,int* current_bit,int bits_count){
	unsigned int to_return = 0;
	char curr_char = feed.at(*current_char);
	for(int read_bit = 0;read_bit < bits_count;read_bit++){
		if((*current_bit) == 8){
			(*current_bit) = 0;
			(*current_char)++;
			curr_char = feed.at(*current_char);
		}
		unsigned char bit_to_add = (unsigned char)(curr_char << (7 - (*current_bit))) >> 7;
		to_return |= (bit_to_add << read_bit);
		(*current_bit)++;
	}
	return to_return;
}

