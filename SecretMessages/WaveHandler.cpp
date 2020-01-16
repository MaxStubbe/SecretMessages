
#define _CRT_SECURE_NO_DEPRECATE
#include "WaveHandler.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include "Timer.cpp"

WaveHandler::WaveHandler()
{
	//little endian
}

// find the file size 
int getFileSize(FILE* inFile) {
	int fileSize = 0;
	fseek(inFile, 0, SEEK_END);

	fileSize = ftell(inFile);

	fseek(inFile, 0, SEEK_SET);
	return fileSize;
}

void WaveHandler::Read(std::string path)
{
	using namespace std;
	wav_hdr wavHeader;
	audio_data audioData;
	int headerSize = sizeof(wav_hdr), filelength = 0;

	const char* filePath = path.c_str();

	FILE* wavFile = fopen(filePath, "r");

	if (wavFile == NULL) {
		printf("Can not open wave file\n");
	}

	//fread(&wavHeader, headerSize, 1, wavFile);

	//read header
	size_t bytesRead = fread(&wavHeader, 1, headerSize, wavFile);
	if (bytesRead > 0)
	{
		//Read the data
		uint16_t bytesPerSample = wavHeader.bitsPerSample / 8;      //Number of bytes per sample
		uint64_t numSamples = wavHeader.ChunkSize / bytesPerSample; //How many samples are in the wav file?
		static const uint16_t BUFFER_SIZE = 4096;
		int8_t* buffer = new int8_t[BUFFER_SIZE];



		while ((bytesRead = fread(buffer, sizeof buffer[0], BUFFER_SIZE / (sizeof buffer[0]), wavFile)) > 0)
		{
			/** DO SOMETHING WITH THE WAVE DATA HERE **/
			cout << "Read " << bytesRead << " bytes." << endl;
		}

		delete[] buffer;
		buffer = nullptr;
		filelength = getFileSize(wavFile);

		print_header(wavHeader);
	}
	fclose(wavFile);
}

void WaveHandler::Read_2(std::string path)
{
	using namespace std;
	ifstream wav{ path,ifstream::binary };
	string temp;
	for (int i = 0; i < 4; ++i) {
		temp += wav.get();
	}
	cout << "ChunckID: " << temp << "\n";

	char* chuncksize_a = new char[4];
	wav.get(chuncksize_a, 4);
	unsigned long chuncksize = (chuncksize_a[3] << 24) | (chuncksize_a[2] << 16) | (chuncksize_a[1] << 8) | chuncksize_a[0];
	cout << "ChunckSize: " << chuncksize << "\n";
	temp.clear();
	for (int i = 0; i < 4; ++i) {
		temp += wav.get();
	}
	cout << "Format: " << temp << "\n";

	wav.seekg(12);
	temp.clear();
	for (int i = 0; i < 4; ++i) {
		temp += wav.get();
	}
	cout << "SubChunk1ID: " << temp << "\n";


	wav.seekg(40);
	char* chuncksize_b = new char[4];
	wav.get(chuncksize_b, 4);
	unsigned long chuncksize_c = (chuncksize_b[3] << 24) | (chuncksize_b[2] << 16) | (chuncksize_b[1] << 8) | chuncksize_b[0];
	cout << "SubChunk2Size: " << chuncksize_c << "\n";

	//Put data in a array
	//wav.seekg(44);
	std::vector<char> data;
	for (int i = 0; i < chuncksize_c; ++i) {
		data.push_back(wav.get());
	}

	std::vector<double> rawSignal;
	std::vector<bitset<16>> bits;

	
	for (int i = 0; i < data.size(); i += 2)
	{

		auto test = bitset<8>(data[i]);
		auto test2 = bitset<8>(data[i + 1]);
		auto temp = test | test2;
		int c = (data[i + 1] << 8) | data[i];
		bitset<16> bit = bitset<16>(c);
		bits.push_back(bit);
		rawSignal.push_back(c / 32768.0);
	}

	std::vector<bitset<8>> result;
	string set;
	for (int i = 0; i < bits.size(); i += 16) {
		int temp = (bits[i].test(15));
		set += std::to_string(temp);
		if (set.size() >= 8) {
			bitset<8> bit = bitset<8>(set);
			result.push_back(bit);
			set.clear();
		}
	}

	

	int counter = 0;
	string answer;
	for (auto& current_byte : result) {
		unsigned long i = current_byte.to_ulong();
		if (i <= CHAR_MAX) {
			char c = static_cast<char>(i);
			if (i == '\0') {
				cout << answer << "\n";
				answer.clear();
				counter++;
			}
			answer += c;
		}
	}
	cout << "Counted: " << counter << "\n";

}

