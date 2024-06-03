#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sqlite3.h>

#include "XOR.hpp"
#include "AuthUtilities.hpp"
#include <QPixmap>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Click)
{
    ui->setupUi(this);
    // Note, index 0 -> sign up page, index 1 -> login page, index -> main page
    ui->PageManager->setCurrentIndex(1);
    QPixmap img("../../MainPageImg.png");
    ui->LockIMG->setPixmap(img);
    ui->LockIMGSignUp->setPixmap(img);
    ui->LockIMG2fa->setPixmap(img);

    // Start up the python mods
    PythonInit();

}

MainWindow::~MainWindow()
{
    // free all python mods
    PythonDestroy();
    delete ui;
}


// This function is called when the login button
// on the login page is clicked. It should collect the
// data in each field and log the user in.
void MainWindow::on_LoginBTN_clicked() {
    QString username;
    QString password;

    // grab data from username and password field
    username = ui->EmailEntryLogin->toPlainText();
    password = ui->PasswordLogin->toPlainText();

    // check for empty entries
    if (username == "" || password == "") {
        ui->MessagePane->append("No field can be empty");
        return;
    }

    if (Login(username.toStdString(), password.toStdString())) {
        ui->PageManager->setCurrentIndex(9);
    }

}


void MainWindow::on_SignUpBTN_clicked()
{
    // data to be collected from text fields
    QString firstName;
    QString lastName;
    QString email;
    QString password;

    // grab the data from its field
    firstName = ui->FirstNameEntry->toPlainText();
    lastName = ui->LastNameEntry->toPlainText();
    email = ui->EmailEntry->toPlainText();
    password = ui->PasswordEntry->toPlainText();

    // check for empty entries
    if (firstName == "" || lastName == "" || email == "" || password == "") {
        ui->ErrorSignupTXT->append("No field can be empty");
    }

    if (NewAccount(firstName.toStdString(), lastName.toStdString(), email.toStdString(), password.toStdString())) {
        ui->PageManager->setCurrentIndex(1);
    }
}


void MainWindow::on_LoginRedirect_clicked()
{
    ui->PageManager->setCurrentIndex(1);
}


void MainWindow::on_SignUpRedirect_clicked()
{
   ui->PageManager->setCurrentIndex(0);
}

void MainWindow::on_CreateBTN_clicked()
{
    ui->PageManager->setCurrentIndex(3);
}


void MainWindow::on_CreateBackBTN_clicked()
{
    ui->PageManager->setCurrentIndex(2);
}


void MainWindow::on_CreateSaveBTN_clicked()
{
    QString serviceName;
    int cipherType; // 0 -> XOR, 1 -> blowfish
    QString username;
    QString password;

    // assign fields
    serviceName = ui->CreateServiceNameEntry->toPlainText();
    cipherType = ui->CreateCipherTypeEntry->currentIndex();
    username = ui->CreateUsernameEntry->toPlainText();
    password = ui->CreatePasswordEntry->toPlainText();

    // check for empty fields
    if (serviceName == "" || username == "" || password == "") {
        ui->CreatePanel->append("No fields can be empty!");
        return;
    }

    // check to see if this service is alr saved
    std::vector<std::string> names = user->get_service_namees();
    for (unsigned long i = 0; i < names.size(); i++) {
        if (names.at(i) == serviceName.toStdString()) {
            ui->CreatePanel->append("You already have a cipher saved for this service.!");
            return;
        }
    }

    // now save the cipher
    user->CreateCipher(serviceName.toStdString(), username.toStdString(), password.toStdString(), cipherType);
    ui->CreatePanel->append("Saved!");
}


void MainWindow::on_RetrieiveBTN_clicked()
{
    // get the list of saved ciphers for their service names
    std::vector<std::string> service_names = user->get_service_namees();
    std::cout << service_names.size() << std::endl;

    // clear anything currently in the drop down
    ui->RetrieveSelectEntry->clear();

    // add back the default value
    ui->RetrieveSelectEntry->addItem("None");

    // populate the rest with the users saved logins
    for (unsigned long i = 0; i < service_names.size(); i++) {
        ui->RetrieveSelectEntry->addItem(QString::fromStdString(service_names.at(i)));
    }

    ui->PageManager->setCurrentIndex(4);
}


