#include "peak_compressor.h"
#include "free_functions.h"

Compressor::Envelope::Envelope() {
	envelopeLevel_ = 0;
	setSustainLevel(65535);
	setAttackSamples(1);
	setReleaseSamples(1);
}
uint16_t Compressor::Envelope::operator() (bool gate) {
	if (gate) {
		envelopeLevel_ = attackStep_ < (sustainLevel_ - envelopeLevel_) ? (envelopeLevel_ + attackStep_) : sustainLevel_;
	} else {
		envelopeLevel_ = releaseStep_ > envelopeLevel_ ? (envelopeLevel_ - releaseStep_) : 0;
	}
	return envelopeLevel_ >> 16;
}
void Compressor::Envelope::setAttackSamples(uint16_t attackSamples) {
	attackStep_ = sustainLevel_ / (attackSamples + 1);
}
void Compressor::Envelope::setSustainLevel(uint16_t sustainLevel) {
	sustainLevel <<= 16;
	uint16_t attackSamples = sustainLevel_ / attackStep_;
	uint16_t releaseSamples = sustainLevel_ / releaseStep_;
	sustainLevel_ = sustainLevel;
	attackStep_ = sustainLevel_ / attackSamples;
	releaseStep_ = sustainLevel_ / releaseSamples;
}
void Compressor::Envelope::setReleaseSamples(uint16_t releaseSamples) {
	releaseStep_ = sustainLevel_ / (releaseSamples + 1);
}
Compressor::NoiseGate::NoiseGate() {
	gate_ = false;
	setOpenThreshold(1);
	setCloseThreshold(1);
}
bool Compressor::NoiseGate::operator() (int16_t input) {
	uint16_t inputAmplitude = input > 0 ? input : (0 - input);
	gate_ = inputAmplitude > openThreshold_ ? true : gate_;
	gate_ = inputAmplitude < closeThreshold_ ? false : gate_;
	return gate_;
}
void Compressor::NoiseGate::setOpenThreshold(uint16_t openThreshold) {
	openThreshold_ = openThreshold;
}
void Compressor::NoiseGate::setCloseThreshold(uint16_t closeThreshold) {
	closeThreshold_ = closeThreshold;
}
Compressor::Compressor(uint16_t hz_sampleRate, uint16_t ms_MaxAttackTime, uint16_t ms_MaxReleaseTime) {
	maxAttackSamples_ = (hz_sampleRate * ms_MaxAttackTime) / 1000;
	maxReleaseSamples_ = (hz_sampleRate * ms_MaxReleaseTime) / 1000;
}
int16_t Compressor::operator() (int16_t input) {
	uint16_t ratioLevel_ = 65535ul - linearToLogarithmic(envelope_(NoiseGate_(input)));
	return ((int32_t)input * ratioLevel_) >> 16;
}
void Compressor::setThreshold(uint16_t pot_Threshold) {
	uint16_t logarithmic_Threshold = linearToLogarithmic(pot_Threshold);
	NoiseGate_.setOpenThreshold(logarithmic_Threshold);
	NoiseGate_.setCloseThreshold(logarithmic_Threshold >> 1);
}
void Compressor::setRatio(uint16_t pot_Ratio) {
	envelope_.setSustainLevel(pot_Ratio);
}
void Compressor::setAttack(uint16_t pot_Attack) {
	envelope_.setAttackSamples(((uint32_t)maxAttackSamples_ * linearToLogarithmic(pot_Attack)) >> 16);
}
void Compressor::setRelease(uint16_t pot_Release) {
	envelope_.setAttackSamples(((uint32_t)maxReleaseSamples_ * linearToLogarithmic(pot_Release)) >> 16);
}
