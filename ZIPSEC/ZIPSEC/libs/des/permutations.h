#ifndef PERMUTATIONS_H
#define PERMUTATIONS_H

#include <cstdint>
#include <iostream>
#include "constants.h"

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
#define mask_6_bits  0x3FU
#define exp_starting_mask 0x3F0000000

#define get_single_bit(original, mask_length, position) (((ulong)(original) >> (uchar)((mask_length) - (position))) & ((uchar)1))

//se voglio settare il terzo bit da sinistra di un intero a 64 bit allora sposto il bit col valore da inserire di 63 - 2 = 61 in modo che si trovi al terzo posto da sinistra e poi faccio l'OR
//NOTA: si presuppone che il numero di partenza sia settato con tutti i bit a 0, questa funzione non setta a 0 i bit che sono a 1
#define set_single_bit(original, mask_length, value, position) ((original)) |= (value << (((mask_length) - 1) - ((position) - 1)))


#define setbit32(from, originalP, newP, to) to |= (( ( from & ((ulong) 1 << (32 - originalP) ) ) << (originalP - 1) ) >> (newP - 1))
#define setbit64(from, originalP, newP, to) to |= (( ( from & ((ulong) 1 << (64 - originalP) ) ) << (originalP - 1) ) >> (newP - 1))

//#define setbit32s(from, originalP, newP, to) to |= ( ( (from >> (32 - originalP)) & 1 ) <<  (32 - newP))

//definizione delle tabelle di permutazione
static const uchar key_left_half_table[] = {57, 49, 41, 33, 25, 17, 9,
                                            1, 58, 50, 42, 34, 26, 18,
                                            10, 2, 59, 51, 43, 35, 27,
                                            19, 11, 3, 60, 52, 44, 36};

static const uchar key_right_half_table[] = {63, 55, 47, 39, 31, 23, 15,
                                             7, 62, 54, 46, 38, 30, 22,
                                             14, 6, 61, 53, 45, 37, 29,
                                             21, 13, 5, 28, 20, 12, 4};

static const uchar permuted_choice_2_table[] = {14, 17, 11, 24, 1, 5,
                                                3, 28, 15, 6, 21, 10,
                                                23, 19, 12, 4, 26, 8,
                                                16, 7, 27, 20, 13, 2,
                                                41, 52, 31, 37, 47, 55,
                                                30, 40, 51, 45, 33, 48,
                                                44, 49, 39, 56, 34, 53,
                                                46, 42, 50, 36, 29, 32};

