#ifndef __WaveHandler_h__
#define __WaveHandler_h__

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <istream>

typedef struct WAV_HEADER {
	char                RIFF[4];        // RIFF Header
	unsigned long       ChunkSize;      // RIFF Chunk Size  
	char                WAVE[4];        // WAVE Header      
	char                fmt[4];         // FMT header       
	unsigned long       Subchunk1Size;  // Size of the fmt chunk                                
	unsigned short      AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM 
	unsigned short      NumOfChan;      // Number of channels 1=Mono 2=Sterio                   
	unsigned long       SamplesPerSec;  // Sampling Frequency in Hz                             
	unsigned long       bytesPerSec;    // bytes per second 
	unsigned short      blockAlign;     // 2=16-bit mono, 4=16-bit stereo 
	unsigned short      bitsPerSample;  // Number of bits per sample      
	char                Subchunk2ID[4]; // "data"  string   
	unsigned long       Subchunk2Size;  // Sampled data length    

}wav_hdr;

typedef struct AUDIODATA {
	std::vector<char>	data;
}audio_data;






class WaveHandler
{
public:
	WaveHandler();
	void Read(std::string path);
	void Read_2(std::string path);
	void Read_3(std::string path);
	void Read_4(std::string path);
	void Read_5(std::string path);
	void Write(std::string path, std::string message);
};

#endif