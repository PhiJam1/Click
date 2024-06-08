#ifndef AUTHUTILITIES_HPP
#define AUTHUTILITIES_HPP
#include <string>

#include "User.hpp"
#include "mainwindow.h"

#define TIME_INTERVAL 600 // 600 seconds is 10 min


#define NAME_ERROR_MSG      "Issue with getting python file"
#define MODULE_ERROR_MSG    "Issue loading module"
#define FUNC_ERROR_MSG      "Issue loading function"
#define CALLING_ERROR_MSG   "Issue calling functions"
#define ARGS_ERROR_MSG      "Issue with args"

typedef enum {
    NAME_ERROR,
    MODULE_ERROR,
    FUNC_ERROR,
    CALLING_ERROR,
    ARGS_ERROR
} PythonError;

void PythonInit();
void PythonDestroy();

// callback functions for database queries
int EmailInUse(void* data, int argc, char** argv, char** /* azColName */);
int GetSalt(void* data, int argc, char** argv, char** /* azColName */);
int CheckPassword(void* data, int argc, char** argv, char** /* azColName */);

// Functions that interface with the python code
void GetSaltAndHash(std::string& hash, const char * password);
std::string Send2FACode(std::string email, int interval);
bool Verify2FACode(std::string key, std::string TFAkey, int interval);
void CheckForNewDevice(std::string email);
void SendLoginAttemptWarning(std::string email);

#endif // AUTHUTILITIES_HPP
