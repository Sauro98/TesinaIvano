#include "huffman_tree.h" // class's header file
// class constructor
Huffman_Tree::Huffman_Tree(std::string _original_string){
     original_string = _original_string;
}

// class destructor
Huffman_Tree::~Huffman_Tree()
{
	free(frequencies);
}

std::string Huffman_Tree::encode(){
	 frequencies = get_char_frequencies();
     order_frequence_array();
     generate_tree();
     std::string encoded = translate_original_in_encoded();
     return encoded;
}

std::string Huffman_Tree::decode(std::string encoded){
	std::string decoded = "";
	#if DEBUG > 0
	std::string encoded_b = transform_encoded_string_to_binary(encoded);
	println("DECODING - ENCODED BINARY: " + encoded_b);
	#endif
	node_f* current_node = root_node;
	for(int e_i = 0;e_i < encoded.length();e_i++){
		unsigned char current = encoded.at(e_i);
		for(int c_i=0;c_i < 8;c_i++){
			unsigned char current_i = ((unsigned char)(current << c_i)) >> 7;
			current_node = (current_i == 0)?current_node->left_child:current_node->right_child;
			if(current_node->left_child == NULL && current_node->right_child == NULL){
				decoded += current_node->c_f.value;
				current_node = root_node;
			}
		}
	}
	return decoded;
}

std::string Huffman_Tree::transform_encoded_string_to_binary(std::string encoded){
	std::string encoded_b = "";
	for(int e_i=0;e_i < encoded.length();e_i++){
		unsigned char current = encoded.at(e_i);
		for(int c_i = 0;c_i < 8;c_i++){
			unsigned char value = ((unsigned char)(current << c_i)) >> 7;
			encoded_b += (value == 0)?"0":"1";
		}
	}
	return encoded_b;
}

//getter for the frequency of characters
//will return an array of charachter frequencies, use frequencies_array_length to loop through it
char_f* Huffman_Tree::get_char_frequencies(){
    long length = original_string.length();
    char_f* frequencies_to_return = new char_f[300];
    char_f frequencies[300];
    long current_length = 0;
    for(long index = 0;index < length;index++){
            char current_char = original_string.at(index);
            bool found = false;
            for(long f_index = 0;f_index < current_length;f_index++){
                    if(frequencies[f_index].value == current_char){
                        found = true;
                        frequencies[f_index].frequence++;
                    }
            }
            if(!found){
                char_f cf = char_f(current_char,1);
                frequencies[current_length] = cf;
                current_length++;
            }
    }
    frequencies_array_length = current_length;
    for(int i=0;i<current_length;i++){
            frequencies_to_return[i] = frequencies[i];            
    }
    
    return frequencies_to_return; 
}

void Huffman_Tree::order_frequence_array(){
     bool swapped = true;
     while(swapped){
     	swapped = false;
        for(int index = 0;index < frequencies_array_length - 1;index++){
            if(frequencies[index].frequence > frequencies[index + 1].frequence){
                char_f temp = frequencies[index + 1];
                frequencies[index + 1] = frequencies[index];
                frequencies[index] = temp;
                swapped = true;
            }
        }
     }
     return;
}

//order dictionary for DEfLATE coding
void Huffman_Tree::order_dictionary(char_d dictionary[]){
     bool swapped = true;
     while(swapped){
     	swapped = false;
        for(int index = 0;index < frequencies_array_length - 1;index++){
            if( (int) (dictionary[index].symbol) > (int)(dictionary[index + 1].symbol)){
                char_d temp = dictionary[index + 1];
                dictionary[index + 1] = dictionary[index];
                dictionary[index] = temp;
                swapped = true;
            }
        }
     }
     return;
}

int Huffman_Tree::max_int_value(){
   return std::numeric_limits<int>::max();
}

//will return the node with the lowest frequence in the two queues
node_f* Huffman_Tree::get_smallest_node_in_queues(Queue* q1,Queue* q2){
    int smallest_from_q1 = (q1->get_length() > 0)?q1->peek()->c_f.frequence:max_int_value();
    int smallest_from_q2 = (q2->get_length() > 0)?q2->peek()->c_f.frequence:max_int_value();
    if(smallest_from_q1 <= smallest_from_q2)
        return q1->dequeue();
    else
        return q2->dequeue();
}
//will generate the prefix tree
void Huffman_Tree::generate_tree(){
     Queue symbols_queue = Queue(frequencies_array_length);
     Queue tree_queue = Queue(frequencies_array_length);
     for(int i=0;i<frequencies_array_length;i++){
         symbols_queue.enqueue(new node_f(frequencies[i]));
     }
     //symbols_queue.print_queue();
     do{
         node_f* left = get_smallest_node_in_queues(&symbols_queue,&tree_queue);
         node_f* right = get_smallest_node_in_queues(&symbols_queue,&tree_queue);
         node_f* parent = new node_f(left,right);
         left -> parent = parent;
         right -> parent = parent;
         tree_queue.enqueue(parent);
     }while(tree_queue.get_length() + symbols_queue.get_length() > 1);
     root_node = tree_queue.dequeue();
     #if DEBUG > 1
     print_LVR_tree(root_node,"");
     #endif
     return;
}

