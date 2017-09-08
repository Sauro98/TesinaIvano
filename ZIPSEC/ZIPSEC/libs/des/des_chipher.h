#ifndef DES_CHIPHER_H
#define DES_CHIPHER_H

#include <stdlib.h>
#include "encoder.h"
#include <time.h>

#define MODE_DES 0
#define MODE_3DES 1 
//#define chipher_debug_mode

class DES_Chipher
{
	public:
		DES_Chipher(std::string _key, int _mode);
		std::string crypt(std::string original);
		std::string decrypt(std::string original);
	private:
		int mode;
		std::string key;
		std::string exec_crypt(std::string original,bool crypt);
		static char get_char(ulong original, int index);
		static ulong get_ulong_from_char(char original, int index);
		static ulong get_ulong_from_string(const char* original, int offset);
		static std::string get_string_from_ulong(ulong original);
		
};

#endif
