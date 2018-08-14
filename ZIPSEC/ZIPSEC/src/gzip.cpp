#include "gzip.h"
#include <time.h>
#include <array>

gzip::gzip()
{
}

gzip::gzip(char * input_file_name, char * output_file_name)
{
	in_f_name = input_file_name;
	infn_l = (int)strlen(in_f_name);
	o_f_name = output_file_name;
	if(o_f_name != NULL)
		ofn_l = (int)strlen(o_f_name);
	crc_table_computed = false;
	output = "";
	//println("infname " << std::string(in_f_name,infn_l));

}

gzip::~gzip()
{

}

void gzip::check_file_name() {

	if (o_f_name != NULL && (ofn_l < 4 || strcmp(o_f_name + (ofn_l - 3), GZIP_EXT) != 0 && strcmp(o_f_name + (ofn_l - 3), gzip_EXT) != 0)) {
		println("zipsec-error: output file name must end with \".gz\"");
		exit(0);
	}
}




void gzip::do_head(bool flg_fname, char* mtime)
{
	//IDs
	output += ID1;
	output += (unsigned char)ID2;
	//CM
	output += CM;
	//FLG
	unsigned char FLG = BASE_FLG;
	if (flg_fname)
		FLG |= FNAME_FLG;
	FLG |= FHCRC_FLG;
	output += FLG;
	//MTIME
	output += mtime[0];
	output += mtime[1];
	output += mtime[2];
	output += mtime[3];
	//XFL
	output += XFL;
	//OS
	output += (char)OS;

	if (flg_fname)
		output += std::string(in_f_name, infn_l) + (char)0;

	//crc 16
	unsigned long full_hcrc = calculate_crc((unsigned char*)output.c_str(),(int) output.length());
	//uint16_t hcrc_test = 
	//hcrc_test |= 

	uint16_t hcrc = ((unsigned char)full_hcrc) << 8;
	hcrc |= (unsigned char)full_hcrc;

	//println("crc out " << hcrc);
	//println("length " << output.length());

	output += ((char)(hcrc >> 8));
	output += (char)hcrc;



}

void gzip::do_body() {
	int file_length = 0;
	char* file_content = read_input_file(in_f_name, &file_length);
	Deflate_encoder encoder = Deflate_encoder();
	println("Compressing...");
	std::string compressed = encoder.encode(file_content, file_length);
	unsigned long crc_32 = calculate_crc((unsigned char*)file_content, file_length);
	//println("file len "<<std::string(file_content).length());
	//println("\""<<file_content<<"\"");
	//println("crc write "<<crc_32);
	//println("curr len " << output.length());
	output += compressed;
	output += (unsigned char)crc_32;
	output += (unsigned char)(crc_32 >> 8);
	output += (unsigned char)(crc_32 >> 16);
	output += (unsigned char)(crc_32 >> 24);
	output += (unsigned char)file_length;
	output += (unsigned char)(file_length >> 8);
	output += (unsigned char)(file_length >> 16);
	output += (unsigned char)(file_length >> 24);
	double compression_ratio = (double)file_length / (double)output.length();
	compression_ratio *= 10;
	println("Original size: " << file_length << ", compressed size: " << output.length() << ". Compression ratio: " << compression_ratio << "%");
	delete[] file_content;

}

void gzip::compress() {
	if (strcmp(in_f_name + (infn_l - 5), GZIP_EXT) == 0 || strcmp(in_f_name + (infn_l - 5), gzip_EXT) == 0) {
		println("zipsec-error: file is already compressed with gzip, try decompression instead");
		exit(0);
	}
	struct stat result;
	uint64_t mod_time = 0;
	if (stat(in_f_name, &result) == 0)
	{
		mod_time = result.st_mtime;
	}
	char mtime[4];
	mtime[0] = (unsigned char)mod_time;
	mtime[1] = (unsigned char)(mod_time >> 8);
	mtime[2] = (unsigned char)(mod_time >> 16);
	mtime[3] = (unsigned char)(mod_time >> 24);
	do_head(true, mtime);
	do_body();
}

