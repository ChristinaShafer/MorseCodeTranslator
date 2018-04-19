/*
Christina Shafer
CIS 29 - Fall 2017
Lab 1
Use binary operations, bitsets and elementary containers.  Read in file "Morse.bin", separate into Morse
Code values, translate to ASCII.
*/
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <bitset>
#include <cstdlib>
#include <string>
#include <vector>
#include <tuple>
using namespace std;

//class to hold file data info
class BinaryFiles
{
	streampos size;
	unsigned char* memblock;
public:
	void readInFile(string sfile)
	{
		ifstream file(sfile, ios::in | ios::binary);
		if (file.is_open())
		{
			file.seekg(0, ios::end);
			size = file.tellg();
			memblock = new unsigned char[size];
			file.seekg(0, ios::beg);
			file.read((char*)memblock, size);
			file.close();
			return;
		}
	}
	int getSize() { return size;}
	unsigned char* getMemblock() { return memblock;}
};

//class to manipulate a bitstring to vector of bitsets
class BitManipulator {
public:
	BinaryFiles BF;
	vector<bitset<2> > twoBits;
	vector<bitset<8> > eightBits;

	void stringToEight(unsigned char* bitstring) {
		cout << "ORIGINAL TEXT:" << endl << endl;
		cout << bitstring << endl;
		for (int i = 0; i < BF.getSize(); i++) {
		//for (int i = 0; i < 1325; i++) {
			bitset<8> test(bitstring[i]);
			eightBits.push_back(test);
		}
		system("pause");
		return;
	};
	void eightToTwo() {
		bitset<2> temp;
		for (int j = 0; j < eightBits.size(); j++) {
			for (int i = 7; i >= 0; i--) {
				temp[0] = eightBits[j][i - 1];
				temp[1] = eightBits[j][i];
				twoBits.push_back(temp);
				i--;
			}
		}
	}
};

//class to create a morse code hashtable vector holding both ASCII and morse code values
class MorseCode {
	vector<pair<char, string> > morseValues;
	char letterValue;
	int morseBinarySum;
public:
	MorseCode() { Initialize(); letterValue = ' '; morseBinarySum = 0;}
	void Initialize() {
		morseValues.resize(16384);
		insert('A', ".-");
		insert('B', "-...");
		insert('C', "-.-.");
		insert('D', "-..");
		insert('E', ".");
		insert('F', "..-.");
		insert('G', "--.");
		insert('H', "....");
		insert('I', "..");
		insert('J', ".---");
		insert('K', "-.-");
		insert('L', ".-..");
		insert('M', "--");
		insert('N', "-.");
		insert('O', "---");
		insert('P', ".--.");
		insert('Q', "--.-");
		insert('R', ".-.");
		insert('S', "...");
		insert('T', "-");
		insert('U', "..-");
		insert('V', "...-");
		insert('W', ".--");
		insert('X', "-..-");
		insert('Y', "-.--");
		insert('Z', "--..");
		insert('0', "-----");
		insert('1', ".----");
		insert('2', "..---");
		insert('3', "...--");
		insert('4', "....-");
		insert('5', ".....");
		insert('6', "-....");
		insert('7', "--...");
		insert('8', "---..");
		insert('9', "----.");
		insert('\'', ".----.");
		insert('@', ".--.-.");
		insert(':', "---...");
		insert(',', "--..--");
		insert('$', "...-..-");
		insert('=', "-...-");
		insert('!', "-.-.--");
		insert('.', ".-.-.-");
		insert('?', "..--..");
		insert('"', ".-..-.");
		insert(' ', "/");
	}

	char getChar(int MorseIntVal) {
		return morseValues[MorseIntVal].first;
	}
	string getCode(int MorseIntVal) {
		return morseValues[MorseIntVal].second;
	}
	int getMorseInt(char ASCIIVal) {
		for (int i = 0; i < morseValues.size(); i++) {
			if (morseValues[i].first = ASCIIVal)
				return i;
		}
	}
	void insert(char c, string s) {
		pair<char, string> MorsePair(c, s);
		int index = hashFunct(MorsePair.second);
		morseValues[index] = MorsePair;
		return;
	}
	int hashFunct(string s) {
		string bitstring;
		for (int i = 0; i < s.length(); i++) {
			if (s[i] == '.') {
				bitstring += '1';
				bitstring += '0';
			}
			else if (s[i] == '-') {
				bitstring += '0';
				bitstring += '1';
			}
			else if (s[i] == '/') {
				bitstring += '1';
				bitstring += '1';
			}
		}
		int tempVal = 0;
		int multiplier = 1;
		for (int i = bitstring.length() - 1; i >= 0; i--) {

			if (bitstring[i] == '1')
				tempVal += multiplier;
			multiplier *= 2;
		}
		return tempVal;
	}
};

//class to take vector of morse value bits, find morse code values and ascii values and output
class BinaryToMorse {
public:
	BitManipulator startingBits;
	MorseCode MC;
	vector<int> MorseIntValues;
	string outputString;
	

	void Initialize() 
	{
		outputString = "";
		startingBits.stringToEight(startingBits.BF.getMemblock());
		startingBits.eightToTwo();
	};
	//take vector of bitsets<2>, parse by 00
	void StartingBitsToMorseIntValues()
	{
		int BeginChar = 0;
		int EndChar = 0;
		for (int i = 0; i < startingBits.twoBits.size(); i++) {
			if (startingBits.twoBits[i][0] == 0 && startingBits.twoBits[i][1] == 0) {
				EndChar = i - 1;
				int MorseIntVal;
				if (BeginChar <= EndChar) {
					MorseIntVal = MorseCharBtoI(BeginChar, EndChar);
					MorseIntValues.push_back(MorseIntVal);
				}
				BeginChar = i;
				EndChar = i;
			}
			else if (startingBits.twoBits[i][0] == 1 && startingBits.twoBits[i][1] == 1)
			{
				int MorseIntVal = 3;
				MorseIntValues.push_back(MorseIntVal);
				BeginChar = i+1;
				EndChar = i+1;
			}
			
		}
	}
	//Morse segment binary to Int
	int MorseCharBtoI(int startplace, int endplace) {
		int tempVal = 0;
		int tempMultiplier = 1;
		if (startplace>endplace)
		{
			return NULL;
		}
		else
		{
			//gets the total int value of the binary sets passed
			for (int i = 0; i <= (endplace - startplace); i++)
			{
				tempVal += startingBits.twoBits[endplace-i][0]*(tempMultiplier);
				tempVal += startingBits.twoBits[endplace-i][1] * (tempMultiplier*2);
				tempMultiplier *=4;
			}
			return tempVal;
		}
	};
	void MorseIntValuesToString()
	{
		outputString = "";
		for (int i = 0; i < MorseIntValues.size(); i++)
		{
			outputString = outputString + MC.getChar(MorseIntValues[i]);
		}
		cout << endl << "ASCII VALUES: " << endl;
		cout << endl<<outputString << endl;
		system("pause");
		return;
	}
	void MorseIntValuesToCode()
	{
		outputString = "";
		cout << endl << "MORSE CODE: " << endl;
		for (int i = 0; i < MorseIntValues.size(); i++)
		{
			outputString = outputString + MC.getCode(MorseIntValues[i]);
		}
		cout << endl << outputString << endl;
		system("pause");
		return;
	}
};


int main()
{
	BinaryToMorse BTM;
	BTM.startingBits.BF.readInFile("Morse.bin");
	BTM.Initialize();
	BTM.StartingBitsToMorseIntValues();
	BTM.MorseIntValuesToCode();
	BTM.MorseIntValuesToString();
	return 0;
};

