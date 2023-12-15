#ifndef AUTHUTILITIES_HPP
#define AUTHUTILITIES_HPP

#include <string>
#include "User.hpp"
#include "mainwindow.h"

// function to create accounts and log users in
User* Login(bool new_user);
User* NewAccount(Ui::MainWindow *ui);

// callback functions
int EmailInUse(void* data, int argc, char** argv, char** /* azColName */);
int GetSalt(void* data, int argc, char** argv, char** /* azColName */);
int CheckPassword(void* data, int argc, char** argv, char** /* azColName */);

// helper function
std::string GenSalt();
bool ValidPassword(std::string password, Ui::MainWindow *ui);


#endif // AUTHUTILITIES_HPP