//get the huffman value of a char
std::string Huffman_Tree::get_char_value(node_f* start,char to_find){
	if(start->c_f.value == to_find){
		return "-";
	}else{
		if(start->left_child == NULL && start->right_child == NULL)
			return "";
		//assign 0 for every left node in the path to the destination leaf, 1 for every right node. If the path traveled leads to a leaf that is not the wanted leaf then search another path
		else{
			if(get_char_value(start->left_child,to_find) != ""){
				return "0" + get_char_value(start->left_child,to_find);
			}else if(get_char_value(start->right_child,to_find) != ""){
				return "1" + get_char_value(start->right_child,to_find);
			}else{
				return "";
			}
		}
			
	}	
}

std::string Huffman_Tree::translate_original_in_encoded(){
	char_d dictionary[frequencies_array_length];
	//as RFC 1951
	//FIRST I GET THE ORIGINAL HUFFMAN PREFIX AND DETERMINE THE GRATER CODE LENGTH
		int MAX_BITS = 0;	
		#ifdef TEST
			///HARDCODED VALUES FOR TESTING
			dictionary[5] = char_d('A',"000");
			dictionary[6] = char_d('B',"000");
			dictionary[2] = char_d('C',"000");
			dictionary[3] = char_d('D',"000");
			dictionary[4] = char_d('E',"000");
			dictionary[0] = char_d('F',"00");
			dictionary[1] = char_d('G',"0000");
			dictionary[7] = char_d('H',"0000");
			MAX_BITS = 4;
		 #else	
			for(int i=0;i<frequencies_array_length;i++){
				char current = frequencies[i].value;
				std::string prefix = get_char_value(root_node,current);
				prefix = prefix.substr(0,prefix.length() - 1);
				dictionary[i] = char_d(current,prefix);
				if(prefix.length() > MAX_BITS) MAX_BITS = prefix.length();
	     	}
	     #endif
	     //order dictionary by symbol
	     order_dictionary(dictionary);
	//END
	
	//THEN I GET THE MINIMUM VALUE OF THE PREFIX FOR EACH CODE LENGTH; MORE SPECIFICATIONS AT https://tools.ietf.org/html/rfc1951
	    //if maximum prefix length is 4 I need a array of length 5 to store the 4 bits values count
	    MAX_BITS++;
	    
	    #if DEBUG > 0
	    println("MAX_BITS "<<MAX_BITS);
	    #endif
	    
	    //binary length counter (keeps track of how many symbols have a certain code length)
	    int bl_count[MAX_BITS];
	    //init array to all 0
	    for(int i = 0;i < MAX_BITS;i++){
	    	bl_count[i] = 0;
	    }
	    for(int i=0;i<frequencies_array_length;i++){
			bl_count[dictionary[i].value.length()] ++;
		}
		
		#if DEBUG > 0
		for(int i = 1;i < MAX_BITS;i++){
	    	println("bl_count["<<i<<"] = "<<bl_count[i]);
	    }
	    #endif
	    
	    int next_code[MAX_BITS];
	    int code = 0;
	    for(int bits = 1; bits < MAX_BITS;bits++){
	    	code = (code + bl_count[bits - 1]) << 1;
	    	next_code[bits] = code;
	    }
	    
	    #if DEBUG > 0
	    for(int i = 1;i < MAX_BITS;i++){
	    	println("next_code["<<i<<"] = "<<next_code[i]);
	    }
	    #endif
    //END
    
    //THEN I ASSIGN A NUMERICAL VALUE TO EVERY SYMBOL IN THE TREE
    	code_d deflate_dictionary[ASCII_CHARACHTERS_COUNT];
    	//d_i = dictionary index
    	int d_i = 0;
	    //c_ai = code assign idex
	    for(int c_ai = 0; c_ai < ASCII_CHARACHTERS_COUNT; c_ai++){
	    	
	    	#if DEBUG > 1
	    	print("CURRENT ASCII SYMBOL: "<<(char)c_ai);
	    	#endif
	    	if((char) c_ai == dictionary[d_i].symbol){
	    		int code_length = dictionary[d_i].value.length();
	    		unsigned char def_code = next_code[code_length];
	    		//the next code of the current code length will be consecutive to this one as rfc 1951
				next_code[code_length] ++;
				deflate_dictionary[c_ai] = code_d(def_code,code_length,(char)c_ai);
				d_i++;
	    		#if DEBUG > 1
				print("; found in dictionary ");
				println("code "<<(int)def_code<<" Length "<<code_length);
				#endif
	    	}else{
	    		deflate_dictionary[c_ai] = code_d(0,0,(char)c_ai);
	    		#if DEBUG > 1
	    		print("; not found in dictionary ");
				println("code "<<0<<" Length "<<0);
	    		#endif
	    	}
	    }
    //END
    
    //TEST TO UNDERSTAND STANDARDIZED HUFFMAN
	    //print only code lengths
	    #if DEBUG > 0
	    //init array to all 0
	    for(int i = 0;i < MAX_BITS;i++){
	    	bl_count[i] = 0;
	    }
	    //
	    for(int i = 1;i < ASCII_CHARACHTERS_COUNT;i++){
	    	code_d curr = deflate_dictionary[i];
	    	println(" Length "<<curr.code_length);
	    	bl_count[curr.code_length] ++;
	    }
	    #endif
	    code = 0;
	    bl_count[0] = 0;
	    for(int bits = 1; bits < MAX_BITS;bits++){
	    	code = (code + bl_count[bits - 1]) << 1;
	    	next_code[bits] = code;
	    }
	    for(int i = 1;i < ASCII_CHARACHTERS_COUNT;i++){
			int len = deflate_dictionary[i].code_length;
			if(len > 0){
				int new_code = next_code[len];
				next_code[len]++;
				println(new_code<<" should be the same as "<<(int)deflate_dictionary[i].value);
			}
		} 
	    
    //END
    	
    //NOW I TRANSLATE THE ORIGINAL STRING INTO THE ENCODED STRING
    	/*std::string encoded = "";
    	unsigned char next_e_c = 0;
		unsigned char temp_buf = 0;
		int bitIndex = 0;
		int string_index = 0;
		while(string_index < original_string.length())){
			char current_char = original_string.at(string_index);
			//dictionary index
			for(int d_i = 0;d_i < frequencies_array_length;d_i++){
			    if(current_char == deflate_dictionary[d_i].symbol){
			     	encoded_b += dictionary[d_i].value;
			     	d_i += frequencies_array_length;
			    }
			}
		}*/
	//END
    /*
			     //binary representation of encoded string
			     std::string encoded_b = "";
			     //string index
			     for(int s_i = 0;s_i < original_string.length();s_i++){
			     	char current = original_string.at(s_i);
			     	//dictionary index
			     	for(int d_i = 0;d_i < frequencies_array_length;d_i++){
			     		if(current == dictionary[d_i].symbol){
			     			encoded_b += dictionary[d_i].value;
			     			d_i += frequencies_array_length;
			     		}
			     	}
			     }
			     //encoded string now contains the binary rapresentation of the actual encoded string, to get its real value the string length must be a multiple of 8
			     int missing_length = (encoded_b.length() % 8 == 0)?0:8 - (encoded_b.length() % 8);
			     for(int m_i = 0;m_i < missing_length;m_i++){
			     	encoded_b += "0";
			     }
			     //println("ENCODED BINARY: " + encoded_b);
			     //now the string has to be divided in groups of eight and each char will represent a bit in the encoded string
			     std::string encoded = "";
				 for(int eb_i = 0;eb_i < encoded_b.length();eb_i+=8){
				 	std::string e_sub = encoded_b.substr(eb_i,8);
				 	unsigned char current = (unsigned char) 0;
				 	for(int sub_i=0;sub_i < 8;sub_i++){
				 		int curr_value = (e_sub.at(sub_i) == '0')?0:1;
				 		//start adding from the first bit in the left (128)
				 		unsigned char to_add = curr_value * (unsigned char)pow(2,7 - sub_i) ;
				 		current |= to_add;
				 	}
				 	encoded += current;
				 	//println("ENCODING CURRENT: " + encoded);
				 }
    */
    return "";
}

