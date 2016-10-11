

#include "functions.h"
#include "FreqAnal.h"

const int errorGap = 3; // lower means less results. errorGap>=0;




using namespace std;

//==========\/==DECRYPTION==\/===========

short
XOR(short x, short y) {
	bool xbin[7];
	decToBin(x, xbin);
	bool ybin[7];
	decToBin(y, ybin);
	bool zbin[7];
	for (int i = 0; i < 7; i++)
		zbin[i] = (!xbin[i] != !ybin[i]);

	return binToDec(zbin);
}

void
	decToBin(short & dec, bool bin[]) {
	// suitable only for dec < 128, bin < 1000000
	for (int i = 6, power = 64; i >= 0; i--, power /= 2)
		if (dec >= power) {
			dec -= power;
			bin[i] = 1;
		} else
			bin[i] = 0;
}

short
	binToDec(bool bin[]) {
	// suitable only for dec < 128, bin < 1000000
	short dec = 0;
	for (int i = 6, power = 64; i >= 0; i--, power /= 2)
		if (bin[i] == 1) {
			dec += power;
		}

	return dec;
}

void
Decrypt(int codeLength, int bodyN[], string words[], int& length){
	// codeLength == period
	// 127 - 32 = 95
	bool codes[codeLength][95]; // codes[j][i] corresponds to i+32 ASCII value
	for(int index = 0; index < codeLength; index++)
			limitVal(codes[index], codeLength, bodyN, length, index);

	int codeValues[codeLength];
	int text[length];

	cout << " || in text separates segments of length " << codeLength << ", i.e the length of the key" <<endl;
	cout << " Length per one char : " << (double) length / (double) codeLength << ", including all non-alphabetic characters" << endl;
	cout << endl;

	ofstream output("jimmy.txt",  std::ios::in | std::ios::out | std::ios::ate);
	applyCode(words, codeValues, codes, codeLength, bodyN, output, text, length, 0);
	output.close();
}

void
Decrypt(int codeLength, int bodyN[], string words[], int& length,const char knownCode[]){
	// codeLength == period
	// 127 - 32 = 95
	bool codes[codeLength][95]; // codes[j][i] corresponds to i+32 ASCII value
	for(int i=0; i<codeLength; i++ )
		if(knownCode[i] != ' '){
			for(int j = 0; j< 95; j++)
				codes[i][j]= 0;
			codes[i][(int)knownCode[i]-32] = 1;
		}

	for(int index = 0; index < codeLength; index++)
		if(knownCode[index] == ' ')
			limitVal(codes[index], codeLength, bodyN, length, index);

	int codeValues[codeLength];
	int text[length];
	ofstream output("jimmy.txt",  std::ios::in | std::ios::out | std::ios::ate);

	applyCode(words, codeValues, codes, codeLength, bodyN, output, text, length, 0);

	output.close();
}

void
	applyCode(string words[], int codeValues[], bool codes[][95], int& period, int bodyN[], ofstream& output, int text[], int& length, int depth){
	if(depth == period){

	if(checkLegal(words, text, length)){
		for(int i=0; i<period; i++)
			cout << (char)codeValues[i];
			cout << endl;
			printText(text, length, period);
			encryptAndPrint(bodyN, length, codeValues, period);
			cout << endl;
	}
	//TODO include ofstream

		return;
	}
	else{

		for(int i = 0; i < 95; i++)
			if(codes[depth][i]){
			applyCodeIndex(i+32, depth, period, bodyN, text, length);
			codeValues[depth] = i + 32;
			applyCode(words, codeValues, codes, period, bodyN, output, text, length, depth+1);
			}
		return;
	}

}

void
	applyCodeIndex(int code, int index, int& period, int bodyN[], int text[],int& length){

	int temp;

	for (; index < length; index += period) {
		temp = XOR(bodyN[index], code);
		if (temp < 123 && temp > 96) {
			text[index] = temp;
			continue;
		}
		if (temp < 91 && temp > 64) {
			text[index] = temp + 32;
			continue;
		}
		text[index] = 0;
		}
}

