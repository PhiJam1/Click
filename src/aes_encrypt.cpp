//STEPS TO COMPILE AND ENCRYPT/DECRYPT USING AES 128-bit
//Make object files for encryption and decryption
//g++ aes_encrypt.cpp -o encrypt
//g++ aes_decrypt.cpp -o decrypt
//Run ./encrypt for encryption of a file
//Run ./decrypt for decryption of a file

#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>

#include "aes_structures.hpp"


using namespace std;

/* Serves as the initial round during encryption
 * AddRoundKey is simply an XOR of a 128-bit block with the 128-bit key.
 */
void AddRoundKey(unsigned char * state, unsigned char * roundKey) {
	for (int i = 0; i < 16; i++) {
		state[i] ^= roundKey[i];
	}
}

/* Perform substitution to each of the 16 bytes
 * Uses S-box as lookup table 
 */
void SubBytes(unsigned char * state) {
	for (int i = 0; i < 16; i++) {
		state[i] = s[state[i]];
	}
}

// Shift left, adds diffusion
void ShiftRows(unsigned char * state) {
	unsigned char tmp[16];

	/* Column 1 */
	tmp[0] = state[0];
	tmp[1] = state[5];
	tmp[2] = state[10];
	tmp[3] = state[15];
	
	/* Column 2 */
	tmp[4] = state[4];
	tmp[5] = state[9];
	tmp[6] = state[14];
	tmp[7] = state[3];

	/* Column 3 */
	tmp[8] = state[8];
	tmp[9] = state[13];
	tmp[10] = state[2];
	tmp[11] = state[7];
	
	/* Column 4 */
	tmp[12] = state[12];
	tmp[13] = state[1];
	tmp[14] = state[6];
	tmp[15] = state[11];

	for (int i = 0; i < 16; i++) {
		state[i] = tmp[i];
	}
}

 /* MixColumns uses mul2, mul3 look-up tables
  * Source of diffusion
  */
void MixColumns(unsigned char * state) {
	unsigned char tmp[16];

	tmp[0] = (unsigned char) mul2[state[0]] ^ mul3[state[1]] ^ state[2] ^ state[3];
	tmp[1] = (unsigned char) state[0] ^ mul2[state[1]] ^ mul3[state[2]] ^ state[3];
	tmp[2] = (unsigned char) state[0] ^ state[1] ^ mul2[state[2]] ^ mul3[state[3]];
	tmp[3] = (unsigned char) mul3[state[0]] ^ state[1] ^ state[2] ^ mul2[state[3]];

	tmp[4] = (unsigned char)mul2[state[4]] ^ mul3[state[5]] ^ state[6] ^ state[7];
	tmp[5] = (unsigned char)state[4] ^ mul2[state[5]] ^ mul3[state[6]] ^ state[7];
	tmp[6] = (unsigned char)state[4] ^ state[5] ^ mul2[state[6]] ^ mul3[state[7]];
	tmp[7] = (unsigned char)mul3[state[4]] ^ state[5] ^ state[6] ^ mul2[state[7]];

	tmp[8] = (unsigned char)mul2[state[8]] ^ mul3[state[9]] ^ state[10] ^ state[11];
	tmp[9] = (unsigned char)state[8] ^ mul2[state[9]] ^ mul3[state[10]] ^ state[11];
	tmp[10] = (unsigned char)state[8] ^ state[9] ^ mul2[state[10]] ^ mul3[state[11]];
	tmp[11] = (unsigned char)mul3[state[8]] ^ state[9] ^ state[10] ^ mul2[state[11]];

	tmp[12] = (unsigned char)mul2[state[12]] ^ mul3[state[13]] ^ state[14] ^ state[15];
	tmp[13] = (unsigned char)state[12] ^ mul2[state[13]] ^ mul3[state[14]] ^ state[15];
	tmp[14] = (unsigned char)state[12] ^ state[13] ^ mul2[state[14]] ^ mul3[state[15]];
	tmp[15] = (unsigned char)mul3[state[12]] ^ state[13] ^ state[14] ^ mul2[state[15]];

	for (int i = 0; i < 16; i++) {
		state[i] = tmp[i];
	}
}

/* Each round operates on 128 bits at a time
 * The number of rounds is defined in AESEncrypt()
 */
void Round(unsigned char * state, unsigned char * key) {
	SubBytes(state);
	ShiftRows(state);
	MixColumns(state);
	AddRoundKey(state, key);
}

 // Same as Round() except it doesn't mix columns
void FinalRound(unsigned char * state, unsigned char * key) {
	SubBytes(state);
	ShiftRows(state);
	AddRoundKey(state, key);
}

/* The AES encryption function
 * Organizes the confusion and diffusion steps into one function
 */
