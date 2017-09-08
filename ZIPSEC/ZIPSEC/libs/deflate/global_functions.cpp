#ifndef GLOBAL_C
#define GLOBAL_C

#include "constants.h"

static int get_static_literal_length(int literal);
static int* construct_static_literal_and_lengths_tree();
static int get_static_length_extra_bits(int length,int* extra_bits);
static int get_static_distance_extra_bits(int distance,int* extra_bits);
static int* generate_codes_from_lengths(int* lengths, int size,bool hclen);
static int char_to_uint(char c);


static int char_to_uint(char c){
	if((int) c < 0)
		return (uint8_t)c;
	else
		return (int)c;
}

static int get_static_literal_length(int literal){
	if(literal >= 0 && literal <= 143)
		return 8;
	else if (literal >= 144 && literal <= 255)
		return 9;
	else if (literal >= 256 && literal <= 279)
		return 7;
	else if (literal >= 280 && literal <= 287)
		return 8;
	else 
		return 0;
}



static int* construct_static_literal_and_lengths_tree(){
	#if DEBUG > 1
		println("\tSTART POPULATION OF LENGTHS ARRAY");
	#endif
	int lengths[288];
	for(int i = 0;i<=287;i++){
		lengths[i] = get_static_literal_length(i);
	}
	#if DEBUG > 1
		//println("\tEND");
	#endif
	#if DEBUG > 1
		println("\tSTART POPULATION OF BL_COUNT ARRAY");
	#endif
	int* bl_count = new int[MAX_BITS_STATIC];
	for(int i = 0;i < MAX_BITS_STATIC;i++){
		bl_count[i] = 0;
	}
	for(int i = 0;i<= 287;i++){
		bl_count[lengths[i]] ++;
	}
	/*for(int a = 0; a < MAX_BITS_STATIC; a++){
		println("BL_COUNT["<<a<<"]: "<<bl_count[a]);
	}*/
	#if DEBUG > 1
		//println("\tEND");
	#endif
	int code = 0;
    bl_count[0] = 0;
    int* next_code = new int[MAX_BITS_STATIC];
	next_code[0] = 0;
    #if DEBUG > 1
		println("\tSTART GENERATION OF SMALLEST CODES FOR LENGTHS");
	#endif
    for (int bits = 1; bits < MAX_BITS_STATIC; bits++) {
    	code = (code + bl_count[bits-1]) << 1;
    	next_code[bits] = code;
    }
    
    //IT SOMEHOW TURNS TO 1024 AT THIS POINT I HAVE NO CLUE WHY
    lengths[0] = 8;
    
    #if DEBUG > 1
		//println("\tEND");
	#endif
    //TODO: loop through all values and find the code for them. return that array of length 288
    #if DEBUG > 1
		println("\tSTART POPULATION OF CODES ARRAY");
	#endif
    int* codes = new int[288];
    for(int c_i=0;c_i <= 287;c_i++){
    	int length = lengths[c_i];
		int code = next_code[length];
    	codes[c_i] = code;
    	next_code[length]++;
    }
    #if DEBUG > 1
		//println("\tEND");
	#endif

	delete[] next_code;
	delete[] bl_count;

    return codes;
}

