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

void set_bit(uint16_t& number, int bit) {
	number = number | (1 << bit);
}

void reset_bit(uint16_t& number, int bit) {
	number = number & ~(1 << bit);
}

void flip_bit(uint16_t& number, int bit) {
	number = number ^ (1 << bit);
}

void and_bitmask(uint16_t& number, uint16_t mask) {
	number = number & mask;
}

void or_bitmask(uint16_t& number, uint16_t mask) {
	number = number | mask;
}

bool test_bit(uint16_t number, int bit) {
	return number & uint16_t (pow(2,bit));//TODO: pow is expensive operation, find other solution?
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

	uint16_t x = 0b10100110;
	uint16_t y = 0b10100010;
	uint16_t z = 0b10100101;
	uint16_t w = 0b00001111;
	uint16_t v = 0b00000101;
	uint16_t g = 0b10000000;

	//flip_bit(x, 2);
	//set_bit(y, 2);
	reset_bit(x, 2);


	and_bitmask(z, w);

	bool t = test_bit(g, 6);

	WaveHandler wavehandler = WaveHandler(); 
	//wavehandler.Write(wavefilepath,"Test String For Ya Boii");
	wavehandler.Read_4("de-oude-schicht.wav");

}
