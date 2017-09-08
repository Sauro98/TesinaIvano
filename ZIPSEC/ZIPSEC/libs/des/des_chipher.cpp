#include "des_chipher.h"

DES_Chipher::DES_Chipher(std::string _key,int _mode){
	mode = _mode;
	if (mode > 1) {
		println("zipsec-error: crypting mode is invalid, options are des and 3des");
		exit(0);
	}
	int length = (int)_key.length();
	if (mode == MODE_DES && length != 8) {
		println("zipsec-error: la chiave per l'algoritmo DES deve essere lunga 8 caratteri, quella inserita ha lunghezza pari a "<<length<<" caratteri");
		exit(0);
	}else if(length != 8 && length != 16 && length != 24){
		println("la chiave per l'algoritmo 3des deve essere lunga 8 o 16 o 24 caratteri, quella inserita ha lunghezza pari a "<<length<<" caratteri");
		exit(0);
	}
	key = _key;
	
}

std::string DES_Chipher::crypt(std::string original){
	return exec_crypt(original,true);
}

std::string DES_Chipher::exec_crypt(std::string original,bool crypt){
	
	//SIZE ADJUSTMENTS

	int length = (int) original.length();
	int missing_length = (length % 8 != 0) ? 8 - (length % 8) : 0;
	int valid_length = length + missing_length;

	std::string valid_length_array = "";
	std::string modified_array = "";

	for (int index = 0; index < length; index++) {
		valid_length_array += original[index];
	}
	for (int index = length; index < valid_length; index++) {
		valid_length_array += (char)0;
	}

	//END

	int perc = 0;
	int prevperc = -1;
	const char* content_array = valid_length_array.c_str();

	if (mode == MODE_DES || key.length() == 8) {
		//ACTUAL CRYPTING
		Encoder encoder = Encoder(get_ulong_from_string(key.c_str(),0));
		for (int crypt_index = 0; crypt_index < valid_length; crypt_index += 8) {

			//perc printing
			if (valid_length < 100) perc = 100;
			else perc = crypt_index / (valid_length / 100);
			if (perc % 10 == 0 && perc != prevperc) {
				if (perc != 0)
					rollback_line();
				print(perc << " % ... ");
				prevperc = perc;
			}
			//crypting
			ulong to_crypt = get_ulong_from_string(content_array, crypt_index);
			ulong crypted = (crypt) ? encoder.encode(to_crypt) : encoder.decode(to_crypt);
			modified_array += get_string_from_ulong(crypted);

		}
		//END
	}else{

		int offset1 = 0;
		int offset2 = 0;
		int offset3 = 0;
		if (key.length() >= 16)
			offset2 = 8;
		if (key.length() == 24)
			offset3 = 16;

		//primo passaggio con la prima chiave
		Encoder encoder = Encoder(get_ulong_from_string(key.c_str(), (crypt)?offset1:offset3));
		Encoder encoder2 = Encoder(get_ulong_from_string(key.c_str(), offset2));
		Encoder encoder3 = Encoder(get_ulong_from_string(key.c_str(), (crypt) ? offset3 : offset1));

		for (int crypt_index = 0; crypt_index < valid_length; crypt_index += 8) {

			//perc printing
			if (valid_length < 100) perc = 100;
			else perc = crypt_index / (valid_length / 100);
			if (perc % 10 == 0 && perc != prevperc) {
				if (perc != 0)
					rollback_line();
				print(perc << " % ... ");
				prevperc = perc;
			}

			ulong to_crypt = get_ulong_from_string(content_array, crypt_index);
			ulong crypted = (crypt) ? encoder.encode(to_crypt) : encoder.decode(to_crypt);
			crypted = (crypt) ? encoder2.decode(crypted) : encoder2.encode(crypted);
			crypted = (crypt) ? encoder3.encode(crypted) : encoder3.decode(crypted);

			modified_array += get_string_from_ulong(crypted);

		}

	}

	println("");
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

ulong DES_Chipher::get_ulong_from_string(const char* original,int offset){
	ulong string_as_ulong = 0;
	string_as_ulong |= get_ulong_from_char(original[offset + 0],0);
	string_as_ulong |= get_ulong_from_char(original[offset + 1],1);
	string_as_ulong |= get_ulong_from_char(original[offset + 2],2);
	string_as_ulong |= get_ulong_from_char(original[offset + 3],3);
	string_as_ulong |= get_ulong_from_char(original[offset + 4],4);
	string_as_ulong |= get_ulong_from_char(original[offset + 5],5);
	string_as_ulong |= get_ulong_from_char(original[offset + 6],6);
	string_as_ulong |= get_ulong_from_char(original[offset + 7],7);
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