void gzip::decompress(char* content, int length) {
	if(content == NULL)
		if (strcmp(in_f_name + (infn_l - 3), GZIP_EXT) != 0 && strcmp(in_f_name + (infn_l - 3), gzip_EXT) != 0) {
			println("zipsec-error: file is not compressed with gzip, try compression instead");
			exit(0);
		}

	int file_length = 0;
	char* file_content = NULL;

	if (content == NULL)
		file_content = read_input_file(in_f_name, &file_length);
	else {
		file_content = content;
		file_length = length;
	}

	if ((unsigned char)file_content[0] != ID1 || (unsigned char)file_content[1] != ID2) {
		println("zipsec-error: input file is not valid");
		exit(0);
	}
	if (file_content[2] != CM) {
		println("zipsec-error: compression mode not supported");
		exit(0);
	}

	flags flgs;
	flgs.ftext = (file_content[3] & 1) == 1;
	flgs.fhcrc = (file_content[3] & 2) == 2;
	flgs.fextra = (file_content[3] & 4) == 4;
	flgs.fname = (file_content[3] & 8) == 8;
	flgs.fcomment = (file_content[3] & 16) == 16;

	//flgs.print_flags();

	if ((file_content[3] & 32) != 0 || (file_content[3] & 64) != 0 || (file_content[3] & 128) != 0) {
		println("zipsec-error: invalid file flags, file might be corrupted");
		exit(0);
	}

	uint32_t mtime = file_content[4];
	mtime |= file_content[5] << 8;
	mtime |= file_content[6] << 16;
	mtime |= file_content[7] << 24;


	//xfl byte is not useful for the purposes of this application

	int os = file_content[9];
	if (os != OS)
		println("infgen-warning: this file was not compressed in a windows os, this may cause errors");

	int currbyte = 10;

	if (flgs.fextra) {
		int xlen = 0;
		xlen = file_content[currbyte] << 8;
		currbyte++;
		xlen |= file_content[currbyte];
		//skip extra bytes, not useful for the purposes of this program
		currbyte += xlen;
	}
	if (flgs.fname) {
		std::string file_name = "";
		while ((int)file_content[currbyte] != 0) {
			file_name += file_content[currbyte];
			currbyte++;
		}
		//println("file name "<<file_name);
		o_f_name = new char[file_name.length() + 1];
		for (int a = 0; a < file_name.length(); a++)
			o_f_name[a] = file_name.at(a);
		o_f_name[file_name.length()] = (char)0;
		// 0 byte
		currbyte++;
	}
	if (flgs.fcomment) {
		std::string file_comment = "";
		while ((int)file_content[currbyte] != 0) {
			file_comment += file_content[currbyte];
			currbyte++;
		}
		println("zipsec-file comment: " << file_comment);
	}

	if (flgs.fhcrc) {
		//println("currbyte " << currbyte);
		uint16_t hcrc = (unsigned char)file_content[currbyte];
		currbyte++;
		hcrc |= ((uint16_t)((unsigned char)file_content[currbyte]) << 8);
		currbyte++;
		//println("hcrc read "<<hcrc);
		//calculate hcrc
		unsigned long full_hcrc = calculate_crc((unsigned char*)file_content, currbyte - 2);
		uint16_t hcrc_test = ((uint16_t)((unsigned char)full_hcrc) << 8);
		hcrc_test |= (unsigned char)full_hcrc;

		if (hcrc != hcrc_test) {
			println("zipsec-error: file header is corrupted");
			//system("PAUSE");
			exit(0);
		}
	}


	//println("currbyte " << currbyte);
	println("Decompressing...");
	Deflate_decoder decoder(std::string(file_content + currbyte, file_length - currbyte));
	int compressed_size = 0;
	output = decoder.decode(&compressed_size);
	currbyte += compressed_size + 1;
	uint32_t crc_test = calculate_crc((unsigned char*)output.c_str(),(int) output.length());
	//println("\""<<output<<"\"");
	//println("comp size "<<compressed_size);
	//println("dec len "<<output.length());
	//println("crc " << crc_test);
	//println("currbyte "<<currbyte);
	uint32_t crc = (unsigned char)file_content[currbyte];
	currbyte++;
	crc |= (unsigned char)(file_content[currbyte]) << 8;
	currbyte++;
	crc |= (unsigned char)(file_content[currbyte]) << 16;
	currbyte++;
	crc |= (unsigned char)(file_content[currbyte]) << 24;
	currbyte++;

	//println("crc read "<<crc);

	if (crc != crc_test) {
		println("zipsec-error: file content is corrupted");
		exit(0);
	}

	uint32_t isize = (unsigned char)file_content[currbyte];
	currbyte++;
	isize |= (unsigned char)(file_content[currbyte]) << 8;
	currbyte++;
	isize |= (unsigned char)(file_content[currbyte]) << 16;
	currbyte++;
	isize |= (unsigned char)(file_content[currbyte]) << 24;
	currbyte++;

	if (isize != output.length()) {
		println("zipsec-error: file content is corrupted");
		exit(0);
	}


}

