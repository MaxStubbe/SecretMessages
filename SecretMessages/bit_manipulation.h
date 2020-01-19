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



uint32_t merge_8_bit_to_32_big_endian(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth) {
	return ((fourth << 24) | (third << 16) | (second << 8) | first);
}

uint32_t merge_8_bit_to_32_big_endian(const std::vector<char>& data) {
	if (data.size() != 4) {
		throw std::runtime_error("bit_manipulation error: vector is not size 4");
	}
	return merge_8_bit_to_32_big_endian(data[3], data[2], data[1], data[0]);
}

uint32_t merge_8_bit_to_32_little_endian(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth) {
	return ((first << 24) | (second << 16) | (third << 8) | fourth);
}

uint32_t merge_8_bit_to_32_little_endian(const std::vector<char>& data) {
	if (data.size() != 4) {
		throw std::runtime_error("bit_manipulation error: vector is not size 4");
	}
	return merge_8_bit_to_32_little_endian(data[3], data[2], data[1], data[0]);
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
		throw std::runtime_error("bit_manipulation error: index out of bound");
	number = number | (1 << bit);
}

void reset_bit(uint8_t& number, int bit) {
	if (bit > 7 || bit < 0)
		throw std::runtime_error("bit_manipulation error: index out of bound");
	number = number & ~(1 << bit);
}

void flip_bit(uint8_t& number, int bit) {
	if (bit > 7 || bit < 0)
		throw std::runtime_error("bit_manipulation error: index out of bound");
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
		throw std::runtime_error("bit_manipulation error: index out of bound");
	return number & uint8_t(pow(2, bit));
}

#endif