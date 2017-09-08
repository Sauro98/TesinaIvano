#include "huffman_tree.h"
// class constructor
Huffman_Tree::Huffman_Tree(std::string _original_string){
     #if DEBUG > 0
	    println("CONSTRUCTOR");
	#endif
}

// class destructor
Huffman_Tree::~Huffman_Tree()
{
}


int Huffman_Tree::max_int_value(){
   return std::numeric_limits<int>::max();
}

//will return the node with the lowest frequence in the two queues
node_f* Huffman_Tree::get_smallest_node_in_queues(Queue* q1,Queue* q2){
    int smallest_from_q1 = (q1->get_length() > 0)?q1->peek()->c_f.frequence:max_int_value();
    int smallest_from_q2 = (q2->get_length() > 0)?q2->peek()->c_f.frequence:max_int_value();
    if(smallest_from_q1 <= smallest_from_q2)
    	if(q1->get_length() > 0)
        	return q1->dequeue();
        else
        	return new node_f((0,0));
    else
        return q2->dequeue();
}

int get_tree_length(node_f* root_node){
    if(root_node->left_child == NULL && root_node->right_child == NULL){
       return 1;
    }
    int l_depth = get_tree_length(root_node->left_child);
    int r_depth = get_tree_length(root_node->right_child);
    return (l_depth > r_depth)?1 + l_depth:1 + r_depth;
}

void fill_depths_arr(node_f* node,int* leaves,int* non_leaves,int depth){
    if(node == NULL)
        return;
    if(node->has_value){
        leaves[depth]++;
    }
    else
        non_leaves[depth]++;
    fill_depths_arr(node->left_child,leaves,non_leaves,depth + 1);
    fill_depths_arr(node->right_child,leaves,non_leaves,depth + 1);
}


//will generate the prefix tree
code_d* Huffman_Tree::generate_tree(val_f* frequencies,int length,int* out_length,int MAX){
	 if(length == 0){
	 	println("WARNING: GENERATING TREE OF LENGTH 0");
	 	return NULL;
	 }

     Queue symbols_queue = Queue(length);
     Queue tree_queue = Queue(length);
	 *out_length = 0;
     for(int i=0;i<length;i++){
		 if (frequencies[i].frequence != 0) {
			 symbols_queue.enqueue(new node_f(frequencies[i]));
			 (*out_length)++;
		 }
     }
     int depth = 0;
     do{
         node_f* left = get_smallest_node_in_queues(&symbols_queue,&tree_queue);
         node_f* right = get_smallest_node_in_queues(&symbols_queue,&tree_queue);
         node_f* parent = new node_f(left,right);
         left -> parent = parent;
         right -> parent = parent;
         tree_queue.enqueue(parent);
     }while(tree_queue.get_length() + symbols_queue.get_length() > 1);
     node_f* root_node = tree_queue.dequeue();
     depth = get_tree_length(root_node);
     
     
     int* leaves_at_depth = new int[depth];
     int* non_leaves_at_depth = new int[depth];
     
    for(int a = 0; a < depth; a++)
        leaves_at_depth[a] = 0;
    for(int a = 0; a < depth; a++)
        non_leaves_at_depth[a] = 0;
        
     fill_depths_arr(root_node,leaves_at_depth,non_leaves_at_depth,0);  
	 
	 delete root_node;

     if(depth > MAX){
            //STEP A
            
            int set_S = 0;
            for(int a = MAX + 1; a < depth; a++){
                set_S += leaves_at_depth[a];
                leaves_at_depth[a] = 0;
            }
            //println("Set S: "<<set_S);
            //STEP B
            
            set_S -= non_leaves_at_depth[MAX];
            leaves_at_depth[MAX] += non_leaves_at_depth[MAX];
            //STEP C
            
            while(set_S > 0){
                int c_depth = MAX - 1;
                while(leaves_at_depth[c_depth] < 1)
                    c_depth--;
                leaves_at_depth[c_depth] -= 1;
                leaves_at_depth[c_depth + 1] += 2;
                set_S--;
                
            }
            //return directly the result
     }
     code_d* codes = new code_d[(*out_length)];
     int freq_in = 0;
	 while (frequencies[freq_in].frequence == 0)
		 freq_in++;

	 int code_in = 0;

     for(int a = depth - 1; a > -1; a--){
     	for(int b = 0; b < leaves_at_depth[a]; b++){
     		codes[code_in] = Code_dictionary(0,a,frequencies[freq_in].value);
     		freq_in++;
			code_in++;
     	}
     }
     
	 delete[] leaves_at_depth;
	 delete []non_leaves_at_depth;
     return codes;
}



