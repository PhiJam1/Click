#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "User.hpp"


#define ERROR_TXT_HEADER_LOGIN_PAGE         "Please log into the service to continue."
#define ERROR_TXT_HEADER_SINGUP_PAGE        "Sign Up to use the service."
#define ERROR_PYTHON_LIB                    "There was an interval error access a library. Please try again later or contact the developer.\n"
#define ERROR_SAVING_CREDS                  "Error saving cred data\n"
#define UNEXPECTED_ERROR_MSG                "There was an unexpected error\n"

typedef struct {
    std::string first_name;
    std::string last_name;
    std::string email;
    std::string password;
    std::string salt;
    std::string TFAKey;
    int OTPFailCount;
} UserUnverified;

QT_BEGIN_NAMESPACE
namespace Ui {
class Click;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_LoginBTN_clicked();

    void on_SignUpBTN_clicked();

    void on_LoginRedirect_clicked();

    void on_SignUpRedirect_clicked();

    void on_CreateBTN_clicked();

    void on_CreateBackBTN_clicked();

    void on_CreateSaveBTN_clicked();

    void on_RetrieiveBTN_clicked();

    void on_RetrieveBackBTN_clicked();

    void on_RetrieveGetBTN_clicked();

    void on_DeleteBack_clicked();

    void on_DeleteBTN_clicked();

    void on_DeleteBTN_2_clicked();

    void on_EncryptFileBack_clicked();

    void on_EncryptFileBTN_2_clicked();

    void on_EncryptFileBTN_clicked();

    void on_DecryptBackBTN_clicked();

    void on_DecryptFileBTN_clicked();

    void on_DecryptDecryptBTN_clicked();

    void on_DecryptBackBTN_2_clicked();

    void on_AboutBTN_clicked();

    // void on_LogInBTN_clicked();

    void on_LogInBTN_clicked();

    void on_VerifyBTN_clicked();

    void on_ResendCodeBTN_clicked();

    void on_HWBannBTN_clicked();

    void on_HBBackBTN_clicked();

    void on_HBSaveBTN_clicked();

private:
    Ui::Click *ui;

    User * user = nullptr;
    UserUnverified userUnverified;
    // function to create accounts and log users in
    bool Login(std::string email, std::string password);
    bool NewAccount(std::string first_name, std::string last_name, std::string email, std::string password);

    // helper function
    std::string GenSalt();
    bool ValidPassword(std::string password);
};
#endif // MAINWINDOW_H

