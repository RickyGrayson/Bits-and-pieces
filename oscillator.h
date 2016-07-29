#ifndef ARDUINO_DRUM_OSCILLATOR_H
#define ARDUINO_DRUM_OSCILLATOR_H

#include <iostream>

class Oscillator {
    public:
        //enum Waves {Pulse, SawTooth, Sine, Square, Triangle};
        Oscillator(uint16_t sampleRate);
        void update();
        //void getWave(Waves wave);
        int16_t sineWave();
        int16_t squareWave();
        int16_t pulseWave();
        int16_t sawtoothWave();
        int16_t triangleWave();
        void setPitch(uint16_t pitch);
        void setPulseWidth(uint16_t pulseWidth);
        void Sync();
        void setPhase(uint16_t phase);
    private:
        uint16_t nyquist_;
        uint32_t hertz_;
        uint32_t accumulator_, phasedAccumulator_;
        uint32_t pitch_, phase_, pulseWidth_;

};

#endif