code_d* Huffman_Tree::dynamic_tree_encoding(code_d* raw_codes,int codes_length,int alphabet_type){
	int alphabet_length = 0;
	if(alphabet_type == AT_LITERALS) 
		alphabet_length = DYNAMIC_LITERALS_ALPHABET_LENGTH;
	else if (alphabet_type == AT_DISTANCIES)
		alphabet_length = DYNAMIC_DISTANCES_ALPHABET_LENGTH;
	else
		alphabet_length = DYNAMIC_CODES_ALPHABET_LENGTH;
	
	int MAX_BITS = raw_codes[0].code_length;
	//by symbol	
    SORT_ARRAY(raw_codes, raw_codes + codes_length);
    
	    MAX_BITS++;
	    if(MAX_BITS > 16){
	    	println("TREE IS OVERSIZED");
	    	println("codes_length "<<codes_length);
	    	println("MAX_BITS "<<MAX_BITS);
	    	system("PAUSE");
	    }
	    code_d* deflate_dictionary = new code_d[alphabet_length];
    	int d_i = 0;
	    int conf;
	    for(int c_ai = 0; c_ai < alphabet_length; c_ai++){
	    	conf = (raw_codes[d_i].symbol < 0)?char_to_uint(raw_codes[d_i].symbol):raw_codes[d_i].symbol;
           if(d_i < codes_length && c_ai == conf){
	    		int code_length = raw_codes[d_i].code_length;
				deflate_dictionary[c_ai] = code_d(0,code_length,c_ai);
				
				d_i++;
	    	}else{
	    		deflate_dictionary[c_ai] = code_d(0,0,c_ai);
	    	}
        }
	    
	    int* bl_count = new int[MAX_BITS];
	    //init array to all 0
	    for(int i = 0;i < MAX_BITS;i++){
	    	bl_count[i] = 0;
	    }
	    for(int i=0;i<alphabet_length;i++){
			bl_count[deflate_dictionary[i].code_length] ++;
		}
		
	    
	    int* next_code = new int[MAX_BITS];
	    bl_count[0] = 0;
	    int code = 0;
	    for(int bits = 1; bits < MAX_BITS;bits++){
	    	code = (code + bl_count[bits - 1]) << 1;
	    	next_code[bits] = code;
	    }
	    
    //END   
    
    //THEN I ASSIGN A NUMERICAL VALUE TO EVERY SYMBOL IN THE TREE
	    for(int c_ai = 0; c_ai < alphabet_length; c_ai++){
    		int code_length = deflate_dictionary[c_ai].code_length;
    		if(code_length > 0){
	    		int def_code = next_code[code_length];
			    next_code[code_length]++;
			    deflate_dictionary[c_ai].value = def_code;
			}
	    }
	delete [] bl_count;
	delete [] next_code;
	return deflate_dictionary;
    //END 
}

int* Huffman_Tree::get_code_length_codes(code_d* codes,bool literals,int* extra_bits,int* codes_array_length){
	if(codes == NULL){
		println("WARNING: USING EMPTY CODES ALPHABET");
		return NULL;
	}
	int alphabet_length = (literals)?DYNAMIC_LITERALS_ALPHABET_LENGTH:DYNAMIC_DISTANCES_ALPHABET_LENGTH;
	int* code_lengths_array = new int[alphabet_length];
	int cl_i = 0;
	int eb_i = 0;
	for(int a_i = 0; a_i < alphabet_length; cl_i++){
		int curr = codes[a_i].code_length;
		if(a_i != alphabet_length-1 && curr == codes[a_i + 1].code_length){
			int while_i = 0;
			int max_limit = (curr == 0)?138:7;
			int min_limit = (curr == 0)?3:4;
			while(a_i + while_i < alphabet_length && while_i < max_limit && curr == codes[a_i + while_i].code_length)
				while_i ++;
				
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
	*codes_array_length	 = cl_i;
	return code_lengths_array;	
}