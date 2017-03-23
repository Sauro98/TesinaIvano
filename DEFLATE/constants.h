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

#define MAX_BITS  10


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

typedef struct Huffman_node{
        int value;
        Huffman_node* parent;
        Huffman_node* left_child;
        Huffman_node* right_child;
        
        Huffman_node(int _value){
            value = _value;
            parent = NULL;
            left_child = NULL;
            right_child = NULL;
        }
        
        Huffman_node(Huffman_node* _parent){
        	left_child = NULL;
        	right_child = NULL;
        	parent = _parent;
        	value = -1;
        }
        
        ~Huffman_node(){
        	free(parent);
        	free(left_child);
        	free(right_child);
        }
}node_f;


#endif
