#include "lz77_encoder.h" // class's header file

// class constructor
LZ77_Encoder::LZ77_Encoder(std::string _original)
{
	original = _original;
	lh_index = 0;
	original_length = original.length();
	println("original length: "<<original_length);
}

// class destructor
LZ77_Encoder::~LZ77_Encoder()
{
	// insert your code here
}


void LZ77_Encoder::reset(){
	lh_index = 0;
}


//TODO new implementation with iterations

bool LZ77_Encoder::hasMore(){
//	println("More, lh_index: "<<lh_index<<" original_length: "<<original_length);
	return lh_index < original_length;
}

long* LZ77_Encoder::getNext(int* result_length){
		long j = 0;
		long l = 0;
		long o = 0;
		//start from the last element in the search buffer
	    long s_i = lh_index-1;
	    //look back inside the search buffer, while of course being inside the string, until a prefix is found or the end of the buffer is reached
	    //every time the first charachter of the look ahead buffer is found in the search buffer the rest of the buffer is looked forward until the next charachter in the search buffer is different from the 
	    //next charachter in the look ahead buffer. If the length of the newly found prefix is greater or equal to the length of the previously found prefixes, then the index of the current prefix is saved in j
	    while(s_i > -1 && s_i >= (lh_index - SEARCH_BUFFER_LENGTH)){
	        //printintln("\tS_I CYCLE ",s_i);
		        if(original.at(s_i) == original.at(lh_index)){
		            //current prefix length
		            long c_l = 1;
		            long c_j = s_i + 1;
		            
		            while(c_j > -1 && ((lh_index + c_l) != original_length) && original.at(c_j) == original.at(lh_index + c_l)){
			            // print("\t\t");
			            //printint(original.at(c_j)," == ");
			            //println(original.at(i+c_l));
			            c_j++;
			            c_l++;
		            }
		            	
		            if(c_l >= 3 && c_l > l){
		            	l = c_l;
		            	j = s_i;
		            	o = lh_index - j;
		            }
		            	
		        }	
	        s_i--;
		
	    }
	    if(l == 0 && o == 0){
	    	long* to_return = new long[1];
	    	to_return[0] = original.at(lh_index);
	    	*result_length = 1;
	//   	println("lh_index + "<<l<<" + "<<1);
	    	lh_index++;
	//    	println("lh_index: "<<lh_index);
			return to_return; 
	    }else{
	    	long* to_return = new long[2];
	    	to_return[0] = l;
	    	to_return[1] = o;
	    	*result_length = 2;
	//    	println("-lh_index + "<<l<<" + "<<1);
	    	lh_index = lh_index + l;
	//    	println("-lh_index: "<<lh_index);
	    	return to_return;
	    }
}

