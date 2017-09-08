// ZIPSEC.cpp : Defines the entry point for the console application.
//

#include <time.h>

#include "stdafx.h"
#include "libs\deflate\constants.h"
#include "libs\deflate\deflate_encoder.h"
#include "libs\deflate\deflate_decoder.h"
#include "gzip.h"
#include "sgzip.h"


#define INVALID_COMMANDS_ERROR "zipsec-error: invalid arguments, use zipsec help to see the list of valid commands"
#define SUCCESFUL_OPERATION_MESSAGE "Operation completed succesfully"

#define HELP "help"
#define COMPRESS "compress"
#define DECOMPRESS "decompress"
#define CRYPT "crypt"
#define DECRYPT "decrypt"
#define DES "des"
#define TDES "3des"


//#define help "help\0"
void to_lowercase(char* string);
void print_help_commands();
void handle_1_argument(char* argv[]);
void handle_2_arguments(char* argv[]);
void handle_3_arguments(char* argv[]);
void handle_4_arguments(char* argv[]);
void handle_5_arguments(char* argv[]);
void handle_6_arguments(char* argv[]);

void save_output_file(const char* output_file_content, int output_file_length, char* output_file_name);
char* read_input_file(char* file_name, int* file_length);


int main(int argc, char *argv[])
{


	println("");
	clock_t tStart = clock();
	switch (argc) {
	case 2:
		handle_1_argument(argv);
		break;
	case 3:
		handle_2_arguments(argv);
		break;
	case 4:
		handle_3_arguments(argv);
		break;
	case 5:
		handle_4_arguments(argv);
		break;
	case 6:
		handle_5_arguments(argv);
		break;
	case 7:
		handle_6_arguments(argv);
		break;
	default:
		println(INVALID_COMMANDS_ERROR);
		exit(0);
	}
	printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	//system("PAUSE");

	return 0;
}

void handle_1_argument(char* argv[]) {

	//With only one argument the program has to offer:
	// -Help function
	//End
	char* arg1 = argv[1];
	to_lowercase(arg1);
	if (strcmp(arg1, HELP) == 0)
		print_help_commands();
	else {
		println(INVALID_COMMANDS_ERROR);
	}

	/*
	system("PAUSE");
	system("infgen -i debug/zipsec.gzip");
	*/
}

void handle_2_arguments(char* argv[]) {
	//with 2 arguments the program has to offer:
	// -compress input file
	// -decompress input file
	//End
	char* fname = argv[1];
	to_lowercase(fname);
	char* function = argv[2];
	to_lowercase(function);
	if (strcmp(function, COMPRESS) == 0) {
		//compress
		gzip comp(fname, NULL);
		comp.compress();
		comp.write_output(true);
		println(SUCCESFUL_OPERATION_MESSAGE);
	} else if (strcmp(function, DECOMPRESS) == 0) {
		//decompress
		gzip dcomp(fname, NULL);
		dcomp.decompress(NULL, -1);
		dcomp.write_output(false);
		println(SUCCESFUL_OPERATION_MESSAGE);
	} else {
		//error
		println(INVALID_COMMANDS_ERROR);
	}
}

void handle_3_arguments(char* argv[]) {
	//with 3 arguments the program has to offer:
	// -input_file compress output_file
	// -input_file decompress output_file (wrong, file name lies inside of the compressed file)
	// -input_file decrypt key
	//End
	char* fname = argv[1];
	to_lowercase(fname);
	char* function = argv[2];
	to_lowercase(function);
	char* of_name_key = argv[3];//or key for decrypting
	to_lowercase(of_name_key);
	if (strcmp(function, COMPRESS) == 0) {
		//compress
		gzip comp(fname, of_name_key);
		comp.check_file_name();
		comp.compress();
		comp.write_output(true);
		println(SUCCESFUL_OPERATION_MESSAGE);
	} else if (strcmp(function, DECOMPRESS) == 0) {
		//decompress
		println("zipsec-suggestion: output file name is alrady specified inside of the compressed file, try decompressing without specifying the output file name");
	} else if (strcmp(function, DECRYPT) == 0) {
		sgzip comp(fname, NULL);
		comp.decode(std::string(of_name_key));
		comp.write_output(true);
		println(SUCCESFUL_OPERATION_MESSAGE);
	} else {
		//error
		println(INVALID_COMMANDS_ERROR);
	}

}