void MainWindow::on_RetrieveBackBTN_clicked()
{
    ui->PageManager->setCurrentIndex(2);
}


void MainWindow::on_RetrieveGetBTN_clicked()
{
    std::string serviceName = ui->RetrieveSelectEntry->currentText().toStdString();
    if (serviceName == "None") {
        return;
    }
    std::pair<std::string, std::string> cred = user->RetrievePassword(serviceName);
    ui->RetrievePanel->setPlainText(QString::fromStdString(serviceName));
    std::string buff = "Username: " + cred.first;
    ui->RetrievePanel->append(QString::fromStdString(buff));
    buff = "Password: " + cred.second;
    ui->RetrievePanel->append(QString::fromStdString(buff));
}


void MainWindow::on_DeleteBack_clicked()
{
    ui->PageManager->setCurrentIndex(2);
}


void MainWindow::on_DeleteBTN_clicked()
{
    // populate the drop down
    std::vector<std::string> serviceNames = user->get_service_namees();

    ui->DeleteDropDown->clear();
    ui->DeleteDropDown->addItem("None");
    for (unsigned long i = 0; i < serviceNames.size(); i++) {
        ui->DeleteDropDown->addItem(QString::fromStdString(serviceNames.at(i)));
    }
    ui->PageManager->setCurrentIndex(5);
}


void MainWindow::on_DeleteBTN_2_clicked()
{
    std::string name = ui->DeleteDropDown->currentText().toStdString();
    if (name == "None") return;

    user->DeleteData(name);
    ui->DeleteDesc->append("Poof, gone...");
    ui->DeleteDropDown->removeItem(ui->DeleteDropDown->currentIndex());
}


void MainWindow::on_EncryptFileBack_clicked()
{
    ui->PageManager->setCurrentIndex(2);
}


void MainWindow::on_EncryptFileBTN_2_clicked()
{
    std::string key = ui->EncryptFileKey->toPlainText().toStdString();
    ui->EncryptFileDesc->appendPlainText(advancedXorEncryptionFile(key) ? "Success" : "There was an error");
}


void MainWindow::on_EncryptFileBTN_clicked()
{
    ui->PageManager->setCurrentIndex(6);
}


void MainWindow::on_DecryptBackBTN_clicked()
{
    ui->PageManager->setCurrentIndex(2);
}


void MainWindow::on_DecryptFileBTN_clicked()
{
    ui->PageManager->setCurrentIndex(7);
}


void MainWindow::on_DecryptDecryptBTN_clicked()
{
    std::string key = ui->DecryptKey->toPlainText().toStdString();
    ui->DecryptDesc->appendPlainText(advancedXorDecryptionFile(key) ? "Success" : "There was an error");
}


void MainWindow::on_DecryptBackBTN_2_clicked()
{
    ui->PageManager->setCurrentIndex(user == nullptr ? 1 : 2);
}


void MainWindow::on_AboutBTN_clicked()
{
    ui->PageManager->setCurrentIndex(8);
}


void MainWindow::on_LogInBTN_clicked()
{
    if (user != nullptr) {
        ui->LogInBTN->setText("Log in");
        user = nullptr;
    }
    ui->PageManager->setCurrentIndex(1);
}



/*
 * Function to login a user. It will ask for credentials
 * and compare that to what's stored in the database.
 * The new_user parameter is only set to true if the NewAccount()
 * function is calling this function (NewAccount should redirect to login)
 * It just tells the function there is no saved data to load.
*/

