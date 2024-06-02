#include <iostream>
#include <cstdlib>
#include <cctype>
#include <ctime>
#include <bits/stdc++.h>
#include <sqlite3.h>
#include </usr/include/python3.12/Python.h>

#include "User.hpp"
// #include "bcrypt.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "AuthUtilities.hpp"
#define PY_ERROR_MSG "Error with python function\n"





/* Callback functions */


/*
 * Callback function. If this function is ever called, the query will have found a
 * match for the provided email. Of course, that means it is already in use and we need
 * to return with 1 to prevent the OK value.
*/
int EmailInUse(void* data, int argc, char** argv, char** /* azColName */) {
    return 1;
}

/*
 * Callback function that will set the salt to what the query returns.
*/
int GetSalt(void* data, int argc, char** argv, char** /* azColName */) {
    std::vector<std::string *>  user_data = * static_cast<std::vector<std::string *> *>(data);
    if (argc == 0) { // means the query did not get any hits. user/email DNE
        return 1;
    }
    *(user_data.at(0)) = argv[0];
    *(user_data.at(1)) = argv[1];
    *(user_data.at(2)) = argv[2];

    return 0;
}

/*
 * Callback function that will only return the OK value
 * if the password+salt is valid.
*/
int CheckPassword(void* data, int argc, char** argv, char** /* azColName */) {
    auto& [known_hash, salt] = *static_cast<std::pair<std::string, std::string> *>(data);
    // argv[0] is the password hash.


    // Tells the linker to check the current directory for py files
    setenv("PYTHONPATH","./../../",1);      // Set as needed
    Py_Initialize();
    PyObject * name = nullptr;              // The name of the python file
    PyObject * load_module = nullptr;       // Load in the file
    PyObject * func = nullptr;              // The name of the function
    PyObject * callfunc = nullptr;          // Calls the function and holds return value
    PyObject * args = nullptr;              // arguements for the function

    name = PyUnicode_FromString((char*)"AuthUtils");
    if (name == nullptr) {
        std::cout << PY_ERROR_MSG;
        Py_Finalize();
        return 0;
    }


    load_module = PyImport_Import(name);
    if (load_module == nullptr) {
        std::cout << PY_ERROR_MSG;
        Py_DECREF(name);
        Py_Finalize();
        return 0;
    }

    // The actual function we need
    func = PyObject_GetAttrString(load_module, (char*)"Verify_Password");
    if (func == nullptr) {
        std::cout << PY_ERROR_MSG;
        Py_DECREF(load_module);
        Py_DECREF(name);
        return 0;
    }

    args = PyTuple_Pack(2, PyUnicode_FromString(known_hash.c_str()), PyUnicode_FromString(argv[0]));
    std::cout << known_hash << std::flush;
    std::cout << std::endl;
    std::cout << argv[0] << std::endl << std::flush;
    if (args == nullptr) {
        std::cout << PY_ERROR_MSG;
        Py_DECREF(func);
        Py_DECREF(load_module);
        Py_DECREF(name);
        return 0;
    }

    // This calls  the  function
    callfunc = PyObject_CallObject(func, args);
    if (callfunc == nullptr) {
        std::cout << PY_ERROR_MSG;
        Py_DECREF(load_module);
        Py_DECREF(func);
        Py_DECREF(name);
        return 0;
    }

    double ret = PyFloat_AsDouble(callfunc);
  // Free all memory
    Py_DECREF(args);
    Py_DECREF(callfunc);
    Py_DECREF(func);
    Py_DECREF(load_module);
    Py_DECREF(name);
    callfunc = nullptr;
    func = nullptr;
    load_module = nullptr;
    name = nullptr;
    args = nullptr;

    Py_Finalize();
    return ret == 1.0 ? SQLITE_OK : SQLITE_AUTH;
}


