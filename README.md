## Lab 6 Robot Motion

Objective: To program the robot to do basic movements without an outside connection.

#### Prelab

Which pins will I use?

I will use pins that output TimerA to output PWM signals to the motors. The pins I will use are P2.0, P2.1, P2.2, P2.3, P2.4, and P2.5. To achieve PWM, I will use a different output mode with the TimerA control. I can use TimerA output modes Set/Reset, Toggle/Reset, Toggle/Set, or Toggle/Reset. For this lab I will use Set/Reset and Reset/Set modes. As these are opposites in their high/low patterns, I can use them interchangeably depending on which direction I want the motors to spin

The pins I listed above will be used as follows

Pin | Type | Motor | Use
-----|-----|-----|-----
2.0 | GPIO | L | Enable
2.1 | GPIO | L | Directional pin
2.2 | PWM | L | Power PWM signal
2.3 | GPIO | R | Directional pin
2.4 | PWM | R | Power PWM signal
2.5 | GPIO | R | Enable

### Lab

#### Design

Much of the design work was already laid out in the prefabricated robot. All that was left to do was to connect the MSP430 with the Motor Driver chip, and the motor chip with the inputs for the motors. This was fairly simple knowing the pinout of the Motor Driver chip, which is shown below.

![alt text](https://raw.githubusercontent.com/ChrisMKiernan/ECE382_Lab6/master/MotorDriver.PNG "The pinout diagram of the SN754410 Motor Driver Chip")

Using this pinout, the pins labeled 1A, 1Y, etc are input-output pins for the same signal. For a given number, A is the input and Y is the output for that number. The pin "Vcc1" was routed to a 5V signal, and "Vcc2" was routed to a 12V signal. The 5V pin provided the power needed to run the chip, and the 12V pin provided the output voltage for the motors. 

The pins "1,2EN" and "3,4EN" are the enable pins for the Motor Driver. Essentially, these pins turned the output of the associated pins on or off if the signal was high or low, respectively. Because the EN pins on the Motor Driver are associated with 2 output signals each, it made sense to couple signals 1 and 2 to a single motor and couple signals 3 and 4 to the other motor. In this way, one motor could be turned completely off by setting the respective EN pin to LOW. This helped in programming turns. 

#### Code Design

Using Dr Coulston's code, I added declarations for more pins, so that certain signals would not be floating. In the original code, the only pins that were addressed were 2 PWM pins (P2.2 and P2.4) and the button (P1.3) that he used to change the duty cycle. I decided not to use the button, as it would be hard to press the button on the robot while it was moving, and changing the duty cycle would only hurt efforts to calculate how long turn delays needed to be. However, to achieve any motion, I first had to declare Pins P2.0, P2.1, P2.3, and P2.5. These pins provided enable signals and reference voltage signals (directional signals) for the motors, so that these signals were no longer floating. These pins are also addressed above, in the Prelab section.

After declaring the pin directions and "initial conditions" for these pins, I created a series of functions for the different movements that the robot had to do. I first created enable() and stop() functions, which gave a pause between functions so that movements were more easily interpreted. For the stop function, the EN pins simply had to be set to LOW for a set amount of time. The enable function is actually redundant in my code, as I enable the EN pins in each movement function. However, for the purpose of this lab I was not worried about wasting small amounts of time or CPU use, and I would rather be redundant in code than forget to include an enable declaration in each movement function. 

Each of the movement functions were programmed by enabling the EN pins as needed, then ensuring the directional pins were set to the correct direction, then setting a delay using a simple counter. For the turns, I could simply disable one of the motors using the EN pins and power the other motor, so that the robot would turn "around a point."

#### Implementation

The techniques above were implemented to the MSP430 included with the robot using Code Composer Studio v6. The code used on the robot is provided in the Lab6 repository. To create the delays I mentioned, I used a while() loop and a counter. While this isn't the best way to create such a delay, it works and I was more familiar with it than using for() loops or other delays. The physical pins were connected as follows on the robot:

SN754410 Pin | MSP430 Pin | Motor Pin | Motor | Use
-----|-----|-----|-----|-----
1 (1,2EN) | P2.0 | - | L | Enables Motor
2 (1A) | P2.2 | - | L | PWM signal into Motor Dirver
3 (1Y) | - | L RED | L | PWM signal out of Motor Driver
4 (GND) | - | - | - | Ground Pin
5 (GND) | - | - | - | Ground Pin
6 (2Y) | - | L BLK | L | Direction signal out of Motor Driver
7 (2A) | P2.1 | - | L | Direction signal into Motor Driver
8 (Vcc2) | - | - | - | 12V into Motor Driver to power motors
9 (3,4EN) | P2.5 | - | R | Enables Motor
10 (3A) | P2.4 | - | R | PWM signal into Motor Driver
11 (3Y) | - | R RED | R | PWM signal out of Motor Driver
12 (GND) | - | - | - | Ground Pin
13 (GND) | - | - | - | Ground Pin
14 (4Y) | - | R BLK | R | Direction signal out of Motor Driver
15 (4A) | P2.3 | - | R | Direction signal into Motor Driver
16 (Vcc1) | - | - | - | 5V into Motor Driver to power chip

Another hurdle to implementing this design was to power the robot on its own without a connection to the computer. I didn't have to worry about the code because the code always starts from the beginning upon pressing the physical reset button on the MSP430, but I did have to worry about powering the MSP430. With a provided 3.3V regulator, I connected input to 5V from the battery, and output to the Vcc input pin of the MSP430. When using this technique, it was important to disconnect the Vcc jumper on the MSP430 chip when the USB was connected. By disconnecting this, all of the power to the chip was supplied by the USB, so that there wouldn't be competing power sources. When the robot was disconnected, though, the jumper needed to be reattached so that the batteries could power the chip. 

### Debugging

I encountered several issues with this Lab. After successfully achieving forward motion and stopping the motors after a certain amount of time, I moved on to program backward motion. After programming this, I noticed one motor would spin backward correctly while the other would not. I tried several modifications to the code which eventually resulted in a CCS error in which the Lab6.out file would not load to the MSP430. This was essentially one problem that became 2 problems. The .out file error was fixed by replacing the MSP430 and subsequently moving the Vcc jumper based on whether or not the USB was plugged in. After attempting to move on and program the rest of the movements I discovered the same motor which originally would not turn backwards continued to fail in subsequent movements. I then realized that I plugged the PWM output wire for that pin into the wrong pin on the breadboard - I was trying to power the motor from the 3.3V digital output of the MSP430! Moving the wire over from 1A to 1Y solved the issue. 

After several days of testing, I only seemed to make backward progress. On the last day of functionality, the robot would not even move forward correctly. By Capt Trimble's suggestion, I connected an oscilloscope to the pins where I should have been getting a PWM signal, only to find that there was no PWM signal being generated. After triple-checking my code against the device-specific manual, I decided that there must be something inhibiting the chip from creating the PWM signal. I replaced the chip to test how a new chip would behave, then on the first test the robot performed perfectly. I decided another clumsy power dissipation mistake I made earlier must have been the cause. 

### Documentation

Started with code from Dr Coulston's website: http://ecse.bd.psu.edu/cmpen352/lecture/code/lab6.c as a framework for my code.

Received help from C2C Sean Gavan on the usage and implementation of successful functions in C. 