bool MainWindow::Login(std::string email, std::string password) {
    // user information
    // std::cout << Send2FACode("Philipjames2004@gmail.com", 500) << std::flush;
    std::string salt = " ";
    std::string first_name = " ";
    std::string last_name = " ";



    // make email fully lower case
    std::transform(email.begin(), email.end(), email.begin(), [](unsigned char c){ return std::tolower(c); });



    sqlite3* db;
    int rc = sqlite3_open("creds.db", &db);
    if (rc) {
        ui->MessagePane->append("Cannot open database: ");
        ui->MessagePane->append(sqlite3_errmsg(db));
        return false;
    }


    // get the salt for this username
    std::vector<std::string *> userData = {&salt, &first_name, &last_name};
    std::string email_command = "SELECT salt, first_name, last_name FROM credentials WHERE email = '" + email + "';";
    rc = sqlite3_exec(db, email_command.c_str(), GetSalt, &userData, 0); // shallow copy of salt NOT fine
    // if the salt remains empty, that means the email did not have any saved data.
    if (false && salt == " ") {
        ui->MessagePane->append("Invalid Login. Try Again\n");
        sqlite3_close(db);
        return false;
    }

    // if we get the salt, get and check the password.
    std::string selectDataSQL = "SELECT password_hash FROM credentials WHERE email = '" + email + "';";
    std::pair<std::string, std::string> tmp = {password, salt}; // shallow copies are fine here
    try {
        rc = sqlite3_exec(db, selectDataSQL.c_str(), CheckPassword, &tmp, 0);
    } catch (PythonError NAME_ERROR) {
        std::cout << NAME_ERROR_MSG << " FOR CHECKPASSWORD() " << std::endl;
        ui->MessagePane->append(ERROR_PYTHON_LIB);
    } catch (PythonError MODULE_ERROR) {
        std::cout << MODULE_ERROR_MSG << " FOR CHECKPASSWORD() " << std::endl;
        ui->MessagePane->append(ERROR_PYTHON_LIB);
    } catch (PythonError FUNC_ERROR) {
        std::cout << FUNC_ERROR_MSG << " FOR CHECKPASSWORD() " << std::endl;
        ui->MessagePane->append(ERROR_PYTHON_LIB);
    } catch (PythonError CALLING_ERROR) {
        std::cout << CALLING_ERROR_MSG << " FOR CHECKPASSWORD() " << std::endl;
        ui->MessagePane->append(ERROR_PYTHON_LIB);
    } catch (PythonError ARGS_ERROR) {
        std::cout << ARGS_ERROR_MSG << " FOR CHECKPASSWORD() " << std::endl;
        ui->MessagePane->append(ERROR_PYTHON_LIB);
    } catch (...) {
        std::cout << "oh fuck\n" << std::endl;
        ui->MessagePane->append(ERROR_PYTHON_LIB);
    }

    if (rc == SQLITE_OK) {
        // construct a new UNVERIFIED user struct and send back the address
        // they'll still have to get through 2fa to create a actual user object
        UserUnverified userunverified;
        this->userUnverified.first_name = first_name;
        this->userUnverified.last_name = last_name;
        this->userUnverified.email = email;
        this->userUnverified.password = password;
        this->userUnverified.salt = salt;
        // This will send the 2fa email to the user.
        // save the key so the topt object can be recreated
        // to verify the user's entered otp.
        this->userUnverified.TFAKey = Send2FACode(email, TIME_INTERVAL);
        std::cout << "the key: " << this->userUnverified.TFAKey << std::flush;
        sqlite3_close(db);
        return true;
    }

    ui->MessagePane->append("Invalid Login. Try Again\n");
    sqlite3_close(db);
}

/*
 * Function to create a new user account. It will ask for basic user
 * information and have them create a password that follows the rules
 * outlined in ValidPassword(). On success, it will save the provided credentials
 * and redirect the user to the login page.
*/