void handle_4_arguments(char* argv[]) {
	//with 4 arguments the program has to offer
	// -input_file crypt des key *
	// -input_file decrypt des key(wrong, algorithm lies inside of the crypted file) *
	// -input_file crypt 3des key *
	// -input_file decrypt 3des key(wrong, algorithm lies inside of the crypted file) *
	// -input_file decompress decrypt key *
	char* fname = argv[1];
	char* function = argv[2];
	char* mode = argv[3];
	char* key = argv[4];

	to_lowercase(fname);
	to_lowercase(function);
	to_lowercase(mode);
	to_lowercase(key);


	int crypting_mode = -1;
	if (strcmp(mode, DES) == 0)
		crypting_mode = FLG_DES;
	else if (strcmp(mode, TDES) == 0)
		crypting_mode = FLG_3DES;


	if (strcmp(function, CRYPT) == 0) {
		//crypt
		sgzip comp(fname, NULL);
		comp.crypt(crypting_mode, std::string(key));
		comp.write_output(true);
		println(SUCCESFUL_OPERATION_MESSAGE);
	} else if (strcmp(function, DECRYPT) == 0) {
		//decrypt
		println("zipsec-suggestion: algorithm is alrady specified inside of the crypted file, try decrypting without specifying the algorithm name");
	} else if (strcmp(function, DECOMPRESS) == 0) {
		//decompress and decrypt
		if (strcmp(mode, DECRYPT) == 0) {
			sgzip dcomp(fname, NULL);
			dcomp.decode(std::string(key));
			dcomp.write_output(true);
			println(SUCCESFUL_OPERATION_MESSAGE);
		} else
			println(INVALID_COMMANDS_ERROR);
	} else
		println(INVALID_COMMANDS_ERROR);
}

void handle_5_arguments(char* argv[]) {
	//with 5 arguments the program has to offer
	// -input_file crypt output_file des key  *
	// -input_file decrypt output file des key(wrong, algorithm lies inside of the crypted file) (wrong, output file name lies inside of crypted file) *
	// -input_file crypt  output_file 3des key *
	// -input_file decrypt  output file 3des key(wrong, algorithm lies inside of the crypted file) (wrong, output file name lies inside of crypted file) *
	// -input_file compress crypt des key *
	// -input_file compress crypt 3des key *
	char* fname = argv[1];
	char* function = argv[2];
	char* output_file_name_f2 = argv[3];
	char* mode = argv[4];
	char* key = argv[5];

	to_lowercase(fname);
	to_lowercase(function);
	to_lowercase(output_file_name_f2);
	to_lowercase(mode);
	to_lowercase(key);

	int crypting_mode = -1;
	if (strcmp(mode, DES) == 0)
		crypting_mode = FLG_DES;
	else if (strcmp(mode, TDES) == 0)
		crypting_mode = FLG_3DES;


	if (strcmp(function, CRYPT) == 0) {
		//crypt
		sgzip comp(fname, output_file_name_f2);
		comp.check_file_name();
		comp.crypt(crypting_mode, std::string(key));
		comp.write_output(true);
		println(SUCCESFUL_OPERATION_MESSAGE);
	} else if (strcmp(function, DECRYPT) == 0) {
		//decrypt
		println("zipsec-suggestion: algorithm and output file name are alrady specified inside of the crypted file, try decrypting without specifying the algorithm name and output file");
	} else if (strcmp(function, COMPRESS) == 0) {
		if (strcmp(output_file_name_f2, CRYPT) == 0) {
			//compress and crypt
			sgzip comp(fname, NULL);
			comp.compress_and_crypt(crypting_mode, std::string(key));
			comp.write_output(true);
			println(SUCCESFUL_OPERATION_MESSAGE);
		} else
			println(INVALID_COMMANDS_ERROR);
	} else
		println(INVALID_COMMANDS_ERROR);
}

void handle_6_arguments(char* argv[]) {
	//with 6 arguments the program has to offer
	// -input file compress crypt output file des|3des key *
	// -input file decompress decrypt output file des|3des key (wrong, output file name is already inside file)

	char* fname = argv[1];
	char* function = argv[2];
	char* function2 = argv[3];
	char* of_name = argv[4];
	char* mode = argv[5];
	char* key = argv[6];

	to_lowercase(fname);
	to_lowercase(function);
	to_lowercase(function2);
	to_lowercase(of_name);
	to_lowercase(mode);
	to_lowercase(key);

	int crypting_mode = -1;
	if (strcmp(mode, DES) == 0)
		crypting_mode = FLG_DES;
	else if (strcmp(mode, TDES) == 0)
		crypting_mode = FLG_3DES;
	else {
		println("zipsec-error: ivalid algorithm");
		exit(0);
	}

	if (strcmp(function, COMPRESS) == 0) {
		//compression
		if (strcmp(function2, CRYPT) == 0) {
			//crypting
			sgzip comp(fname, of_name);
			comp.check_file_name();
			comp.compress_and_crypt(crypting_mode, std::string(key));
			comp.write_output(true);
		} else
			println(INVALID_COMMANDS_ERROR);
	} else if (strcmp(function, DECOMPRESS) == 0) {
		//decompression
		println("zipsec-suggestion: algorithm and output file name are alrady specified inside of the crypted file, try decrypting without specifying the algorithm name and output file");
	} else
		println(INVALID_COMMANDS_ERROR);
}

void print_help_commands() {
	println("");
	println("zipsec usage:");
	println("");
	println("zipsec input_file [compress|decompress] [crypt|decrypt] [output_file] [des|3des] [key]");
	println("");
	println("  input_file : file to compress/crypt");
	println("  compress|decompress: the operation that you want to do");
	println("  crypt|decrypt: the operation that you want to do");
	println("  output_file : name of the compressed/crypted output file (only for crypting/compressing)");
	println("  des|3des: the crypting algorithm to be used");
	println("  key: the key for the algorithm");
}

void to_lowercase(char* string) {
	for (int i = 0; i < strlen(string); i++)
		string[i] = tolower(string[i]);
}