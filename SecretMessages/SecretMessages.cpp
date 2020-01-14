#pragma warning(disable:4996)
#include <iostream>
#include "WaveHandler.h"

int main()
{
	const std::string wavefilepath = "D:/Avans/Minor/periode 2/C++2/eindopdracht/SecretMessages/SecretMessages/de-oude-schicht.wav";

	std::string input;
	std::cout << "Pick wav file: ";
	std::cin >> input;
	std::cin.get();

	std::cout << std::endl;

	WaveHandler wavehandler = WaveHandler(); 
	wavehandler.Read(wavefilepath);

}
