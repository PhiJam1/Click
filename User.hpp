#ifndef USER_HPP
#define USER_HPP
#include <iostream>
#include <vector>

#include "CipherType.hpp"

#define PERSONAL_INFO_START_DELIMITER "++"
#define PERSONAL_INFO_END_DELIMITER "##"
#define CIPHER_STRUCT_ENDING_DELIMITER  "@@"
#define FILE_ENDING_DELIMITER "--"

typedef enum errors_t {
    REPEATED_LOGIN_CREATION,
    DATA_NOT_SAVED,
    USER_NOT_FOUND,
    CORRUPT_FILE
} errors_t;

class User {

private:
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string password;
    std::string salt;
    std::vector<CipherInfo> ciphers;

public:
    int SetCiphers(char ** argv);

    User(std::string firstName, std::string getLastName, std::string email, std::string password, std::string salt);

    std::string getFirstName();

    std::string getLastName();

    std::string getEmail();

    std::string ChangeEmail(std::string new_email);

    std::vector<std::string> get_service_namees();

    void SaveUserData();

    void CreateCipher(std::string login_name, std::string username, std::string password, int idx);

    std::pair<std::string, std::string> RetrievePassword(std::string serviceName);

    void DeleteData(std::string name);
};

#endif
