#ifndef __FileHandler_h__
#define __FileHandler_h__

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <istream>

class FileHandler
{
public:
	bool utf8_check_is_valid(const std::string& string);
	void Read_WAV(std::string path);
	void Write_WAV(std::string path, std::string message);
};

#endif