static int* generate_codes_from_lengths(int* lengths, int size,bool hclen){
    int MAX_BITS = 0;
    for(int a = 0; a < size;a++){
        if(lengths[a] > MAX_BITS)
            MAX_BITS = lengths[a];
    }
    MAX_BITS++;
    //println("MAX BITS "<<MAX_BITS);
	#if DEBUG > 1
		println("\tSTART POPULATION OF BL_COUNT ARRAY");
	#endif
	int* bl_count = new int[MAX_BITS];
	for(int i = 0;i < MAX_BITS;i++){
		bl_count[i] = 0;
	}
	for(int i = 0;i < size;i++){
		bl_count[lengths[i]]++;
	}
	
    bl_count[0] = 0;
	/*print("BL CONTENT[");
	for(int i = 0;i < MAX_BITS;i++){
		print(" "<<bl_count[i]);
	}
	println("]");*/
	#if DEBUG > 1
		//println("\tEND");
	#endif
	int code = 0;
    int* next_code = new int[MAX_BITS];
    next_code[0] = 0;
    #if DEBUG > 1
		println("\tSTART GENERATION OF SMALLEST CODES FOR LENGTHS");
	#endif
    for (int bits = 1; bits < MAX_BITS; bits++) {
    	code = (code + bl_count[bits-1]) << 1;
    	next_code[bits] = code;
    }
    
    /*print("NEXT CODE [");
	for(int i = 0;i < MAX_BITS;i++){
		print(" "<<next_code[i]);
	}
	println("]");*/
    
    #if DEBUG > 1
		//println("\tEND");
	#endif
    //TODO: loop through all values and find the code for them. return that array of length 288
    #if DEBUG > 1
		println("\tSTART POPULATION OF CODES ARRAY");
	#endif
    int* codes = new int[size];
    if(hclen){
    	int counter = 0;
	    int hclr_i = 0;
	    //hclen_reference_ord contiene gli indici su hcl_references in ordine crescente 0...18 in modo da poter assegnare i codici in ordine
	    while(counter < size){
	    	int index = hclen_reference_ord[hclr_i];
	    	hclr_i++;
	    	if(index < size){
		    	int length = lengths[index];
				//println("length "<<length);
		    	codes[index] = next_code[length];
				//println("code "<<codes[index]);
		    	if(length != 0)
		    		next_code[length]++;
		    	counter++;
	    	}
	    }
    }else{
    	//print("CONTENT [");
    	for(int c_i = 0; c_i < size; c_i++){
    		int length = lengths[c_i];
    		codes[c_i] = next_code[length];
    		if(length > 0 && length < MAX_BITS){
    			next_code[length]++;
    			//println(" "<<codes[c_i]);
    		}
    	}
    	//println(" ]");
    }
	delete[] bl_count;
	delete[] next_code;
    #if DEBUG > 1
		//println("\tEND");
	#endif
    return codes;
}

static int get_static_length_extra_bits(int length,int* extra_bits){
	if(length == 3){
		*extra_bits = 0;
		return 0;
	}
	if(length == 4){
		*extra_bits = 0;
		return 0;
	}
	if(length == 5){
		*extra_bits = 0;
		return 0;
	}
	if(length == 6){
		*extra_bits = 0;
		return 0;
	}
	if(length == 7){
		*extra_bits = 0;
		return 0;
	}
	if(length == 8){
		*extra_bits = 0;
		return 0;
	}
	if(length == 9){
		*extra_bits = 0;
		return 0;
	}
	if(length == 10){
		*extra_bits = 0;
		return 0;
	}
	if(length >= 11 && length <= 12){
		*extra_bits = 1;
		return length - 11;
	}
	if(length >= 13 && length <= 14){
		*extra_bits = 1;
		return length - 13;
	}
	if(length >= 15 && length <= 16){
		*extra_bits = 1;
		return length - 15;
	}
	if(length >= 17 && length <= 18){
		*extra_bits = 1;
		return length - 17;
	}
	if(length >= 19 && length <= 22){
		*extra_bits = 2;
		return length - 19;
	}
	if(length >= 23 && length <= 26){
		*extra_bits = 2;
		return length - 23;
	}
	if(length >= 27 && length <= 30){
		*extra_bits = 2;
		return length - 27;
	}
	if(length >= 31 && length <= 34){
		*extra_bits = 2;
		return length - 31;
	}
	if(length >= 35 && length <= 42){
		*extra_bits = 3;
		return length - 35;
	}
	if(length >= 43 && length <= 50){
		*extra_bits = 3;
		return length - 43;
	}
	if(length >= 51 && length <= 58){
		*extra_bits = 3;
		return length - 51;
	}
	if(length >= 59 && length <= 66){
		*extra_bits = 3;
		return length - 59;
	}
	if(length >= 67 && length <= 82){
		*extra_bits = 4;
		return length - 67;
	}
	if(length >= 83 && length <= 98){
		*extra_bits = 4;
		return length - 83;
	}
	if(length >= 99 && length <= 114){
		*extra_bits = 4;
		return length - 99;
	}
	if(length >= 115 && length <= 130){
		*extra_bits = 4;
		return length - 115;
	}
	if(length >= 131 && length <= 162){
		*extra_bits = 5;
		return length - 131;
	}
	if(length >= 163 && length <= 194){
		*extra_bits = 5;
		return length - 163;
	}
	if(length >= 195 && length <= 226){
		*extra_bits = 5;
		return length - 195;
	}
	if(length >= 227 && length <= 257){
		*extra_bits = 5;
		return length - 227;
	}
	if(length == 258){
		*extra_bits = 0;
		return 0;
	}
	println("Invalid legnth " << length);
	return -1;
}

