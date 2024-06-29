#include <iostream>
#include <bits/stdc++.h>
#include <sqlite3.h>
#include </usr/include/python3.12/Python.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <sstream>

#include <sstream>
#include <vector>

#include "User.hpp"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "AuthUtilities.hpp";
#include "Secrets.hpp"
#include "Kyber.hpp"
#define PY_ERROR_MSG "Error with python function\n"
typedef struct PyInfo {
    PyObject * name = nullptr;
    PyObject * load_module = nullptr;
    PyObject * func = nullptr;
    PyObject * callfunc = nullptr;
    PyObject * args = nullptr;
} KPyInfo;


void KPythonCleanUp(PyInfo& info) {

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

std::string KyberEncryptDriverPassword(std::string plaintext, std::string password) {
    std::vector<std::vector<int>> u;
    std::vector<int> v;

    KPyInfo info;
    info.name = PyUnicode_FromString((char*)"Kyber");
    if (PyErr_Occurred()) PyErr_Print();
    if (info.name == nullptr) {
        KPythonCleanUp(info);
        throw NAME_ERROR;
    }


    info.load_module = PyImport_Import(info.name);
    if (PyErr_Occurred()) PyErr_Print();
    if (info.load_module == nullptr) {
        KPythonCleanUp(info);
        throw MODULE_ERROR;
    }

    // The actual function we need
    info.func = PyObject_GetAttrString(info.load_module, (char*)"EncryptionDriver");
    if (PyErr_Occurred()) PyErr_Print();
    if (info.func == nullptr) {
        KPythonCleanUp(info);
        throw FUNC_ERROR;
    }

    info.args = PyTuple_Pack(2,
                             PyUnicode_FromString(plaintext.c_str()),
                             PyUnicode_FromString(password.c_str())
                             );
    if (info.args == nullptr) {
        KPythonCleanUp(info);
        throw ARGS_ERROR;
    }

    // This calls  the  function
    info.callfunc = PyObject_CallObject(info.func, info.args);
    if (PyErr_Occurred()) PyErr_Print();
    if (info.callfunc == nullptr) {
        KPythonCleanUp(info);
        throw CALLING_ERROR;
    }

    // Get the return values. We should get (u, v).
    PyObject * t1 = PyTuple_GetItem(info.callfunc, 0);
    PyObject * t2 = PyTuple_GetItem(info.callfunc, 1);

    Py_ssize_t outerSize = PyList_Size(t1);
    for (Py_ssize_t i = 0; i < outerSize; ++i) {
        PyObject *innerList = PyList_GetItem(t1, i);
        Py_ssize_t innerSize = PyList_Size(innerList);
        std::vector<int> buff;
        for (Py_ssize_t j = 0; j < innerSize; ++j) {
            PyObject *item = PyList_GetItem(innerList, j);
            long long value = PyLong_AsLongLong(item);
            buff.push_back(value);
        }
        u.push_back(buff);
        std::cout << std::flush;
    }

    // Process the second element (list of numpy.int64)
    Py_ssize_t size = PyList_Size(t2);
    for (Py_ssize_t i = 0; i < size; ++i) {
        PyObject *item = PyList_GetItem(t2, i);
        long long value = PyLong_AsLongLong(item);
        v.push_back(value);

    }
    std::string ciphertext = "";
    for (auto i = 0; i < u.size(); i++) {
        for (auto j = 0; j < u.at(i).size(); j++) {
            ciphertext += std::to_string(u.at(i).at(j))
                        + (j + 1 < u.at(i).size() ? "," : "!");
        }
    }
    for (auto i = 0; i < v.size(); i++) {
        ciphertext += std::to_string(v.at(i))
                    + (i + 1 < v.size() ? "," : "");
    }
    std::cout << std::endl;
    if (PyErr_Occurred()) {
        PyErr_Print();
    }

    // Free all memory
    KPythonCleanUp(info);

    return ciphertext;
}

std::string KyberDecryptDriverPassword(std::string ciphertext, std::string password) {
    // Send this information to python function
    KPyInfo info;
    info.name = PyUnicode_FromString((char*)"Kyber");
    if (PyErr_Occurred()) PyErr_Print();
    if (info.name == nullptr) {
        KPythonCleanUp(info);
        throw NAME_ERROR;
    }


    info.load_module = PyImport_Import(info.name);
    if (PyErr_Occurred()) PyErr_Print();
    if (info.load_module == nullptr) {
        KPythonCleanUp(info);
        throw MODULE_ERROR;
    }

    // The actual function we need
    info.func = PyObject_GetAttrString(info.load_module, (char*)"DecryptionDriver");
    if (PyErr_Occurred()) PyErr_Print();
    if (info.func == nullptr) {
        KPythonCleanUp(info);
        throw FUNC_ERROR;
    }

    info.args = PyTuple_Pack(2,
                             PyUnicode_FromString(password.c_str()),
                             PyUnicode_FromString(ciphertext.c_str())
                             );
    if (info.args == nullptr) {
        KPythonCleanUp(info);
        throw ARGS_ERROR;
    }

    // This calls  the  function
    info.callfunc = PyObject_CallObject(info.func, info.args);
    if (PyErr_Occurred()) PyErr_Print();
    if (info.callfunc == nullptr) {
        KPythonCleanUp(info);
        throw CALLING_ERROR;
    }
    std::string plaintext = _PyUnicode_AsString(info.callfunc);

    // Process the second element (list of numpy.int64)

    std::cout << std::endl;
    if (PyErr_Occurred()) {
        PyErr_Print();
    }

    // Free all memory
    KPythonCleanUp(info);
    return plaintext;
}
