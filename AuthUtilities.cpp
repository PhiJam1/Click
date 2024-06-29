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
#define PY_ERROR_MSG "Error with python function\n"

// This fails when the Python include file (and <Python.h>) is added to AuthUtilities.h
// I'm not sure why this is. StackOverFlow says there may be an issue in the Python lib

typedef struct PyInfo {
    PyObject * name = nullptr;
    PyObject * load_module = nullptr;
    PyObject * func = nullptr;
    PyObject * callfunc = nullptr;
    PyObject * args = nullptr;
} PyInfo;


/* Callback functions */

/*
 * Callback function to set the mac address
 *
 */
int GetMacAddresses(void* data, int argc, char** argv, char** /* azColName */) {
    // data will be a pointer to a std::string that we assign the mac address to
    if (argc == 0) {
        return SQLITE_NOTFOUND;
    }
    std::string * user_data = static_cast<std::string *> (data);
    *user_data = *argv;
    return SQLITE_OK;
}

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

bool Verify2FACode(std::string code, std::string TFAkey, int interval) {
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
    info.func = PyObject_GetAttrString(info.load_module, (char*)"CheckOTP");
    if (PyErr_Occurred()) PyErr_Print();
    if (info.func == nullptr) {
        PythonCleanUp(info);
        throw FUNC_ERROR;
    }

    info.args = PyTuple_Pack(3,
                             PyUnicode_FromString(code.c_str()),
                             PyUnicode_FromString(TFAkey.c_str()),
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

    double ret = PyFloat_AsDouble(info.callfunc);

    if (PyErr_Occurred()) {
        PyErr_Print();
    }

    // Free all memory
    PythonCleanUp(info);
    // 1.0 return value signifies the code was verified.
    return ret == 1.0;
}
// This function will check if the current mac address is a new mac address or if it is
// part of the banned group. In either case, it will send out a relevent email.
// It will only return false if the current mac address is banned or there is an error
bool CheckMacAddress(std::string email) {
    std::string currMacAddr = GetMacAddress();

    // open the database and get the list of known mac addresses for this user.
    sqlite3 * db;
    int rc = sqlite3_open("creds.db", &db);
    if (rc) {
        return false;
    }

    // create a table if none exists for known mac addresses
    const char * create_table_known_mac = "CREATE TABLE IF NOT EXISTS known_devices ("
                                         "email TEXT PRIMARY KEY, "
                                         "mac_address TEXT);";
    rc = sqlite3_exec(db, create_table_known_mac, 0, 0, 0);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return false;
    }

    // get the mac addresses associated with this email.
    std::string check_email =  "SELECT mac_address FROM known_devices WHERE email = '" + email + "';";
    std::string macList = "";
    rc = sqlite3_exec(db, check_email.c_str(), GetMacAddresses, (void *) &macList, 0);

    // Turn the string to a vector
    std::vector<std::string> macs;
    GenerateMacList(macs, macList);
    // Check if it is a known mac address
    // This function will send out an email if the current mac address is unknown
    CheckKnownMacAddress(macs, currMacAddr, email);

    // Get the list of hardware banned devices
    const char * create_table_banned_mac = "CREATE TABLE IF NOT EXISTS banned_devices ("
                                         "email TEXT PRIMARY KEY, "
                                         "mac_address TEXT);";
    rc = sqlite3_exec(db, create_table_banned_mac, 0, 0, 0);
    std::string getBannedDev =  "SELECT mac_address FROM banned_devices WHERE email = '" + email + "';";
    macList = "";
    rc = sqlite3_exec(db, getBannedDev.c_str(), GetMacAddresses, (void *) &macList, 0);
    if (rc != SQLITE_OK) {
        return false;
    }
    sqlite3_close(db);
    db = nullptr;

    macs.clear();
    GenerateMacList(macs, macList);
    return !isBanned(macs, currMacAddr, email);
}

