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
	bool utf8_check_is_valid(const std::string& string) const;
	void Read_WAV(std::string path) const;
	void Write_WAV(std::string path_in, std::string path_out, std::string message) const;
	void Read_AIFF(std::string path) const;
	void Write_AIFF(std::string path_in, std::string path_out, std::string message) const;


};

#endif
