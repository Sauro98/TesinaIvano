//File for constant definitions
//Use this file for defining global datatypes, macros and to include libraries
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <iostream>
#include <limits>
#include <stdint.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <stdint.h>

#define print(x) std::cout<<x
#define println(x) std::cout<<x<<std::endl
#define printintln(x,y) std::cout<<x<<y<<std::endl
#define printint(x,y) std::cout<<x<<y

//DEBUG VALUES:
// 0: NO DEBUG
// 1: LIGHT DEBUG
// 2: DEEP DEBUG

#define DEBUG 1


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
#define MASK_16_BITS 65535U

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
        long value;
        long frequence;
        Value_frequence(long v,long f){
            value = v;
            frequence = f;
        }
        Value_frequence(){}
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
            c_f = val_f(0,f1 + f2);
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
        }
        
        ~Huffman_node(){
        	free(parent);
        	free(left_child);
        	free(right_child);
        }
}node_f;

typedef struct Char_dictionary{
	//the char symbol
	unsigned long symbol;
	//its prefix value in the tree
	std::string value;
	Char_dictionary(long _symbol,std::string _value){
		symbol = _symbol;
		value = _value;
	}
	Char_dictionary(){}
}char_d;

typedef struct Code_dictionary{
	//the int value
	int value;
	//its actual code length
	int code_length;
	//the charachter that the code represents
	long symbol;
	
	Code_dictionary(int _value,int _code_length,long _symbol){
		value = _value;
		code_length = _code_length;
		symbol = _symbol;
	}
	
	Code_dictionary(){
	}
}code_d;

#endif
