#pragma warning(disable:4996)
#include <iostream>
#include "WaveHandler.h"
#include "FileHandler.h"


int main()
{
	const std::string wavefilepath = "de-oude-schicht.wav";
	//auto temp = cur_byte_order();
	//std::string input;
	//std::cout << "Pick wav file: ";
	//std::cin >> input;
	//std::cin.get();

	//std::cout << std::endl;


	WaveHandler wavehandler = WaveHandler(); 
	FileHandler fh = FileHandler();
	//fh.Write_WAV("file-example_1mb.wav","file-example_1mb-OUT.wav" ,  "Dit is een testbericht");
	//fh.Read_WAV("file-example_1mb-OUT.wav");
	fh.Read_AIFF("lounge1.aif");
}
