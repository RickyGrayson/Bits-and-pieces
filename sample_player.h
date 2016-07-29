#ifndef ARDUINO_DRUM_SAMPLE_PLAYER_H
#define ARDUINO_DRUM_SAMPLE_PLAYER_H

#include <iostream>

class SamplePlayer {
	public:
		SamplePlayer(const uint8_t* sample, uint16_t sampleSize, uint16_t sampleRate, uint16_t playbackRate);
		int16_t operator() (bool gate);
		void setPitch(uint16_t pot_Pitch);
		void toggleReverse();
	private:
		bool gate_;
		bool reverse_;
		uint32_t pitch_;
		uint16_t volume_;
        const uint8_t* sample_;
        uint16_t sampleSize_;
        uint16_t sampleRate_;
        uint16_t playbackRate_;
        uint32_t accumulator_;
};

#endif
