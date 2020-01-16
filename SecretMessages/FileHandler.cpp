#define _CRT_SECURE_NO_DEPRECATE
#include "FileHandler.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>

void FileHandler::Read_WAV(std::string path)
{
	//Step 1: Load the file
	using namespace std;
	ifstream wav{ path, ifstream::binary };

	//Step 2: Read the header.
	wav.seekg(40);
	uint8_t* chuncksize_d = new uint8_t[4];
	chuncksize_d[0] = wav.get();
	chuncksize_d[1] = wav.get();
	chuncksize_d[2] = wav.get();
	chuncksize_d[3] = wav.get();
	uint32_t data_size = (chuncksize_d[3] << 24) | (chuncksize_d[2] << 16) | (chuncksize_d[1] << 8) | chuncksize_d[0];


	wav.seekg(40);
	char* chuncksize = new char[4];
	wav.read(chuncksize, 4);
	uint32_t data_size2 = (chuncksize[3] << 24) | (chuncksize[2] << 16) | (chuncksize[1] << 8) | chuncksize[0];
	cout << "Data Size: " << data_size2 << "\n";

	//Step 3: Read all data
	wav.seekg(44);
	char* data = new char[data_size];
	wav.read(data, data_size);
	std::vector<bitset<8>> bits2;
	for (int i = 0; i < data_size; i += 2) {
		bitset<8> bit = bitset<8>((int8_t)(data[i]));
		bits2.push_back(bit);
	}
	
	std::vector<bitset<8>> result2;
	string set2;
	for (int i = 0; i < bits2.size(); i += 1) {
		set2 += std::to_string(bits2[i].test(0));
		if (set2.size() >= 8) {
			bitset<8> bit = bitset<8>(set2);
			result2.push_back(bit);
			set2.clear();
		}
	}
	
	string answer2;
	for (int i = 0; i < result2.size(); i += 1) {
		bitset<8> current_byte = result2[i];
		unsigned long nmbr = current_byte.to_ulong();
		//TODO: FULL UTF-8 CHECK.
		if (nmbr <= CHAR_MAX) {
			char c = static_cast<char>(nmbr);
			//Step 6: Keep reading till you find a null byte(0000 0000).
			if (nmbr == '\0') {
				//Step 7: Check if message is UTF - 8. If yes : done, if no : clear currently read bytesand go to step 1.
				if (utf8_check_is_valid(answer2) && answer2.size() > 1) {
					cout << answer2 << "\n";
					break;
				}
				answer2.clear();
			}
			else {
				answer2 += c;
			}
		}
	}
}

void FileHandler::Write_WAV(std::string path, std::string message)
{
	using namespace std;
	ifstream wav{ path,ifstream::binary };
	ofstream wav_out{ "test.wav",ifstream::binary };

	//Write the header
	char* buffer = new char[44];
	wav.read(buffer, 44);
	wav_out.write(buffer, 44);



	//Find out the chunksize
	wav.seekg(40);
	char* chuncksize = new char[4];
	wav.read(chuncksize, 4);
	unsigned long data_size = (chuncksize[3] << 24) | (chuncksize[2] << 16) | (chuncksize[1] << 8) | chuncksize[0];
	cout << "Data Size: " << data_size << "\n";

	//Put data in a vector of 16 bitsets
	wav.seekg(44);
	char* data = new char[data_size];
	wav.read(data, data_size);

	//Code message to bits
	std::vector<bitset<8>> bits;
	for (int i = 0; i < message.size(); i += 1) {
		bits.push_back(bitset<8>(message[i]));
	}
	bits.push_back(bitset<8>('\0'));

	for (int i = 0; i < message.size(); i += 1) {
		std::cout << static_cast<char>(bits[i].to_ulong());
	}
	std::cout << "\n";

	//Put message in data.
	int counter = 0;
	int bit_id = 7;
	for (int i = 0; i < data_size && counter < bits.size(); i += 2)
	{
		char bit = data[i];
		uint8_t test = bit;
		bitset<8> bit_set = bitset<8>(test);
		if (bit_set.test(0) != bits[counter].test(bit_id)) {
			bit_set.flip(0);
			data[i] = static_cast<char>(bit_set.to_ulong());
		}


		--bit_id;
		if (bit_id < 0) {
			bit_id = 7;
			++counter;
		}
	}


	wav_out.write(data, data_size);
}

bool FileHandler::utf8_check_is_valid(const std::string& string)
{
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
