#ifndef KEY_MANAGER_H
#define KEY_MANAGER_H

#include "permutations.h"

class Key_manager
{
	private:
		ulong key;
		ulong subkeys[16];
		
		void create_subkeys();
		uint get_new_key(uint original,int cycle);
	public:
		Key_manager();
		Key_manager(ulong _key);
		ulong get_subkey(int cycle);
};

#endif
