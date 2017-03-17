#ifndef GLOBAL_C
#define GLOBAL_C

#include "constants.h"

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
    
    for(int a = 1; a < MAX_BITS; a++){
		println("NEXT_CODE["<<a<<"]: "<<next_code[a]);
	}
    
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

#endif
