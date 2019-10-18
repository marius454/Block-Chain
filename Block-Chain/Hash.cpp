#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>
#include <sstream>
#include <iomanip>
#include <time.h>
#include <chrono>
#include <iomanip>
#include <cstdint>

int numDigits(unsigned long long number);
std::string Hash(std::string input);

std::string Hash(std::string input)
{
	short n = 4;
	std::string strHash[4];
	std::string result;

	double constants[4]{ 3.14159, 2.71828, sqrt(2), 1.61803 };

	unsigned long long hash[4]{ 0x4F2C01A9, 0x586176D4, 0x9A097473, 0x51B4A584 };
	char *ch = new char[input.size() + 1];
	strcpy_s(ch, input.size() + 1, input.c_str());
	for (unsigned long i = 0; i <= input.size(); i++) {
		int seed = (int)ch[i];
		for (unsigned short j = 0; j < n; j++) {
			hash[j] = hash[j] * (int)round(pow(seed + 1, constants[j]));
		}

		if (i == input.size()) {
			for (unsigned short j = 0; j < n; j++) {
				std::stringstream stream;
				stream << std::hex << hash[j];
				std::string result(stream.str());
				strHash[j] = result.substr(0, 8);
				strHash[j].resize(8, '0');
				std::reverse(strHash[j].begin(), strHash[j].end());
			}
		}
		else {
			int rem;
			unsigned long long reverse[4]{ 0x0, 0x0, 0x0, 0x0 };
			for (unsigned short j = 0; j < n; j++) {
				hash[j] = hash[j] / std::pow(16, numDigits(hash[j]) - 8);
			}
			for (unsigned short j = 0; j <= 9; j++)
			{
				for (unsigned short k = 0; k < n; k++) {
					rem = hash[k] % 16;
					reverse[k] = reverse[k] * 16 + rem;
					hash[k] /= 16;
				}
			}
			for (unsigned short j = 0; j < n; j++) {
				hash[j] = reverse[j];
			}
		}
	}
	std::string resultHash;
	for (unsigned short i = 0; i < n; i++) {
		resultHash.append(strHash[i]);
	}
	result = resultHash;

	return result;
}


int numDigits(unsigned long long number)
{
	int digits = 0;
	while (number > 0) {
		number /= 16;
		digits++;
	}
	return digits;
}