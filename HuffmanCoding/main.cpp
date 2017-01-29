#include "Huffman_Tree.h"

int main(){
    Huffman_Tree tree = Huffman_Tree("Miusov, as a man man of breeding and deilcacy, could not but feel some inwrd qualms, when he reached the Father Superior's with Ivan: he felt ashamed of havin lost his temper. He felt that he ought to have disdaimed that despicable wretch, Fyodor Pavlovitch, too much to have been upset by him in Father Zossima's cell, and so to have forgotten himself. \"Teh monks were not to blame, in any case,\" he reflceted, on the steps. \"And if they're decent people here (and the Father Superior, I understand, is a nobleman) why not be friendly and courteous withthem? I won't argue, I'll fall in with everything, I'll win them by politness, and show them that I've nothing to do with that Aesop, thta buffoon, that Pierrot, and have merely been takken in over this affair, just as they have.");
    std::string enc = tree.encode();
    std::string dec = tree.decode(enc);
    println("ENCODED: \n" + enc);
    printintln("\nENCODED LENGTH = ",enc.length());
    println("DECODED: " + dec);
    printintln("DECODED LENGTH = ",dec.length());
    println("Yay");
    system("pause");
    return 0; 
}