static const uchar key_shifts_table[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

#define expansion_permutation(original, modified)                             \
        original |= ((original) & 1) <<32;                                    \
        (original) <<= 1;                                                     \
        (original) |= (((original) & ((ulong)1 << 32)) >> 32);                \
        (modified) |= (((original) & exp_starting_mask) >> 28) << 42;         \
        (modified) |= (((original) & (exp_starting_mask >> 4  )) >> 24) <<36; \
        (modified) |= (((original) & (exp_starting_mask >> 8  )) >> 20) <<30; \
        (modified) |= (((original) & (exp_starting_mask >> 12 )) >> 16) <<24; \
        (modified) |= (((original) & (exp_starting_mask >> 16 )) >> 12) <<18; \
        (modified) |= (((original) & (exp_starting_mask >> 20 )) >>  8) <<12; \
        (modified) |= (((original) & (exp_starting_mask >> 24 )) >>  4) <<6;  \
        (modified) |= (((original) & (exp_starting_mask >> 28 )));

#define fiestel_permutation(original, modified)\
    setbit32(original,16,1,modified);\
    setbit32(original,7 ,2,modified);\
    setbit32(original,20,3,modified);\
    setbit32(original,21,4,modified);\
    setbit32(original,29,5,modified);\
    setbit32(original,12,6,modified);\
    setbit32(original,28,7,modified);\
    setbit32(original,17,8,modified);\
    setbit32(original,1 ,9,modified);\
    setbit32(original,15,10,modified);\
    setbit32(original,23,11,modified);\
    setbit32(original,26,12,modified);\
    setbit32(original,5 ,13,modified);\
    setbit32(original,18,14,modified);\
    setbit32(original,31,15,modified);\
    setbit32(original,10,16,modified);\
    setbit32(original,2 ,17,modified);\
    setbit32(original,8 ,18,modified);\
    setbit32(original,24,19,modified);\
    setbit32(original,14,20,modified);\
    setbit32(original,32,21,modified);\
    setbit32(original,27,22,modified);\
    setbit32(original,3 ,23,modified);\
    setbit32(original,9 ,24,modified);\
    setbit32(original,19,25,modified);\
    setbit32(original,13,26,modified);\
    setbit32(original,30,27,modified);\
    setbit32(original,6 ,28,modified);\
    setbit32(original,22,29,modified);\
    setbit32(original,11,30,modified);\
    setbit32(original,4 ,31,modified);\
    setbit32(original,25,32,modified);


#define initial_permutation(original, modified)\
    setbit64(original, 58, 1, modified);\
    setbit64(original, 50, 2, modified);\
    setbit64(original, 42, 3, modified);\
    setbit64(original, 34, 4, modified);\
    setbit64(original, 26, 5, modified);\
    setbit64(original, 18, 6, modified);\
    setbit64(original, 10, 7, modified);\
    setbit64(original, 2, 8, modified);\
    setbit64(original, 60, 9, modified);\
    setbit64(original, 52, 10, modified);\
    setbit64(original, 44, 11, modified);\
    setbit64(original, 36, 12, modified);\
    setbit64(original, 28, 13, modified);\
    setbit64(original, 20, 14, modified);\
    setbit64(original, 12, 15, modified);\
    setbit64(original, 4, 16, modified);\
    setbit64(original, 62, 17, modified);\
    setbit64(original, 54, 18, modified);\
    setbit64(original, 46, 19, modified);\
    setbit64(original, 38, 20, modified);\
    setbit64(original, 30, 21, modified);\
    setbit64(original, 22, 22, modified);\
    setbit64(original, 14, 23, modified);\
    setbit64(original, 6, 24, modified);\
    setbit64(original, 64, 25, modified);\
    setbit64(original, 56, 26, modified);\
    setbit64(original, 48, 27, modified);\
    setbit64(original, 40, 28, modified);\
    setbit64(original, 32, 29, modified);\
    setbit64(original, 24, 30, modified);\
    setbit64(original, 16, 31, modified);\
    setbit64(original, 8, 32, modified);\
    setbit64(original, 57, 33, modified);\
    setbit64(original, 49, 34, modified);\
    setbit64(original, 41, 35, modified);\
    setbit64(original, 33, 36, modified);\
    setbit64(original, 25, 37, modified);\
    setbit64(original, 17, 38, modified);\
    setbit64(original, 9, 39, modified);\
    setbit64(original, 1, 40, modified);\
    setbit64(original, 59, 41, modified);\
    setbit64(original, 51, 42, modified);\
    setbit64(original, 43, 43, modified);\
    setbit64(original, 35, 44, modified);\
    setbit64(original, 27, 45, modified);\
    setbit64(original, 19, 46, modified);\
    setbit64(original, 11, 47, modified);\
    setbit64(original, 3, 48, modified);\
    setbit64(original, 61, 49, modified);\
    setbit64(original, 53, 50, modified);\
    setbit64(original, 45, 51, modified);\
    setbit64(original, 37, 52, modified);\
    setbit64(original, 29, 53, modified);\
    setbit64(original, 21, 54, modified);\
    setbit64(original, 13, 55, modified);\
    setbit64(original, 5, 56, modified);\
    setbit64(original, 63, 57, modified);\
    setbit64(original, 55, 58, modified);\
    setbit64(original, 47, 59, modified);\
    setbit64(original, 39, 60, modified);\
    setbit64(original, 31, 61, modified);\
    setbit64(original, 23, 62, modified);\
    setbit64(original, 15, 63, modified);\
    setbit64(original, 7, 64, modified);


//set_single_bit(modified,64, get_single_bit(original,64,initial_permutation_table[position - 1]),position);\

#define final_permutation(original, modified)\
    setbit64(original, 40, 1, modified);\
    setbit64(original, 8, 2, modified);\
    setbit64(original, 48, 3, modified);\
    setbit64(original, 16, 4, modified);\
    setbit64(original, 56, 5, modified);\
    setbit64(original, 24, 6, modified);\
    setbit64(original, 64, 7, modified);\
    setbit64(original, 32, 8, modified);\
    setbit64(original, 39, 9, modified);\
    setbit64(original, 7, 10, modified);\
    setbit64(original, 47, 11, modified);\
    setbit64(original, 15, 12, modified);\
    setbit64(original, 55, 13, modified);\
    setbit64(original, 23, 14, modified);\
    setbit64(original, 63, 15, modified);\
    setbit64(original, 31, 16, modified);\
    setbit64(original, 38, 17, modified);\
    setbit64(original, 6, 18, modified);\
    setbit64(original, 46, 19, modified);\
    setbit64(original, 14, 20, modified);\
    setbit64(original, 54, 21, modified);\
    setbit64(original, 22, 22, modified);\
    setbit64(original, 62, 23, modified);\
    setbit64(original, 30, 24, modified);\
    setbit64(original, 37, 25, modified);\
    setbit64(original, 5, 26, modified);\
    setbit64(original, 45, 27, modified);\
    setbit64(original, 13, 28, modified);\
    setbit64(original, 53, 29, modified);\
    setbit64(original, 21, 30, modified);\
    setbit64(original, 61, 31, modified);\
    setbit64(original, 29, 32, modified);\
    setbit64(original, 36, 33, modified);\
    setbit64(original, 4, 34, modified);\
    setbit64(original, 44, 35, modified);\
    setbit64(original, 12, 36, modified);\
    setbit64(original, 52, 37, modified);\
    setbit64(original, 20, 38, modified);\
    setbit64(original, 60, 39, modified);\
    setbit64(original, 28, 40, modified);\
    setbit64(original, 35, 41, modified);\
    setbit64(original, 3, 42, modified);\
    setbit64(original, 43, 43, modified);\
    setbit64(original, 11, 44, modified);\
    setbit64(original, 51, 45, modified);\
    setbit64(original, 19, 46, modified);\
    setbit64(original, 59, 47, modified);\
    setbit64(original, 27, 48, modified);\
    setbit64(original, 34, 49, modified);\
    setbit64(original, 2, 50, modified);\
    setbit64(original, 42, 51, modified);\
    setbit64(original, 10, 52, modified);\
    setbit64(original, 50, 53, modified);\
    setbit64(original, 18, 54, modified);\
    setbit64(original, 58, 55, modified);\
    setbit64(original, 26, 56, modified);\
    setbit64(original, 33, 57, modified);\
    setbit64(original, 1, 58, modified);\
    setbit64(original, 41, 59, modified);\
    setbit64(original, 9, 60, modified);\
    setbit64(original, 49, 61, modified);\
    setbit64(original, 17, 62, modified);\
    setbit64(original, 57, 63, modified);\
    setbit64(original, 25, 64, modified);

#define get_key_left_half_permutation(original, modified)\
    for(uchar position = 1;position <= 28;position++){\
        set_single_bit(modified,28,get_single_bit(original,64,key_left_half_table[position - 1]),position);\
    }

#define get_key_right_half_permutation(original, modified)\
    for(uchar position = 1;position <= 28;position++){\
        set_single_bit(modified,28,get_single_bit(original,64,key_right_half_table[position - 1]),position);\
    }

#define permutation_choice_2(key_fusion, modified)\
    for(uchar position = 1;position <= 48 ;position++){\
        set_single_bit(modified,48,get_single_bit(key_fusion,56,permuted_choice_2_table[position - 1]),position);\
    }\

#endif
