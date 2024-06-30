# Click

### What is Click
How many websites do you visit? Out of those, how many require a login. How many passwords are locked up inside your head, or even worse, written on pen and paper? Gone are the days of this archaic and dangerous practice. Click is a password manager that allows you to save all your passwords under one master password. <br>

### What makes Click Secure
  * Master passwords are never stored. Only its hash is.
  * Saved passwords are stored in an encrypted manner. You can choose the encryption algorithm:
    -  Kyber
    -  Blowfish
    -  A dynamic XOR encryption scheme personally designed
  * Two factor authentication is required. You will get an email with a one time passcode when you try to log in.
  * Hardware scanning is supported. You'll be emailed information about devices that login into your account
    - You can set a hardware banned list of devices for known malicious actors.

    
**That's right! Click has both classical cryptography algorithms and post quantum encryption algorithms in case you want to protect your data against tomorrow's quantum computers.**
### Functionality Overview 
The GUI is thanks to QT Framework. This is built as a widget application. User input collected with the GUI is sent to the `User` class. If something needs to be encrypted or decrypted, it will call upon the underlying (and personally implemented) encryption algorithms. If data needs to be stored, changed, or retrieved, it will communicate with the SQLite3 database to get that done. 

### How to use
I developed this off of QT Framework on Fedora and Ubuntu. I complied with QMake. After cloning this repo, you will need to open the `ClickGUI.pro` file with the QT IDE. Once you build and run, a new build directory should be created. In that directory, the databases `creds.db` and 'ciphers.db' will be created for you. It is where all your personal information will end up. 

### Development Process
I started by designing and implementing my own classical encryption scheme, which I call dynamic xor. It uses a constantly, deterministically changing key to encrypt plaintext with the xor operation. It is not vulnerable to a frequency analysis or any trivial code breaking tactic (other than brute force, of course). I then implemented Blowfish myself. I connected the database to allow for data persistence. I then I took this command line application and gave it a full GUI with QT framework. 

Once I had that working. I used both C++ and Python to implement Kyber. I also added hardware scanning and two factor authentication at this stage. 

### Screenshots From Application
#### Login Page
![image](https://github.com/PhiJam1/Click/assets/79670114/290bfd24-70a4-47c0-9a39-1fec2b0eeba7)
#### Sign Up Page
![image](https://github.com/PhiJam1/Click/assets/79670114/f8569262-617d-4294-a12b-ca5a5785495b)
#### 2FA Page
![image](https://github.com/PhiJam1/Click/assets/79670114/29102c30-cd52-4568-bf84-3c6e320f8634)
#### Main Page
![image](https://github.com/PhiJam1/Click/assets/79670114/ce6e533d-30d4-4ac1-ac9b-480d7bc11096)
#### Create Cipher Page
![image](https://github.com/PhiJam1/Click/assets/79670114/f331e083-6d91-44ba-9a3e-8297b90e1b8e)

#### Retrieve Cipher Page
![image](https://github.com/PhiJam1/Click/assets/79670114/ede84945-7c06-4019-aa2d-e3804c2454c2)

#### Delete Page
![image](https://github.com/PhiJam1/Click/assets/79670114/5125e249-b25f-4940-898a-bf5cd1a63fe0)

#### Hardware Ban Page
![image](https://github.com/PhiJam1/Click/assets/79670114/1c0eea30-85fc-4110-aefd-b00b771e6ff2)

#### Encrypt a File Page
![image](https://github.com/PhiJam1/Click/assets/79670114/d4ef919a-cc87-4387-8d9b-d5c7e9034fe9)

#### Decrypt a File Page
![image](https://github.com/PhiJam1/Click/assets/79670114/eab816a5-a6df-40ea-8278-0a06bdf8462f)


### Disclaimers
This is an education venture and not meant for use with real passwords. I personally implemented all the encryption algorithms the application uses. While this helped me gain a much better understanding of cryptography (the primary goal of this project), you should only use professional implementations of these algorithms for any cryptographically secure use. Other concerns are with random numbers and SQL injection. I do not generate cryptographically secure numbers in most of these algorithms and do not check for malicious SQL injection input. The goal of this project is to understand secure login with password hashing, two factor authentication, one time passcodes, encryption algorithms, and get exposure to the QT framework for building desktop applications. 


