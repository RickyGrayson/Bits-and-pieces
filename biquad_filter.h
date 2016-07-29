#ifndef ARDUINO_DRUM_BIQUAD_FILTER_H
#define ARDUINO_DRUM_BIQUAD_FILTER_H

#include <iostream>

class BiquadFilter {
	public:
		int16_t operator() (int16_t input);
		void setCutOff(uint16_t pot_CutOff);
		void setResonance(uint16_t pot_Resonance);
	protected:
		BiquadFilter(uint16_t sampleRate);
		virtual void calculateCoefficients(uint16_t angle, uint16_t resonance) = 0;
		uint16_t nyquist_;
		uint16_t angle_, resonance_;
		int16_t b0_, b1_, b2_, a0_, a1_, a2_;
		int16_t delayedFeedBack_[2];
};
class LowPassBiquadFilter: public BiquadFilter {
    public:
        LowPassBiquadFilter(uint16_t sampleRate);
	private:
		void calculateCoefficients(uint16_t angle, uint16_t resonance);
};
class HighPassBiquadFilter: public BiquadFilter {
    public:
        HighPassBiquadFilter(uint16_t sampleRate);
	private:
		void calculateCoefficients(uint16_t angle, uint16_t resonance);
};

#endif