void gzip::write_output(bool compressed) {
	if (o_f_name == NULL) {
		generate_output_file_name(&ofn_l, compressed);
	}
	std::ofstream output_file;
	output_file.open(o_f_name, std::ios::binary);
	output_file.write(output.c_str(), output.length());
	output_file.close();
	remove(in_f_name);

}

//--Helpers
void gzip::generate_output_file_name(int* length, bool compressed) {
	if (compressed) {

		int output_name_size = 0;

		//get last occurrence of charachter '.'
		char* last_occurrence = strrchr(in_f_name, '.');

		if (last_occurrence == NULL)       //se non trova il carattere '.' nel nome
			output_name_size = infn_l + 4; // .gz\0 4 caratteri
		else
			output_name_size = (int)(last_occurrence - in_f_name ) + 1 + 3;  // gz\0 3 caratteri

		o_f_name = new char[output_name_size];

		if (last_occurrence != NULL)
			for (int a = 0; a < (int)(last_occurrence - in_f_name) + 1; a++)
				o_f_name[a] = in_f_name[a];
		else {
			for (int a = 0; a < infn_l; a++)
				o_f_name[a] = in_f_name[a];
			o_f_name[infn_l] = '.';
		}

		for (int a = 1; a < 3; a++)
			o_f_name[output_name_size - a - 1] = gzip_extension[2 - a];
		o_f_name[output_name_size - 1] = '\0';
		*length = output_name_size;
	} else {
		o_f_name = new char[infn_l - 5]; // \0
		for (int a = 0; a < infn_l - 5; a++) {
			o_f_name[a] = in_f_name[a];
		}

		*length = infn_l - 5;
	}

}

char* gzip::read_input_file(char* file_name, int* file_length) {
	println("Reading file...");
	FILE * pFile;
	long lSize;
	char * buffer;
	size_t result;
	char* fn = file_name;
	pFile = fopen(file_name, "rb");
	if (pFile == NULL) {
		println("zipsec-error: File not found");
		exit(0);
	}
	// obtain file size:
	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	rewind(pFile);
	// allocate memory to contain the whole file:
	buffer = new char[lSize];
	if (buffer == NULL) {
		println("zipsec-error: memory error");
		exit(0);
	}
	// copy the file into the buffer:
	result = fread(buffer, 1, lSize, pFile);
	if (result != lSize) {
		println("zipsec-error: an error occourred while reading the file");
		exit(0);
	}
	/* the whole file is now loaded in the memory buffer. */
	// terminate
	fclose(pFile);
	*file_length = lSize;
	return buffer;
}

//--CRC related functions
void gzip::make_crc_table()
{
	unsigned long c;
	int n, k;

	for (n = 0; n < 256; n++) {
		c = (unsigned long)n;
		for (k = 0; k < 8; k++) {
			if (c & 1) {
				c = 0xedb88320L ^ (c >> 1);
			} else {
				c = c >> 1;
			}
		}
		crc_table[n] = c;
	}
	crc_table_computed = true;
}

unsigned long gzip::update_crc(unsigned long crc,
	unsigned char *buf, int len)
{
	unsigned long c = crc ^ 0xffffffffL;
	int n;

	if (!crc_table_computed)
		make_crc_table();
	for (n = 0; n < len; n++) {
		c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
	}
	return c ^ 0xffffffffL;
}

unsigned long gzip::calculate_crc(unsigned char *buf, int len)
{
	return update_crc(0L, buf, len);
}

