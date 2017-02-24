#include "des_chipher.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

#define chipher_mode

int main(int argc, char** argv) {
	ulong key = 1;
	std::string string_key = "";
	std::string to_encode = "";
	
	do{
		println("Inserisci il testo da crittare:");
		std::getline( std::cin, to_encode);
		println("\nInserisci una chiave:");
		std::getline( std::cin, string_key);
		
		#ifdef chipher_mode
		DES_Chipher chipher = DES_Chipher(string_key);
		std::string crypted = chipher.crypt(to_encode);
		println("\n\n\n\n\n");
		println("CRYPTED: " + crypted);
		std::string decrypted = chipher.decrypt(crypted);
		println("DECRYPTED: " + decrypted);
		println("\n\n\n\n\n");
		#endif
		
		system("PAUSE");
		system("CLS");
	}while(key != 0);
	
	return 0;
}

		
