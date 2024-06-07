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
#include "AuthUtilities.hpp";
#include "Secrets.hpp"
#define PY_ERROR_MSG "Error with python function\n"

// This fails when the Python include file (and <Python.h>) is added to AuthUtilities.h
// I'm not sure why this is.

typedef struct PyInfo {
    PyObject * name = nullptr;
    PyObject * load_module = nullptr;
    PyObject * func = nullptr;
    PyObject * callfunc = nullptr;
    PyObject * args = nullptr;
} PyInfo;


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

void PythonInit() {
    setenv("PYTHONPATH","./../../",1);
    Py_Initialize();
}

void PythonCleanUp(PyInfo& info) {

    if (info.name != nullptr) {
        Py_DECREF(info.name);
        info.name = nullptr;
    }
    if (info.load_module != nullptr) {
        Py_DECREF(info.load_module);
        info.load_module = nullptr;
    }
    if (info.func != nullptr) {
        Py_DECREF(info.func);
        info.func = nullptr;
    }
    if (info.callfunc != nullptr) {
        Py_DECREF(info.callfunc);
        info.callfunc = nullptr;
    }
    if (info.args != nullptr) {
        Py_DECREF(info.args);
        info.args = nullptr;
    }
}

void PythonDestroy() {
    Py_Finalize();
}

/*
 * Callback function that will only return the OK value
 * if the password+salt is valid.
*/
int CheckPassword(void* data, int argc, char** argv, char** /* azColName */) {

    auto& [known_hash, salt] = *static_cast<std::pair<std::string, std::string> *>(data);
    // argv[0] is the password hash.
    PyInfo info;
    info.name = PyUnicode_FromString((char*)"AuthUtils");
    if (info.name == nullptr) {
        PythonCleanUp(info);
        throw NAME_ERROR;
    }


    info.load_module = PyImport_Import(info.name);
    if (info.load_module == nullptr) {
        PythonCleanUp(info);
        throw MODULE_ERROR;
    }

    // The actual function we need
    info.func = PyObject_GetAttrString(info.load_module, (char*)"Verify_Password");
    if (info.func == nullptr) {
        PythonCleanUp(info);
        throw FUNC_ERROR;
    }

    info.args = PyTuple_Pack(2, PyUnicode_FromString(known_hash.c_str()), PyUnicode_FromString(argv[0]));
    if (info.args == nullptr) {
        PythonCleanUp(info);
        throw ARGS_ERROR;
    }

    // This calls  the  function
    info.callfunc = PyObject_CallObject(info.func, info.args);
    if (info.callfunc == nullptr) {
        PythonCleanUp(info);
        throw CALLING_ERROR;
    }

    double ret = PyFloat_AsDouble(info.callfunc);

    if (PyErr_Occurred()) {
        PyErr_Print();
    }
    // Free all memory
    PythonCleanUp(info);

    return ret == 1.0 ? SQLITE_OK : SQLITE_AUTH;
}


void GetSaltAndHash(std::string& hash, const char * password) {

    PyInfo info;
    info.name = PyUnicode_FromString((char*)"AuthUtils");
    if (info.name == nullptr) {
        PythonCleanUp(info);
        throw NAME_ERROR;
    }


    info.load_module = PyImport_Import(info.name);
    if (info.load_module == nullptr) {
        PythonCleanUp(info);
        throw MODULE_ERROR;
    }

    // The actual function we need
    info.func = PyObject_GetAttrString(info.load_module, (char*)"Register_Password");
    if (info.func == nullptr) {
        PythonCleanUp(info);
        throw FUNC_ERROR;
    }

    info.args = PyTuple_Pack(1, PyUnicode_FromString(password));
    if (info.args == nullptr) {
        PythonCleanUp(info);
        throw ARGS_ERROR;
    }

    // This calls  the  function
    info.callfunc = PyObject_CallObject(info.func, info.args);
    if (info.callfunc == nullptr) {
        PythonCleanUp(info);
        throw CALLING_ERROR;
    }

    hash = _PyUnicode_AsString(info.callfunc);

    if (PyErr_Occurred()) {
        PyErr_Print();
    }

    // Free all memory
    PythonCleanUp(info);
}

// Function that will call on python functions
// to send an email with verification code for 2fa.

std::string Send2FACode(std::string email, int interval) {
    PyInfo info;
    info.name = PyUnicode_FromString((char*)"AuthUtils");
    if (PyErr_Occurred()) PyErr_Print();
    if (info.name == nullptr) {
        PythonCleanUp(info);
        throw NAME_ERROR;
    }


    info.load_module = PyImport_Import(info.name);
    if (PyErr_Occurred()) PyErr_Print();
    if (info.load_module == nullptr) {
        PythonCleanUp(info);
        throw MODULE_ERROR;
    }

    // The actual function we need
    info.func = PyObject_GetAttrString(info.load_module, (char*)"GenerateOTP");
    if (PyErr_Occurred()) PyErr_Print();
    if (info.func == nullptr) {
        PythonCleanUp(info);
        throw FUNC_ERROR;
    }

    info.args = PyTuple_Pack(4,
                        PyUnicode_FromString(email.c_str()),
                        PyUnicode_FromString(SYS_EMAIL),
                        PyUnicode_FromString(SYS_EMAIL_KEY),
                        PyLong_FromLong(TIME_INTERVAL)
                        );
    if (info.args == nullptr) {
        PythonCleanUp(info);
        throw ARGS_ERROR;
    }

    // This calls  the  function
    info.callfunc = PyObject_CallObject(info.func, info.args);
    if (PyErr_Occurred()) PyErr_Print();
    if (info.callfunc == nullptr) {
        PythonCleanUp(info);
        throw CALLING_ERROR;
    }

    std::string key = _PyUnicode_AsString(info.callfunc);

    if (PyErr_Occurred()) {
        PyErr_Print();
    }

    // Free all memory
    PythonCleanUp(info);
    std::cout << key << std::flush;

    return key;
}

bool Verify2FACode(std::string key, int interval) {
    return true;
}
