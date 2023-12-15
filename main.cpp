#include "mainwindow.h"

#include <QApplication>
#include <cstdio>
#include <iostream>
int main(int argc, char *argv[])
{
    FILE* pipe = popen("pwd", "r");
    if (!pipe) {
        std::cerr << "Error: Unable to open pipe.\n";
        return 1;
    }

    char buffer[128];
    std::string result = "";

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    if (pclose(pipe) != 0) {
        std::cerr << "Error: Command not successful.\n";
        return 1;
    }

    // Remove trailing newline character from the result
    result.erase(result.find_last_not_of("\n") + 1);

    std::cout << "Current working directory: " << result << std::endl;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
