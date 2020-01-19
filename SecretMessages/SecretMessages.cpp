#define _CRTDBG_MAP_ALLOC
#include <iostream>
#include "FileHandler.h"
#include <iostream>

int main()
{
	bool running = true;
	while (running) {
		try {
			std::string input;
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
				    if (path.substr(found + 1) == "wav") {
					    Read_WAV(input);
				    }
				    else if (path.substr(found + 1) == "aif" || path.substr(found + 1) == "aiff") {
					    Read_AIFF(input);
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
					    Write_WAV(input, output, message);
				    }
				    else if (path.substr(found + 1) == "aif" || path.substr(found + 1) == "aiff") {
					    Write_AIFF(input, output, message);
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