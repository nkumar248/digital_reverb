#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

#include "AllpassFilter.h"
#include "FeedbackCombFilter.h"

#include <vector>

DaisySeed hw;

daisy::GPIO gpio_in_btn;
bool btn_q = true;	// button varibale for edge detection
bool fx_enable = true;

// initialize const parameters
static constexpr uint16_t BLOCK_SIZE = 16;

float resample = 3* 25000.0f / 48000.0f;

// gain
float gain = 0.8f;

// dry & wet
float dry = 0.7f;
float wet = 0.3f;

const float ADC_THRESHOLD = 0.001f;
float prev_adc_value = 0.0f;
float adc_value = 0.0f;
float prev_adc_value_2 = 0.0f;
float adc_value_2 = 0.0f;

// initialize global modules

// current architecture: 4 Allpass in series followed 8 Feedback Comb Filters in parallel
AllpassFilter<BLOCK_SIZE> allpassFilter01(resample*562, gain);
AllpassFilter<BLOCK_SIZE> allpassFilter02(resample*347, gain);
AllpassFilter<BLOCK_SIZE> allpassFilter03(resample*113, gain);
AllpassFilter<BLOCK_SIZE> allpassFilter04(resample*37, gain);

std::vector<AllpassFilter<BLOCK_SIZE>*> allpassFilters = {
	&allpassFilter01,
	&allpassFilter02,
	&allpassFilter03,
	&allpassFilter04
};

FeedbackCombFilter<BLOCK_SIZE> feedbackCombFilter01(resample*1287, 0.723, gain);
FeedbackCombFilter<BLOCK_SIZE> feedbackCombFilter02(resample*1432, 0.783, gain);
FeedbackCombFilter<BLOCK_SIZE> feedbackCombFilter03(resample*1687, 0.773, gain);
FeedbackCombFilter<BLOCK_SIZE> feedbackCombFilter04(resample*1601, 0.802, gain);
FeedbackCombFilter<BLOCK_SIZE> feedbackCombFilter05(resample*2053, 0.753, gain);
FeedbackCombFilter<BLOCK_SIZE> feedbackCombFilter06(resample*2251, 0.773, gain);
FeedbackCombFilter<BLOCK_SIZE> feedbackCombFilter07(resample*2432, 0.813, gain);
FeedbackCombFilter<BLOCK_SIZE> feedbackCombFilter08(resample*2621, 0.756, gain);

std::vector<FeedbackCombFilter<BLOCK_SIZE>*> feedbackCombFilters = {
	&feedbackCombFilter01,
	&feedbackCombFilter02,
	&feedbackCombFilter03,
	&feedbackCombFilter04,
	&feedbackCombFilter05,
	&feedbackCombFilter06,
	&feedbackCombFilter07,
	&feedbackCombFilter08
};


void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	std::array<float, BLOCK_SIZE> signal;
	if (fx_enable) {
		// copy into array (ony left)
		std::array<float, BLOCK_SIZE> signal;
		for (uint16_t i = 0; i < BLOCK_SIZE; i++) {
			signal[i] = in[0][i];
		}

		// process ..

		// .. through allpass filters in series (in-place)
		for (uint8_t i = 0; i < 4; i++) {
			allpassFilters[i]->process(signal);
		}

		// .. through feedback comb filters in parallel
		std::array<std::array<float, BLOCK_SIZE>, 8> outputs;
		for (uint8_t i = 0; i < 8; i++) {
			outputs[i] = feedbackCombFilters[i]->process(signal);
		}

		// get weighted sum
		std::array<float, BLOCK_SIZE> outputAvg = {0.0f};
		for (uint8_t i = 0; i < 8; i++) {
			for (uint16_t j = 0; j < BLOCK_SIZE; j++) {
				outputAvg[j] += 0.125 * outputs[i][j];
			}
		}
		
		// write to output with wet / dry mix 
		for (size_t i = 0; i < size; i++)
		{
			out[0][i] = dry * in[0][i] + wet * outputAvg[i];
			out[1][i] = dry * in[0][i] + wet * outputAvg[i];
		}
	} else {
		// no effect
		for (size_t i = 0; i < size; i++)
		{
			out[0][i] = in[0][i];
			out[1][i] = in[1][i];
		}
	}
}

int main(void)
{
	hw.Init();

	gpio_in_btn.Init(seed::D8, GPIO::Mode::INPUT, GPIO::Pull::PULLUP); // use internal pull-up

	// initialize ADC
	AdcChannelConfig adcConfig[2];
	adcConfig[0].InitSingle(hw.GetPin(15));	// GPIO15 / ADC 0
	adcConfig[1].InitSingle(hw.GetPin(17)); // GPIO17 / ADC 2
	hw.adc.Init(adcConfig, 2);
	hw.adc.Start();

	hw.SetAudioBlockSize(BLOCK_SIZE); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartAudio(AudioCallback);

	while(1) {
		bool btn_d = gpio_in_btn.Read();
		if(btn_d == 0 && btn_q == 1) { // detect falling edge
			fx_enable = !fx_enable;
		}
		btn_q = btn_d;
		
		// read potentiometer 1 -> modulate delay lengths
		prev_adc_value = adc_value;
		adc_value = hw.adc.GetFloat(0);

		// read potentiometer 2 -> modulate wet / dry mix 
		prev_adc_value_2 = adc_value_2;
		adc_value_2 = hw.adc.GetFloat(1);


		if (fabs(adc_value - prev_adc_value) >= ADC_THRESHOLD) {
			for (auto* ap : allpassFilters) {
				ap->setDelayLength(adc_value);
			}
			for (auto* fbcf : feedbackCombFilters) {
				fbcf->setDelayLength(adc_value);
			}
		}

		wet = adc_value_2;
		dry = 1.0f - wet;

		// delay 10ms for debouncing
		System::Delay(10);
	}
}
