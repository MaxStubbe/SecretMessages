#define _CRT_SECURE_NO_DEPRECATE
#include "FileHandler.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include "bit_manipulation.h"

void FileHandler::Read_WAV(std::string path) const
{
	//Step 1: Load the file
	using namespace std;
	ifstream wav{ path, ifstream::binary };

	if (wav.fail()) {
		throw std::runtime_error("Failed to open: " + path);
	}

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
	uint32_t data_size2 = merge_8_bit_to_32_little_endian(chuncksize[3], chuncksize[2], chuncksize[1], chuncksize[0]);//(chuncksize[3] << 24) | (chuncksize[2] << 16) | (chuncksize[1] << 8) | chuncksize[0];
	std::cout << "Data Size: " << data_size2 << "\n";

	//Step 3: Read all data
	wav.seekg(44);
	char* data = new char[data_size];
	wav.read(data, data_size);
	std::vector<bitset<8>> bits2;
	for (int i = 0; i < data_size; i += 2) {
		bitset<8> bit = bitset<8>((int8_t)(data[i]));
		bits2.push_back(bit);
	}
	
	//Step 4: Put data in bitsets, getting the most right bit of every byte.
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
					std::cout << answer2 << "\n";
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

void FileHandler::Read_WAV_optimized(std::string path) const
{
	//Step 1: Load the file
	using namespace std;
	ifstream wav{ path, ifstream::binary };
	if (wav.fail()) {
		throw std::runtime_error("Failed to open: " + path);
	}

	//Step 2: Check the size of the data chunk.
	wav.seekg(40);
	char* chuncksize = new char[4];
	wav.read(chuncksize, 4);
	uint32_t data_size = merge_8_bit_to_32_little_endian(chuncksize[3], chuncksize[2], chuncksize[1], chuncksize[0]);//(chuncksize[3] << 24) | (chuncksize[2] << 16) | (chuncksize[1] << 8) | chuncksize[0];
	std::cout << "Data Size: " << data_size << "\n";

	//Step 3: Put all data from datachunk in a char* for easy acces.
	wav.seekg(44);
	char* data = new char[data_size];
	wav.read(data, data_size);

	//Step 4: Loop through all data, getting the most right bit of every left byte. (Little Endian)
	std::vector<bitset<8>> result;
	string set;						//Use string because they can easily be converted to bitsets, and allows to put the bits in the right order very easy.
	for (int i = 0; i < data_size; i += 2) { //SKip every right byte.
		set += std::to_string(bitset<8>((int8_t)(data[i])).test(0));
		if (set.size() >= 8) {
			bitset<8> bit = bitset<8>(set);
			result.push_back(bit);
			set.clear();
		}
	}

	//Step 5: Loop through all the newly made bytes, checking for a utf-8 message.
	string answer;
	for (int i = 0; i < result.size(); i += 1) {
		bitset<8> current_byte = result[i];
		unsigned long nmbr = current_byte.to_ulong();
		if (nmbr <= CHAR_MAX) {//Step 5.1: Check if it can be a legit character.
			char c = static_cast<char>(nmbr);
			//Step 5.2: Keep reading till you find a null byte(0000 0000).
			if (nmbr == '\0') {
				//Step 5.3: Check if message is UTF - 8. If yes : done, if no : clear currently read bytesand go to step 5.
				if (utf8_check_is_valid(answer) && answer.size() > 1) {
					std::cout << answer << "\n";
					break;
				}
				answer.clear();
			}
			else {
				answer += c;
			}
		}
	}
}

void FileHandler::Write_WAV(std::string path_in, std::string path_out, std::string message) const
{
	using namespace std;
	ifstream wav_in{ path_in,ifstream::binary };

	if (wav_in.fail()) {
		throw std::runtime_error("Failed to open: " + path_in);
	}

	ofstream wav_out{ path_out,ifstream::binary };

	if (wav_out.fail()) {
		throw std::runtime_error("Failed to open or create: " + path_out);
	}

	//Write the header
	char* buffer = new char[44];
	wav_in.read(buffer, 44);
	wav_out.write(buffer, 44);

	//Find out the chunksize
	wav_in.seekg(40);
	char* chuncksize = new char[4];
	wav_in.read(chuncksize, 4);
	unsigned long data_size = (chuncksize[3] << 24) | (chuncksize[2] << 16) | (chuncksize[1] << 8) | chuncksize[0];
	std::cout << "Data Size: " << data_size << "\n";

	//Put data in a vector of 16 bitsets
	wav_in.seekg(44);
	char* data = new char[data_size];
	wav_in.read(data, data_size);

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

void FileHandler::Read_AIFF(std::string path) const
{
	//Step 1: Load the file
	using namespace std;
	ifstream aiff{ path, ifstream::binary };

	if (aiff.fail()) {
		throw std::runtime_error("Failed to open: " + path);
	}

	//Step 2: Read the header.
	aiff.seekg(0);
	char* form = new char[4];
	aiff.read(form, 4);
	std::cout << form[0] << form[1] << form[2] << form[3] << "\n";

	aiff.seekg(4);
	char* filesize = new char[4];
	aiff.read(filesize, 4);
	uint32_t data_size_other = merge_8_bit_to_32_big_endian(filesize);// (filesize[0] << 24) | (filesize[1] << 16) | (filesize[2] << 8) | filesize[3];
	
	aiff.seekg(8);
	char* aiff_t = new char[4];
	aiff.read(aiff_t, 4);
	std::cout << aiff_t[0] << aiff_t[1] << aiff_t[2] << aiff_t[3] << "\n";

	bool ssnd_found = false;
	int offset = 0;
	int ssnd_pos = 0;
	uint32_t data_size;
	uint32_t data_offset = 0;
	while (!ssnd_found) {
		aiff.seekg(12+offset);
		char* id = new char[4];
		aiff.read(id, 4);
		std::cout << id[0] << id[1] << id[2] << id[3] << "\n";

		char* size = new char[4];
		aiff.read(size, 4);
		uint32_t chunck_size = merge_8_bit_to_32_big_endian(size);// (size[0] << 24) | (size[1] << 16) | (((uint8_t)size[2]) << 8) | size[3];



		if (id[0] == 'S') {
			if (id[1] == 'S') {
				if (id[2] == 'N') {
					if (id[3] == 'D') {
						std::cout << "Hurray! " << chunck_size << "\n" ;
						ssnd_found = true;
						

						char* of = new char[4];
						aiff.read(of, 4);
						data_offset = (of[0] << 24) | (of[1] << 16) | (((uint8_t)of[2]) << 8) | of[3];
						ssnd_pos = 12 + offset + 16 + data_offset;
						data_size = chunck_size - data_offset - 8;
					}
				}
			}
		}

		offset += 8 + chunck_size;
	}
	aiff.seekg(ssnd_pos);
	char* data = new char[data_size];
	aiff.read(data, data_size);

	std::cout << "Data Size: " << data_size << "\n";
	
	std::vector<bitset<8>> bits2;
	for (int i = 1; i < data_size; i += 2) {//Big endian, so use second part
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
					std::cout << answer2 << "\n";
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

void FileHandler::Write_AIFF(std::string path_in, std::string path_out, std::string message) const
{
	using namespace std;
	ifstream aiff_in{ path_in,ifstream::binary };

	if (aiff_in.fail()) {
		throw std::runtime_error("Failed to open: " + path_in);
	}

	ofstream aiff_out{ path_out,ifstream::binary };

	if (aiff_out.fail()) {
		throw std::runtime_error("Failed to open or create: " + path_out);
	}

	aiff_in.seekg(0);
	char* form = new char[4];
	aiff_in.read(form, 4);
	std::cout << form[0] << form[1] << form[2] << form[3] << "\n";

	aiff_in.seekg(4);
	char* filesize = new char[4];
	aiff_in.read(filesize, 4);
	uint32_t data_size_other = merge_8_bit_to_32_big_endian(filesize);// (filesize[0] << 24) | (filesize[1] << 16) | (filesize[2] << 8) | filesize[3];

	aiff_in.seekg(8);
	char* aiff_t = new char[4];
	aiff_in.read(aiff_t, 4);
	std::cout << aiff_t[0] << aiff_t[1] << aiff_t[2] << aiff_t[3] << "\n";

	bool ssnd_found = false;
	int offset = 0;
	int ssnd_pos = 0;
	uint32_t data_size;
	uint32_t data_offset = 0;
	while (!ssnd_found) {
		aiff_in.seekg(12 + offset);
		char* id = new char[4];
		aiff_in.read(id, 4);
		std::cout << id[0] << id[1] << id[2] << id[3] << "\n";

		char* size = new char[4];
		aiff_in.read(size, 4);
		uint32_t chunck_size = merge_8_bit_to_32_big_endian(size);// (size[0] << 24) | (size[1] << 16) | (((uint8_t)size[2]) << 8) | size[3];



		if (id[0] == 'S') {
			if (id[1] == 'S') {
				if (id[2] == 'N') {
					if (id[3] == 'D') {
						std::cout << "Hurray! " << chunck_size << "\n";
						ssnd_found = true;


						char* of = new char[4];
						aiff_in.read(of, 4);
						data_offset = (of[0] << 24) | (of[1] << 16) | (((uint8_t)of[2]) << 8) | of[3];
						ssnd_pos = 12 + offset + 16 + data_offset;
						data_size = chunck_size - data_offset - 8;
					}
				}
			}
		}

		offset += 8 + chunck_size;
	}
	aiff_in.seekg(ssnd_pos);
	char* data = new char[data_size];
	aiff_in.read(data, data_size);

	std::cout << "Data Size: " << data_size << "\n";

	//Write the header
	aiff_in.seekg(0);
	char* buffer = new char[ssnd_pos];
	aiff_in.read(buffer, ssnd_pos);
	aiff_out.write(buffer, ssnd_pos);

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
	for (int i = 1; i < data_size && counter < bits.size(); i += 2)
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

	//write data to file
	aiff_out.write(data, data_size);
}

bool FileHandler::utf8_check_is_valid(const std::string& string) const
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
