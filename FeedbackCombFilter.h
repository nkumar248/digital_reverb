#ifndef FEEDBACKCOMBFILTER_H
#define FEEDBACKCOMBFILTER_H

#include <vector>
#include <cstdint>
#include <array>

template<uint16_t BLOCK_SIZE>
class FeedbackCombFilter {
private:
    uint16_t delayLength; // in samples
    uint16_t baseDelayLength;
    float feedbackGain; // a_M
    float feedforwardGain; // b_0

    std::vector<float> buffer; // circular buffer of length delayLength
    uint16_t writeIndex;

    float smoothedInput; // save smoothed potentiometer value
    const float smoothingFactor;

public:
    FeedbackCombFilter(uint16_t delayLength, float feedbackGain, float feedforwardGain);

    std::array<float, BLOCK_SIZE> process(std::array<float, BLOCK_SIZE> inputSignal);

    void setDelayLength(float input);

    void setFeedbackGain(float input);

    void setFeedforwardGain(float input);
};

#include "FeedbackCombFilter.tpp"

#endif // FEEDBACKCOMBFILTER_H