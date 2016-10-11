
#include "functions.h"
#include "FreqAnal.h"

const string textFile  = "EncryptedText/Xor-3-god.txt";
const bool inputType   = 0; // 0-numbers, 1-characters;
const char 	 separator = ','; // char that separates characters input; i.e ',' , ' ' , '\n' , '\t' , '\0'
const bool	 isCode    = false; // do you have a code
const char codeC[]     = {'g', 'o', 'd'};	// specify the code letter by letter. If you dont know a specific letter  write '\0'
const int codePeriodMin= 3;		// Minimal length of key to try
const int codePeriodMax= 3; 	// Maximal  ..... .. ... .. ...

using namespace std;

int main(){
	// TODO add support for different inputTypes


	// XOR encryption => encrypting a file twice we get the original

	//Preparing required objects
	vector<string> bodyS;
	input(textFile, separator, bodyS); 	// input -> bodyS
	int ln = bodyS.size();
	int bodyN[ln];
	convertToInt(bodyS, bodyN, inputType); // converts vector<string> to (int)char;
	string words[10000];
	getComWords(words);
	bodyS.clear(); // delete if vector<string> version is needed


	if(isCode){ // isCode provided
		for(unsigned int i = 0; i < sizeof(codeC)/sizeof(codeC[1]); i++)
			if(codeC[i] == '\0'){
				Decrypt(sizeof(codeC)/sizeof(codeC[1]), bodyN, words, ln, codeC);
				return 0;
			}
		encryptAndPrint(bodyN, ln, codeC);
	}
	else{
	for(int i = codePeriodMin; i <= codePeriodMax; i++)
		Decrypt(i, bodyN, words, ln);
	}

}






