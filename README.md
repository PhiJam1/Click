# Click

### What is Click
How many websites do you visit. Out of those, how many require a login. How many passwords are locked up inside your head, or even worse, written on pen and paper? Gone are the days of this archaic and dangerous practice. Click is a password manager that allows you to save all your passwords under one master password. <br>

First of all, without your master password, the content of this application is just garage. All your data is stored in a safely encrypted manner. You can even choose the encryption algorithm to do this! Only your master password can decrypt your personal information. The best part is that we don't even store your master password, only the hash! This means that if someone got their hands on ALL saved data, without you giving them the master password, it is useless.

Additionally, you can choose to encrypt single files with a key of your choice. 

### How is This Possible
The GUI is thanks to QT Framework. This is built as a widget application. User input collected with the GUI is sent to the `User` class which decides what to do with it. If something needs to be encrypted or decrypted, it will call upon the underlying (and personally implemented) encryption algorithms. If some data needs to be stored, changed, or retrieved, it will communicate with the SQLite3 database to get that done. 

### How to use
I developed this off of QT Framework on Ubuntu. I complied with QMake. This can be seen under the `ClickGUI.pro` file. Once you build and run, a new build directory should be created. In that directory, the database `creds.db` will be created for you. It is where all your personal information will end up. 

### Development Proccess
I started this project without a GUI. I wrote all of the backend encryption algorithms in VS code and that can be found in this repo. From there, I ported this over to QT's IDE to build the GUI. Note that all of these files seem lumped under one directory in this repo, but QT has its own way to storing files nicly that doesn't seem to translate well with git. 
