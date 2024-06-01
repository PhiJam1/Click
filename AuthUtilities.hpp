#ifndef AUTHUTILITIES_HPP
#define AUTHUTILITIES_HPP

#include <string>
#include "User.hpp"
#include "mainwindow.h"


// callback functions for database queries
int EmailInUse(void* data, int argc, char** argv, char** /* azColName */);
int GetSalt(void* data, int argc, char** argv, char** /* azColName */);
int CheckPassword(void* data, int argc, char** argv, char** /* azColName */);

// Functions that interface with the python code
bool Check2fa(std::string email);
void GetSaltAndHash(std::string& hash, const char * password);


#endif // AUTHUTILITIES_HPP