bool MainWindow::NewAccount(std::string first_name, std::string last_name, std::string email, std::string password) {
    // set up database if not already done so
    sqlite3 * db;
    int rc = sqlite3_open("creds.db", &db);
    if (rc) {
        ui->ErrorSignupTXT->append("Could not open the database");
        return false;
    }

    // create a table if none exists
    const char * create_table = "CREATE TABLE IF NOT EXISTS credentials (email TEXT PRIMARY KEY, password_hash TEXT, salt TEXT, first_name TEXT, last_name TEXT);";
    rc = sqlite3_exec(db, create_table, 0, 0, 0);
    if (rc != SQLITE_OK) {
        ui->ErrorSignupTXT->append("Error creating creds table\n");
        sqlite3_close(db);
        return false;
    }

    // user information
    std::string salt = " ";

    // get the email all lowercase
    std::transform(email.begin(), email.end(), email.begin(), [](unsigned char c){ return std::tolower(c); });

    // see if that email is already in the database
    std::string check_email =  "SELECT first_name FROM credentials WHERE email = '" + email + "';";
    int ret = sqlite3_exec(db, check_email.c_str(), EmailInUse, 0, 0);
    if (ret != SQLITE_OK) {
        ui->ErrorSignupTXT->append("That email is already in use\n");
        sqlite3_close(db);
        return false;
    }

    // Get a password and assign a salt. Ensure the password is strong enough.
    if (!ValidPassword(password)) {
        ui->ErrorSignupTXT->append("Choose a stronger password!\n");
        sqlite3_close(db);
        return false;
    }

    std::string hash = "";
    GetSaltAndHash(hash, password.c_str());
    // Save the hash, salt, and other user data
    std::string insert = "INSERT INTO credentials (email, password_hash, salt, first_name, last_name) VALUES ('" + email + "', " +
                         "'" + hash + "', " +
                         "'" + salt + "', " +
                         "'" + first_name + "', " +
                         "'" + last_name + "');";

    rc = sqlite3_exec(db, insert.c_str(), 0, 0, 0);
    if (rc != SQLITE_OK) {
        ui->ErrorSignupTXT->append("Error saving cred data\n");
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);
    return true;
}

/*
 * Function that will enforces password rules.
*/
bool MainWindow::ValidPassword(std::string password) {
    // check for length
    if (password.size() < 5 || password.size() > 15) {
        ui->ErrorSignupTXT->append("Password must be between 5 and 15 characters long\n");
        return false;
    }
    bool lower_case = false;
    bool upper_case = false;
    bool num = false;
    bool letter = false;
    bool special_char = false;
    for (int i = 0; i < password.size(); i++) {
        char let = password.at(i);
        if (tolower(let) == let) lower_case = true;
        if (toupper(let) == let) upper_case = true;
        if (isdigit(let)) num = true;
        if (isalpha(let)) letter = true;
        if (!isalnum(let)) special_char = true;
    }
    if (!lower_case) {
        ui->ErrorSignupTXT->append("Password must contain at least one lower case letter\n");
        return false;
    } else if (!upper_case) {
        ui->ErrorSignupTXT->append("Password must contain at least one upper case letter\n");
        return false;
    } else if (!num) {
        ui->ErrorSignupTXT->append("Password must contain at least one number\n");
        return false;
    } else if (!letter) {
        ui->ErrorSignupTXT->append("Password must contain at least one letter\n");
        return false;
    } else if (!special_char) {
        ui->ErrorSignupTXT->append("Password must contain at least one special character\n");
        return false;
    }

    return true;
}

/*
 * Function that will generate a salt to pair with a password.
 * note, these random  numbers aren't secure, but that's fine because
 * we can still ensure security if the salt is breached. Also, we store
 * the salt as plaintext next to the password hash, so its not a secret. It
 * is only used to prevent a dictionary/table attack for known plaintext - hash combos.
*/
std::string MainWindow::GenSalt() {
    srand((int) time(0));
    int len = (rand() % 10) + 5; // length will vary from 5 to 14 inclusive
    std::string salt = "";
    for (int i = 0; i < len; i++) {
        char r = (rand() % 94) + 33; // from 33 to 126 inclusive
        salt += (char) r;
    }
    return salt;
}

void MainWindow::on_VerifyBTN_clicked()
{
    // if it worked
    ui->LogInBTN->setText("  Log Out");
}

