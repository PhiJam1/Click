#include <iostream>
#include <vector>
#include <exception>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <sqlite3.h>

#include "User.hpp"
#include "XOR.hpp"
#include "blowfish.hpp"
#include "Kyber.hpp"


int User::SetCiphers(char ** argv) {
    this->ciphers.push_back((CipherInfo) {(CipherType_t) std::stoi(argv[0]), argv[1], argv[2], argv[3]});
    return 0;

}
int SetCiphersCallBack(void* data, int argc, char** argv, char** /* azColName */) {
    User * obj = static_cast<User *>(data);
    return obj->SetCiphers(argv);
}

// This constructor is used for new users
User::User(std::string firstName, std::string lastName, std::string email, std::string password, std::string salt) {
    this->firstName = firstName;
    this->lastName = lastName;
    this->email = email;
    this->password = password;
    this->salt = salt;

    // get ciphers (if any) from the database
    sqlite3 * db;
    int rc = sqlite3_open("ciphers.db", &db);
    if (rc) {
        std::cout << "Error opening up user database\n";
        return;
    }
    std::string query = "SELECT type, service_name, username, ciphertext FROM " + this->email + ";";
    rc = sqlite3_exec(db, query.c_str(), SetCiphersCallBack, this, 0);
    if (rc != SQLITE_OK) std::cout << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db);
}

std::string User::getFirstName() {
    return this->firstName;
}

std::string User::getLastName() {
    return this->lastName;
}

std::string User::getEmail() {
    return this->email;
}

// this will overwrite everything alr there
void User::SaveUserData() {
    // open the database
    sqlite3 * db;
    int rc = sqlite3_open("ciphers.db", &db);
    if (rc) {
        std::cout << "Error opening the ciphers database\n";
        return;
    }
    // delete everything in this user's table so we can overwrite all the data

    std::string dropTableQuery = "DROP TABLE IF EXISTS " + this->email + ";";
    rc = sqlite3_exec(db, dropTableQuery.c_str(), 0, 0, 0);

    // recreate the table
    std::string create_table = "CREATE TABLE IF NOT EXISTS " + this->email + " (type TEXT, service_name TEXT, username TEXT, ciphertext TEXT);";
    rc = sqlite3_exec(db, create_table.c_str(), 0, 0, 0);
    if (rc != SQLITE_OK) {
        std::cout << "Error creating creds table\n";
        sqlite3_close(db);
        return;
    }

    for (int i = 0; i < ciphers.size(); i++) {
        std::string insert = "INSERT INTO " + this->email + " (type, service_name, username, ciphertext) VALUES (" +
                             "'" + std::to_string(ciphers.at(i).type) + "', " +
                             "'" + ciphers.at(i).service_name + "', " +
                             "'" + ciphers.at(i).username + "', " +
                             "'" + ciphers.at(i).ciphertext + "');";
        rc = sqlite3_exec(db, insert.c_str(), 0, 0, 0);
        std::cout << insert << std::endl;
        if (rc != SQLITE_OK) std::cout << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_close(db);
}

std::vector<std::string> User::get_service_namees() {
    std::vector<std::string> ret;
    for (unsigned long i = 0; i < this->ciphers.size(); i++) {
        ret.push_back(this->ciphers.at(i).service_name);
    }
    return ret;
}

void User::CreateCipher(std::string login_name, std::string username, std::string password_u, int idx) {
    CipherType_t type = idx == 0 ? XOR : BLOWFISH;
    std::string ciphertext = "";
    if (type == XOR) {
        ciphertext = advancedXorEncryptionPassword(password_u, password.substr(0,5));
    } else if (type == BLOWFISH) {
        ciphertext = EncryptDriverPassword(password_u, password.substr(0, 4)); // should rename that function
    } else if (type == KYBER) {
        ciphertext = KyberEncryptDriverPassword(password_u, password.substr(0, 8)); // should rename that function
    }
    ciphers.push_back((CipherInfo) {type, login_name, username, ciphertext});
    SaveUserData();
}

std::pair<std::string, std::string> User::RetrievePassword(std::string serviceName) {

    // get the index in the ciphers vector
    int idx = -1;
    std::pair<std::string, std::string> ret;
    ret.first = "ERROR"; ret.second = "ERROR";
    for (unsigned long i = 0; i < this->ciphers.size(); i++) {
        if (this->ciphers.at(i).service_name == serviceName) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        return ret;
    }

    std::string plaintext = "ERROR-1";
    if (ciphers.at(idx).type == XOR) {
        std::vector<int> ciphertext;
        std::stringstream iss(ciphers.at(idx).ciphertext);
        int num = -1;
        while (iss >> num) {
            ciphertext.push_back(num);
        }
        plaintext = advancedXorDecryptionPassword(ciphertext, password.substr(0, 5));
    } else if (ciphers.at(idx).type == BLOWFISH) {
        plaintext = DecryptDriverPassword(ciphers.at(idx).ciphertext, password.substr(0, 4));
    } else if (ciphers.at(idx).type == KYBER) {
        plaintext = KyberDecryptDriverPassword(ciphers.at(idx).ciphertext, password.substr(0, 8));
    }
    ret.first = ciphers.at(idx).username;
    ret.second = plaintext;
    return ret;
}

void User::DeleteData(std::string name) {
    for (unsigned long i = 0; i < this->ciphers.size(); i++) {
        if (this->ciphers.at(i).service_name == name) {
            ciphers.erase(ciphers.begin() + i);
            break;
        }
    }
    this->SaveUserData();
}

