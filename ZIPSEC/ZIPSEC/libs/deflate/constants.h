//File for constant definitions
//Use this file for defining global datatypes, macros and to include libraries
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <iostream>
#include <limits>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <ctype.h>

#define print(x) std::cout<<x
#define println(x) std::cout<<x<<std::endl
#define rollback_line() std::cout<<'\r'

#define Byte 1
#define KBYTE 1024 * Byte
#define MBYTE 1024 * KBYTE

#define SEARCH_BUFFER_LENGTH ((32 * KBYTE) - 1)
#define LOOK_AHEAD_BUFFER_LENGTH 258


#define MAX_SYMBOLS (16383)
#define HASHTABLE_MAX_SIZE (258)

#define HASHTABLE_SUBSTRING_LENGTH 4

//DEBUG VALUES:
// 0: NO DEBUG
// 1: LIGHT DEBUG
// 2: DEEP DEBUG

#define DEBUG 0

#define DYNAMIC_LITERALS_ALPHABET_LENGTH 286
#define DYNAMIC_DISTANCES_ALPHABET_LENGTH 30
#define DYNAMIC_CODES_ALPHABET_LENGTH 19

#define AT_LITERALS 0
#define AT_DISTANCIES 1
#define AT_CODES  2

//constants for decoding

//last block code

#define BFINAL 1

//block types
#define BTYPE00 0
#define BTYPE01 1
#define BTYPE10 2
#define BTYPE11 3

//end code
#define END_CODE 256

#define MAX_BITS_STATIC 10


#define MASK_1_BIT 1U
#define MASK_2_BITS 3U
#define MASK_3_BITS 7U
#define MASK_4_BITS 15U
#define MASK_5_BITS 31U
#define MASK_6_BITS 63U
#define MASK_7_BITS 127U
#define MASK_8_BITS 255U
#define MASK_9_BITS 511U
#define MASK_10_BITS 1023U
#define MASK_11_BITS 2047U
#define MASK_12_BITS 4095U
#define MASK_13_BITS 8191U
#define MASK_14_BITS 16383U
#define MASK_15_BITS 32767U
#define MASK_16_BITS 65535U

//compression types

#define STATIC 0
#define DYNAMIC 1
#define NON_COMPRESSED 2

#define SORT_ARRAY(x,y) std::sort(x,y)


//                             16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15
const int hclen_reference[] = {16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15};
const int hclen_reference_ord[] = {3,17,15,13,11,9,7,5,4,6,8,10,12,14,16,18,0,1,2};

typedef struct Static_Huffman_node{
        int value;
        Static_Huffman_node* parent;
        Static_Huffman_node* left_child;
        Static_Huffman_node* right_child;
        
        Static_Huffman_node(int _value){
            value = _value;
            parent = NULL;
            left_child = NULL;
            right_child = NULL;
        }
        
        Static_Huffman_node(Static_Huffman_node* _parent){
        	left_child = NULL;
        	right_child = NULL;
        	parent = _parent;
        	value = -1;
        }
        
        ~Static_Huffman_node(){
        	free(parent);
        	free(left_child);
        	free(right_child);
        }
}s_node_f;

typedef struct Value_frequence{
        int value;
        int frequence;
        Value_frequence(int v,int f){
            value = v;
            frequence = f;
        }
        Value_frequence(){}

		bool operator<(const Value_frequence& two)const{
			return frequence < two.frequence;
		}
}val_f;

typedef struct Huffman_node{
        val_f c_f;
        Huffman_node* parent;
        Huffman_node* left_child;
        Huffman_node* right_child;
        bool has_value;
        
        Huffman_node(val_f _c_f){
            c_f = _c_f;
            parent = NULL;
            left_child = NULL;
            right_child = NULL;
            has_value = true;
        }
        
        Huffman_node(Huffman_node* left,Huffman_node* right){
            int f1 = left->c_f.frequence;
            int f2 = right->c_f.frequence;
            c_f = val_f(-1,f1 + f2);
            parent = NULL;
            left_child = left;
            right_child = right;
            has_value = false;
        }
        
        Huffman_node(Huffman_node* _parent){
        	left_child = NULL;
        	right_child = NULL;
        	parent = _parent;
        	c_f = val_f(0,0);
        	has_value = false;
        }
        
        ~Huffman_node(){
        	//free(parent);
        	free(left_child);
        	free(right_child);
        }
}node_f;

typedef struct Value_dictionary{
	//the char symbol
	int symbol;
	//its prefix value in the tree
	std::string value;
	Value_dictionary(int _symbol,std::string _value){
		symbol = (_symbol >=0 )?_symbol:(uint8_t)((char)_symbol);
		value = _value;
	}
	Value_dictionary(){}

	bool operator<(const Value_dictionary& two) const {
		return symbol < two.symbol;
	}
}val_d;

typedef struct Code_dictionary{
	//the int value
	int value;
	//its actual code length
	int code_length;
	//the original value that the code represents
	int symbol;
	
	Code_dictionary(int _value,int _code_length,int _symbol){
		value = _value;
		code_length = _code_length;
		symbol = (_symbol >= 0) ? _symbol : (uint8_t)((char)_symbol);
	}
	
	Code_dictionary(){
	}

	bool operator<(const Code_dictionary& two)const {
		return symbol < two.symbol;
	}

}code_d;

typedef struct lz77_result{
    int literal_length;
    int distance;
    bool is_pair;
    lz77_result(int _length,int _distance){
        literal_length = _length;
        distance = _distance;
        is_pair = true;
    }
    lz77_result(int _literal){
        literal_length = _literal;
        is_pair = false;
    }
    lz77_result(){
    }
}lz77_r;

#endif
