#include "des_chipher.h"

DES_Chipher::DES_Chipher(std::string _key){
	int length = _key.length();
	if(length == 8){
		key = get_ulong_from_string(_key,0);
	}else{
		println("Key inserted is not of valid length (must be 8 bytes | 64 bit)");
		print("Inserted key is ");
		print(length);
		print(" bytes | ");
		print((length * 8));
		println(" bit long");
		println("Key set to default value: (aaaaaaaa)");
		_key = "aaaaaaaa";
		key = get_ulong_from_string(_key,0);
	}
}

std::string DES_Chipher::crypt(std::string original){
	return exec_crypt(original,true);
}

std::string DES_Chipher::exec_crypt(std::string original,bool crypt){
	#ifdef chipher_debug_mode
	println("\n\n----------STARTING CHIPHER----------");
	#endif
	
	Encoder encoder= Encoder(key);
	int length = original.length();
	int missing_length = (length %8 != 0)? 8 - (length % 8) : 0;
	int valid_length = length + missing_length;
	
	#ifdef chipher_debug_mode
	printf("STRING LENGTH: %d\n",length);
	printf("MISSING LENGTH: %d\n",missing_length);
	printf("VALID LENGTH: %d\n",valid_length);
	#endif
	
	std::string valid_length_array = ""; 
	std::string modified_array = "";
	
	for(int index = 0;index < length;index++){
		valid_length_array += original[index];
	}
	for(int index = length;index < valid_length;index++){
		valid_length_array += (char) 0;
	}

	#ifdef chipher_debug_mode
	println("COPIED ARRAY:");
	for(int index = 0;index < valid_length;index++){
		print(valid_length_array.at(index));
	}
	println("\nEND OF ARRAY");
	#endif
	
	
	
	for(int crypt_index = 0;crypt_index < valid_length;crypt_index += 8){
		
		ulong to_crypt = get_ulong_from_string(valid_length_array,crypt_index);
		
		#ifdef chipher_debug_mode
		printf("INPUT ARRAY AS ULONG: %llX %llu\n",to_crypt,to_crypt);
		#endif
		
		ulong crypted = (crypt)?encoder.encode(to_crypt):encoder.decode(to_crypt);
		
		modified_array += get_string_from_ulong(crypted);
		
		#ifdef chipher_debug_mode
		printf("CRYPTED ARRAY AS ULONG: %llX %llu\n",crypted,to_crypt);
		
		printf("MODIFIED_ARRAY_POS 0: %c\n",modified_array.at(crypt_index + 0));
		printf("MODIFIED_ARRAY_POS 1: %c\n",modified_array.at(crypt_index + 1));
		printf("MODIFIED_ARRAY_POS 2: %c\n",modified_array.at(crypt_index + 2));
		printf("MODIFIED_ARRAY_POS 3: %c\n",modified_array.at(crypt_index + 3));
		printf("MODIFIED_ARRAY_POS 4: %c\n",modified_array.at(crypt_index + 4));
		printf("MODIFIED_ARRAY_POS 5: %c\n",modified_array.at(crypt_index + 5));
		printf("MODIFIED_ARRAY_POS 6: %c\n",modified_array.at(crypt_index + 6));
		printf("MODIFIED_ARRAY_POS 7: %c\n",modified_array.at(crypt_index + 7));
		
		println("CRYPTED ARRAY " + modified_array);
		std::cout<<"CRYPTED ARRAY LENGTH " << modified_array.length() << std::endl;
		#endif
	}
	
	#ifdef chipher_debug_mode
	printf("\n----------END OF CHIPHER----------\n\n");
	#endif
	
	return modified_array;
}

std::string DES_Chipher::decrypt(std::string original){
	return exec_crypt(original,false);
}

char DES_Chipher::get_char(ulong original,int index){
	original <<= (8 * index);
	original >>= 56;
	original &= 0xFF;
	return (char) original;
}

ulong DES_Chipher::get_ulong_from_char(char original,int index){
	ulong to_return = 0;
	to_return |= original;
	to_return &= 0xFF;
	to_return <<= ((7-index) * 8);
	return to_return; 
}

ulong DES_Chipher::get_ulong_from_string(std::string original,int offset){
	ulong string_as_ulong = 0;
	string_as_ulong |= get_ulong_from_char(original.at(offset + 0),0);
	string_as_ulong |= get_ulong_from_char(original.at(offset + 1),1);
	string_as_ulong |= get_ulong_from_char(original.at(offset + 2),2);
	string_as_ulong |= get_ulong_from_char(original.at(offset + 3),3);
	string_as_ulong |= get_ulong_from_char(original.at(offset + 4),4);
	string_as_ulong |= get_ulong_from_char(original.at(offset + 5),5);
	string_as_ulong |= get_ulong_from_char(original.at(offset + 6),6);
	string_as_ulong |= get_ulong_from_char(original.at(offset + 7),7);
	return string_as_ulong;
}

std::string DES_Chipher::get_string_from_ulong(ulong original){
	std::string ulong_as_string = "";
	ulong_as_string += get_char(original,0);
	ulong_as_string += get_char(original,1);
	ulong_as_string += get_char(original,2);
	ulong_as_string += get_char(original,3);
	ulong_as_string += get_char(original,4);
	ulong_as_string += get_char(original,5);
	ulong_as_string += get_char(original,6);
	ulong_as_string += get_char(original,7);
	return ulong_as_string;
}
