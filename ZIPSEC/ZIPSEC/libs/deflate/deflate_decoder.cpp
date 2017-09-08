#include "deflate_decoder.h"


int current_byte = 0;
int current_bit = 0;

Deflate_decoder::Deflate_decoder(std::string _original){
	original = _original;
	int* static_codes = construct_static_literal_and_lengths_tree();
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
	
	static_root = new s_node_f((Static_Huffman_node*)NULL);
    
    for(int c_i = 0;c_i < 288;c_i++){
    	int code = static_codes[c_i];
    	int code_length = get_static_literal_length(c_i);
    	s_node_f* current_node = static_root;
    	for(int a = 0;a < code_length; a++){
    		int curr = (code >> (code_length - 1 - a)) & 1;
    		//(code >> length - 1 - current_encoded_bit) & 1;
    		if(curr == 0){
    			if(current_node->left_child == NULL)
    				current_node->left_child = new s_node_f(current_node);
    			current_node = current_node->left_child;
    		}else{
    			if(current_node->right_child == NULL)
    				current_node->right_child = new s_node_f(current_node);
    			current_node = current_node->right_child;
    		}
    		if(a == code_length - 1)
    			current_node->value = c_i;
    	}
    }
}

std::string Deflate_decoder::decode(int* compressed_size){
    //println("START DECODING");
	std::ostringstream debug_result;
	//input string pointers
	
	bool lastwasliteral = true;
	
	//output string
	std::string output = "";
	
	#if DEBUG > 1
		for(int a = 0; a < original.length();a++){
	            for(int b = 0; b < 8;b++){
	                    print(read_bits(&current_byte,&current_bit,1));
	            }
	            println("");
	    }
	    current_byte = 0;
		current_bit = 0;
	#endif
	
	int bfinal = 0;
	do{
        bfinal = read_bits(1);
		int btype = read_bits(2);
		#if DEBUG > 0
			debug_result << "BTYPE: ";
			debug_result << btype;
			debug_result << "\nBFINAL: ";
			debug_result << bfinal;
		#endif
		if(btype == BTYPE00){
            while(current_bit != 8){
                read_bits(1);
            }
            uint16_t length = read_bits(16,false);
            uint16_t nlength = read_bits(16,false);
            uint16_t nl_test = length ^ 65535;
            if(nlength == nl_test){
                for(int a = 0;a < length;a++){
                    output += (char)read_bits(8);        
                }
            }     
			
		}else if(btype == BTYPE10){
            #if DEBUG > 0
				debug_result << "\nDYNAMIC DECODING\n";
			#endif  
			int HLIT = read_bits(5) + 257;
			int HDIST = read_bits(5) + 1;
			int HCLEN = read_bits(4) + 4;
            #if DEBUG > 0
				debug_result<<"HLIT "<<HLIT<<"\n";
				debug_result<<"HDIST "<<HDIST<<"\n";
				debug_result<<"HCLEN "<<HCLEN<<"\n";
			#endif
			if(HLIT > 286 || HDIST > 30 || HCLEN > 19){
                   println("Error: invalid lengths");
                   return "error";
            }
            int* codes_code_lengths = new int[HCLEN]; 
            for(int hclen_i = 0; hclen_i < HCLEN; hclen_i++){
                codes_code_lengths[hclen_i] = read_bits(3);
            }
            int* codes = generate_codes_from_lengths(codes_code_lengths,HCLEN,true);
            
            #if DEBUG > 0
            	for(int a = 0; a < HCLEN; a++){
            		if(codes_code_lengths[hclen_reference_ord[a]] > 0 && hclen_reference_ord[a] < HCLEN)
            		debug_result<<"code "<<a<<" "<<codes_code_lengths[hclen_reference_ord[a]]<<" "<< hclen_reference_ord[a]<<"\n";
            	}
			#endif
            node_f* codes_root = generate_tree_from_array(codes,codes_code_lengths,HCLEN,true);
               
            int* litlen_code_lengths = new int[HLIT];
            int* dist_code_lengths = new int[HDIST];
			int codes_err = get_alphabet_code_lengths(&litlen_code_lengths[0], HLIT, codes_root);
			codes_err += get_alphabet_code_lengths(&dist_code_lengths[0], HDIST, codes_root);
			if ( codes_err > 0) {
				println("Error: too many code lengths");
				return "Error: too many code lengths";
			}
            
            #if DEBUG > 0
            	for(int a = 0; a < HLIT; a++){
            		debug_result<<"litlen "<<a<<" "<<litlen_code_lengths[a]<<"\n";
            	}for (int a = 0; a < HDIST; a++) {
					debug_result << "dist " << a << " " << dist_code_lengths[a] << "\n";
				}
			#endif
            
            int* litlen_codes = generate_codes_from_lengths(litlen_code_lengths,HLIT,false);
            
            
            int* distance_codes = generate_codes_from_lengths(dist_code_lengths,HDIST,false);
            
            node_f* litlen_tree = generate_tree_from_array(litlen_codes,litlen_code_lengths,HLIT,false);
            node_f* distances_tree = generate_tree_from_array(distance_codes,dist_code_lengths,HDIST,false);
            
            int encoded_literal = 0;
            while(encoded_literal != END_CODE){
            	encoded_literal = get_next_dynamic_value(litlen_tree);
            	if(encoded_literal < END_CODE){
            		output += (char)encoded_literal;
            		#if DEBUG > 0
            		debug_result<<((char)encoded_literal);
            		#endif
            	}else if(encoded_literal > END_CODE){
            		int length = get_length_from_code(encoded_literal);
					int e_b = 0;
					get_static_length_extra_bits(length,&e_b);
					e_b = read_bits(e_b);
					length += e_b;
            		int encoded_distance = get_next_dynamic_value(distances_tree);
            		int distance = get_distance_from_code(encoded_distance);
            		int d_e_b = 0;
            		get_static_distance_extra_bits(distance,&d_e_b);
            		d_e_b = read_bits(d_e_b);
            		distance += d_e_b;
            		#if DEBUG > 0
            		debug_result<<"\nmatch("<<length<<","<<distance<<") ";
            		#endif
            		int p_l = (int)output.length();
            		
					for(int i = p_l - distance;i < (p_l - distance) + length; i++){
						if(i >= 0){
							output += output.at(i);
							#if DEBUG > 0
							debug_result << " " << output.at(i);
							#endif
							//print(output.at(i));
						} else{
							#if DEBUG > 0
							std::ofstream myfile;
						    myfile.open ("decompressed_debug.txt");
						    myfile << debug_result.str();
						    myfile << "\n";
						    myfile << output;
						    myfile.close();
						    #endif
							println("Error: distance too far back (" << distance << " / " << p_l << ")");
						} 
					}

					#if DEBUG > 0
						debug_result << "\n";
					#endif
            	}

            }
			delete []codes_code_lengths;
			delete []litlen_code_lengths;
			delete []dist_code_lengths;

		}else if(btype == BTYPE01){
			#if DEBUG > 0
				debug_result << "\nSTATIC DECODING\n";
			#endif
			int decoded_value = 0;
			do{
				decoded_value = get_next_static_value();
				if(decoded_value < 256){
					#if DEBUG > 0
						if(!lastwasliteral)
						debug_result <<"literal '";
						debug_result << (char)decoded_value;
					#endif
					output += (char)decoded_value;
					lastwasliteral = true;
				}else if(decoded_value > 256){
					int length = get_length_from_code(decoded_value);
					int e_b = 0;
					get_static_length_extra_bits(length,&e_b);
					int extra_bits = read_bits(e_b);
					length += extra_bits;
					#if DEBUG > 0
						if(lastwasliteral)
						debug_result << "\n";
						debug_result << "match (";
						debug_result << length;
						
					#endif
					
					//DISTANCES DO NOT HAVE TO BE READ IN REVERSE ORDER SO ADD TRUE WHEN READING A DISTANCE WITH read_bits
					
					int distance_code = read_bits(5,true);
					int distance = get_distance_from_code(distance_code);
					e_b = 0;
					get_static_distance_extra_bits(distance,&e_b);
					extra_bits = read_bits(e_b);
					distance += extra_bits;
					
					#if DEBUG > 0
						debug_result << ",";
						debug_result << distance;
						debug_result << ") \n";
					#endif
					
					//println("\n eb "<<extra_bits<<" l "<<e_b);
					int p_l = (int)output.length();
					for(int i = p_l - distance;i < (p_l - distance) + length; i++){
						if(i >= 0){
							output += output.at(i);
						} else{
                           #if DEBUG > 0
							std::ofstream myfile;
								myfile.open("decompressed_debug.txt", std::ios_base::binary);
								myfile << debug_result.str();
								myfile << "\n";
								myfile << output.c_str();
								myfile.close();
								println("END DECODING");
                            #endif
							println("Error: distance too far back ("<<distance<<" / "<<p_l);
							return "sdasdsa";
						}
					}
					lastwasliteral = false;
		
				}
			}while(decoded_value != END_CODE);
			
		}else{
			#if DEBUG > 0
				println("CRITICAL ERROR");
			#endif
		}
	}while(bfinal != BFINAL);
	*compressed_size = current_byte; 
	#if DEBUG > 1
		println("OUTPUT: "<<output);
	#endif
	
	//println(debug_result);
	#if DEBUG > 0
		std::ofstream myfile;
	    myfile.open ("decompressed_debug.txt",std::ios_base::binary);
	    myfile << debug_result.str();
	    myfile << "\n";
	    myfile << output;
	    myfile.close();
	    println("END DECODING");
    #endif
	return output;
}


