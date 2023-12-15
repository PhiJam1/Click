#ifndef XOR_H
#define XOR_H

#include <iostream>
#include <string>
#include <vector>

std::string xorEncryptPassword(std::string plaintext, std::string key);
std::string xorDecryptPassword(std::vector<int> ciphertext, std::string key);
std::string advancedXorDecryptionPassword(std::vector<int> ciphertext, std::string key);
std::string advancedXorEncryptionPassword(std::string plaintext, std::string key);
void advancedXorEncryptionFile(std::string plaintextFileName, std::string key);
void advancedXorDecryptionFile(std::string ciphertextFileName, std::string key);



#endif /* XOR_H */
