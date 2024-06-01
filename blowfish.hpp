#ifndef BLOWFISH_HPP
#define BLOWFISH_HPP
#include <string>

/*
 * The following is an implementation of blowfish
 * Blowfish is a type of symmetric Feistel cipher. It takes in 64 bits at a time
 * and has variable key length. The algortim uses an 18 subkeys held in the P array
 * and 4 substitution boxes held in the S array. These are standardized values and come
 * from the digits of pi. First we extend the key by xoring it with entries in the P
 * array. P[0] is xored with the first 32 bits of the input key, and so on.
 * We then run the plaintext through 16 rounds of encryption. For this, you split
 * up the 64 bit plaintext in a left side and right side.
 * For the new right side, you just assign the old left side. For the new left side,
 * you xor the left side with the corresponding P entry, then run that through a
 * funciton F. Then xor that output with the old right side.
 * The function splits up its 32 bit input into 4 8 bit groupings and prefroms a
 * number of bitwise XOR and ADD operations.
 *
 * More specific information can be found at:
 * https://www.geeksforgeeks.org/blowfish-algorithm-with-examples/#
 */


void generateSubKeys(int *pBoxes);
void keyInit(std::string key);
void KeyCleanUp();

std::string f(std::string plaintext);
std::string XORStr(std::string s1, std::string s2);

std::string encrypt(std::string plaintext);
std::string decrypt(std::string ciphertext);

std::string EncryptDriverPassword(std::string plaintext, std::string key);
std::string DecryptDriverPassword(std::string ciphertext, std::string key);

std::string GetHexString(std::string str);
std::string GetStrFromHex(std::string str);

#endif // BLOWFISH_HPP
