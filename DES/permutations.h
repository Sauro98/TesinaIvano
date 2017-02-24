#ifndef PERMUTATIONS_H
#define PERMUTATIONS_H

#include <stdint.h>
#include <iostream>

//definizione di nomi accorciati per gli interi a 8, 16, 32, 64 bit senza bit di segno
#define ulong uint64_t
#define uint uint32_t
#define ushort uint16_t
#define uchar uint8_t

//definizione maschere per operazioni bit a bit
#define mask_64_bits 0xFFFFFFFFFFFFFFFFULL
#define mask_56_bits 0xFFFFFFFFFFFFFFFULL
#define mask_48_bits 0xFFFFFFFFFFFFULL
#define mask_32_bits 0xFFFFFFFF
#define mask_28_bits 0xFFFFFFF
#define mask_16_bits 0xFFFF
#define mask_8_bits  0xFF
#define mask_6_bits  0x3F

//definizione macro 

#define print(x) std::cout<<x
#define println(x) std::cout<<x<<std::endl

//definizione delle tabelle di permutazione
		static const int initial_permutation_table[] = {58,50,42,34,26,18,10,2
		            							 ,60,52,44,36,28,20,12,4 
		            							 ,62,54,46,38,30,22,14,6 
		            							 ,64,56,48,40,32,24,16,8 
		            							 ,57,49,41,33,25,17,9 ,1 
		            							 ,59,51,43,35,27,19,11,3 
		            							 ,61,53,45,37,29,21,13,5 
		            							 ,63,55,47,39,31,23,15,7};
		            				
		static const int final_permutation_table[] = {40,8 ,48,16,56,24,64,32,
		            			   				39,7 ,47,15,55,23,63,31,
		            			   				38,6 ,46,14,54,22,62,30,
		            			   				37,5 ,45,13,53,21,61,29,
		            	 		   				36,4 ,44,12,52,20,60,28,
		            			   				35,3 ,43,11,51,19,59,27,
		            			   				34,2 ,42,10,50,18,58,26,
		            			   				33,1 ,41,9 ,49,17,57,25 };
		
		static const int expansion_table[] = {32,1 ,2 ,3 ,4 ,5 ,
		            	   				4 ,5 ,6 ,7 ,8 ,9 ,
		            	   				8 ,9 ,10,11,12,13,
		            	   				12,13,14,15,16,17,
		            	   				16,17,18,19,20,21,
		            	   				20,21,22,23,24,25,
		            	   				24,25,26,27,28,29,
		            	   				28,29,30,31,32,1};
		
		static const int feistel_permutation_table[] = {16,7 ,20,21,
		            				 			  29,12,28,17,
		            				 	 		  1 ,15,23,26,
		            				 			  5 ,18,31,10,
		            				 			  2 ,8 ,24,14,
		            				 			  32,27,3 ,9 ,
		            				 			  19,13,30, 6,
		            				 			  22,11,4 ,25};
		
		static const int key_left_half_table[] = {57,49,41,33,25,17,9 ,
		            		   				1 ,58,50,42,34,26,18,
		            		   				10,2 ,59,51,43,35,27,
		            		   				19,11,3 ,60,52,44,36};
		            		   
		static const int key_right_half_table[] = {63,55,47,39,31,23,15,
		            					     7 ,62,54,46,38,30,22,
		            						 14,6 ,61,53,45,37,29,
		            						 21,13,5 ,28,20,12,4};
		
		static const int permuted_choice_2_table[] = {14,17,11,24,1 ,5 ,
		            			   				3 ,28,15,6 ,21,10,
		            			   				23,19,12,4 ,26,8 ,
		            			   				16,7 ,27,20,13,2 ,
		            			   				41,52,31,37,47,55,
		            			   				30,40,51,45,33,48,
		            			   				44,49,39,56,34,53,
		            			   				46,42,50,36,29,32};
		
		static const int key_shifts_table[] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

class Permutations
{
	
	public:
		//funzioni per effettuare le varie permutazioni
		static ulong initial_permutation(ulong original);
		static ulong final_permutation(ulong original);
		static uint fiestel_permutation(uint original);
		static ulong expansion_permutation(uint original);
		static uint get_key_left_half_permutation(ulong original);
		static uint get_key_right_half_permutation(ulong original);
		static ulong permutation_choice_2(uint left_key,uint right_key);
		
		//operazioni bit a bit
		static void set_single_bit(ulong* original,int mask_length,ulong value,int position);
		static ulong get_single_bit(ulong original,int mask_length,int position);

};

#endif
