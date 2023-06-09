// I took most of this off of geeksforgeeks. Our challenge will be
// to find a way to make the private key connect to the user's password.
// That probably means we calculate the private key before the public key
// - if that even possible
#include <bits/stdc++.h>
#if 0
//Use the euclidean algorithm for gcd
int gcd(int x, int y) {
    if (x < y) {
        //ensures x is always greater
        int temp = x;
        x = y;
        y = temp;
    }
    while (1) {
        //x = n * y + d
        //gcd(x, y) = gcd(y, d)
        int d = x % y;
        if (d == 0) {
            return y;
        } else {
            x = y;
            y = d;
        }
    }
}

void decrypt() {

}

void encrypt() {

}

 
// Code to demonstrate RSA algorithm
int main()
{
    // Two random prime numbers
    double p = 3;
    double q = 7;
 
    // First part of public key:
    double n = p * q;
 
    // Finding other part of public key.
    // e stands for encrypt
    double e = 2;
    double phi = (p - 1) * (q - 1);
    while (e < phi) {
        // e must be co-prime to phi and
        // smaller than phi.
        if (gcd(e, phi) == 1)
            break;
        else
            e++;
    }
 
    // Private key (d stands for decrypt)
    // choosing d such that it satisfies
    // d*e = 1 + k * totient
    int k = 2; // A constant value
    double d = (1 + (k * phi)) / e;
 
    // Message to be encrypted
    double msg = 2;
 
    printf("Message data = %lf", msg);
 
    // Encryption c = (msg ^ e) % n
    double c = pow(msg, e);
    c = fmod(c, n);
    printf("\nEncrypted data = %lf", c);
 
    // Decryption m = (c ^ d) % n
    double m = pow(c, d);
    m = fmod(m, n);
    printf("\nOriginal Message Sent = %lf", m);
 
    return 0;
}
#endif