#include "sgzip.h"

sgzip::sgzip(char* input_file_name, char* output_file_name) :gzip(input_file_name, output_file_name){
	sgzip_output = "";
}

sgzip::~sgzip(){
}

void sgzip::compress_and_crypt(int crypt_mode, std::string key){
	DES_Cipher chipher(key,crypt_mode);
	do_head(crypt_mode,true);
	do_body(true, &chipher);

}

void sgzip::crypt(int crypt_mode, std::string key){
	DES_Cipher chipher(key, crypt_mode);
	do_head(crypt_mode, false);
	do_body(false, &chipher);
}

void sgzip::do_head(int crypt_mode,bool compressed) {
	unsigned char header_byte = 0;
	
	if (crypt_mode == MODE_DES)
		header_byte |= FLG_DES;
	else
		header_byte |= FLG_3DES;

	if (compressed)
		header_byte |= FLG_COMPRESSED;

	//println("hb "  << (int)header_byte);
	sgzip_output += header_byte;

	if (compressed) {
		struct stat result;
		uint64_t mod_time = 0;
		if (stat(in_f_name, &result) == 0)
		{
			mod_time = result.st_mtime;
		}
		//println("input mod time "<<mod_time);
		char mtime[4];
		mtime[0] = (unsigned char)mod_time;
		mtime[1] = (unsigned char)(mod_time >> 8);
		mtime[2] = (unsigned char)(mod_time >> 16);
		mtime[3] = (unsigned char)(mod_time >> 24);
		gzip::do_head(true, mtime);
	}
	else {
		//println("in_f_name " << (in_f_name));
		//moved to: do_body();
		//sgzip_output += std::string(in_f_name, infn_l) + char(0);
	}
}


void sgzip::do_body(bool compressed,DES_Cipher* chipher) {
	if (compressed) {
		gzip::do_body();
		println("Crypting...");
		sgzip_output += chipher->crypt(output);
	}
	else {
		int file_length = 0;
		char* file_content = read_input_file(in_f_name, &file_length);
		//CRC32
		int missing_length = (file_length % 8 != 0) ? 8 - (file_length % 8) : 0;
		std::string to_control = std::string(file_content, file_length);
		for (int a = 0; a < missing_length; a++)
			to_control += (char)0;

		uint32_t crc_32 = calculate_crc((unsigned char*)to_control.c_str(), file_length + missing_length);
		sgzip_output += (unsigned char)crc_32;
		sgzip_output += (unsigned char)(crc_32 >> 8);
		sgzip_output += (unsigned char)(crc_32 >> 16);
		sgzip_output += (unsigned char)(crc_32 >> 24);
		//println("crc write "<<crc_32);
		//println("l "<<file_length);
		//FILE_NAME
		sgzip_output += std::string(in_f_name, infn_l) + char(0);


		println("Crypting...");
		std::string crypted = chipher->crypt(std::string(file_content, file_length));
		sgzip_output += crypted;
		
	}
}

void sgzip::write_output(bool crypted) {
	if (o_f_name == NULL) {
		generate_output_file_name(&ofn_l, crypted);
	}
	std::ofstream output_file;
	output_file.open(o_f_name, std::ios::binary);
	output_file.write(sgzip_output.c_str(), sgzip_output.length());
	output_file.close();
	remove(in_f_name);
}

