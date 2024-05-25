#ifndef BLOWFISH_HPP
#define BLOWFISH_HPP
#include <string>

/*
 * The following is an implementation of blowfish
 * TODO: brief explanation of how it works.
 */

std::string f(std::string plaintext);

void generateSubKeys(int *pBoxes);

void encrypt(std::string plaintext);

std::string EncryptDriverPassword(std::string plaintext, std::string key);

std::string DecryptDriverPassword(std::string ciphertext, std::string key);

std::string GetHexString(std::string str);

std::string GetStrFromHex(std::string str);

void KeyCleanUp();

#endif // BLOWFISH_HPP
