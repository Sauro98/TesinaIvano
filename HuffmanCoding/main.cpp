#include "Huffman_Tree.h"
#include <ctime>
#include<fstream>
#include <string>

int main(){
    std::string text = "";
    std::string line;
    std::ifstream myfile ("text.txt");
  if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {
      text += line;
    }
    myfile.close();
  }

  else std::cout << "Unable to open file\n"; 
    printint("Lunghezza originale: ", text.length());
    println(" bytes");
    //println("Testo: " + line);
    
    clock_t begin = clock();
    
    Huffman_Tree tree = Huffman_Tree(text);
    std::string enc = tree.encode();
    clock_t end = clock();
    
    
    //println("ENCODED: \n" + enc);
    printint("\nENCODED LENGTH = ",enc.length());
    println(" bytes");
    
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printint("Tempo impiegato a comprimere: " , elapsed_secs);
    println(" secondi");
    
    
    /*std::string dec = tree.decode(enc);
    println("\nDECODED: " + dec);
    printintln("DECODED LENGTH = ",dec.length());*/
    println("Yay");
    system("pause");
    return 0; 
}