void AESEncrypt(unsigned char * message, unsigned char * expandedKey, unsigned char * encryptedMessage) {
	unsigned char state[16]; // Stores the first 16 bytes of original message

	for (int i = 0; i < 16; i++) {
		state[i] = message[i];
	}

	int numberOfRounds = 9;

	AddRoundKey(state, expandedKey); // Initial round

	for (int i = 0; i < numberOfRounds; i++) {
		Round(state, expandedKey + (16 * (i+1)));
	}

	FinalRound(state, expandedKey + 160);

	// Copy encrypted state to buffer
	for (int i = 0; i < 16; i++) {
		encryptedMessage[i] = state[i];
	}
}

int main() {
    cout << "---------------------------------------------------" << endl;
    cout << "              128-bit AES Encryption" << endl;
    cout << " (Make sure that you encrypt and decrypt properly) " << endl;
    cout << "---------------------------------------------------" << endl;

    /* Changes
    The intent is to be able to call this function (which we'll rename to aes_encrypt_manager (or similiar)) from the user class.
    We should be able to pass in 2 string, the message to be encrypted, and the key. To develop with this in mind, just sent
    two varibale at the start of this function and work as if those are the paramters.
    Like:
    std::string plaintext = "password123";
    std::string key = "key";

    Then, just output to the terminal the encrypted text after saving it to a string varibale (that we will later return to the user class)
    Then, when you decrypt, follow the same process, just take whatever you saved in the output variable in this file as
    the cipher text for the decrypt file
    Like:
    std::string ciphertext = <what ever you outputte>
    std::string key = "key";

    Note, it may be easier to output just a string of hex values, or numbers as the output. If it is easier, just do that.
    Don't worry about making it into chars or a readable phrase, whatevery is easier for you.
    
    
    
    */

    /* Used for files
    string filename;
    cout << "Enter the filename of the file that you want to encrypt: ";
    cin >> filename;

    ifstream plaintextFile(filename);
    if (!plaintextFile.is_open()) {
        cout << "Unable to open the file that you wanted to encrypt. (It doesn't exist)" << endl;
        return 1;
    }

    stringstream buffer;
    buffer << plaintextFile.rdbuf();
    string plaintext = buffer.str();

    */

    string plaintext = "password123";

    /* Used for files

    plaintextFile.close();

    cout << "Contents of the file: " << plaintext << endl;
    */
    
    
    // convert the text file to an array
    char message[plaintext.length() + 1];
    strcpy(message, plaintext.c_str());

    // Pad the message to 16 bytes accordingly
    int originalLen = strlen(message);
    int paddedMessageLen = originalLen;

    if ((paddedMessageLen % 16) != 0) {
        paddedMessageLen = (paddedMessageLen / 16 + 1) * 16;
    }

    unsigned char *paddedMessage = new unsigned char[paddedMessageLen];
    for (int i = 0; i < paddedMessageLen; i++) {
        if (i >= originalLen) {
            paddedMessage[i] = 0;
        } else {
            paddedMessage[i] = message[i];
        }
    }

    unsigned char *encryptedMessage = new unsigned char[paddedMessageLen];

    string str;
    ifstream infile("src/aes_keyfile", ios::in | ios::binary);
    if (!infile.is_open()) {
        cout << "Unable to open aes_keyfile. (It doesn't exist)" << endl;
        return 1;
    }

    getline(infile, str);
    infile.close();

    istringstream hex_chars_stream(str);
    unsigned char key[16];
    int i = 0;
    unsigned int c;
    while (hex_chars_stream >> hex >> c) {
        key[i] = c;
        i++;
    }

    unsigned char expandedKey[176];

    KeyExpansion(key, expandedKey);

    for (int i = 0; i < paddedMessageLen; i += 16) {
        AESEncrypt(paddedMessage + i, expandedKey, encryptedMessage + i);
    }

    // Print the encrypted message in hex in the terminal
    cout << "Encrypted message in hex:" << endl;
    std::vector<int> encryptedMessageVect;
    for (int i = 0; i < paddedMessageLen; i++) {
        encryptedMessageVect.push_back((int) encryptedMessage[i]);
    }

    ofstream ofs("ciphertext.txt");
    for (int i = 0; i < encryptedMessageVect.size(); i++) {
        ofs << encryptedMessageVect.at(i) << " ";
    }

    #if 0

    // New file that is created when encrypted
    ofstream outfile("ciphertext.txt", ios::out | ios::binary); 
    if (!outfile.is_open()) {
        cout << "Unable to create ciphertext file." << endl;
        return 1;
    }

    outfile.write(reinterpret_cast<const char*>(encryptedMessage), paddedMessageLen);
    outfile.close();

    cout << "Wrote encrypted message to the file that was specified." << endl;
    #endif
    // Free memory
    delete[] paddedMessage;
    delete[] encryptedMessage;

    return 0;
}
