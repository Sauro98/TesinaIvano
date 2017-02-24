#ifndef DES_CHIPHER_H
#define DES_CHIPHER_H

#include <stdlib.h>
#include "encoder.h" 

//#define chipher_debug_mode

class DES_Chipher
{
	public:
		DES_Chipher(std::string _key);
		std::string crypt(std::string original);
		std::string decrypt(std::string original);
		static char get_char(ulong original,int index);
		static ulong get_ulong_from_char(char original,int index);
		static ulong get_ulong_from_string(std::string original,int offset);
		static std::string get_string_from_ulong(ulong original);
	private:
		ulong key;
		std::string exec_crypt(std::string original,bool crypt);
		
};

#endif
