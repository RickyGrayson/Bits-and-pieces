#include "biquad_filter.h"
#include "free_functions.h"

BiquadFilter::BiquadFilter(uint16_t sampleRate) {
	nyquist_ = sampleRate >> 1;
	delayedFeedBack_[0] =  0;
	delayedFeedBack_[1] =  0;
}
int16_t BiquadFilter::operator() (int16_t input) {
	//would like a circular buffer.
	int32_t feedBack = ((input * a0_) - (delayedFeedBack_[0] * a1_) - (delayedFeedBack_[1] * a2_)) / a0_;
	int32_t filterOutput = ((feedBack * b0_) + (delayedFeedBack_[0] * b1_) + (delayedFeedBack_[1] * b2_)) / a0_;
	delayedFeedBack_[1] = delayedFeedBack_[0];
	delayedFeedBack_[0] = feedBack;
	return filterOutput > 65535 ? 65535 : filterOutput < (0 - 65535) ? (0 - 65535) : filterOutput;
}
void BiquadFilter::setCutOff(uint16_t pot_CutOff) {
	angle_ = (65535 * pot_CutOff) / nyquist_;
	calculateCoefficients(angle_, resonance_);
}
void BiquadFilter::setResonance(uint16_t pot_Resonance) {
	resonance_ = pot_Resonance;
	calculateCoefficients(angle_, resonance_);
}
LowPassBiquadFilter::LowPassBiquadFilter(uint16_t sampleRate) : BiquadFilter(sampleRate) {
}
void LowPassBiquadFilter::calculateCoefficients(uint16_t angle, uint16_t resonance) {
    int16_t cosine = sine(angle + 16384);
	int16_t alpha = (sine(angle) * (65535 - resonance)) /65535;
	b1_ = 65535 - cosine;
	b0_ = b1_ / 2;
	b2_ = b0_;
	a0_ = 65535 + alpha;
	a1_ = 0 - (cosine * 2);
	a2_ = 65535 - alpha;
}
HighPassBiquadFilter::HighPassBiquadFilter(uint16_t sampleRate) : BiquadFilter(sampleRate) {
}
void HighPassBiquadFilter::calculateCoefficients(uint16_t angle, uint16_t resonance) {
    int16_t cosine = sine(angle + 16384);
	int32_t alpha = (sine(angle) * (65535 - resonance)) /65535;
	b1_ = 0 - (65535 + cosine);
	b0_ = (65535 + cosine) / 2;
	b2_ = b0_;
	a0_ = 65535 + alpha;
	a1_ = 0 - (cosine * 2);
	a2_ = 65535 - alpha;
}
