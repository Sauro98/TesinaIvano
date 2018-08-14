#include "key_manager.h"

Key_manager::Key_manager(){
	
}

Key_manager::Key_manager(ulong _key){
	key = _key;
	create_subkeys();
}

void Key_manager::create_subkeys(){
	uint left_key = 0;
	get_key_left_half_permutation(key, left_key);
	uint right_key = 0;
	get_key_right_half_permutation(key, right_key);
	for(uchar cycle = 0;cycle < 16;cycle++){
		left_key = get_new_key(left_key,cycle);
		right_key = get_new_key(right_key,cycle);
        ulong key_fusion = (((ulong)left_key << (uchar)28)) | right_key;
		ulong k = 0;
        permutation_choice_2(key_fusion, k);
        subkeys[cycle] = k;
    }
}

ulong Key_manager::get_subkey(int cycle){
	return subkeys[cycle];
}
