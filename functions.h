#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_




void Decrypt(int codeLength, int bodyN[], string words[], int& length);
void Decrypt(int codeLength, int bodyN[], string words[], int& length,const char knownCode[]);
	short XOR(short x, short y);
		void decToBin(short& dec, bool bin[]);
		short binToDec(bool bin[]);
	void applyCode(string words[], int codeValues[], bool codes[][95], int& period, int bodyN[], ofstream& output, int text[], int& length, int depth = 0);
	void applyCodeIndex(int  code, int index, int& period, int bodyN[], int text[], int& length);
	void limitVal(bool codes[95], int& period, int bodyN[], int& length,int index);
	bool checkLegal(string words[], int text[], int& textSize);
void encrypt(int bodyN[], int text[], int& length, string codeS);
void encryptAndPrint(int bodyN[], int& length, string codeS);
void encryptAndPrint(int bodyN[], int& length, int code[], int& period);
// - - - - INPUT - - - - //
void input(string textFile, char separator, vector<string>& bodyS); // textType 0-numbers; 1-chars
void convertToInt(vector<string> bodyS, int bodyN[], bool isTextChar);
void getComWords(string words[]);
// - - - - PRINT - - - -//
void printArray(bool A[], int size);
void printText(int	 A[], int size, int period);
void printArray(int A[], int& size);
void printArray(double A[], int size);

#endif /* FUNCTIONS_H_ */

