#pragma warning(disable:4996)
#include <iostream>
#include "WaveHandler.h"

enum class byte_order {
	little_endian,
	big_endian
};

byte_order cur_byte_order() {
	const unsigned short val
	{ *reinterpret_cast<const unsigned short*>("az") };
	return val == 0x617AU ?
		byte_order::big_endian :
		byte_order::little_endian;
}


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
	//wavehandler.Write(wavefilepath,"Test String For Ya Boii");
	wavehandler.Read_4("de-oude-schicht.wav");

}
