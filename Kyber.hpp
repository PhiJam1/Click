#ifndef KYBER_HPP
#define KYBER_HPP
#include <string>

std::string KyberEncryptDriverPassword(std::string plaintext, std::string password);
std::string KyberDecryptDriverPassword(std::string ciphertext, std::string password);

#endif // KYBER_HPP
