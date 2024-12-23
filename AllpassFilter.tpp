#include "AllpassFilter.h"

#include <cmath>

// Constructor implementation
template<uint16_t BLOCK_SIZE>
AllpassFilter<BLOCK_SIZE>::AllpassFilter(uint16_t delayLength, float gain) 
    : delayLength(delayLength), baseDelayLength(delayLength), gain(gain), writeIndex(0), smoothedInput(0.0f), smoothingFactor(0.1f)
{
    this->buffer.resize(static_cast<int>(3 * this->delayLength), 0.0f); // init as 2x delay length so user can modulate from 0.5x to 2x
}

// Process implementation
template<uint16_t BLOCK_SIZE>
void AllpassFilter<BLOCK_SIZE>::process(std::array<float, BLOCK_SIZE>& inputSignal) {
    // difference equation: y(n) = g*x(n) + x(n-D) - g*y(n-D)
    // save x(n-D) - g*y(n-D) in buffer
    // calc g*x(n) + buffer(n-D) and write to inputSignal

    for (uint16_t i = 0; i < BLOCK_SIZE; i++) {
        float inputSample = inputSignal[i]; // x(n)

        uint16_t readIdx = this->writeIndex;
        float bufferOutput = this->buffer[readIdx]; // x(n-D) - g*y(n-D) (saved at n = writeIndex basically as it wraps around)

        // calculate output sample
        float outputSample = this->gain * inputSample + bufferOutput; // y(n)

        // write to signal in-place
        inputSignal[i] = outputSample;

        // update buffer by writing current x(n) - g * y(n) to the buffer (which will be x(n-D) - g*y(n-D) for a future iteration)
        this->buffer[this->writeIndex] = inputSample - this->gain * outputSample;

        this->writeIndex = (this->writeIndex + 1) % this->delayLength;
    }
}


template<uint16_t BLOCK_SIZE>
void AllpassFilter<BLOCK_SIZE>::setDelayLength(float input) {
    // parameter smoothing
    this->smoothedInput = (1.0f - this->smoothingFactor) * this->smoothedInput + this->smoothingFactor * input;

    // map input logarithmically
    float min_factor = 0.5f;
    float max_factor = 3.0f;
    float factor = min_factor * powf(max_factor / min_factor, this->smoothedInput);

    // modulate delay length from 0.5x to 3x delay length
    // approach 1:
    this->delayLength = factor * this->baseDelayLength;
    this->writeIndex = 0;

    // approach 2:
    /*
   this->delayLength = factor * this->baseDelayLength;
   this->buffer.resize(this->delayLength);
   this->writeIndex = 0;
   */
}



template<uint16_t BLOCK_SIZE>
void AllpassFilter<BLOCK_SIZE>::setGain(float input) {
    // do something with gain
}