bool utf8_check_is_valid(const std::string& string)
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

void GetUnicodeChar(unsigned int code, char chars[5]) {
	if (code <= 0x7F) {
		chars[0] = (code & 0x7F); chars[1] = '\0';
	}
	else if (code <= 0x7FF) {
		// one continuation byte
		chars[1] = 0x80 | (code & 0x3F); code = (code >> 6);
		chars[0] = 0xC0 | (code & 0x1F); chars[2] = '\0';
	}
	else if (code <= 0xFFFF) {
		// two continuation bytes
		chars[2] = 0x80 | (code & 0x3F); code = (code >> 6);
		chars[1] = 0x80 | (code & 0x3F); code = (code >> 6);
		chars[0] = 0xE0 | (code & 0xF); chars[3] = '\0';
	}
	else if (code <= 0x10FFFF) {
		// three continuation bytes
		chars[3] = 0x80 | (code & 0x3F); code = (code >> 6);
		chars[2] = 0x80 | (code & 0x3F); code = (code >> 6);
		chars[1] = 0x80 | (code & 0x3F); code = (code >> 6);
		chars[0] = 0xF0 | (code & 0x7); chars[4] = '\0';
	}
	else {
		// unicode replacement character
		chars[2] = 0xEF; chars[1] = 0xBF; chars[0] = 0xBD;
		chars[3] = '\0';
	}

}

void WaveHandler::print_header(wav_hdr wavHeader) {
		std::cout << "RIFF header                :" << wavHeader.RIFF[0] << wavHeader.RIFF[1] << wavHeader.RIFF[2] << wavHeader.RIFF[3] << "\n";
		std::cout << "WAVE header                :" << wavHeader.WAVE[0] << wavHeader.WAVE[1] << wavHeader.WAVE[2] << wavHeader.WAVE[3] << "\n";
		std::cout << "FMT                        :" << wavHeader.fmt[0] << wavHeader.fmt[1] << wavHeader.fmt[2] << wavHeader.fmt[3] << "\n";
		std::cout << "Data size                  :" << wavHeader.ChunkSize << "\n";
		std::cout << "Sampling Rate              :" << wavHeader.SamplesPerSec << "\n";
		std::cout << "Number of bits used        :" << wavHeader.bitsPerSample << "\n";
		std::cout << "Number of channels         :" << wavHeader.NumOfChan << "\n";
		std::cout << "Number of bytes per second :" << wavHeader.bytesPerSec << "\n";
		std::cout << "Data length                :" << wavHeader.Subchunk2Size << "\n";
		std::cout << "Audio Format               :" << wavHeader.AudioFormat << "\n"; // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
		std::cout << "Block align                :" << wavHeader.blockAlign << "\n";
		std::cout << "Data string                :" << wavHeader.Subchunk2ID[0] << wavHeader.Subchunk2ID[1] << wavHeader.Subchunk2ID[2] << wavHeader.Subchunk2ID[3] << "\n";
}


