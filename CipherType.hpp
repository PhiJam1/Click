#ifndef CIPHERTYPE_HPP
#define CIPHERTYPE_HPP

#include <string>

typedef enum {
    XOR,
    BLOWFISH,
    AES,
    KYBER
} CipherType_t;

typedef struct {
    CipherType_t type;
    std::string service_name;
    std::string username;
    std::string ciphertext;
} CipherInfo;

#endif // CIPHERTYPE_HPP
