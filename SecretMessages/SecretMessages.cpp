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
	//fh.Read_WAV("de-oude-schicht.wav");
	fh.Write_WAV("file-example_1mb.wav", "Dit is een testbericht");
	fh.Read_WAV("test.wav");
	//fh.Write_WAV("de-oude-schich.wav"," ");
	//wavehandler.Read_4("de-oude-schicht.wav");
	//wavehandler.Write(wavefilepath,"Test String For Ya Boii");
	//wavehandler.Read_3("de-oude-schicht.wav");
	//wavehandler.Read_3("lounge1.wav");
	//wavehandler.Read_4("lounge1.aif");
}
