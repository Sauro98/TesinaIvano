#ifndef GLOBAL_C
#define GLOBAL_C

#include "constants.h"

static long get_mask_for_length(int length);
static int get_static_literal_length(int literal);
static void print_binary_literal(unsigned int literal,int length);
static long* construct_static_literal_and_lengths_tree();
static int get_static_length_extra_bits(long length,int* extra_bits);

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

static void print_binary_literal(unsigned int literal,int length){
	int mask = get_mask_for_length(length);
	for(int a = 0;a < length;a++){
		int current = ((literal <<  a) & mask) >> (length - 1);
		print(current);
	}
	println("");
}

static long* construct_static_literal_and_lengths_tree(){
	#if DEBUG > 1
		println("\tSTART POPULATION OF LENGTHS ARRAY");
	#endif
	int lengths[288];
	for(int i = 0;i<=287;i++){
		lengths[i] = get_static_literal_length(i);
	}
	#if DEBUG > 1
		println("\tEND");
	#endif
	#if DEBUG > 1
		println("\tSTART POPULATION OF BL_COUNT ARRAY");
	#endif
	int bl_count[MAX_BITS];
	for(int i = 0;i < MAX_BITS;i++){
		bl_count[i] = 0;
	}
	for(int i = 0;i<= 287;i++){
		bl_count[lengths[i]] ++;
	}
	/*for(int a = 0; a < MAX_BITS; a++){
		println("BL_COUNT["<<a<<"]: "<<bl_count[a]);
	}*/
	#if DEBUG > 1
		println("\tEND");
	#endif
	long code = 0;
    bl_count[0] = 0;
    long next_code[MAX_BITS];
    #if DEBUG > 1
		println("\tSTART GENERATION OF SMALLEST CODES FOR LENGTHS");
	#endif
    for (int bits = 1; bits <= MAX_BITS; bits++) {
    	code = (code + bl_count[bits-1]) << 1;
    	next_code[bits] = code;
    }
    
    /*for(int a = 1; a < MAX_BITS; a++){
		println("NEXT_CODE["<<a<<"]: "<<next_code[a]);
	}*/
    
    //IT SOMEHOW TURNS TO 1024 AT THIS POINT I HAVE NO CLUE WHY
    lengths[0] = 8;
    
    #if DEBUG > 1
		println("\tEND");
	#endif
    //TODO: loop through all values and find the code for them. return that array of length 288
    #if DEBUG > 1
		println("\tSTART POPULATION OF CODES ARRAY");
	#endif
    long* codes = new long[288];
    for(int c_i=0;c_i <= 287;c_i++){
    	int length = lengths[c_i];
    	codes[c_i] = next_code[length];
    	next_code[length]++;
    	if(c_i == 101 || c_i == 277 || c_i == 68){
    	print("CHAR: "<<c_i<<" CODE ");
    	print_binary_literal((unsigned int)codes[c_i],(unsigned int)lengths[c_i]);
        }
    }
    #if DEBUG > 1
		println("\tEND");
	#endif
    return codes;
}



static long get_mask_for_length(int length){
	switch(length){
		case 1:
			return MASK_1_BIT;
		case 2:
			return MASK_2_BITS;
		case 3:
			return MASK_3_BITS;
		case 4:
			return MASK_4_BITS;
		case 5:
			return MASK_5_BITS;
		case 6:	
			return MASK_6_BITS;	
		case 7:
			return MASK_7_BITS;
		case 8:
			return MASK_8_BITS;;
		case 9:
			return MASK_9_BITS;
	}
	return 0;
}

static int get_static_length_extra_bits(long length,int* extra_bits){
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
}

#endif
