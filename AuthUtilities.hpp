#ifndef AUTHUTILITIES_HPP
#define AUTHUTILITIES_HPP

#include <string>
#include "User.hpp"
#include "mainwindow.h"

// functions to create accounts and log users in
User* Login(bool new_user);
User* NewAccount(Ui::Click *ui);

// callback functions
int EmailInUse(void* data, int argc, char** argv, char** /* azColName */);
int GetSalt(void* data, int argc, char** argv, char** /* azColName */);
int CheckPassword(void* data, int argc, char** argv, char** /* azColName */);

// helper function
std::string GenSalt();
bool ValidPassword(std::string password, Ui::Click *ui);
void GetSaltAndHash(std::string& hash, const char * password);
#endif // AUTHUTILITIES_HPP