static int get_static_distance_extra_bits(int distance,int* extra_bits){
	if(distance == 1){
		*extra_bits = 0;
		return 0;
	}
	if(distance == 2){
		*extra_bits = 0;
		return 0;
	}
	if(distance == 3){
		*extra_bits = 0;
		return 0;
	}
	if(distance == 4){
		*extra_bits = 0;
		return 0;
	}
	if(distance >= 5 && distance <= 6){
		*extra_bits = 1;
		return distance - 5;
	}
	if(distance >= 7 && distance <= 8){
		*extra_bits = 1;
		return distance - 7;
	}
	if(distance >= 9 && distance <= 12){
		*extra_bits = 2;
		return distance - 9;
	}
	if(distance >= 13 && distance <= 16){
		*extra_bits = 2;
		return distance - 13;
	}
	if(distance >= 17 && distance <= 24){
		*extra_bits = 3;
		return distance - 17;
	}
	if(distance >= 25 && distance <= 32){
		*extra_bits = 3;
		return distance - 25;
	}
	if(distance >= 33 && distance <= 48){
		*extra_bits = 4;
		return distance - 33;
	}
	if(distance >= 49 && distance <= 64){
		*extra_bits = 4;
		return distance - 49;
	}
	if(distance >= 65 && distance <= 96){
		*extra_bits = 5;
		return distance - 65;
	}
	if(distance >= 97 && distance <= 128){
		*extra_bits = 5;
		return distance - 97;
	}
	if(distance >= 129 && distance <= 192){
		*extra_bits = 6;
		return distance - 129;
	}
	if(distance >= 193 && distance <= 256){
		*extra_bits = 6;
		return distance - 193;
	}
	if(distance >= 257 && distance <= 384){
		*extra_bits = 7;
		return distance - 257;
	}
	if(distance >= 385 && distance <= 512){
		*extra_bits = 7;
		return distance - 385;
	}
	if(distance >= 513 && distance <= 768){
		*extra_bits = 8;
		return distance - 513;
	}
	if(distance >= 769 && distance <= 1024){
		*extra_bits = 8;
		return distance - 769;
	}
	if(distance >= 1025 && distance <= 1536){
		*extra_bits = 9;
		return distance - 1025;
	}
	if(distance >= 1537 && distance <= 2048){
		*extra_bits = 9;
		return distance - 1537;
	}
	if(distance >= 2049 && distance <= 3072){
		*extra_bits = 10;
		return distance - 2049;
	}
	if(distance >= 3073 && distance <= 4096){
		*extra_bits = 10;
		return distance - 3073;
	}
	if(distance >= 4097 && distance <= 6144){
		*extra_bits = 11;
		return distance - 4097;
	}
	if(distance >= 6145 && distance <= 8192){
		*extra_bits = 11;
		return distance - 6145;
	}
	if(distance >= 8193 && distance <= 12288){
		*extra_bits = 12;
		return distance - 8193;
	}
	if(distance >= 12289 && distance <= 16384){
		*extra_bits = 12;
		return distance - 12289;
	}
	if(distance >= 16385 && distance <= 24576){
		*extra_bits = 13;
		return distance - 16385;
	}
	if(distance >= 24577 && distance <= 32768){
		*extra_bits = 13;
		return distance - 24577;
	}
	println("Invalid distance "<<distance);
	return -1;
}

#endif
