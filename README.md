[WORK IN PROGRESS]

# Digital Reverb on MCU
Schroeder Reverb Variation implemented on a microcontroller (Daisy Seed). The Project included soldering various components on a PCB board as well as the software implementation of the reverb. 

## Hardware
<p align="center">
  <img src="https://github.com/user-attachments/assets/53c19c0d-a246-4ece-9cc3-dd77a06c6dfb"
       alt="IMG_5688"
       width="400"/>
</p>

The hardware setup included soldering various components on a PCB board using both THS and SMD techniques.

Soldered components include:
- Barrel Power Connector, LED power indicator
- Control Elements: Potentiometers, Button
- Audio Input/Outut: TRS jacks
- Small components: Resistors, Capacitors


## Concept
This project is based on the original Schroeder Reverb from the 1960s. The Schroeder Reverb as described in [1] consists of three Allpass Filters (AP) followed by a parallel bank of Feedback Comb Filters (FBC). All filters contain varying delay and decay times. This reverb design is a Feedback Delay Network (FDN) which is an ideal leightweight design option for programming on a microcontroler. I enhanced this design by adding an additional AP and four additional FBCs to enhance the reverberation effect. While the decay parameters are copied as is (and slightly varied for the added filters), I increased the delay legths significantly by ear (the base values did not produce a strong reverberation in my implementation). 

Also, I added two potentiometers that control the reverb "size" and the dry/wet mix. The reverb size parameter changes the delay lengths in all filters through a logarithmic mapping. I had included modulating the decay in all filters using the reverb size as well but this did not sound good in practice.  


![schroeder_reverb-1](https://github.com/user-attachments/assets/f9ec3a4c-8161-424d-862d-dac8188b8870)
Black denotes the original Schroeder Reverb architecture ([1]), red denotes my additions for a stronger reverberation.


## Software
The concept was implemented in C++ from scratch (using a blank DaisySeed project template). I followed an object-oriented approach to keep the code organized and optimized memory use by changing data types for variables, passing known values as compile-time constants, and using fixed-size circular buffers for example. The current implementation only considers mono audio. I implemented parameter smoothing and potentiometer reading with thresholding for the reverb size (so that the buffers are not resized constantly due to noise). 


## Demo

### Example 1: Drum Loop


https://github.com/user-attachments/assets/d616ea1d-ac11-44e4-9ed1-79dd8cb6d8c9

Reverb: Off.


https://github.com/user-attachments/assets/9247a909-1b5f-4931-9d35-ac58ff8791a5

Reverb: On, Size: ~0%, Wet: 50%


https://github.com/user-attachments/assets/254d724f-d54a-419e-bde3-ca60c3e0abf6

Reverb: On, Size: ~100%, Wet: 50%

### More examples soon
[TODO]


### Glitch example
Reverb size modulation during audio playback causes audible glitches.

https://github.com/user-attachments/assets/49df506e-d5ab-418a-9e3a-f1bea5bb03ea


## Evaluation and Improvements
This was my first attempt at writing an audio effect in general and it was a big learning experience. Key learnings included digital reverb concepts and design choices, digital audio processing, C++ programming, and hands-on "hardware construction" (pcb soldering). The results sound good in my opinion, especially considering that the architecture is fairly simple. Even the reverb tails sound pretty good after increasing the parallel FBC bank to 8 units. 

Above is an example of how the MCU produces audible glitches when the reverb size is modulated during audio playback. This happens because the internal buffers' maximum read/write index (the actual data structures are fixed-size) are changed during playback and this causes abrupt changes in the signals read from the different buffers which in turn causes audible glitches. I implemented smoothing (the buffers' maximum read/write indices are adapted gradually upon user input) but this did not solve the problem. From experience with professional digital plugins I know that those have these audible glitches too so it may not be a fully solvable problem. 

Inspired by building this reverb I want to next either build a different audio effect (e.g. distortion), a synthesizer, or a different reverb (e.g. a convolution reverb). 


## Disclaimer
This project was conceptualized and completed in a voluntary participation in a course at ETH Zurich. It was a "passion participation" and I did not receive any credits for it as it was a course from the Electrical Engineering department.


[1]: https://ccrma.stanford.edu/~jos/pasp/Schroeder_Reverberators.html
