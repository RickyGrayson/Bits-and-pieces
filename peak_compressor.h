#ifndef ARDUINO_DRUM_COMPRESSOR_H
#define ARDUINO_DRUM_COMPRESSOR_H

#include <iostream>

class Compressor {
	// 16Bit lo-fi peak compressor with threshold hysteresis and auo-gain.
	public:
		Compressor(uint16_t hz_sampleRate, uint16_t ms_MaxAttackTime, uint16_t ms_MaxReleaseTime);
		int16_t operator()(int16_t input);
		void setThreshold(uint16_t pot_Threshold);
		void setRatio(uint16_t pot_Ratio);
		void setAttack(uint16_t pot_Attack);
		void setRelease(uint16_t pot_Release);
	private:
		class Envelope {
			public:
				Envelope();
				uint16_t operator()(bool gate);
				void setAttackSamples(uint16_t attackSamples);
				void setSustainLevel(uint16_t sustainLevel);
				void setReleaseSamples(uint16_t releaseSamples);
			private:
				uint32_t envelopeLevel_, sustainLevel_;
				uint32_t attackStep_, releaseStep_;
		};
		class NoiseGate {
			public:
				NoiseGate();
				bool operator() (int16_t input);
				void setOpenThreshold(uint16_t openThreshold);
				void setCloseThreshold(uint16_t closeThreshold);
			private:
				bool gate_;
				uint16_t openThreshold_, closeThreshold_;
		};
		Envelope envelope_;
		NoiseGate NoiseGate_;
		uint16_t maxAttackSamples_, maxReleaseSamples_;
};
#endif