int Deflate_decoder::get_alphabet_code_lengths(int* code_lengths,int size,node_f* tree_root){
    for(int index = 0; index < size;){
        int value = get_next_dynamic_value(tree_root);
        int repeats = 0;
        int repeated = 0;
        if(value < 16){
            code_lengths[index] = value;
            index++;
        }else if (value == 16){
            int extra_bits = read_bits(2,false);
            repeats = 3 + extra_bits;
            repeated = code_lengths[index - 1]; 
        }else if (value == 17){
            int extra_bits = read_bits(3,false);
            repeats = 3 + extra_bits;
            repeated = 0; 
        }else if(value == 18){
            int extra_bits = read_bits(7,false);
            repeats = 11 + extra_bits;
            repeated = 0;
        }
        for(int i = 0; i < repeats;i++){
            code_lengths[index + i] = repeated;
        } 
        index += repeats;
        if(index > size)
			return 1;   
    }
    return 0;
}

node_f* Deflate_decoder::generate_tree_from_array(int* codes,int* lengths,int codes_length,bool is_hclen){
    node_f* tree_root = new node_f((Huffman_node*)NULL);
    
    for(int c_i = 0;c_i < codes_length;c_i++){
    	int code = codes[c_i];
    	int code_length = lengths[c_i];
    	node_f* current_node = tree_root;
    	for(int a = 0;a < code_length; a++){
    		int curr = (code >> (code_length - 1 - a)) & 1;
    		if(curr == 0){
    			if(current_node->left_child == NULL)
    				current_node->left_child = new node_f(current_node);
    			current_node = current_node->left_child;
    		}else{
    			if(current_node->right_child == NULL)
    				current_node->right_child = new node_f(current_node);
    			current_node = current_node->right_child;
    		}
    		if(a == code_length - 1){
    			current_node->c_f = (is_hclen)?val_f(hclen_reference[c_i],0):val_f(c_i,0);
                current_node->has_value = true;
            }
    	}
    }
    return tree_root;
}

