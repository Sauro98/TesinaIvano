#include "huffman_tree.h"
// class constructor
Huffman_Tree::Huffman_Tree(std::string _original_string){
     original_string = _original_string;
     #if DEBUG > 0
	    println("CONSTRUCTOR");
	#endif
}

// class destructor
Huffman_Tree::~Huffman_Tree()
{
}

//getter for the frequency of characters
//will return an array of charachter frequencies, use frequencies_array_length to loop through it
val_f* Huffman_Tree::get_value_frequencies(std::vector<long> input_vector,int* frequencies_length){
    long length = input_vector.size();
    val_f* frequencies_to_return = new val_f[300];
    val_f frequencies[300];
    long current_length = 0;
    for(long index = 0;index < length;index++){
            long current_value = input_vector[index];
            bool found = false;
            for(long f_index = 0;f_index < current_length;f_index++){
                    if(frequencies[f_index].value == current_value){
                        found = true;
                        frequencies[f_index].frequence++;
                    }
            }
            if(!found){
                val_f vf = val_f(current_value,1);
                frequencies[current_length] = vf;
                current_length++;
            }
    }
    for(int i=0;i<current_length;i++){
            frequencies_to_return[i] = frequencies[i];            
    }
    *frequencies_length = current_length;
    order_frequence_array(frequencies_to_return,current_length);
    return frequencies_to_return; 
}

void Huffman_Tree::order_frequence_array(val_f* frequencies,int length){
     bool swapped = true;
     while(swapped){
     	swapped = false;
        for(int index = 0;index < length - 1;index++){
            if(frequencies[index].frequence > frequencies[index + 1].frequence){
                val_f temp = frequencies[index + 1];
                frequencies[index + 1] = frequencies[index];
                frequencies[index] = temp;
                swapped = true;
            }
        }
     }
     return;
}

//order dictionary for DEfLATE coding
void Huffman_Tree::order_dictionary(val_d dictionary[],int d_length){
     bool swapped = true;
     while(swapped){
     	swapped = false;
        for(int index = 0;index < d_length - 1;index++){
            if( (int) (dictionary[index].symbol) > (int)(dictionary[index + 1].symbol)){
                val_d temp = dictionary[index + 1];
                dictionary[index + 1] = dictionary[index];
                dictionary[index] = temp;
                swapped = true;
            }
        }
     }
     return;
}


long Huffman_Tree::max_long_value(){
   return std::numeric_limits<long>::max();
}

