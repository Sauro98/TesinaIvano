#include<iostream>
#include "permutations.h"

ulong Permutations::initial_permutation(ulong original){
	int mask_length = 64;
	ulong permuted = 0;
	for(int position = 1;position < mask_length + 1;position++){
		int new_position = initial_permutation_table[position - 1];
		ulong new_value = get_single_bit(original,mask_length,new_position);
		set_single_bit(&permuted,mask_length,new_value,position);
	}
	return permuted;
}

ulong Permutations::final_permutation(ulong original){
	int mask_length = 64;
	ulong permuted = 0;
	for(int position = 1;position < mask_length + 1;position++){
		int new_position = final_permutation_table[position - 1];
		ulong new_value = get_single_bit(original,mask_length,new_position);
		set_single_bit(&permuted,mask_length,new_value,position);
	}
	return permuted;
}

uint Permutations::fiestel_permutation(uint original){
	int mask_length = 32;
	uint permuted = 0;
	for(int position = 1;position < mask_length +1;position++){
		int new_position = feistel_permutation_table[position - 1];
		ulong new_value = get_single_bit(original,mask_length,new_position);
		ulong permuted_long = permuted;
		set_single_bit(&permuted_long,mask_length,new_value,position);
		permuted = (uint) permuted_long;
	}
	return permuted;
}

ulong Permutations::expansion_permutation(uint original){
	int old_mask_length = 32;
	int new_mask_length = 48;
	ulong permuted = 0;
	for(int position = 1;position < new_mask_length + 1;position++){
		int new_position = expansion_table[position - 1];
		ulong new_value = get_single_bit(original,old_mask_length,new_position);
		set_single_bit(&permuted,new_mask_length,new_value,position);
	}
	return permuted;
}

uint Permutations::get_key_left_half_permutation(ulong original){
	int old_mask_length = 64;
	int new_mask_length = 28;
	uint permuted = 0;
	for(int position = 1;position < new_mask_length +1;position++){
		int new_position = key_left_half_table[position - 1];
		ulong new_value = get_single_bit(original,old_mask_length,new_position);
		ulong permuted_long = permuted;
		set_single_bit(&permuted_long,new_mask_length,new_value,position);
		permuted = (uint) permuted_long;
	}
	return permuted;
}

uint Permutations::get_key_right_half_permutation(ulong original){
	int old_mask_length = 64;
	int new_mask_length = 28;
	uint permuted = 0;
	for(int position = 1;position < new_mask_length +1;position++){
		int new_position = key_right_half_table[position - 1];
		ulong new_value = get_single_bit(original,old_mask_length,new_position);
		ulong permuted_long = permuted;
		set_single_bit(&permuted_long,new_mask_length,new_value,position);
		permuted = (uint) permuted_long;
	}
	return permuted;
}

ulong Permutations::permutation_choice_2(uint left_key,uint right_key){
	int old_mask_length = 56;
	int new_mask_length = 48;
	
	ulong key_fusion = left_key;
	key_fusion <<= 28;
	key_fusion &= mask_56_bits;
	key_fusion |= right_key;
	
	ulong permuted =0;
	
	for(int position = 1;position < new_mask_length + 1;position++){
		int new_position = permuted_choice_2_table[position - 1];
		ulong new_value = get_single_bit(key_fusion,old_mask_length,new_position);
		set_single_bit(&permuted,new_mask_length,new_value,position);
	}
	
	return permuted;
}



void Permutations::set_single_bit(ulong* original,int mask_length,ulong value,int position){
	//se voglio settare il terzo bit da sinistra di un intero a 64 bit allora sposto il bit col valore da inserire di 63 - 2 = 61 in modo che si trovi al terzo posto da sinistra e poi faccio l'OR
	//NOTA: si presuppone che il numero di partenza sia settato con tutti i bit a 0, questa funzione non setta a 0 i bit che sono a 1 
	(*original) |= (value << ((mask_length - 1) - (position - 1)));
}

ulong Permutations::get_single_bit(ulong original,int mask_length,int position){
	ulong mask;
	switch(mask_length){
		case 6:
			mask = (ulong) mask_6_bits;
		case 8:
			mask = (ulong) mask_8_bits;
			break;
		case 16:
			mask = (ulong) mask_16_bits;
			break;
		case 28:
			mask = (ulong) mask_28_bits;
			break;
		case 32:
			mask = (ulong) mask_32_bits;
			break;
		case 48:
			mask = (ulong) mask_48_bits;
			break;
		case 56:
			mask = (ulong) mask_56_bits;
			break;
		case 64:
			mask = (ulong) mask_64_bits;
			break;
		default:
			mask = (ulong) mask_64_bits;
			break;
	}
	//se voglio il terzo bit da sinistra sposto i bit a sinistra di 2 (position - 1)
	ulong result = original << (position - 1);
	//nel caso fosse di lunghezza diversa da 64 metto a 0 i bit più a sinistra che non mi interessano
	result &= mask;
	//ora che ho il bit interessato a sinistra e tutti gli eventuali bit precedenti a 0 lo sposto per ultimo a destra, in modo da avere 0 o 1 come risultato
	result >>= mask_length - 1;
	return result;
}
