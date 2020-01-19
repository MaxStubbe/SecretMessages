﻿#define _CRTDBG_MAP_ALLOC
#include <iostream>
#include "FileHandler.h"
#include <sstream>
#include <iostream>
#include <fstream>



#include <string>         // std::string, std::u32string
#include <locale>         // std::wstring_convert
#include <codecvt>        // std::codecvt_utf8
#include <cstdint>        // std::uint_least32_t

std::string Get_Message() {
    std::ifstream message{ "message.txt" };
    if (message.fail()) {
        throw std::runtime_error("Failed to open: message.txt");
    }
    return static_cast<std::stringstream const&>(std::stringstream() << message.rdbuf()).str();
}

int main()
{
	bool running = true;
	FileHandler fh = FileHandler();
	while (running) {
		std::string input;
		std::string output;
		std::string message;
		std::cout << "what would you like to do: read or write?" << std::endl;
		std::getline(std::cin, input);
		try {
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
				    if (path.substr(found + 1) == "wav") {
					    fh.Read_WAV(input);
				    }
				    else if (path.substr(found + 1) == "aif" || path.substr(found + 1) == "aiff") {
					    fh.Read_AIFF(input);
				    }
					else {
						throw std::runtime_error("File: " + input + " is not of type: aiff or wav");
					}
			    } else if(input == "write"){
				    //handle input
				    std::cout << "give input path: " << std::endl;
				    std::getline(std::cin, input);
				    std::cout << "give output path: " << std::endl;
				    std::getline(std::cin, output);
				    std::cout << "write text: " << std::endl;
				    std::getline(std::cin, message);
				    std::cout << "writing: " << output << std::endl;

					if (message == "Use file") {
						message = Get_Message();
					}

				    //get file
				    std::string path = static_cast<std::string>(input);
				    std::size_t found = path.find_last_of(".");
				    if (path.substr(found + 1) == "wav") {
					    fh.Write_WAV(input, output, message);
				    }
				    else if (path.substr(found + 1) == "aif" || path.substr(found + 1) == "aiff") {
					    fh.Write_AIFF(input, output, message);
				    }
					else {
						throw std::runtime_error("File: " + input + " is not of type: aiff or wav");
					}
			    }
		    }
        }
        catch (const char* msg) {
            std::cerr << msg << "\n";
        }
        catch (std::runtime_error msg) {
            std::cerr << msg.what() << "\n";
        }
	}

	_CrtDumpMemoryLeaks();
}