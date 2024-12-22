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

Also, I added two potentiometers controls that control the reverb "size" and the dry/wet mix. The reverb size parameter changes the delay lengths in all filters through a logarithmic mapping. 

## Software
The concept was implemented in C++ from scratch (using a blank DaisySeed project template). I followed an object-oriented approach to keep the code organized and optimized memory use by changing data types for variables, passing known values as compile-time constants, and using fixed-size circular buffers for example.

## Demo

### Example 1: Drum Loop

Reverb: Off.

Reverb: On, Size: ~0%, Wet: 50%

Reverb: On, Size: ~100%, Wet:50%


### Example 2: Singing Vocals

Reverb: Off.

Reverb: On, Size: ~0%, Wet: 50%

Reverb: On, Size: ~100%, Wet:50%


## Evaluation and Improvements


[1]: https://ccrma.stanford.edu/~jos/pasp/Schroeder_Reverberators.html
