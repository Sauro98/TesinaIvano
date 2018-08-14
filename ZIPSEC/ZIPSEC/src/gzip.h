#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include "libs/deflate/constants.h"
#include "libs/deflate/deflate_encoder.h"
#include "libs/deflate/deflate_decoder.h"

#define GZIP_EXT ".GZ"
#define gzip_EXT ".gz"


static const char gzip_extension[2]{ 'g','z' };

//for file last modification data
#define stat _stat

//ID bytes
#define ID1 31
#define ID2 139

//compression method: 8 - deflate
#define CM 8

//flag bits
#define BASE_FLG 0
#define FTEXT_FLG 1
#define FHCRC_FLG 2
#define FEXTRA_FLG 4
#define FNAME_FLG 8
#define FCOMMENT_FLG 16 

//extra flags: 4: fast algrithm
#define XFL 4

//operating system 0 - windows
#define OS 0


class gzip
{

public:
	gzip();
	gzip(char* input_file_name,char* output_file_name);
	~gzip();

	//checks file extension
	void check_file_name();

	//does the compression
	void compress();
	//does the decompression
	void decompress(char* content,int length);

	//writes data into file
	void write_output(bool compressed);

protected:
	std::string output;
	char* in_f_name;
	int infn_l;
	char* o_f_name;
	int ofn_l;

	//helpers
	void generate_output_file_name(int* length,bool compressed);
	char* read_input_file(char* file_name, int* file_length);

	//as RFC1952, chapter 8 
	unsigned long crc_table[256];
	bool crc_table_computed;
	void make_crc_table();
	unsigned long update_crc(unsigned long crc, unsigned char* buf, int len);
	unsigned long calculate_crc(unsigned char *buf, int len);

	//compressing data functions
	void do_head(bool flg_fname, char* mtime);
	void do_body();

	typedef struct flags {
		bool ftext;
		bool fhcrc;
		bool fextra;
		bool fname;
		bool fcomment;

		flags() {
			ftext = false;
			fhcrc = false;
			fextra = false;
			fname = false;
			fcomment = false;
		}

		void print_flags() {
			if (ftext) 
				println("ftext");
			if (fextra)
				println("fextra");
			if (fname)
				println("fname");
			if (fhcrc)
				println("fhcrc");
			if (fcomment)
				println("fcomment");
		}
	}flags;
};