int Deflate_decoder::get_next_static_value(){
	s_node_f* curr_node = static_root;
	
	
	while(curr_node->value == -1){
		int movement = read_bits(1);
		//print(movement);
		if(movement == 0)
			curr_node = curr_node->left_child;
		else
			curr_node = curr_node->right_child;
	}
	
	//println("");
	int value = curr_node->value;
	//println("value "<<value<<" "<<value);
	return value;
}

int Deflate_decoder::get_next_dynamic_value(node_f* tree_root){
	node_f* curr_node = tree_root;
	int value = 0;
	try{
		while(!curr_node->has_value){
			//print(movement);
			if(read_bits(1) == 0)
				curr_node = curr_node->left_child;
			else
				curr_node = curr_node->right_child;
			if(curr_node == NULL)
				throw 1;
		}
		value = curr_node->c_f.value;
	}catch(...){
	}
	//println("");
	return value;
	//println("value "<<value<<" "<<value);
}

int Deflate_decoder::get_distance_from_code(int code){
	if(code <= 4)
		return code + 1;
	switch(code){
		case 5:
			return 7;
		case 6:
			return 9;
		case 7:
			return 13;
		case 8:
			return 17;
		case 9:
			return 25;
		case 10:
			return 33;
		case 11:
			return 49;
		case 12:
			return 65;
		case 13:
			return 97;
		case 14:
			return 129;
		case 15:
			return 193;
		case 16:
			return 257;
		case 17:
			return 385;
		case 18:
			return 513;
		case 19:
			return 769;
		case 20:
			return 1025;
		case 21:
			return 1537;
		case 22:
			return 2049;
		case 23:
			return 3073;
		case 24:
			return 4097;
		case 25:
			return 6145;
		case 26:
			return 8193;
		case 27:
			return 12289;
		case 28:
			return 16385;
		case 29:
			return 24577;
		
	}
	println("Invalid distance read" << code);
	return 0;
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
	println("Invalid length read " << code);
	return 0;
}

int Deflate_decoder::read_bits(int bits_count,bool reversed){
	unsigned int to_return = 0;
	char curr_char = original.at(current_byte);
	for(int read_bit = 0;read_bit < bits_count;read_bit++){
		if(current_bit == 8){
			current_bit = 0;
			current_byte++;
			curr_char = original.at(current_byte);
		}
		unsigned char bit_to_add = (unsigned char)(curr_char << (7 - (current_bit))) >> 7;
		if(!reversed)
			to_return |= (bit_to_add << (read_bit));
		else
			to_return |= (bit_to_add << (bits_count - 1 - read_bit));
		current_bit++;
	}
	return to_return;
}

int Deflate_decoder::read_bits(int bits_count){
	return read_bits(bits_count,false);
}

