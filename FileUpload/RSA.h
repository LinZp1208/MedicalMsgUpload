#include "stdafx.h"
#include "FileUploadDlg.h"
//#ifndef 1
//extern "C" {
//#include <openssl/applink.c>
//};
//#endif // !1

//#include <openssl/applink.c>

#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <stdio.h>
#include <string.h>

//RSA* createRSAWithFilename(const char*, int);
//CString public_encrypt(CString data);//公钥加密
//CString private_decrypt(unsigned char * enc_data, int data_len);//私钥解密
//
//CString private_encrypt(CString data);//公钥解密
//CString public_decrypt(unsigned char * enc_data, int data_len);//私钥加密
//
//void printLastError(char *msg);
//RSA * createRSAWithFilename(const char * filename, int nPublic);
//unsigned char* CStringToChar(CString str);
//CString CharToCString(char* str);

int public_encrypt(unsigned char * data, int data_len, unsigned char *encrypted);
int private_decrypt(unsigned char * enc_data, int data_len, unsigned char *decrypted);
int private_encrypt(unsigned char * data, int data_len, unsigned char *encrypted);
int public_decrypt(unsigned char * enc_data, int data_len, unsigned char *decrypted);
void printLastError(char *msg);
RSA * createRSAWithFilename(const char * filename, int nPublic);
unsigned char* CStringToChar(CString str);
CString CharToCString(char* str);
void RSADecypt(unsigned char* input, int inputlength, unsigned char*output, int &outputlength);
void RSAEncypt(unsigned char* input, int inputlength, unsigned char*output, int &outputlength);


