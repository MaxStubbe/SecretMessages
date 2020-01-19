#ifndef __FileHandler_h__
#define __FileHandler_h__

#include <string>

std::string Get_Message();
void Read_WAV(std::string path);
void Write_WAV(std::string path_in, std::string path_out, std::string message);
void Read_AIFF(std::string path);
void Write_AIFF(std::string path_in, std::string path_out, std::string message);

#endif