void
	limitVal(bool codes[95], int& period, int bodyN[], int& length, int index) {

	for (int i = 0; i < 95; i++)
		codes[i] = 1;

	int temp;
	for (int i = 32; i < 127; i++) {
		double abc[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0 };
		// a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z

		for (int j = index; j < length; j += period) {
			temp = XOR(i, bodyN[j]);
			if (temp > 126 || temp < 32) { // Sieves through viable characters
				codes[i - 32] = 0;
				break;
			} else {
				if (temp > 96 && temp < 123) { // counts abcd....z for frequency analysis
					++abc[temp - 97];
					continue;
				}
				if (temp > 64 && temp < 91) {// counts abcd....z for frequency analysis
					++abc[temp - 65];
					continue;
				}

			}
		}
		if (codes[i - 32] == 1)
			if (!freqAnalysis(abc)) // frequency Analysis
				codes[i - 32] = 0;

	}

	for (int i = 0; i < 95; i++) // Checks if there is at least one viable character
		if (codes[i] == 1)
			return;


	// Code below finds viable characters with loser requirements

	// a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z
	double abc[95][26]; // counts abcd...z for frequency analysis

	for (int ix = 0; ix < 95; ix++) // initiates abc[][]
		for (int i = 0; i < 26; i++) {
			abc[ix][i] = 0;
		}

		for (int i = 0; i < 95; i++)
			codes[i] = 1;

		for (int i = 32; i < 127; i++) {
			for (int j = index; j < length; j += period) {
				temp = XOR(i, bodyN[j]);
				if (temp > 126 || temp < 32) { // Sieves through viable characters
					codes[i - 32] = 0;
					break;
				} else {
					if (temp > 96 && temp < 123) { // counts abcd....z for frequency analysis
						++abc[i-32][temp - 97];
						continue;
					}
					if (temp > 64 && temp < 91) { // counts abcd....z for frequency analysis
						++abc[i-32][temp - 65];
						continue;
					}
				}

			}
		}

	int errorCount[95];
	int min = 99999999;
	for (int ix = 0; ix < 95; ix++)
		if (codes[ix] == 1) {
			int temp = loseFreqAnalysis(abc[ix]);
			errorCount[ix] = temp;
			if (temp < min)
				min = temp;
			}
			else errorCount[ix] = 0;
	for (int ix = 0; ix < 95; ix++)
		if (codes[ix] == 1)
			if (errorCount[ix] ==0 || errorCount[ix] > min + errorGap)
				codes[ix] = 0;
}

bool
	checkLegal(string words[], int text[], int& textSize) {
	int legal = 0;
	int notlegal = 0;
	string current = "";

	int limit = textSize / 50;

	for (int i = 0; i < textSize; i++) {
		if(legal > limit){
			return true;
		}
		if(notlegal > 30)
			return false;

		if (text[i] == 0) {
			if (current != "") {
				if (binary_search(words, words + 10000, current)) {
					++legal;
				}
				else notlegal++;
				current = "";
			}

		} else
			current += (char) text[i];
	}
	return legal > textSize;
	//return legal*100 > textSize;
}


//------------^--DECRYPTION--^------------

//===========\/==ENCRYPTION==\/===========

void
encrypt(int bodyN[], int text[], int& length, string codeS){

	int period = codeS.length();
	int code[period];
	for(int i=0; i <period; i++)
		code[i] = (int)codeS[i];

	for(int index = 0; index < period; index++)
		for(int i = index; i < length; i+=period)
			text[i] = XOR(code[index], bodyN[i]);

}

void
encryptAndPrint(int bodyN[], int& length, string codeS){

	int text[length];

	int period = codeS.length();
		int code[period];
		for(int i=0; i <period; i++)
			code[i] = (int)codeS[i];

		for(int index = 0; index < period; index++)
			for(int i = index; i < length; i+=period)
				text[i] = XOR(code[index], bodyN[i]);

	printText(text, length, period);
}

void
encryptAndPrint(int bodyN[], int& length, int code[], int& period){

	int text[length];

		for(int index = 0; index < period; index++)
			for(int i = index; i < length; i+=period)
				text[i] = XOR(code[index], bodyN[i]);

	printText(text, length, period);
}

//------------^--ENCRYPTION--^------------

//=============\/===INPUT===\/=============

void
input(string textFile, char separator,vector<string>& bodyS){

	ifstream input(textFile.c_str());

	if(input.is_open()){
	while(!input.eof()){
		string line;
		getline(input, line, separator);
		bodyS.push_back(line);
	}
	}
	else{
		cout << "file was not opened" << endl;
	}
	input.close();
}

void
convertToInt(vector<string> bodyS, int bodyN[], bool isTextChar) {
	/// isTextChar == true 	=> bodyS is made out of character
	/// isTextChar == false => bodyS is made out of numbers
	if (isTextChar) {	//characters
		for (unsigned int i = 0; i < bodyS.size(); i++)
			bodyN[i] = (int) bodyS[i][0];
	} else {	//numbers
		for (unsigned int i = 0; i < bodyS.size(); i++)
			bodyN[i] = stoi(bodyS[i]); // if this shows an error stoi could not be resolved ignore it just run -std=c++11 or higer
	}
}

void getComWords(string words[]) {
	ifstream scan;
	scan.open("commonWords.txt");
	for (int i = 0; i < 10000; i++) {
		scan >> words[i];
		if(words[i].length()<3)
			words[i]="";
	}
	sort(words, words + 10000);
	scan.close();
}

//-------------^----INPUT----^-------------

//=============\/===PRINT===\/=============


void printArray(bool A[], int size) {
	for (int i = 0; i < size; i++)
		cout << A[i];
	cout << "\n";

}

void printText(int A[], int size, int period) {
	cout << "[ ||";
	if (A[0] == 0)
		cout << '.';
	else
		cout << A[0];
	for (int i = 1; i < size; i++){
		if (A[i] == 0)
			cout << '.';
		else
			cout << (char) A[i];

		if((i+1)%period == 0)
			cout << "||";
	}
	cout << "|| ]" << endl;
}

void printArray(int A[], int& size) {
	cout << "[ " << A[0];
	for (int i = 1; i < size; i++)
		cout << "," << A[i];
	cout << " ]" << endl;
}

void printArray(double A[], int size) {
	cout << "[ " << A[0];
	for (int i = 1; i < size; i++)
		cout << "," << A[i];
	cout << " ]" << endl;
}
//--------------^---PRINT---^--------------







