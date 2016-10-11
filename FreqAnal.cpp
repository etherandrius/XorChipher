#include "FreqAnal.h"

#include <iostream>
#include <cmath>

using namespace std;

void countLetters(double abc[], int text[], int length){ // assusems text only has lower case letters and null when converted to char
	for(int i = 0; i < length; i++)
		if(text[i] != 0)
			++abc[text[i]-97];
}

bool freqAnalysis(double abc[]) {

	/*
	 a 	8.167%, b 1.492%, c 2.782%, d 4.253%, e 12.702%, f 2.228%, g 2.015%, h 6.094%, i 6.966%
	 j 	0.153%, k 0.772%, l 4.025%, m 2.406%, n 6.749% , o 7.507%, p 1.929%, q 0.095%, r 5.987%
	 s 	6.327%, t 9.056%, u 2.758%, v 0.978%, w 2.361% , x 0.150%, y 1.974%, z 	0.074%
	 */ // 26
	double error = 1; // 0.06 => 6%
	int errorCount = 0;

	double total = 0;
	for (int i = 0; i < 26; i++) {
		total += abc[i];
	}

	double freq[] = { 8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094,
			6.966, 0.153, 0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095,
			5.987, 6.327, 9.056, 2.758, 0.978, 2.361, 0.150, 1.974, 0.074 };

	for (int i = 0; i < 26; i++) {
		freq[i] *= total * 0.01;
	}
	//===============================
	// Penalizes zeros and low values
	if(total > 20)
	{
	if (abc[0] < 2)
		++errorCount;
	if (abc[4] < 3)
		++errorCount;
	if (abc[7] == 0)
		++errorCount;
	if (abc[8] == 0)
		++errorCount;
	if (abc[13] == 0)
		++errorCount;
	if (abc[14] == 0)
		++errorCount;
	if (abc[18] < 2)
		++errorCount;
	if (abc[19] == 0)
		++errorCount;
	}

	for (int i = 0; i < 26; i++)
		if (abc[i] == 0)
			++errorCount;
	//================================

	for (int i = 0; i < 26; i++) // solving abs(abc - freq) > freq*error
		if (abc[i] > freq[i]) {
			if ((abc[i] < freq[i] * (1 + error)) == false)
				++errorCount;
		}

	return errorCount < 13;
}

int loseFreqAnalysis(double abc[]) {
		double error = 6; // error 6 => 6%
		int errorCount = 0;


		double total = 0;
		for (int i = 0; i < 26; i++) {
			total += abc[i];
		}

		double freq[] = { 8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094,
				6.966, 0.153, 0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095,
				5.987, 6.327, 9.056, 2.758, 0.978, 2.361, 0.150, 1.974, 0.074 };

	for(int i =  0; i < 26; i++)
		if(abc[i]!=1){
		abc[i] *=100;
		abc[i]/=total;
		}
		else abc[i]= -1;

	for(int i = 0; i < 26; i++)
		if(abc[i] == 0)
			errorCount++;

	for(int i = 0; i < 26; i++)
		if(abc[i]>freq[i]){
			if( abc[i] - freq[i] > 6)
				errorCount++;
		}

	return errorCount;
}

