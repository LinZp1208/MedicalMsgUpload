#include "stdafx.h"
#include"RSA.h"
extern "C" {
#include <openssl/applink.c>
};

const char* pub_fp = "my_public.pem";
const char* pri_fp = "my_private.pem";

//CString public_encrypt(CString data)
//{
//	unsigned char encrypted[1000];
//	RSA * rsa = createRSAWithFilename(pub_fp, 1);
//	RSA_public_encrypt(data.GetLength(), CStringToChar(data), encrypted, rsa, RSA_PKCS1_PADDING);
//
//	CString result(encrypted);
//	return result;
//}
//
//CString private_decrypt(unsigned char * enc_data, int data_len)
//{
//	unsigned char decrypted[1000];
//	RSA * rsa = createRSAWithFilename(pri_fp, 0);
//	RSA_private_decrypt(data_len, enc_data, decrypted, rsa, RSA_PKCS1_PADDING); 
//
//	CString result(decrypted);
//	return result;
//}
//
//CString private_encrypt(CString data)
//{
//	unsigned char encrypted[1000];
//	RSA * rsa = createRSAWithFilename(pri_fp, 0);
//	RSA_private_encrypt(data.GetLength(), CStringToChar(data), encrypted, rsa, RSA_PKCS1_PADDING);
//
//	CString result(encrypted);
//	return result;
//}
//
//CString public_decrypt(unsigned char * enc_data, int data_len)
//{
//	unsigned char decrypted[1000];
//	RSA * rsa = createRSAWithFilename(pub_fp, 1);
//	RSA_public_decrypt(data_len, enc_data, decrypted, rsa, RSA_PKCS1_PADDING);
//	CString result(decrypted);
//	return result;
//}
//
//void printLastError(char *msg)
//{
//	char * err = (char*)malloc(130);
//	ERR_load_crypto_strings();
//	ERR_error_string(ERR_get_error(), err);
//	CString str;
//	str.Format("%s ERROR: %s\r\n", msg, err);
//	((CFileUploadDlg*)(AfxGetApp()->m_pMainWnd))->m_MsgBox.ReplaceSel(str);
//	free(err);
//}
//
//RSA * createRSAWithFilename(const char * filename, int nPublic)
//{
//	FILE * fp = fopen(filename, "rb");
//
//	if (fp == NULL)
//	{
//		CString str;
//		str.Format("Unable to open rsafile %s \r\n", filename);
//		((CFileUploadDlg*)(AfxGetApp()->m_pMainWnd))->m_MsgBox.ReplaceSel(str);
//		return NULL;
//	}
//	RSA *rsa = RSA_new();
//
//	if (nPublic)
//	{
//		rsa = PEM_read_RSA_PUBKEY(fp, &rsa, NULL, NULL);
//	}
//	else
//	{
//		rsa = PEM_read_RSAPrivateKey(fp, &rsa, NULL, NULL);
//	}
//	return rsa;
//}
//
//unsigned char* CStringToChar(CString str)
//{
//	USES_CONVERSION;
//	unsigned char* p = (unsigned char*)(LPCTSTR)str;
//	//str.ReleaseBuffer();
//	return p;
//}
//CString CharToCString(char* str)
//{
//	CString pData(str);
//	return pData;
//

///////////////////////////////////////////////////////////////////////////////////

int public_encrypt(unsigned char * data, int data_len,unsigned char *encrypted)
{
	RSA * rsa = createRSAWithFilename(pub_fp, 1);
	int result = RSA_public_encrypt(data_len, data, encrypted, rsa, RSA_PKCS1_PADDING);
	return result;
}
int private_decrypt(unsigned char * enc_data, int data_len,unsigned char *decrypted)
{
	RSA * rsa = createRSAWithFilename(pri_fp, 0);
	int  result = RSA_private_decrypt(data_len, enc_data, decrypted, rsa, RSA_PKCS1_PADDING);
	return result;
}


int private_encrypt(unsigned char * data, int data_len,unsigned char *encrypted)
{
	RSA * rsa = createRSAWithFilename(pri_fp, 0);
	int result = RSA_private_encrypt(data_len, data, encrypted, rsa, RSA_PKCS1_PADDING);
	return result;
}
int public_decrypt(unsigned char * enc_data, int data_len,unsigned char *decrypted)
{
	RSA * rsa = createRSAWithFilename(pub_fp, 1);
	int  result = RSA_public_decrypt(data_len, enc_data, decrypted, rsa, RSA_PKCS1_PADDING);
	return result;
}

void printLastError(char *msg)
{
	char * err = (char*)malloc(130);
	ERR_load_crypto_strings();
	ERR_error_string(ERR_get_error(), err);
	CString str;
	str.Format("%s ERROR: %s\r\n", msg, err);
	((CFileUploadDlg*)(AfxGetApp()->m_pMainWnd))->m_MsgBox.ReplaceSel(str);
	free(err);
}

RSA * createRSAWithFilename(const char * filename, int nPublic)
{
	FILE * fp = fopen(filename, "rb");

	if (fp == NULL)
	{
		CString str;
		str.Format("Unable to open rsafile %s \r\n", filename);
		((CFileUploadDlg*)(AfxGetApp()->m_pMainWnd))->m_MsgBox.ReplaceSel(str);
		return NULL;
	}
	RSA *rsa = RSA_new();

	if (nPublic)
	{
		rsa = PEM_read_RSA_PUBKEY(fp, &rsa, NULL, NULL);
	}
	else
	{
		rsa = PEM_read_RSAPrivateKey(fp, &rsa, NULL, NULL);
	}
	return rsa;
}


unsigned char* CStringToChar(CString str)
{
	USES_CONVERSION;
	unsigned char* p = (unsigned char*)(LPCTSTR)str;
	//str.ReleaseBuffer();
	return p;
}
CString CharToCString(char* str)
{
	CString pData(str);
	return pData;
}

void RSAEncypt(unsigned char* input, int inputlength, unsigned char*output, int &outputlength)
{
	int EncryptTimes = inputlength / 117+1;

	if (inputlength>117)
	{
		for (int i = 0; i < EncryptTimes; i++)
		{
			int encrypted_length = public_encrypt(input + 117 * i, (inputlength - 117 * (i + 1) > 0) ? 117 : (inputlength - 117 * (i)), output + 128 * i);
			if (encrypted_length == -1)
			{
				printLastError((char*)"Public Encrypt failed ");
				exit(0);
			}

		}
		outputlength = EncryptTimes * 128;
	}
	else 
	{
		int encrypted_length = public_encrypt(input, inputlength, output);
		if (encrypted_length == -1)
		{
			printLastError((char*)"Public Encrypt failed ");
			exit(0);
		}
		outputlength = 128;
	}
	//int EncryptTimes =1;

	//int encrypted_length = public_encrypt(input,50, output);
	//if (encrypted_length == -1)
	//{
	//	printLastError((char*)"Public Encrypt failed ");
	//	exit(0);
	//}
	//
	//outputlength = 128;

}
void RSADecypt(unsigned char* input, int inputlength, unsigned char*output, int &outputlength)
{
	int DecryptTimes = inputlength / 128;
	outputlength = 0;
	for (int i = 0; i<DecryptTimes; i++)
	{
		int decrypted_length = private_decrypt(input+i*128, 128, output+ outputlength);
		if (decrypted_length == -1)
		{
			printLastError((char*)"Private Decrypt failed ");
			exit(0);
		}
		outputlength += decrypted_length;
	}
}
