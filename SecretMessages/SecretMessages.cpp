#pragma warning(disable:4996)
#include <iostream>
#include "WaveHandler.h"
#include "FileHandler.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


int main()
{
	//auto temp = cur_byte_order();
	//std::string input;
	//std::cout << "Pick wav file: ";
	//std::cin >> input;
	//std::cin.get();



	//std::cout << std::endl;
	char x = 0b11111111;
	char y = 'ä';
	std::string z = "ä";
	
	try {
		FileHandler fh = FileHandler();
		//fh.Write_WAV("file-example_1mb.wav","file-example_1mb-OUT.wav" ,  "Dit is een testbericht");
		//fh.Read_WAV("file-example_1mb-OUT.wav");
		//fh.Read_WAV_optimized("lounge1.wav");
		fh.Write_AIFF("base-16.aiff","base-16-out.aif", "Hällo");
		fh.Read_AIFF("base-16-out.aif");
		//fh.Read_AIFF("lounge1.aif");
		//fh.Read_AIFF("infinitely-many-numbers.aif");
	}
	catch(const char* msg) {
		std::cerr << msg;
	}
	
	

	_CrtDumpMemoryLeaks();
}
