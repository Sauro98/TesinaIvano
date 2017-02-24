#include "lz77_encoder.h" // class's header file

// class constructor
LZ77_Encoder::LZ77_Encoder()
{
	// insert your code here
}

// class destructor
LZ77_Encoder::~LZ77_Encoder()
{
	// insert your code here
}

lz77_r* LZ77_Encoder::encode(std::string original,int* encoded_length){
    int original_length = original.length();
    printintln("O_L: ",original_length);
	lz77_r* encoded_raw = new lz77_r[original_length];
	int raw_index = 0;
    //index of the first element in the look ahead buffer
    int i = 0;
    //index of the last element in the prefix
    int j = 0;
    //while the first element in the look ahead buffer is not the last element in the string
    while(i < original_length ){
        printintln("I CYCLE ",i);
        int l = 0;
        int o = 0;
        //start from the last element in the search buffer
        int s_i = i-1;
        bool prefix_found = false;
        //look back inside the search buffer, while of course being inside the string, until a prefix is found or the end of the buffer is reached
        //every time the first charachter of the look ahead buffer is found in the search buffer the rest of the buffer is looked forward until the next charachter in the search buffer is different from the 
        //next charachter in the look ahead buffer. If the length of the newly found prefix is greater or equal to the length of the previously found prefixes, then the index of the current prefix is saved in j
        while(s_i > -1 && s_i >= i - SEARCH_BUFFER_LENGTH && i != original_length - 1){
            //printintln("\tS_I CYCLE ",s_i);
            if(original.at(s_i) == original.at(i)){
            	//current prefix length
            	int c_l = 1;
            	int c_j = s_i + 1;
            	
            	while(c_j > -1 && ((i + c_l) != (original_length - 1)) && original.at(c_j) == original.at(i + c_l)){
	                print("\t\t");
	                printint(original.at(c_j)," == ");
	                println(original.at(i+c_l));
	                c_j++;
	                c_l++;
            	}
            	
            	if(c_l >= l ){
            		l = c_l;
            		j = s_i;
            		o = i - j;
            	}
            	
            }
            s_i--;
        }
        printintln("AT: ", i + l);
        printintln("l: ",l);
        lz77_r reference(o,l,original.at(i + l));
        printint("\tTupla( " , o);
        printint(",",l);
        printint(",",original.at(i + l));
        println(")");
        i = i + l + 1;
        encoded_raw[raw_index] = reference;
        raw_index++;
    }
    lz77_r* encoded = new lz77_r[raw_index];
    for(int n_i = 0;n_i < raw_index;n_i++){
    	encoded[n_i] = encoded_raw[n_i];
    }
    *encoded_length = raw_index;
    return encoded;
}

std::string LZ77_Encoder::decode(lz77_r* encoded, int length){
	std::string decoded = "";
	for(int index = 0;index < length;index++){
		int t_length = encoded[index].l;
		int t_origin = decoded.length() - encoded[index].o;
		char next = encoded[index].a;
		for(int l_index = 0;l_index < t_length;l_index++){
			decoded += decoded.at(t_origin + l_index);
		}
		decoded += next;
	}
	println("DECODED: " + decoded);
}