// This fucntion is called when a device attempts to login in 3 times unsuccessfully with
// the OTP or login.
void SendLoginAttemptWarning(std::string email) {

    // First, grab the mac address of the current machine trying to login
    std::string macAddr = GetMacAddress();
    if (macAddr == "") { // there was some error getting make address
        std::cout << "Error getting mac address\n";
        return;
    }

    // Now send out a warning email. 2FA has kept their information safe (at least thus far), but
    // someone knowns their email and password.

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
    info.func = PyObject_GetAttrString(info.load_module, (char*)"SendWarningEmail");
    if (PyErr_Occurred()) PyErr_Print();
    if (info.func == nullptr) {
        PythonCleanUp(info);
        throw FUNC_ERROR;
    }

    info.args = PyTuple_Pack(4,
                            PyUnicode_FromString(email.c_str()),
                            PyUnicode_FromString(SYS_EMAIL),
                            PyUnicode_FromString(SYS_EMAIL_KEY),
                            PyUnicode_FromString(macAddr.c_str())
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

    if (PyErr_Occurred()) {
        PyErr_Print();
    }

    // Free all memory
    PythonCleanUp(info);
}


// Function that will get the permentant mac address of the device
// This method may not work for all linux distros. It depends on where
// macaddress is stored and if the interface name (wlp0s20f3, in this case)
// is consistent with the machines I tested on.
std::string GetMacAddress() {
    std::string macAddress;
    std::string path = "/sys/class/net/wlp0s20f3/device/ieee80211/phy0/macaddress";
    std::ifstream ifs(path.c_str());

    if (ifs) {
        std::getline(ifs, macAddress);
    } else {
        std::cerr << "Failed to read MAC address from " << path << std::endl;
    }

    return macAddress;
}

// Pretty much will preform the python spilt($) function
// Format will be 'xx:xx$xx:xx:$
void GenerateMacList(std::vector<std::string>& macs, std::string macAddr) {
    std::string buff = "";
    macs.clear();
    for (char c : macAddr) {
        if (c == '$') {
            macs.push_back(buff);
            buff = "";
        } else {
            buff += c;
        }
    }
}

// function that will check if the device that was just signed into is new.
// If it is, nothing will happen. If it is not, then send an email to the user
// with this warning.
void CheckKnownMacAddress(std::vector<std::string>& macs, std::string& currMacAddr, std::string & email) {
    std::string total = "";
    for (unsigned long i = 0; i < macs.size(); i++) {
        if (macs.at(i) == currMacAddr) {
            return; // This device is recognized
        }
        total += macs.at(i) + "$";
    }
    total += currMacAddr + "$";
    // if we get here, then this must be a new device
    // Add this new mac address to the database

    // This is a new user. Add the current mac address to the list
    sqlite3 * db;
    int rc = sqlite3_open("creds.db", &db);
    if (rc) {
        return;
    }
    std::string insert = "INSERT INTO known_devices (email, mac_address) VALUES "
                         "('" + email + "', '" + currMacAddr + "$');";

    rc = sqlite3_exec(db, insert.c_str(), 0, 0, 0);
    sqlite3_close(db);

    // Now send out the warning email
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
    info.func = PyObject_GetAttrString(info.load_module, (char*)"NewDeviceDetected");
    if (PyErr_Occurred()) PyErr_Print();
    if (info.func == nullptr) {
        PythonCleanUp(info);
        throw FUNC_ERROR;
    }

    info.args = PyTuple_Pack(4,
                             PyUnicode_FromString(email.c_str()),
                             PyUnicode_FromString(SYS_EMAIL),
                             PyUnicode_FromString(SYS_EMAIL_KEY),
                             PyUnicode_FromString(currMacAddr.c_str())
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

    if (PyErr_Occurred()) {
        PyErr_Print();
    }

    // Free all memory
    PythonCleanUp(info);
}


bool isBanned(std::vector<std::string>& bannedMacs, std::string currMacAddr, std::string email) {
    for (unsigned long i = 0; i< bannedMacs.size(); i++) {
        if (bannedMacs.at(i) == currMacAddr) {
            // The current device has been banned. Send an email warning the user of the login attempt
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
            info.func = PyObject_GetAttrString(info.load_module, (char*)"BannedDeviceLoginAttempt");
            if (PyErr_Occurred()) PyErr_Print();
            if (info.func == nullptr) {
                PythonCleanUp(info);
                throw FUNC_ERROR;
            }

            info.args = PyTuple_Pack(4,
                                     PyUnicode_FromString(email.c_str()),
                                     PyUnicode_FromString(SYS_EMAIL),
                                     PyUnicode_FromString(SYS_EMAIL_KEY),
                                     PyUnicode_FromString(currMacAddr.c_str())
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

            if (PyErr_Occurred()) {
                PyErr_Print();
            }

            // Free all memory
            PythonCleanUp(info);
            return true;
        }
    }
    return false;
}