//will return the node with the lowest frequence in the two queues
node_f* Huffman_Tree::get_smallest_node_in_queues(Queue* q1,Queue* q2){
    int smallest_from_q1 = (q1->get_length() > 0)?q1->peek()->c_f.frequence:max_long_value();
    int smallest_from_q2 = (q2->get_length() > 0)?q2->peek()->c_f.frequence:max_long_value();
    if(smallest_from_q1 <= smallest_from_q2)
    	if(q1->get_length() > 0)
        	return q1->dequeue();
        else
        	return new node_f(val_f(0,0));
    else
        return q2->dequeue();
}
//will generate the prefix tree
node_f* Huffman_Tree::generate_tree(val_f* frequencies,int length){
     Queue symbols_queue = Queue(length);
     Queue tree_queue = Queue(length);
     for(int i=0;i<length;i++){
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
     node_f* root_node = tree_queue.dequeue();
     //#if DEBUG > 1
     //	print_LVR_tree(root_node,"");
     //#endif
     return root_node;
}

//get the huffman value of a char
std::string Huffman_Tree::get_char_value(node_f* start,long to_find){
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

code_d* Huffman_Tree::dynamic_tree_encoding(node_f* root_node,val_f* frequencies,int f_length,int alphabet_type){
	int alphabet_length = 0;
	if(alphabet_type == AT_LITERALS) 
		alphabet_length = DYNAMIC_LITERALS_ALPHABET_LENGTH;
	else if (alphabet_type == AT_DISTANCIES)
		alphabet_length = DYNAMIC_DISTANCES_ALPHABET_LENGTH;
	else
		alphabet_length = DYNAMIC_CODES_ALPHABET_LENGTH;
	
    val_d dictionary[f_length]; //as RFC 1951
	//FIRST I GET THE ORIGINAL HUFFMAN PREFIX AND DETERMINE THE GRATER CODE LENGTH
	int MAX_BITS = 0;	
	
	for(int i=0;i<f_length;i++){
		long current = frequencies[i].value;
		std::string prefix = get_char_value(root_node,current);
		prefix = prefix.substr(0,prefix.length() - 1);
		dictionary[i] = val_d(current,prefix);
		if(prefix.length() > MAX_BITS) MAX_BITS = prefix.length();
	}
    //order dictionary by symbol
    order_dictionary(dictionary,f_length);
   	//THEN I GET THE MINIMUM VALUE OF THE PREFIX FOR EACH CODE LENGTH; MORE SPECIFICATIONS AT https://tools.ietf.org/html/rfc1951
	    //if maximum prefix length is 4 I need a array of length 5 to store the 4 bits values count
	    MAX_BITS++;
	    
	    code_d* deflate_dictionary = new code_d[alphabet_length];
    	//d_i = dictionary index
    	int d_i = 0;
	    
	    for(int c_ai = 0; c_ai < alphabet_length; c_ai++){
           if(d_i < f_length && c_ai == (int)dictionary[d_i].symbol){
	    		int code_length = dictionary[d_i].value.length();
				deflate_dictionary[c_ai] = code_d(0,code_length,c_ai);
				d_i++;
	    	}else{
	    		deflate_dictionary[c_ai] = code_d(0,0,c_ai);
	    	}
        }
	    
	    #if DEBUG > 0
	    //println("GENERATING CODES");
	    //println("\tMAX_BITS "<<MAX_BITS);
	    #endif
	    
	    //binary length counter (keeps track of how many symbols have a certain code length)
	    int bl_count[MAX_BITS];
	    //init array to all 0
	    for(int i = 0;i < MAX_BITS;i++){
	    	bl_count[i] = 0;
	    }
	    for(int i=0;i<alphabet_length;i++){
			bl_count[deflate_dictionary[i].code_length] ++;
		}
		
		#if DEBUG > 0
		for(int i = 1;i < MAX_BITS;i++){
	    	//println("\tbl_count["<<i<<"] = "<<bl_count[i]);
	    }
	    #endif
	    
	    int next_code[MAX_BITS];
	    bl_count[0] = 0;
	    int code = 0;
	    for(int bits = 1; bits < MAX_BITS;bits++){
	    	code = (code + bl_count[bits - 1]) << 1;
	    	next_code[bits] = code;
	    }
	    
	    #if DEBUG > 0
	    for(int i = 1;i < MAX_BITS;i++){
	    	//println("\tnext_code["<<i<<"] = "<<next_code[i]);
	    }
	    //println("END\n");
	    #endif
    //END   
    
    //THEN I ASSIGN A NUMERICAL VALUE TO EVERY SYMBOL IN THE TREE
        #if DEBUG > 0
        //println("ASSIGNING CODES");
        #endif
	    //c_ai = code assign idex
	    for(int c_ai = 0; c_ai < alphabet_length; c_ai++){
	    	
	    	#if DEBUG > 1
	    	//print("\tCURRENT ASCII SYMBOL: "<<(char)c_ai<<" "<<c_ai);
	    	#endif
    		int code_length = deflate_dictionary[c_ai].code_length;
    		if(code_length > 0){
	    		unsigned char def_code = next_code[code_length];
	    		//the next code of the current code length will be consecutive to this one as rfc 1951
			    next_code[code_length] ++;
			    deflate_dictionary[c_ai].value = def_code;
	    		#if DEBUG > 1
				//println("code "<<(int)def_code<<" Length "<<code_length);
				#endif
			}
	    }
	    #if DEBUG > 0
	    //println("END\n");
	    #endif
	return deflate_dictionary;
    //END 
}

int* Huffman_Tree::get_code_length_codes(code_d* codes,bool literals,int* extra_bits,int* codes_array_length){
	int alphabet_length = (literals)?DYNAMIC_LITERALS_ALPHABET_LENGTH:DYNAMIC_DISTANCES_ALPHABET_LENGTH;
	int* code_lengths_array = new int[alphabet_length];
	int cl_i = 0;
	int eb_i = 0;
	for(int a_i = 0; a_i < alphabet_length; cl_i++){
		long curr = codes[a_i].code_length;
		if(a_i != alphabet_length-1 && curr == codes[a_i + 1].code_length){
			int while_i = 0;
			int max_limit = (curr == 0)?138:8;
			int min_limit = (curr == 0)?3:4;
			while(a_i + while_i < alphabet_length && while_i < max_limit && curr == codes[a_i + while_i].code_length)
				while_i ++;
				
			//while_i--;
				
			if(while_i < min_limit){
				code_lengths_array[cl_i] = curr;
				a_i++;
			}else{
				
				if(curr == 0){
					if(while_i < 11){
						code_lengths_array[cl_i] = 17;
						extra_bits[eb_i] = while_i - 3;
					}else{
						code_lengths_array[cl_i] = 18;
						extra_bits[eb_i] = while_i - 11;
					}
					eb_i++;
					a_i += while_i;
				}else{
					code_lengths_array[cl_i] = curr;
					cl_i++;
					code_lengths_array[cl_i] = 16;
					extra_bits[eb_i] = while_i - 4;
					a_i += while_i;
					eb_i++;
				}
			}
		}else{
			code_lengths_array[cl_i] = curr;
			a_i++;
		}
	}
	//println("cl_i "<<cl_i);
	*codes_array_length	 = cl_i;
	return code_lengths_array;	
}






































/*
// decoding function with RFC 1951 standard static tree
std::string Huffman_Tree::static_tree_decoding(int* lengths,std::string encoded){
    //FIRST I RECREATE THE STATIC TREE
    int MAX_BITS = 0;
    for(int l_i = 0; l_i < ASCII_CHARACHTERS_COUNT;l_i++){
        if(lengths[l_i]> MAX_BITS)
            MAX_BITS = lengths[l_i];        
    }
    MAX_BITS++;
    
    #if DEBUG > 0
	    println("GENERATING CODES");
	    println("\tMAX_BITS "<<MAX_BITS);
	#endif
    
    int bl_count[MAX_BITS];
    for(int i=0;i < MAX_BITS;i++){
        bl_count[i] = 0;        
    }
    for(int l_i = 0;l_i < ASCII_CHARACHTERS_COUNT;l_i++){
        bl_count[lengths[l_i]]++;
    }
    
    #if DEBUG > 0
		for(int i = 1;i < MAX_BITS;i++){
	    	println("\tbl_count["<<i<<"] = "<<bl_count[i]);
	    }
	#endif
	
    int next_code[MAX_BITS];
    bl_count[0] = 0;
    int code = 0;
    for(int bits = 1; bits < MAX_BITS;bits++){
   	    code = (code + bl_count[bits - 1]) << 1;
	    next_code[bits] = code;
    }
    
    #if DEBUG > 0
	    for(int i = 1;i < MAX_BITS;i++){
	    	println("\tnext_code["<<i<<"] = "<<next_code[i]);
	    }
	    println("END\n");
	#endif
    
    code_d dictionary[ASCII_CHARACHTERS_COUNT];
    for(int c_i = 0;c_i < ASCII_CHARACHTERS_COUNT; c_i++){
    	char code = 0;
    	if(lengths[c_i] != 0){
    		code = next_code[lengths[c_i]];
    		next_code[lengths[c_i]] ++;
    	}
    	dictionary[c_i] = code_d(code,lengths[c_i],(char)c_i);
    	#if DEBUG > 1
	    	print("\tSYMBOL: "<<(char)c_i<<" "<<c_i);
	    	println("CODE "<<(int)code<<" LENGTH "<<legnths[c_i]);
	    #endif
    }
    
    //create root to generate new tree
    node_f* root = new node_f(NULL);*/
    
    /*
		EXPLANATION OF THE NEXT FOR LOOP
		
		for every char that has a prefix 
			loop through prefix bits and if
				- the bit is 0 then we move to the left branch of the current node (and create it if necessary)
				- the bit is 1 then we move to the right branch of the current node (and create it if necessary)
				if the end of the prefix is reached then insert the symbol that the current prefix represents in the current node
				move on to the next prefix
			at the end of the loop we are left with a tree that will allow us to loop through the encoded string bits and decode it
			
	*//*
    
    for(int c_i = 0;c_i < ASCII_CHARACHTERS_COUNT;c_i++){
    	code_d code = dictionary[c_i];
    	if(code.code_length > 0){
    		node_f* current_node = root;
    		for(int a = 0;a < code.code_length; a++){
    			int curr = (unsigned char)(code.value << ( 8 - (code.code_length - a))) >> 7;
    			if(curr == 0){
    				if(current_node->left_child == NULL)
    					current_node->left_child = new node_f(current_node);
    				current_node = current_node->left_child;
    			}else{
    				if(current_node->right_child == NULL)
    					current_node->right_child = new node_f(current_node);
    				current_node = current_node->right_child;
    			}
    			if(a == code.code_length - 1)
    				current_node->c_f = val_f(code.symbol,1);
    		}
    	}
    }
    
    std::string decoded = "";
	node_f* current_node = root;
	for(int e_i = 0;e_i < encoded.length();e_i++){
		unsigned char current = encoded.at(e_i);
		for(int c_i=0;c_i < 8;c_i++){
			unsigned char current_i = ((unsigned char)(current << c_i)) >> 7;
			current_node = (current_i == 0)?current_node->left_child:current_node->right_child;
			if(current_node->left_child == NULL && current_node->right_child == NULL){
				decoded += current_node->c_f.value;
				current_node = root;
			}
		}
	}
	return decoded;    
    //END
}*/

//prints tree values in left - center - right order
void Huffman_Tree::print_LVR_tree(node_f* start,std::string prefix){
	printint(prefix + "charachter ",start->c_f.value);
	printintln(" frequence ",start->c_f.frequence);
	if(start->left_child != NULL)
		print_LVR_tree(start->left_child,prefix + "  0: ");
	if(start->right_child != NULL)
		print_LVR_tree(start->right_child,prefix + "  1: ");
}

