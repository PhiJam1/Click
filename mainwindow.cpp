#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "XOR.hpp"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Note, index 0 -> sign up page, index 1 -> login page, index -> main page
    ui->Login_Page->setCurrentIndex(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}


// This function is called when the login button
// on the login page is clicked. It should collect the
// data in each field and log the user in.
void MainWindow::on_LoginBTN_clicked() {
    QString username;
    QString password;

    // grab data from username and password field
    username = ui->EmailEntry_3->toPlainText();
    password = ui->PasswordEntry_2->toPlainText();

    // check for empty entries
    if (username == "" || password == "") {
        ui->ErrorTXT->append("No field can be empty");
        return;
    }

    Login(username.toStdString(), password.toStdString());
    if (user != nullptr) {
        ui->Login_Page->setCurrentIndex(2);
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
        ui->Login_Page->setCurrentIndex(1);
    }
}


void MainWindow::on_LoginRedirect_clicked()
{
    ui->Login_Page->setCurrentIndex(1);
}


void MainWindow::on_SignUpRedirect_clicked()
{
   ui->Login_Page->setCurrentIndex(0);
}





void MainWindow::on_CreateBTN_clicked()
{
    ui->Login_Page->setCurrentIndex(3);
}


void MainWindow::on_CreateBackBTN_clicked()
{
    ui->Login_Page->setCurrentIndex(2);
}


void MainWindow::on_CreateSaveBTN_clicked()
{
    QString serviceName;
    int cipherType; // 0 -> XOR, 1 -> blowfish
    QString username;
    QString password;

    // assign fields
    serviceName = ui->CreateSN->toPlainText();
    cipherType = ui->CreateCT->currentIndex();
    username = ui->CreateUsername->toPlainText();
    password = ui->CreatePassword->toPlainText();

    // check for empty fields
    if (serviceName == "" || username == "" || password == "") {
        ui->CreateTXT->append("No fields can be empty!");
        return;
    }

    // check to see if this service is alr saved
    std::vector<std::string> names = user->get_service_namees();
    for (unsigned long i = 0; i < names.size(); i++) {
        if (names.at(i) == serviceName.toStdString()) {
            ui->CreateTXT->append("You already have a cipher saved for this service.!");
            return;
        }
    }

    // now save the cipher
    user->CreateCipher(serviceName.toStdString(), username.toStdString(), password.toStdString(), cipherType);
    ui->CreateTXT->append("Saved!");
}


void MainWindow::on_RetrieiveBTN_clicked()
{
    // get the list of saved ciphers for their service names
    std::vector<std::string> service_names = user->get_service_namees();
    std::cout << service_names.size() << std::endl;

    // clear anything currently in the drop down
    ui->RetrieveSelectDrop->clear();

    // add back the default value
    ui->RetrieveSelectDrop->addItem("None");

    // populate the rest with the users saved logins
    for (unsigned long i = 0; i < service_names.size(); i++) {
        ui->RetrieveSelectDrop->addItem(QString::fromStdString(service_names.at(i)));
    }

    ui->Login_Page->setCurrentIndex(4);
}


void MainWindow::on_RetrieveBackBTN_clicked()
{
    ui->Login_Page->setCurrentIndex(2);
}


void MainWindow::on_RetrieveGetBTN_clicked()
{
    std::string serviceName = ui->RetrieveSelectDrop->currentText().toStdString();
    if (serviceName == "None") {
        return;
    }
    std::pair<std::string, std::string> cred = user->RetrievePassword(serviceName);
    ui->RetrieveTXT->setPlainText(QString::fromStdString(serviceName));
    std::string buff = "Username: " + cred.first;
    ui->RetrieveTXT->append(QString::fromStdString(buff));
    buff = "Password: " + cred.second;
    ui->RetrieveTXT->append(QString::fromStdString(buff));
}


void MainWindow::on_DeleteBack_clicked()
{
    ui->Login_Page->setCurrentIndex(2);
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
    ui->Login_Page->setCurrentIndex(5);
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
    ui->Login_Page->setCurrentIndex(2);
}


void MainWindow::on_EncryptFileBTN_2_clicked()
{
    std::string key = ui->EncryptFileKey->toPlainText().toStdString();
    ui->EncryptFileDesc->appendPlainText(advancedXorEncryptionFile(key) ? "Success" : "There was an error");
}


void MainWindow::on_EncryptFileBTN_clicked()
{
    ui->Login_Page->setCurrentIndex(6);
}


void MainWindow::on_DecryptBackBTN_clicked()
{
    ui->Login_Page->setCurrentIndex(2);
}


void MainWindow::on_DecryptFileBTN_clicked()
{
    ui->Login_Page->setCurrentIndex(7);
}


void MainWindow::on_DecryptDecryptBTN_clicked()
{
    std::string key = ui->DecryptKey->toPlainText().toStdString();
    ui->DecryptDesc->appendPlainText(advancedXorDecryptionFile(key) ? "Success" : "There was an error");
}

