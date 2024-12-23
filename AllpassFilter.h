#ifndef ALLPASSFILTER_H
#define ALLPASSFILTER_H

#include <cstdint>
#include <array>
#include <vector>

template<uint16_t BLOCK_SIZE>
class AllpassFilter {
private:
    uint16_t delayLength; // in samples
    uint16_t baseDelayLength;
    float gain; // 0.0 < gain < 1.0

    std::vector<float> buffer; // circular buffer of length delayLength
    uint16_t writeIndex;

    float smoothedInput; // save smoothed potentiometer value
    const float smoothingFactor;

public:
    AllpassFilter(uint16_t delayLength, float gain); // Constructor

    void process(std::array<float, BLOCK_SIZE>& inputSignal); // Process new input signal in-place

    void setDelayLength(float input); // delay length adaption using potentiometer

    void setGain(float input); // gain adaption using potentiometer
};

#include "AllpassFilter.tpp"

#endif // ALLPASSFILTER_H