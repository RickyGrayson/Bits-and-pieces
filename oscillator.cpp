#include "oscillator.h"
#include "free_functions.h"

Oscillator::Oscillator(uint16_t sampleRate) {
    nyquist_ = sampleRate >> 1;
    hertz_ = (1 << 31) / (nyquist_ >> 1);
    accumulator_ = 0;
}
void Oscillator::update() {
    accumulator_ += pitch_;
    phasedAccumulator_ = accumulator_ + phase_;
}
int16_t Oscillator::sineWave() {
    return sine(phasedAccumulator_ >> 16);
}
int16_t Oscillator::squareWave() {
    int32_t squareWaveAccumulumulator = phasedAccumulator_ + (1 << 31);
    if ((squareWaveAccumulumulator == 0) || (squareWaveAccumulumulator == (1 << 31))) {
        return 0;
    }
    return (squareWaveAccumulumulator & (1 << 31)) ? 0 - 32767 : 32767;
}
int16_t Oscillator::sawtoothWave() {
    int16_t saw = (phasedAccumulator_ + (1 << 31)) >> 17;
    return saw == 0 ? 0 : saw - 32768;
}
int16_t Oscillator::triangleWave() {
    int32_t trianglePhase = phasedAccumulator_ + (1 << 30);
    int16_t triangle = trianglePhase & (1 << 31) ? trianglePhase >> 15 : ((0 - trianglePhase)  >> 15);
    return triangle == 65536 ? 32767 : triangle == 0 ? 0 - 32767 : triangle - 32768;
}
void Oscillator::setPitch(uint16_t pitch) {
    pitch_ = hertz_ * (((pitch + 1) * nyquist_) >> 16);
}
void Oscillator::setPulseWidth(uint16_t pulseWidth) {
    pulseWidth_ = pulseWidth << 16;
}
void Oscillator::Sync() {
    accumulator_ = 0;
}
void Oscillator::setPhase(uint16_t phase) {
    phase_ = phase << 16;
}
