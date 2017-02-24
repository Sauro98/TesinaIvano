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

typedef struct Lz77_reference{
        //distance from prefix start in the encoded string
        int o;
        //length of the prefix
        int l;
        //next charachter in string
        char a;
        //constructor
        Lz77_reference(int _o,int _l,char _a){
            o = _o;
            l = _l;
            a = _a;
        }
        Lz77_reference(){}
        
        
}lz77_r;


#endif