void GetSaltAndHash(std::string& hash, const char * password) {

    // Tells the linker to check the current directory for py files
    setenv("PYTHONPATH","./../../",1);      // Set as needed
    Py_Initialize();
    PyObject * name = nullptr;              // The name of the python file
    PyObject * load_module = nullptr;       // Load in the file
    PyObject * func = nullptr;              // The name of the function
    PyObject * callfunc = nullptr;          // Calls the function and hold return value
    PyObject * args = nullptr;              // arguements for the function

    name = PyUnicode_FromString((char*)"AuthUtils");
    if (name == nullptr) {
        std::cout << PY_ERROR_MSG;
        Py_Finalize();
        return;
    }


    load_module = PyImport_Import(name);
    if (load_module == nullptr) {
        std::cout << PY_ERROR_MSG;
        Py_DECREF(name);
        Py_Finalize();
        return;
    }

    // The actual function we need
    func = PyObject_GetAttrString(load_module, (char*)"Register_Password");
    if (func == nullptr) {
        std::cout << PY_ERROR_MSG;
        Py_DECREF(load_module);
        Py_DECREF(name);
        return;
    }

    args = PyTuple_Pack(1, PyUnicode_FromString(password));
    if (args == nullptr) {
        std::cout << PY_ERROR_MSG;
        Py_DECREF(func);
        Py_DECREF(load_module);
        Py_DECREF(name);
        return;
    }

    // This calls  the  function
    callfunc = PyObject_CallObject(func, args);
    if (callfunc == nullptr) {
        std::cout << PY_ERROR_MSG;
        Py_DECREF(load_module);
        Py_DECREF(func);
        Py_DECREF(name);
        return;
    }

    hash = _PyUnicode_AsString(callfunc);

    // Free all memory
    Py_DECREF(args);
    Py_DECREF(callfunc);
    Py_DECREF(func);
    Py_DECREF(load_module);
    Py_DECREF(name);
    callfunc = nullptr;
    func = nullptr;
    load_module = nullptr;
    name = nullptr;
    args = nullptr;

    Py_Finalize();
}

// Function that will call on python functions
// to send an email with verification code for 2fa.

std::string Send2FACode(std::string email, int interval) {

    // Tells the linker to check the current directory for py files
    setenv("PYTHONPATH","./../../",1);      // Set as needed
    Py_Initialize();
    PyObject * name = nullptr;              // The name of the python file
    PyObject * load_module = nullptr;       // Load in the file
    PyObject * func = nullptr;              // The name of the function
    PyObject * callfunc = nullptr;          // Calls the function and hold return value
    PyObject * args = nullptr;              // arguements for the function

    name = PyUnicode_FromString((char*)"AuthUtils");
    if (name == nullptr) {
        std::cout << PY_ERROR_MSG;
        Py_Finalize();
        return "";
    }


    load_module = PyImport_Import(name);
    if (load_module == nullptr) {
        std::cout << PY_ERROR_MSG;
        Py_DECREF(name);
        Py_Finalize();
        return "";
    }

    // The actual function we need
    func = PyObject_GetAttrString(load_module, (char*)"GenerateOTP");
    if (func == nullptr) {
        std::cout << PY_ERROR_MSG;
        Py_DECREF(load_module);
        Py_DECREF(name);
        return "";
    }

    args = PyTuple_Pack(3, PyUnicode_FromString(email.c_str()), PyUnicode_FromString(SYS_EMAIL), PyLong_FromLong(TIME_INTERVAL));
    if (args == nullptr) {
        std::cout << PY_ERROR_MSG;
        Py_DECREF(func);
        Py_DECREF(load_module);
        Py_DECREF(name);
        return "";
    }

    // This calls  the  function
    callfunc = PyObject_CallObject(func, args);
    if (callfunc == nullptr) {
        std::cout << PY_ERROR_MSG;
        Py_DECREF(load_module);
        Py_DECREF(func);
        Py_DECREF(name);
        return "";
    }

    std::string key = _PyUnicode_AsString(callfunc);

    // Free all memory
    Py_DECREF(args);
    Py_DECREF(callfunc);
    Py_DECREF(func);
    Py_DECREF(load_module);
    Py_DECREF(name);
    callfunc = nullptr;
    func = nullptr;
    load_module = nullptr;
    name = nullptr;
    args = nullptr;

    Py_Finalize();
    return key;
}

void Verify2FACode(std::string key, int interval) {

}
