#ifndef XOR_H
#define XOR_H

#include <iostream>
#include <string>
#include <vector>


/*
 * This file contains three sets of XOR based algorithms.
 * The basic xor algorithms will take in a small plaintext string and key and xor them as expected.
 * The advanced password xor algorithms do the same, but increment the key value at each step. This
 * prevents a frequency analysis from breaking the cipher. An even more sophisticated scheme is to change
 * the increment amount by a less predictable amount by tying it to some function.
 * The file based algorithms do the same as the advanced xor ciphers, but work on large files. One thing
 * to note is that they operate on chunks of a file at a time, rather than reading in the whole file. This
 * lets it process arbitrarily large fits with a linear runtime and constant space complexity.
*/

std::string advancedXorDecryptionPassword(std::vector<int> ciphertext, std::string key);
std::string advancedXorEncryptionPassword(std::string plaintext, std::string key);
bool advancedXorEncryptionFile(std::string key);
bool advancedXorDecryptionFile(std::string key);


std::string xorEncryptPassword(std::string plaintext, std::string key);
std::string xorDecryptPassword(std::vector<int> ciphertext, std::string key);
#endif /* XOR_H */
