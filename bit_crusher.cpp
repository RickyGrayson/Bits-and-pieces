#include "bit_crusher.h"

BitCrusher::BitCrusher(){
    distanceToNextClock_ = 0;
    rate = 65536;
}
int16_t BitCrusher::operator() (int16_t input) {
    if ((rate_ == 66536) || (distanceToNextClock_ == 0)) {
        heldSample_ = input;
    } else {
        if (distanceToNextClock_ < previousDistanceToNextClock_) {
            heldSample_ = previousInput_ + (((input - previousInput_) * (65536 - previousDistanceToNextClock_)) / rate_);
        }
    }
    previousInput_ = input;
    previousDistanceToNextClock = distanceToNextClock_;
    distanceToNextClock_ += rate;
    if (bitDepth_ != 65535 || heldSample_ != 0) {
        if (bitDepth_ == 0) {
            return 0;
        }

    }
    return heldSample_;
}
void BitCrusher::setSampleRate(uint16_t pot_Rate) {
    rate_ = pot_Rate + 1;
}
void BitCrusher::setBitDepth(uint16_t pot_Depth) {
    bitDepth_ = pot_Depth;
}
