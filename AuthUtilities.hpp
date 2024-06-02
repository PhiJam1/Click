#ifndef AUTHUTILITIES_HPP
#define AUTHUTILITIES_HPP

#include <string>
#include "User.hpp"
#include "mainwindow.h"

#define SYS_EMAIL "clickpmverify@gmail.com"
#define TIME_INTERVAL 600 // 600 seconds is 10 min


// callback functions for database queries
int EmailInUse(void* data, int argc, char** argv, char** /* azColName */);
int GetSalt(void* data, int argc, char** argv, char** /* azColName */);
int CheckPassword(void* data, int argc, char** argv, char** /* azColName */);

// Functions that interface with the python code
bool Check2fa(std::string email, Ui::Click * ui);
void GetSaltAndHash(std::string& hash, const char * password);


#endif // AUTHUTILITIES_HPP
