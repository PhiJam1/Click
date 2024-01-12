# Click

### What is Click
How many websites do you visit. Out of those, how many require a login. How many passwords are locked up inside your head, or even worse, written on pen and paper? Gone are the days of this archaic and dangerous practice. Click is a password manager that allows you to save all your passwords under one master password. <br>

First of all, without your master password, the contents of this application are just garbage. All your data is stored in a safely encrypted manner. You can even choose the encryption algorithm to do this! Only your master password can decrypt your personal information. The best part is that we don't even store your master password, only the hash! This means that if someone got their hands on ALL saved data, without you giving them the master password, it is useless.

Additionally, you can choose to encrypt single files with a key of your choice without logging in.

### Functionality Overview 
The GUI is thanks to QT Framework. This is built as a widget application. User input collected with the GUI is sent to the `User` class. If something needs to be encrypted or decrypted, it will call upon the underlying (and personally implemented) encryption algorithms. If data needs to be stored, changed, or retrieved, it will communicate with the SQLite3 database to get that done. 

### How to use
I developed this off of QT Framework on Ubuntu. I complied with QMake. After cloning this repo, you will need to open the `ClickGUI.pro` file with the QT IDE. Once you build and run, a new build directory should be created. In that directory, the database `creds.db` will be created for you. It is where all your personal information will end up. 

### Development Proccess
I started this project without a GUI. I wrote all of the backend encryption algorithms in VSCode and that can be found in [this repo](https://github.com/PhiJam1/Click). From there, I ported this over to QT's IDE to build the GUI. Note that all of these files seem lumped under one directory in this repo, but QT has its own way to storing files that doesn't seem to translate well with git. 

<img width="767" alt="image" src="https://github.com/PhiJam1/ClickGUI/assets/79670114/0429114d-c79a-47ab-8adf-13eb85e1119e">

### Screenshots From Application
#### Login Page
![image](https://github.com/PhiJam1/ClickGUI/assets/79670114/30a59abb-250a-416b-bedd-df4afeb1e908)
#### Sign Up Page
![image](https://github.com/PhiJam1/ClickGUI/assets/79670114/1bca9ddb-a7f5-4ed6-9621-0a0d800843f7)
#### Main Page
![image](https://github.com/PhiJam1/ClickGUI/assets/79670114/a8b23572-59c8-462c-824d-fb3b45c06923)
#### Create Cipher Page
![image](https://github.com/PhiJam1/ClickGUI/assets/79670114/6eecfe05-451a-4a0e-b233-6177b541015a)
#### Retrieve Cipher Page
![image](https://github.com/PhiJam1/ClickGUI/assets/79670114/7bbd683f-b0a9-43d0-82b3-7686544bcf5a)
#### Delete Page
![image](https://github.com/PhiJam1/ClickGUI/assets/79670114/662f9986-88c9-49a9-826e-47e59e8a7a40)
#### Encrypt a File Page
![image](https://github.com/PhiJam1/ClickGUI/assets/79670114/179bd4e2-7355-4ea4-b2d9-0b4ced0aff93)
#### Decrypt a File Page
![image](https://github.com/PhiJam1/ClickGUI/assets/79670114/b84ffb21-e2cd-405b-89bb-a1ab795ed024)
#### About Page
![image](https://github.com/PhiJam1/ClickGUI/assets/79670114/617bba8e-4d87-4fd7-a483-d649d05cd1dc)


