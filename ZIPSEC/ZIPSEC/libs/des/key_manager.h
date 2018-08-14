#ifndef KEY_MANAGER_H
#define KEY_MANAGER_H

#include "permutations.h"

#define get_new_key(key, cycle) (key << key_shifts_table[cycle] & 0xFFFFFFFU) | (key >> (uchar)(28) -  key_shifts_table[cycle])
class Key_manager
{
	private:
		ulong key;
		ulong subkeys[16];
		
		void create_subkeys();
	public:
		Key_manager();
		Key_manager(ulong _key);
		ulong get_subkey(int cycle);
};

#endif
