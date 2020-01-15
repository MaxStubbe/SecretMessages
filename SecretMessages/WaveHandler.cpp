
#define _CRT_SECURE_NO_DEPRECATE
#include "WaveHandler.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>

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

		cout << "File is                    :" << filelength << " bytes." << endl;
		cout << "RIFF header                :" << wavHeader.RIFF[0] << wavHeader.RIFF[1] << wavHeader.RIFF[2] << wavHeader.RIFF[3] << endl;
		cout << "WAVE header                :" << wavHeader.WAVE[0] << wavHeader.WAVE[1] << wavHeader.WAVE[2] << wavHeader.WAVE[3] << endl;
		cout << "FMT                        :" << wavHeader.fmt[0] << wavHeader.fmt[1] << wavHeader.fmt[2] << wavHeader.fmt[3] << endl;
		cout << "Data size                  :" << wavHeader.ChunkSize << endl;
		cout << "Sampling Rate              :" << wavHeader.SamplesPerSec << endl;
		cout << "Number of bits used        :" << wavHeader.bitsPerSample << endl;
		cout << "Number of channels         :" << wavHeader.NumOfChan << endl;
		cout << "Number of bytes per second :" << wavHeader.bytesPerSec << endl;
		cout << "Data length                :" << wavHeader.Subchunk2Size << endl;
		cout << "Audio Format               :" << wavHeader.AudioFormat << endl;
		// Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM

		cout << "Block align                :" << wavHeader.blockAlign << endl;
		cout << "Data string                :" << wavHeader.Subchunk2ID[0] << wavHeader.Subchunk2ID[1] << wavHeader.Subchunk2ID[2] << wavHeader.Subchunk2ID[3] << endl;
		



		
		//Fill data with actual data.
		std::vector<char> data;




		
		std::vector<double> rawSignal;
		int size = data.size();
		for (int i = 0; i < size; i += 2)
		{
			int16_t c = (data[i + 1] << 8) | data[i];
			double t = c / 32768.0;
			rawSignal.push_back(t);
		}
	
		std::cout << "test";
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

void WaveHandler::Read_3(std::string path)
{
	//---LOADING THE FILE---//
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

	//Put data in a vector of 16 bitsets
	wav.seekg(44);
	std::vector<bitset<16>> bits;
	string temp;
	for (int i = 0; i < data_size; i +=2) {//16 bit thingies
		int8_t temp = wav.get();
		int8_t temp2 = wav.get();
		int16_t c = temp2 | (temp << 8);
		bitset<16> bit = bitset<16>(c);
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

	int counter = 0;
	string answer;
	for (auto& current_byte : result) {
		if (current_byte.test(7) == 1) {
			//extra byte needed
		}
		unsigned long i = current_byte.to_ulong();
		if (i <= CHAR_MAX) {
			char c = static_cast<char>(i);
			if (i == '\0') {
				if(answer.size() > 5)
					cout << answer << "\n";
				answer.clear();
				counter++;
			}
			answer += c;
		}
	}
	cout << "Counted: " << counter << "\n";



	//---READING THE MESSAGE---//
	//Step 1: Keep reading till you find a null byte (0000 0000).
	//Step 2: Check if message is UTF-8. If yes: done, if no:  clear currently read bytes and go to step 1.



}

std::string IntToUTF8String(int convertMe) {
	if (convertMe == 0)
		return " ";
	if ((convertMe <= 0x7F) && (convertMe > 0x00)) {

		std::string out(".");

		std::bitset<8> x(convertMe);

		unsigned long l = x.to_ulong();
		unsigned char c = static_cast<unsigned char>(l);
		out[0] = c;

		return out;

	}
	else if ((convertMe >= 0x80) && (convertMe <= 0x07FF)) {

		std::string out("..");

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


		std::string out("...");

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
		return " ";
	}

}