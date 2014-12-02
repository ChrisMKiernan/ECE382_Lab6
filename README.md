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
