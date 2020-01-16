#ifndef __BIT_MANIPULATION_h__
#define __BIT_MANIPULATION_h__

#include <cstdint>
#include <cmath>

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
	first_half = ((number >> 8) & 0b11111111);//mask is optional here
	second_half = (number & 0b11111111);
}


void reverse_b8(uint8_t& byte) {
	byte = (byte & 0b11110000) >> 4 | (byte & 0b00001111) << 4;
	byte = (byte & 0b11001100) >> 2 | (byte & 0b00110011) << 2;
	byte = (byte & 0b10101010) >> 1 | (byte & 0b01010101) << 1;
}

void set_bit(uint8_t& number, int bit) {
	if (bit > 7 || bit < 0)
		throw "bit_manipulation error: index out of bound";
	number = number | (1 << bit);
}

void reset_bit(uint8_t& number, int bit) {
	if (bit > 7 || bit < 0)
		throw "bit_manipulation error: index out of bound";
	number = number & ~(1 << bit);
}

void flip_bit(uint8_t& number, int bit) {
	if (bit > 7 || bit < 0)
		throw "bit_manipulation error: index out of bound";
	number = number ^ (1 << bit);
}

void and_bitmask(uint8_t& number, uint8_t mask) {
	number = number & mask;
}

void or_bitmask(uint8_t& number, uint8_t mask) {
	number = number | mask;
}

bool test_bit(uint8_t number, int bit) {
	if (bit > 7 || bit < 0)
		throw "bit_manipulation error: index out of bound";
	return number & uint8_t(pow(2, bit));
}

#endif