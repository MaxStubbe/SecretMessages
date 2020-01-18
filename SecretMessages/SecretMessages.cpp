#pragma warning(disable:4996)
#include <iostream>
#include "WaveHandler.h"
#include "FileHandler.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <sstream>
#include <fstream>


int main()
{
	bool running = true;
	FileHandler fh = FileHandler();
	while (running) {
		std::string input;//= new char[20];//TODO: Exception for more characters.
		std::string output;
		std::string message;
		std::cout << "what would you like to do: read or write?" << std::endl;
		std::getline(std::cin, input);
		
		if (input == "quit") {
			std::cout << "QUIT" << std::endl;;
			running = false;
		}
		else {
			if (input == "read") {
				std::cout << "give file path: " << std::endl;
				std::getline(std::cin, input);
				std::cout << "reading: " << input << std::endl;

				std::string path = static_cast<std::string>(input);
				std::size_t found = path.find_last_of(".");
				//std::cout << " name: " << path.substr(0, found) << '\n'; //check file name
				//std::cout << " file: " << path.substr(found + 1) << '\n'; // check filetype
				if (path.substr(found + 1) == "wav") {
					fh.Read_WAV(input);
				}
				else if (path.substr(found + 1) == "aif" || path.substr(found + 1) == "aiff") {
					fh.Read_AIFF(input);
				}
			}else if(input == "write"){
				//handle input
				std::cout << "give input path: " << std::endl;
				std::getline(std::cin, input);
				std::cout << "give output path: " << std::endl;
				std::getline(std::cin, output);
				std::cout << "write text: " << std::endl;
				std::getline(std::cin, message);
				std::cout << "writing: " << output << std::endl;

				//get file
				std::string path = static_cast<std::string>(input);
				std::size_t found = path.find_last_of(".");
				if (path.substr(found + 1) == "wav") {
					fh.Write_WAV(input, output, message);
				}
				else if (path.substr(found + 1) == "aif" || path.substr(found + 1) == "aiff") {
					fh.Write_AIFF(input, output, message);
				}
			}
		}
	}

	
	//auto temp = cur_byte_order();
	//std::string input;
	//std::cout << "Pick wav file: ";
	//std::cin >> input;
	//std::cin.get();

	//std::cout << std::endl;

	//FileHandler fh = FileHandler();
	//fh.Write_WAV("file-example_1mb.wav","file-example_1mb-OUT.wav" ,  "Dit is een testbericht");
	//fh.Read_WAV("file-example_1mb-OUT.wav");
	//fh.Read_WAV_optimized("lounge1.wav");
	//fh.Read_AIFF("lounge1.aif");
	//fh.Read_AIFF("infinitely-many-numbers.aif");
	//fh.Write_AIFF("base-16.aiff","base-16-out.aif", "Dit is een test");
	//fh.Read_AIFF("base-16-out.aif");

	_CrtDumpMemoryLeaks();
}