//prints tree values in left - center - right order
void Huffman_Tree::print_LVR_tree(node_f* start,std::string prefix){
	printint(prefix + "charachter ",start->c_f.value);
	printintln(" frequence ",start->c_f.frequence);
	if(start->left_child != NULL)
		print_LVR_tree(start->left_child,prefix + "  ");
	if(start->right_child != NULL)
		print_LVR_tree(start->right_child,prefix + "  ");
}
/*println("ORDERED");
     for(int i=0;i<frequencies_array_length;i++){
             printint("Char at ",i);
             char c = frequencies[i].value;
             std::cout<<"  "<<(int)c;
             std::cout<<" "<< c;
             int f = frequencies[i].frequence;
             printintln(" frequence ", f);
     }
     println("----------");*/
     
	 /*println("TREE START"); 
     print_LVR_tree(root_node);
     println("TREE END");*/

     /*println("DICTIONARY");
     for(int i=0;i<frequencies_array_length;i++){
             printint("Symbol at ",i);
             char c = dictionary[i].symbol;
             std::cout<<" "<< c;
             println(" frequence " + dictionary[i].value);
     }
     println("DICTIONARY END");*/

	 		/*printintln("CHAR AT " , sub_i);
	 		printintln("CHAR VALUE = ",e_sub.at(sub_i));
	 		printintln("INT VALUE = ",curr_value);
	 		printintln("POW OF 2 VALUE = ",(unsigned int)to_add);
	 		printintln("CURR VALUE = ",(unsigned int)current);
	 		println("END---");*/
