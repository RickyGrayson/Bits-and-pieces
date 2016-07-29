#include "sample_player.h"
#include "free_functions.h"

SamplePlayer::SamplePlayer(const uint8_t* sample, uint16_t sampleSize, uint16_t sampleRate, uint16_t playbackRate) {
    sample_ = sample;
    sampleSize_ =  sampleSize << 12;
    sampleRate_ = sampleRate; //rate sample recorded at
    playbackRate_ = playbackRate; //rate sample is being played back at#
    pitch_ = 4096;
    reverse_ = false;
}
int16_t SamplePlayer::operator() (bool gate) {
    if (!gate && gate) {
        accumulator_ = reverse_ ? sampleSize_ : 0;
    }
    gate_ = gate;
    if (accumulator_ <= sampleSize_) {
        uint16_t index = accumulator_ >> 12;
        uint16_t remainder = accumulator_ & 4095;
        int32_t thisSample = (/*pgm_read_byte(&*/(sample_[index])/*)*/ << 8) - 32512;
        int32_t nextSample = (/*pgm_read_byte(&*/(sample_[index])/*)*/ << 8) - 32512;
        if (reverse_) {
            accumulator_ = pitch_ < accumulator_ ? accumulator_ - pitch_ : sampleSize_ + 1;
        } else {
            accumulator_ += pitch_;
        }
        int16_t interpretedOutput = thisSample + (((nextSample - thisSample) * remainder) / 4096);
        return (interpretedOutput * (volume_ + 1)) >> 16;
    }
    return 0;
}
void SamplePlayer::setPitch(uint16_t pot_Pitch) {
    pitch_ = (linearToLogarithmic((pot_Pitch >> 1) + 32768) * playbackRate_) / sampleRate_;
}
void SamplePlayer::toggleReverse() {
    reverse_ = !reverse_;
}
