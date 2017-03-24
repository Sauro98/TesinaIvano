#include "Huffman_Tree.h"
#include <ctime>
#include<fstream>
#include <string>

int main(){
	int lengths[256];
    std::string text = "Questo è un testo molto lungo usato solo e semplicemente come test per vedere se l'algoritmo di Huffman usato con le specifiche RFC1951 funziona correttamente, quanto ci impiega e se e' figo";
    println("ORIGINALE "<<text);
    printint("Lunghezza originale: ", text.length());
    println(" bytes");
    
    clock_t begin = clock();
    
    Huffman_Tree tree = Huffman_Tree(text);
    std::string enc = tree.static_tree_encoding(lengths);
    clock_t end = clock();
    
    
    println("ENCODED: \n" + enc);
    printint("\nENCODED LENGTH = ",enc.length());
    println(" bytes");
    
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printint("Tempo impiegato a comprimere: " , elapsed_secs);
    println(" secondi");
    
    begin = clock();
	std::string dec = tree.static_tree_decoding(lengths,enc);
	end = clock();
    println("\nDECODED: " + dec);
    printintln("DECODED LENGTH = ",dec.length());
	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printint("Tempo impiegato a decomprimere: " , elapsed_secs);
    println(" secondi");
    system("pause");
    return 0; 
}
