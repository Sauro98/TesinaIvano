# TesinaIvano

<b>General info</b><br>
I'm trying to implement a ZIP/DEFLATE compressor and decompressor that will allow users to crypt their compressed files
<ul>
<li>light crypting with DES</li>
<li>heavy crypting with 3DES</li>
</ul>

<b>Current status: </b>
<ul>
  <li>Des crypter working fine</li>
  <li>Huffman coding (original style) working fine</li>
  <li>LZ77 (original version) working fine</li>
  <li>DEFLATE COMPRESSION:
    <ul>
    <li>Uncompressed format yet to implement</li>
    <li>Static tree format working fine, tested with infgen</li>
    <li>Dynamic tree format working fine</li>
    </ul>
    <br>
    Compressed blocks formats were tested with <a href = "https://github.com/madler/infgen">infgen</a> 
  </li>
  <li>Optimizations
     <ul>
     <li>faster lz77 implementation working fine<\li>
     <li>fixed length Huffman trees: Work in progress<\li>
     <\ul>
  <\li>
 </ul>
 </br>
 This project is entirely written in c++
