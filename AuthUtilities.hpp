#ifndef AUTHUTILITIES_HPP
#define AUTHUTILITIES_HPP

#include <string>
#include "User.hpp"
#include "mainwindow.h"

#define TIME_INTERVAL 600 // 600 seconds is 10 min

// typedef struct PyInfo {
//     PyObject * name = nullptr;              // The name of the python file
//     PyObject * load_module = nullptr;       // Load in the file
//     PyObject * func = nullptr;              // The name of the function
//     PyObject * callfunc = nullptr;          // Calls the function and holds return value
//     PyObject * args = nullptr;              // arguements for the function
// } PyInfo_t;
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
void Verify2FACode(std::string key, int interval);

#endif // AUTHUTILITIES_HPP
