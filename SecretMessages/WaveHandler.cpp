
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

	
	
	//unsigned long chuncksize = (wav[4 + 1] << 8) | wav[4];;
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

	wav.seekg(44);
	std::vector<char> data;
	for (int i = 0; i < 357936; ++i) {
		data.push_back(wav.get());
	}

	std::vector<double> rawSignal;
	std::vector<bitset<16>> bits;

	
	for (int i = 0; i < data.size(); i += 2)
	{
		int c = (data[i + 1] << 8) | data[i];
		bitset<16> bit = bitset<16>(c);
		bits.push_back(bit);
		//double t = c / 32768.0;
		rawSignal.push_back(c);
	}

	std::vector<bitset<8>> result;
	string set;
	for (int i = 0; i < bits.size(); i += 16) {
		int temp = (bits[i].test(15));
		set += std::to_string(temp);
		if (set.size() >= 8) {
			bitset<8> bit = bitset<8>(set[0]);
			result.push_back(bit);
			set.clear();
		}
	}

	string answer;
	for (auto& c : result) {
		unsigned long i = c.to_ulong();
		if (i <= CHAR_MAX)
			answer+= static_cast<char>(i);
	}
	std::vector<bitset<8>> result2;
	set.clear();
	for (int i = 0; i < answer.size(); i += 16) {
		set += answer[i];
		if (set.size() >= 8) {
			bitset<8> bit = bitset<8>(set[0]);
			result2.push_back(bit);
			set.clear();
		}
	}

	string answer2;
	for (auto& c : result2) {
		unsigned long i = c.to_ulong();
		if (i <= CHAR_MAX)
			answer2 += static_cast<char>(i);
	}

}
