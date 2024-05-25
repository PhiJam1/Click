#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>

#include "XOR.hpp"

/* Password functions */

std::string xorEncryptPassword(std::string plaintext, std::string key) {
    char c;
    std::string ciphertext = "";
    //Xors each character in plaintext with a character in key
    for (int i = 0; i < plaintext.size(); i++) {
        c = plaintext.at(i);
        int x = c ^ key.at(i % key.length());
        ciphertext += (std::to_string(c ^ key.at(i % key.length())) + " ");
    }
    return ciphertext;
}

std::string xorDecryptPassword(std::vector<int> ciphertext, std::string key) {

    std::string plaintext = "";
    for (int i = 0; i < ciphertext.size(); i++) {
        char c = key[i % key.length()];
        //stoi() converts the string to an ascii integer
        c ^= ciphertext.at(i);
        plaintext += c;
    }
    return plaintext;
}

std::string advancedXorDecryptionPassword(std::vector<int> ciphertext, std::string key) {
    std::string plaintext = "";
    for (int i = 0; i < ciphertext.size(); i++) {
        char c = key[i % key.length()];
        c ^= ciphertext.at(i);
        plaintext += c;
        key[i % key.length()] = ((key[i % key.length()] + 1) % 127) + 1;
    }
    return plaintext;
}

std::string advancedXorEncryptionPassword(std::string plaintext, std::string key) {
    std::string ciphertext = "";

    //Xors each character in plaintext with a character in key
    for (int i = 0; i < plaintext.size(); i++) {
        ciphertext += std::to_string((plaintext.at(i) ^ key.at(i % key.length()))) + " ";
        key[i % key.length()] = ((key[i % key.length()] + 1) % 127) + 1;
    }
    return ciphertext;
}

bool advancedXorEncryptionFile(std::string key) {
    //open plaintext file
    std::ifstream plaintextFile;
    plaintextFile.open("../ClickGUI/plaintext.txt");

    if (!plaintextFile.is_open()) {
        std::cout << "Plaintext file could not be opened\n";
        return false;
    }

    //Open cipher text file
    std::ofstream ciphertextFile;
    ciphertextFile.open("../ClickGUI/ciphertext.txt");

    if (!ciphertextFile.is_open()) {
        std::cout << "Error writing data\n";
        return false;
    }

    std::cout << "Encrypting ..." << std::endl;
    char c;

    //Xors each character in plaintext with a character in key
    for (int i = 0; plaintextFile.get(c); i++) {
        ciphertextFile << (c ^ key.at(i % key.length())) << " ";
        int x = ((key[i % key.length()] + 1) % 127) + 1;
        key[i % key.length()] = ((key[i % key.length()] + 1) % 127) + 1;
        //std::cout << key << " - > " << x << std::endl;
    }

    //close files
    ciphertextFile.close();
    plaintextFile.close();

    std::cout << "Data have been saved to 'ciphertext.txt'" << std::endl;
    return true;
}



bool advancedXorDecryptionFile(std::string key) {
    //open encrypted file for reading
    std::ifstream cipherFile;
    cipherFile.open("../ClickGUI/ciphertext.txt");

    if (cipherFile.fail()) {
        //file could not be opened for reading
        std::cout << "Ciphertext file could not be opened\n";
        return false;
    }

    std::ofstream plainFile;
    plainFile.open("../ClickGUI/plaintext.txt");
    if (plainFile.fail()) {
        //File could not be opened for writing
        std::cout << "Plaintext file could not be opened\n";
        return false;
    }

    std::cout << "Decrypting data... " << std::endl;
    std::string buff = "";
    for (int i = 0; cipherFile >> buff; i++) {
        char c = key[i % key.length()];
        //stoi() converts the string to an ascii integer
        c ^= stoi(buff);
        plainFile << c;
        key[i % key.length()] = ((key[i % key.length()] + 1) % 127) + 1;
    }

    //close files
    plainFile.close();
    cipherFile.close();

    std::cout << "Data have been saved to 'plaintext.txt'" << std::endl;
    return true;
}