void sgzip::decode(std::string key){
	if (strcmp(in_f_name + (infn_l - 6), SGZIP_EXT) != 0 && strcmp(in_f_name + (infn_l - 6), sgzip_EXT) != 0) {
		println("zipsec-error: file is not crypted with sgzip, try encryption instead");
		exit(0);

	}

	int file_length = 0;
	char* file_content = read_input_file(in_f_name, &file_length);

	//println("f l "<<file_length);

	int crypt_type = ((file_content[0] & 1) == 1)? FLG_3DES:FLG_DES;
	bool compressed = (file_content[0] == 2 || file_content[0] == 3);

	if ((file_content[0] & ((unsigned char)252)) > 0) {
		//println("test " << (int)((uint8_t)file_content[0]));
		//println("test2"<< (int)(file_content[0] & (unsigned char)252));
		println("zipsec-error: crypted file format is invalid");
		exit(0);
	}

	DES_Cipher chipher(key, crypt_type);

	int current_byte = 1;

	
	if (!compressed) {

		//CRC32

		uint32_t crc_read = (unsigned char)file_content[current_byte];
		current_byte++;
		crc_read |= (unsigned char)(file_content[current_byte]) << 8;
		current_byte++;
		crc_read |= (unsigned char)(file_content[current_byte]) << 16;
		current_byte++;
		crc_read |= (unsigned char)(file_content[current_byte]) << 24;
		current_byte++;

		//println("crc read "<<crc_read);

		//FILE NAME
		std::string ofn = "";
		while (file_content[current_byte] != ((char)0)) {
			ofn += file_content[current_byte];
			current_byte++;
		}
		current_byte ++; // 0 charachter
		o_f_name = new char[ofn.length() + 1];
		for (int a = 0; a < ofn.length(); a++)
			o_f_name[a] = ofn.at(a);
		o_f_name[ofn.length()] = (char) 0;
		//println("o_f_name "<<o_f_name);
		println("Decrypting...");
		sgzip_output = chipher.decrypt(std::string(file_content + current_byte, file_length - current_byte));
		//println("l check "<<file_length - current_byte);
		//println("out "<<sgzip_output<<"|||");
		uint32_t crc_32 =calculate_crc((unsigned char*)sgzip_output.c_str(),(int) sgzip_output.length());
		//println("crc_32 calc "<<crc_32);
		//println("l "<<sgzip_output.length());
		if (crc_32 != crc_read) {
			println("zipsec-error: Decrypting error. The file could be corrupted or password may be wrong");
			exit(0);
		}

	}else {
		println("Decrypting...");
		std::string decrypted = chipher.decrypt(std::string(file_content + 1, file_length - 1));
		//TODO:: sistemare gzip.decompress per accettare come input il testo decrittato ed aggiungere output a sgzip_output
		gzip::decompress((char*)decrypted.c_str(),(int)decrypted.length());
		sgzip_output = output;
	}
}

void sgzip::generate_output_file_name(int* length, bool crypted) {
	if (crypted) {
		int output_name_size = 0;

		//get last occurrence of charachter '.'
		char* last_occurrence = strrchr(in_f_name, '.');

		if (last_occurrence == NULL)       //se non trova il carattere '.' nel nome
			output_name_size = infn_l + 7; // .sgzip\0 7 caratteri
		else
			output_name_size = (int)(last_occurrence - in_f_name) + 1 + 6;  // sgzip\0 6 caratteri

		o_f_name = new char[output_name_size];

		if (last_occurrence != NULL)
			for (int a = 0; a < (int)(last_occurrence - in_f_name) + 1; a++)
				o_f_name[a] = in_f_name[a];
		else {
			for (int a = 0; a < infn_l; a++)
				o_f_name[a] = in_f_name[a];
			o_f_name[infn_l] = '.';
		}

		for (int a = 1; a < 6; a++)
			o_f_name[output_name_size - a - 1] = sgzip_extension[5 - a];
		o_f_name[output_name_size - 1] = '\0';
	} else {
		o_f_name = new char[infn_l - 6]; // remove .sgzip
		for (int a = 0; a < infn_l - 6; a++) {
			o_f_name[a] = in_f_name[a];
		}

		*length = infn_l - 5;
	}

}

void sgzip::check_file_name() {
	if (o_f_name != NULL && (ofn_l < 7 || strcmp(o_f_name + (ofn_l - 6), SGZIP_EXT) != 0 && strcmp(o_f_name + (ofn_l - 6), sgzip_EXT) != 0)) {
		println("zipsec-error: output file name must end with \".sgzip\"");
		exit(0);
	}
}
