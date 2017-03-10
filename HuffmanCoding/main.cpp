#include "Huffman_Tree.h"
#include <ctime>
#include<fstream>
#include <string>

int main(){
	int lengths[256];
    std::string text = "AAABBBCCCDDDEEEFFGGGGHHHH";
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
    
    
	std::string dec = tree.static_tree_decoding(lengths,enc);
    println("\nDECODED: " + dec);
    printintln("DECODED LENGTH = ",dec.length());
    println("Yay");
    system("pause");
    return 0; 
}
