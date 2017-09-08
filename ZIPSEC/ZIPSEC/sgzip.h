#pragma once

#include "gzip.h"
#include "libs\des\des_chipher.h"

#define SGZIP_EXT ".SGZIP"
#define sgzip_EXT ".sgzip"

static const char sgzip_extension[5]{ 's', 'g','z','i','p' };

#define FLG_COMPRESSED 2
#define FLG_DES 0
#define FLG_3DES 1

class sgzip : public gzip{
	public:
		sgzip(char* input_file_name, char* output_file_name);
		~sgzip();
		void compress_and_crypt(int crypt_mode,std::string key);
		void crypt(int crypt_mode,std::string key);
		void do_head(int crypt_mode,bool compressed);
		void do_body(bool compressed,DES_Chipher* chipher);
		void check_file_name();
		void write_output(bool crypted);
		void decode(std::string key);
    protected:
		std::string sgzip_output;
		void generate_output_file_name(int* length, bool crypted);

};

