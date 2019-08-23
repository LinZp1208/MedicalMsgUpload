#include "stdAfx.h"
#include"base64Tool.h"

void ZBase64::Encode(unsigned char* Data, int DataByte, unsigned char*output,int &outlength)
{
	//编码表
	const char EncodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	outlength = 0;


	unsigned char Tmp[4] = { 0 };
	int LineLength = 0;
	for (int i = 0; i<(int)(DataByte / 3); i++)
	{
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		Tmp[3] = *Data++;
		output[outlength] = EncodeTable[Tmp[1] >> 2]; outlength++;
		output[outlength] = EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F]; outlength++;
		output[outlength] = EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F]; outlength++;
		output[outlength] = EncodeTable[Tmp[3] & 0x3F]; outlength++;
		if (LineLength += 4, LineLength == 76) {output[outlength] = '\r'; outlength++;output[outlength] = '\n'; outlength++;LineLength = 0; }
	}
	//对剩余数据进行编码
	int Mod = DataByte % 3;
	if (Mod == 1)
	{
		Tmp[1] = *Data++;
		output[outlength] = EncodeTable[(Tmp[1] & 0xFC) >> 2]; outlength++;
		output[outlength] = EncodeTable[((Tmp[1] & 0x03) << 4)]; outlength++;
		output[outlength] = '='; outlength++;
		output[outlength] = '='; outlength++;
	}
	else if (Mod == 2)
	{
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		output[outlength] = EncodeTable[(Tmp[1] & 0xFC) >> 2]; outlength++;
		output[outlength] = EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)]; outlength++;
		output[outlength] = EncodeTable[((Tmp[2] & 0x0F) << 2)]; outlength++;
		output[outlength] = '='; outlength++;
	}
}

void ZBase64::Decode(unsigned char* Data, int DataByte, unsigned char*output,int& OutByte)
{
	//解码表
	OutByte = 0;

	const char DecodeTable[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		62, // '+'
		0, 0, 0,
		63, // '/'
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
		0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
		0, 0, 0, 0, 0, 0,
		26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
		39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
	};
	int nValue;
	int i = 0;
	while (i < DataByte)
	{
		if (*Data != '\r' && *Data != '\n')
		{
			nValue = DecodeTable[*Data++] << 18;
			nValue += DecodeTable[*Data++] << 12;

			output[OutByte]=(nValue & 0x00FF0000) >> 16;
			OutByte++;
			if (*Data != '=')
			{
				nValue += DecodeTable[*Data++] << 6;
				output[OutByte] = (nValue & 0x0000FF00) >> 8;
				OutByte++;
				if (*Data != '=')
				{
					nValue += DecodeTable[*Data++];
					output[OutByte] = nValue & 0x000000FF;
					OutByte++;
				}
			}
			i += 4;
		}
		else// 回车换行,跳过
		{
			Data++;
			i++;
		}
	}
}