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


bool utf8_check_is_valid(const std::string& string) {
	int c, i, ix, n, j;
	for (i = 0, ix = string.length(); i < ix; i++)
	{
		c = (unsigned char)string[i];
		//if (c==0x09 || c==0x0a || c==0x0d || (0x20 <= c && c <= 0x7e) ) n = 0; // is_printable_ascii
		if (0x00 <= c && c <= 0x7f) n = 0; // 0bbbbbbb
		else if ((c & 0xE0) == 0xC0) n = 1; // 110bbbbb
		else if (c == 0xed && i < (ix - 1) && ((unsigned char)string[i + 1] & 0xa0) == 0xa0) return false; //U+d800 to U+dfff
		else if ((c & 0xF0) == 0xE0) n = 2; // 1110bbbb
		else if ((c & 0xF8) == 0xF0) n = 3; // 11110bbb
		//else if (($c & 0xFC) == 0xF8) n=4; // 111110bb //byte 5, unnecessary in 4 byte UTF-8
		//else if (($c & 0xFE) == 0xFC) n=5; // 1111110b //byte 6, unnecessary in 4 byte UTF-8
		else return false;
		for (j = 0; j < n && i < ix; j++) { // n bytes matching 10bbbbbb follow ?
			if ((++i == ix) || (((unsigned char)string[i] & 0xC0) != 0x80))
				return false;
		}
	}
	return true;
}

#endif