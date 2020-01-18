#pragma warning(disable:4996)
#include <iostream>
#include "WaveHandler.h"
#include "FileHandler.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <bitset>
#include <sstream>
#include <iostream>
#include <fstream>

std::string IntToUTF8String(int convertMe) {

    // We only care about plane 1 right now,
    // but know that we have other options (0x10FFFF)
    // Technically UTF-8 is "limited" to 4 bytes, so it's not
    // Like it matters much anyways these days

    if (convertMe == 0)
        return " ";
    if ((convertMe <= 0x7F) && (convertMe > 0x00)) {

        std::string out("."); // We shove 1 character into the string to act as a buffer for the conversion process.

        std::bitset<8> x(convertMe);

        unsigned long l = x.to_ulong();
        unsigned char c = static_cast<unsigned char>(l);
        out[0] = c;

        return out;

    }
    else if ((convertMe >= 0x80) && (convertMe <= 0x07FF)) {

        std::string out(".."); // We shove 2 characters into the string to act as a buffer for the conversion process.

        int firstShift = (convertMe >> 0x06) ^ 0xC0;
        int secondShift = ((convertMe ^ 0xFFC0) | 0x80) & ~0x40;

        std::bitset<8> first(firstShift);
        std::bitset<8> last(secondShift);

        unsigned long l = first.to_ulong();
        unsigned char c = static_cast<unsigned char>(l);
        out[0] = c;

        unsigned long ltwo = last.to_ulong();
        unsigned char ctwo = static_cast<unsigned char>(ltwo);
        out[1] = ctwo;

        return out;

    }
    else if ((convertMe >= 0x0800) && (convertMe <= 0xFFFF)) {

        std::string out("...");  // We shove 3 characters into the string to act as a buffer for the conversion process.

        int firstShift = ((convertMe ^ 0xFC0FFF) >> 0x0C) | 0xE0;
        int secondShift = (((convertMe ^ 0xFFF03F) >> 0x06) | 0x80) & ~0x40;
        int thirdShift = ((convertMe ^ 0xFFFC0) | 0x80) & ~0x40;

        std::bitset<8> first(firstShift);
        std::bitset<8> second(secondShift);
        std::bitset<8> third(thirdShift);

        unsigned long lone = first.to_ulong();
        unsigned char cone = static_cast<unsigned char>(lone);
        out[0] = cone;

        unsigned long ltwo = second.to_ulong();
        unsigned char ctwo = static_cast<unsigned char>(ltwo);
        out[1] = ctwo;

        unsigned long lthree = third.to_ulong();
        unsigned char cthree = static_cast<unsigned char>(lthree);
        out[2] = cthree;

        return out;

    }
    else {
        return " ";  // If the integer doesn't represent a valid UTF-8 character, we return a string containing a single space.
    }
}

std::string UnicodeToUTF8(unsigned int codepoint)
{
    std::string out;

    if (codepoint <= 0x7f)
        out.append(1, static_cast<char>(codepoint));
    else if (codepoint <= 0x7ff)
    {
        out.append(1, static_cast<char>(0xc0 | ((codepoint >> 6) & 0x1f)));
        out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
    }
    else if (codepoint <= 0xffff)
    {
        out.append(1, static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f)));
        out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
        out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
    }
    else
    {
        out.append(1, static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07)));
        out.append(1, static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f)));
        out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
        out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
    }
    return out;
}

std::string Get_Message() {
    std::ifstream message{ "message.txt" };
    if (message.fail()) {
        throw std::runtime_error("Failed to open: message.txt");
    }
    return static_cast<std::stringstream const&>(std::stringstream() << message.rdbuf()).str();
}



int main()
{
	//auto temp = cur_byte_order();
	//std::string input;
	//std::cout << "Pick wav file: ";
	//std::cin >> input;
	//std::cin.get();
 //   uint32_t thing = 0xc2a7; // §
	//uint32_t thing2 = 0xc3a7; // ç
 //   char dfa = 'ç';
 //   char sjhadsaks = thing;
 //   std::string tmp = UnicodeToUTF8(0xc2a7); //§
 //   std::string tmp2 = IntToUTF8String(0xc2a7); //§
 //   std::string tmp3 = UnicodeToUTF8(0xc3a7); //ç
 //   std::string tmp4 = IntToUTF8String(0xc3a7); //ç

 //   char16_t wut = 0xc3a7;
 //   int wchars_num = MultiByteToWideChar(CP_UTF8, 0, tmp4.c_str(), -1, NULL, 0);
 //   wchar_t* wstr = new wchar_t[wchars_num];
 //   MultiByteToWideChar(CP_UTF8, 0, tmp4.c_str(), -1, wstr, wchars_num);
	//for (char i = -127; i < 127; ++i) {
	//	std::cout << i << ' ';
	//	if (i % 16 == 15)
	//		std::cout << '\n';
	//}
	try {
        std::string message = Get_Message();


		FileHandler fh = FileHandler();
		//fh.Write_WAV("file-example_1mb.wav","file-example_1mb-OUT.wav" ,  "Dit is een testbericht");
		//fh.Read_WAV("file-example_1mb-OUT.wav");
		//fh.Read_WAV_optimized("lounge1.wav");
		//fh.Write_AIFF("base-16.aiff","base-16-out.aif", "Hällo");
		fh.Read_AIFF("base-16-out.aif");
		//fh.Read_AIFF("lounge1.aif");
		//fh.Read_AIFF("infinitely-many-numbers.aif");
	}
	catch(const char* msg) {
		std::cerr << msg;
	}
    catch (std::runtime_error msg) {
        std::cerr << msg.what();
    }
	
	

	_CrtDumpMemoryLeaks();
}
