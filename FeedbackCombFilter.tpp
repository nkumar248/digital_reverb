#include "FeedbackCombFilter.h"

// Constructor implementation
template<uint16_t BLOCK_SIZE>
FeedbackCombFilter<BLOCK_SIZE>::FeedbackCombFilter(uint16_t delayLength, float feedbackGain, float feedforwardGain) 
    : delayLength(delayLength), baseDelayLength(delayLength), feedbackGain(feedbackGain), feedforwardGain(feedforwardGain), writeIndex(0), smoothedInput(0.0f), smoothingFactor(0.1f)
{
    this->buffer.resize(3 * this->delayLength, 0.0f); // init as 2x delay length so user can modulate from 0.5x to 2x
}


// Process implementation in-place
template<uint16_t BLOCK_SIZE>
std::array<float, BLOCK_SIZE> FeedbackCombFilter<BLOCK_SIZE>::process(std::array<float, BLOCK_SIZE> inputSignal) {
    // difference equation: y(n) = b_0 * x(n) - a_M * y(n-M)
    // save y(n-M) in buffer
    std::array<float, BLOCK_SIZE> outputSignal;

    for (uint16_t i = 0; i < BLOCK_SIZE; i++) {
        float inputSample = inputSignal[i];
        
        uint16_t readIndex = this->writeIndex;
        float bufferOutput = this->buffer[readIndex];

        // calculate output sample
        float outputSample = this->feedforwardGain * inputSample - this->feedbackGain * bufferOutput;

        // write to output signal
        outputSignal[i] = outputSample;

        // update buffer
        this->buffer[this->writeIndex] = outputSample; // overwrite 

        this->writeIndex = (this->writeIndex + 1) % this->delayLength;
    }

    return outputSignal;
}


template<uint16_t BLOCK_SIZE>
void FeedbackCombFilter<BLOCK_SIZE>::setDelayLength(float input) {
    // parameter smoothing
    this->smoothedInput = (1.0f - this->smoothingFactor) * this->smoothedInput + this->smoothingFactor * input;

    // map input logarithmically
    float min_factor = 0.5f;
    float max_factor = 3.0f;
    float factor = min_factor * powf(max_factor / min_factor, this->smoothedInput);

    // approach 1:
    // modulate delay length from 0.5x to 3x delay length
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
void FeedbackCombFilter<BLOCK_SIZE>::setFeedbackGain(float input) {
    // do somethig with feedback gain
}

template<uint16_t BLOCK_SIZE>
void FeedbackCombFilter<BLOCK_SIZE>::setFeedforwardGain(float input) {
    // do something with feedforward gain
}