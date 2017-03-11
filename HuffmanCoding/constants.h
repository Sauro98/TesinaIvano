//File for constant definitions
//Use this file for defining global datatypes, macros and to include libraries
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <iostream>
#include <limits>
#include <stdint.h>
#include <stdlib.h>

#define print(x) std::cout<<x
#define println(x) std::cout<<x<<std::endl
#define printintln(x,y) std::cout<<x<<y<<std::endl
#define printint(x,y) std::cout<<x<<y

typedef struct Char_frequence{
        char value;
        long frequence;
        Char_frequence(char v,long f){
            value = (char)v;
            frequence = f;
        }
        Char_frequence(){}
}char_f;

typedef struct Huffman_node{
        char_f c_f;
        Huffman_node* parent;
        Huffman_node* left_child;
        Huffman_node* right_child;
        bool has_char;
        
        Huffman_node(char_f _c_f){
            c_f = _c_f;
            parent = NULL;
            left_child = NULL;
            right_child = NULL;
            has_char = true;
        }
        
        Huffman_node(Huffman_node* left,Huffman_node* right){
            int f1 = left->c_f.frequence;
            int f2 = right->c_f.frequence;
            c_f = char_f('\\',f1 + f2);
            parent = NULL;
            left_child = left;
            right_child = right;
            has_char = false;
        }
        
        Huffman_node(Huffman_node* _parent){
        	left_child = NULL;
        	right_child = NULL;
        	parent = _parent;
        	c_f = char_f('\\',0);
        }
        
        ~Huffman_node(){
        	free(parent);
        	free(left_child);
        	free(right_child);
        }
}node_f;

typedef struct Char_dictionary{
	//the char symbol
	unsigned char symbol;
	//its prefix value in the tree
	std::string value;
	Char_dictionary(unsigned char _symbol,std::string _value){
		symbol = _symbol;
		value = _value;
	}
	Char_dictionary(){}
}char_d;

typedef struct Code_dictionary{
	//the int value
	unsigned char value;
	//its actual code length
	int code_length;
	//the charachter that the code represents
	char symbol;
	
	Code_dictionary(unsigned char _value,int _code_length,char _symbol){
		value = _value;
		code_length = _code_length;
		symbol = _symbol;
	}
	
	Code_dictionary(){
	}
}code_d;




#endif
