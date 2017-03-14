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

//DEBUG VALUES:
// 0: NO DEBUG
// 1: LIGHT DEBUG
// 2: DEEP DEBUG

#define DEBUG 0

typedef struct Lz77_reference{
        //distance from prefix start in the encoded string
        int o;
        //length of the prefix
        int l;
        //next charachter in string
        char a;
        /*//if next char is valid or not
        bool next_valid;
        //constructor
        Lz77_reference(int _o,int _l,char _a){
            Lz77_reference(_o,_l,_a,true);
        }
        */
        Lz77_reference(int _o,int _l,char _a/*bool _next_valid*/){
            o = _o;
            l = _l;
            a = _a;
            //next_valid = _next_valid;
        }
        Lz77_reference(){}
        
}lz77_r;


#endif
