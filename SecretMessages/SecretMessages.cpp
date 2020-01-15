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

uint16_t merge_8_bit_to_16(uint8_t first_half, uint8_t second_half) {
	return (first_half << 8) | second_half;
}

void split_16_bit_to_8(uint16_t& number, uint8_t& first_half, uint8_t& second_half) {
	first_half = (number >> 8); 
	second_half = (number & 0b11111111);
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
	uint8_t v = 0b00000101;
	uint8_t g = 0b10000000;
	uint16_t gv = 0b1000000000000101;
	uint8_t s1 = 0;
	uint8_t s2 = 0;

	auto temp = merge_8_bit_to_16(g, v);

	bool t = test_bit(g, 6);

	split_16_bit_to_8(gv, s1, s2);

	WaveHandler wavehandler = WaveHandler(); 
	//wavehandler.Write(wavefilepath,"Test String For Ya Boii");
	wavehandler.Read_4("de-oude-schicht.wav");

}
