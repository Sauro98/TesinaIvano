#include "key_manager.h"

Key_manager::Key_manager(){
	
}

Key_manager::Key_manager(ulong _key){
	key = _key;
	create_subkeys();
}

void Key_manager::create_subkeys(){
	uint left_key = Permutations::get_key_left_half_permutation(key);
	uint right_key = Permutations::get_key_right_half_permutation(key);
	for(int cycle = 0;cycle < 16;cycle++){
		left_key = get_new_key(left_key,cycle);
		right_key = get_new_key(right_key,cycle);
		ulong sub = Permutations::permutation_choice_2(left_key,right_key);
		subkeys[cycle] = sub;
	}
}

uint Key_manager::get_new_key(uint original,int cycle){
	uint new_key = 0;
	int shifts_count = key_shifts_table[cycle];
	uint exceeding_bits = 0;
	if(shifts_count == 1){
		exceeding_bits = Permutations::get_single_bit(original,28,1);
	}else{
		exceeding_bits = Permutations::get_single_bit(original,28,1) << 1;
		exceeding_bits |= Permutations::get_single_bit(original,28,2);
	}
	new_key = ((original << shifts_count) | exceeding_bits) & mask_28_bits;
	return new_key;
}

ulong Key_manager::get_subkey(int cycle){
	return subkeys[cycle];
}
