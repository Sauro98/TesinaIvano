# TesinaIvano

### General info:   
I'm trying to implement a ZIP/DEFLATE compressor and decompressor that will allow users to crypt their compressed files:
* light crypting with DES
* heavy crypting with 3DES

### Current status:

* Des crypter working fine
* Huffman coding (original style) working fine
* LZ77 (original version) working fine
* DEFLATE COMPRESSION:
    * Uncompressed format working fine, tested with infgen
    * Static tree format working fine, tested with infgen
    * Dynamic tree format working fine, tested with infgen
    * Compressed blocks formats were tested with <a href = "https://github.com/madler/infgen">infgen</a> 
* Optimizations
     * faster lz77 implementation with hash tables working fine
     * fixed length Huffman trees: Working fine
* Gzip file format working fine
* Sgzip file format working fine
  
 This project is entirely written in c++
[![Code Triagers Badge](https://www.codetriage.com/sauro98/zipsec/badges/users.svg)](https://www.codetriage.com/sauro98/zipsec)
