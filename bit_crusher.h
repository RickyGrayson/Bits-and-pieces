#ifndef ARDUINO_DRUM_BIT_CRUSHER_H
#define ARDUINO_DRUM_BIT_CRUSHER_H

#include <iostream>

class BitCrusher {
    public:
        BitCrusher(uint16_t sampleRate);
        int16_t operator() (int16_t input);
        void setSampleRate(uint16_t pot_Rate);
        void setBitDepth(uint16_t pot_Depth);
    private:
        int16_t accumulator_;
        int32_t rate_;
        uint16_t heldSample_;
        uint16_t previousSawTooth_;
        uint16_t previousInput_;
        bool passThru_;
        uint16_t bitDepth_, halfBitDepth_, scaler_;
};

#endif
