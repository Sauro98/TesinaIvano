# TesinaIvano
Backup for high school final project

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
    <li>Uncompressed format working fine</li>
    <li>Static tree format working fine</li>
    <li>Dynamic tree format yet to implement</li>
    </ul>
    Compressed blocks format is checked with <a href = "https://github.com/madler/infgen">infgen</a> 
  </li>
 </ul>
 </br>
 This project is entirely written in c++