void WaveHandler::Read_3(std::string path)
{
	//Step 1: Load the file
	using namespace std;
	ifstream wav{ path,ifstream::binary };

	//Step 2: Read the header.
	wav.seekg(36);
	char* chuncksize_a = new char[5];
	wav.get(chuncksize_a, 5);
	cout << "Data Size: " << chuncksize_a << "\n";


	wav.seekg(36);
	char* chuncksize_c = new char[4];
	wav.read(chuncksize_c, 4);
	cout << "Data Size: " << chuncksize_c << "\n";

	wav.seekg(36);
	char* chuncksize_b = new char[5];
	chuncksize_b[0] = wav.get();
	chuncksize_b[1] = wav.get();
	chuncksize_b[2] = wav.get();
	chuncksize_b[3] = wav.get();
	chuncksize_b[4] ='\0';
	cout << "Data Size: " << chuncksize_b << "\n";

	wav.seekg(40);
	uint8_t* chuncksize_d = new uint8_t[4];
	chuncksize_d[0] = wav.get();
	chuncksize_d[1] = wav.get();
	chuncksize_d[2] = wav.get();
	chuncksize_d[3] = wav.get();
	unsigned long data_size = (chuncksize_d[3] << 24) | (chuncksize_d[2] << 16) | (chuncksize_d[1] << 8) | chuncksize_d[0];


	wav.seekg(40);
	char* chuncksize = new char[4];
	wav.get(chuncksize, 4);
	unsigned long data_size2 = (chuncksize[3] << 24) | (chuncksize[2] << 16) | (chuncksize[1] << 8) | chuncksize[0];
	cout << "Data Size: " << data_size2 << "\n";

	//Step 3: Read all data and put it in a vector
		Timer t;
		t.start();
		//Optie 1
		wav.seekg(44);
		std::vector<bitset<8>> bits;
		for (int i = 0; i < data_size; i +=2) {
			int8_t temp = wav.get();
			int8_t temp2 = wav.get();
			bitset<8> bit = bitset<8>(temp);
			bits.push_back(bit);
		}
		std::cout << " Time: " << t.elapsedMilliseconds() <<"\n";

		t.reset();
		//Optie 2, Much Faster
		wav.seekg(44);
		char* data = new char[data_size];
		wav.read(data, data_size);
		std::vector<bitset<8>> bits2;
		for (int i = 0; i < data_size; i += 2) {
			bitset<8> bit = bitset<8>((int8_t)(data[i]));
			bits2.push_back(bit);
		}
		std::cout << " Time: " << t.elapsedMilliseconds() << "\n";
		t.stop();



		//check if 1 and 2 are equal
		for (int i = 0; i < bits2.size(); i += 1) {
			if (bits[i] != bits2[i]) {
				std::cout << "oof";
			}
		}


	
	//Step 4: Gather all the most insignificant bits.
		t.start();
		//Optie 1
		std::vector<bitset<8>> result;
		string set;
		for (int i = 0; i < bits.size(); i += 1) {
			set += std::to_string(bits[i].test(0));
			if (set.size() >= 8) {
				bitset<8> bit = bitset<8>(set);
				result.push_back(bit);
				set.clear();
			}
		}
		std::cout << " Time: " << t.elapsedMilliseconds() << "\n";
		t.reset();

		//Optie 2
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
		std::cout << " Time: " << t.elapsedMilliseconds() << "\n";

	//Step 5: Check for message
	string answer;
	for (int i = 0; i < result.size(); i += 1) {
		bitset<8> current_byte = result[i];
		unsigned long nmbr = current_byte.to_ulong();
		//TODO: FULL UTF-8 CHECK.
		if (nmbr <= CHAR_MAX) {
			char c = static_cast<char>(nmbr);
			//Step 6: Keep reading till you find a null byte(0000 0000).
			if (nmbr == '\0') {
				//Step 7: Check if message is UTF - 8. If yes : done, if no : clear currently read bytesand go to step 1.
				if (utf8_check_is_valid(answer) && answer.size() > 1) {
					cout << answer << "\n";
					break;
				}
				answer.clear();
			}
			else {
				answer += c;
			}
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

void WaveHandler::Read_4(std::string path)
{
	//---LOADING THE FILE---//
//Step 1: Read the header.
	//SKIP THIS ONE FOR NOW
//Step 2: Read all data and put it in a vector
	using namespace std;
	ifstream wav{ path,ifstream::binary };

	//Find out the chunksize
	//wav.seekg(68);
	wav.seekg(42);
	char* chuncksize = new char[4];
	wav.get(chuncksize, 4);
	unsigned long data_size = (chuncksize[0] << 24) | (chuncksize[1] << 16) | (chuncksize[2] << 8) | chuncksize[3];
	cout << "Size: " << data_size << "\n";

	//Put data in a vector of 16 bitsets
	wav.seekg(88);
	char* data = new char[data_size];
	wav.get(data, data_size);


	std::vector<bitset<8>> bits;
	string temp;
	for (int i = 0; i < data_size; i += 2) {//16 bit thingies
		//int8_t temp = wav.get();
		//int8_t temp2 = wav.get();
		//int16_t c = //temp2 | (temp << 8);
		bitset<8> bit = bitset<8>(data[i+1]);
		bits.push_back(bit);
	}

	//Gather all the most insignificant bits.
	std::vector<bitset<8>> result;
	string set;
	for (int i = 0; i < bits.size(); i += 1) {
		int temp = (bits[i].test(0));
		set += std::to_string(temp);
		if (set.size() >= 8) {
			bitset<8> bit = bitset<8>(set);
			result.push_back(bit);
			set.clear();
		}
	}



	//---READING THE MESSAGE---//
	//Step 1: Keep reading till you find a null byte (0000 0000).
	//Step 2: Check if message is UTF-8. If yes: done, if no:  clear currently read bytes and go to step 1.
	int counter = 0;
	string answer;

	for (int i = 0; i < result.size(); i += 1) {

		bitset<8> current_byte = result[i];
		unsigned long nmbr = current_byte.to_ulong();

		if (nmbr <= CHAR_MAX) {
			char c = static_cast<char>(nmbr);
			char* temp = new char[5];
			GetUnicodeChar(nmbr, temp);
			if (nmbr == '\0') {
				if (utf8_check_is_valid(answer) && answer.size() > 1) {
					//if (answer == "Test String For Ya Boii") {
					cout << answer << "\n";
					//}
				}
				answer.clear();
				counter++;
			}
			else {
				answer += c;
			}
		}
	}
	cout << "Counted: " << counter << "\n";





}

void WaveHandler::Read_5(std::string path)
{//---LOADING THE FILE---//
	//Step 1: Read the header.
		//SKIP THIS ONE FOR NOW
	//Step 2: Read all data and put it in a vector
	using namespace std;
	ifstream wav{ path,ifstream::binary };

	//Find out the chunksize
	wav.seekg(40);
	char* chuncksize = new char[4];
	wav.get(chuncksize, 4);
	unsigned long data_size = (chuncksize[3] << 24) | (chuncksize[2] << 16) | (chuncksize[1] << 8) | chuncksize[0];
	cout << "Data Size: " << data_size << "\n";

	//Put data in a char*
	wav.seekg(44);
	char* data = new char[data_size];
	wav.get(data, data_size);


	//Loop through data, check every 2 bits.
	for (int i = 0; i < data_size; i += 2) {
		(((data[i] & (0x0001 << 15)) >> 15));//check the most left bit
	}








}

void WaveHandler::Write(std::string path, std::string message)
{
	//---LOADING THE FILE---//
	//Step 1: Read the header.
		//SKIP THIS ONE FOR NOW
	//Step 2: Read all data and put it in a vector
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
	wav.get(chuncksize, 4);
	unsigned long data_size = (chuncksize[3] << 24) | (chuncksize[2] << 16) | (chuncksize[1] << 8) | chuncksize[0];
	cout << "Data Size: " << data_size << "\n";

	//Put data in a vector of 16 bitsets
	wav.seekg(44);
	std::vector<char> data;
	for (int i = 0; i < data_size; ++i) {
		data.push_back(wav.get());
	}

	std::vector<bitset<8>> bits;
	for (int i = 0; i < message.size(); i += 1) {
		bits.push_back(bitset<8>(message[i]));
	}
	bits.push_back(bitset<8>('\0'));
	int counter = 0;
	int offset = 0;
	for (int i = 0; i < data.size() && counter < bits.size(); i += 2)
	{
		char bit = data[i];

		bitset<8> bit_set = bitset<8>(bit);
		if (bit_set.test(7) != bits[counter].test(offset)) {
			bit_set.flip(7);
			data[i] = static_cast<char>(bit_set.to_ulong());
		}

		
		++offset;
		if (offset > 7) {
			offset = 0;
			++counter;
		}
	}


	wav_out.write(&data[0], data_size);


}
