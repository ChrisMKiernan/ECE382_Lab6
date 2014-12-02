/*
 * ECE 382 Lab 6 - Robot Motion
 * Author: Christopher Kiernan
 * Purpose: to have controlled movement of the robot: forward, backward, and turns
 */

#include <msp430.h>

void moveFWD(void);
void moveBCK(void);
void turnRGT(void);
void turnLFT(void);
void turnRGTsmall(void);
void turnLFTsmall(void);
void enable(void);
void stop(void);
void stopFNL(void);

void main(void) {
    WDTCTL = WDTPW|WDTHOLD;                 // stop the watchdog timer

    P1DIR &= ~BIT3;							// Allow the button to be used as an input
    P1REN |= BIT3;							// This is later discarded

    P2DIR |= BIT2;							// P2.2 is associated with TA1CCR1
    P2SEL |= BIT2;							// P2.2 is associated with TA1CCTL1

    P2DIR |= BIT4;							// P2.4 is associated with TA1CCR2
    P2SEL |= BIT4;							// P2.4 is associated with TA1CCTL2

    P2DIR |= BIT0;							// P2.0 is the enable pin for L Motor
    P2OUT |= BIT0;

    P2DIR |= BIT5;							// P2.5 is the enable pin for R Motor
    P2OUT |= BIT5;

    P2DIR |= BIT1;							// P2.1 is the directional pin for L motor
    P2OUT |= BIT1;							// Directional pins are used to give either a (+) or (-) net voltage across the motor - resulting in FWD or BCK

    P2DIR |= BIT3;							// P2.3 is the directional pin for R motor
    P2OUT &= ~BIT3;

	TA1CTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:8 presclar off MCLK
    TA1CCR0 = 0x0100;						// set signal period

    TA1CCR1 = 0x0050;
    TA1CCTL1 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode - admittedly unneeded with a duty cycle of 50%, but included to be thorough

    TA1CCR2 = 0x0050;
    TA1CCTL2 = OUTMOD_3;					// set TACCTL2 to Set / Reset mode


   /* while (1) {

    	while((P1IN & BIT3) != 0);			// Wait for a button press
    	while((P1IN & BIT3) == 0);			// and release

        TA1CCR1 = (TA1CCR1 + 0x010) & 0xFF;	// increase duty cycle
        TA1CCR2 = (TA1CCR2 + 0x010) & 0xFF;	// decrease duty cycle


    } // end loop	*/

    while (1) {
    	stop();								// This is the start of the series of commands given to the robot
    	enable();							// For each command, I always stop the robot using the stop() function
		moveFWD();							// The stop() function includes a delay so that the robot's movements are more easily interpreted
		stop();								// After using stop(), I include a redundant enable() function to turn the enable pins back on
		enable();							// All other commands are labeled for what they do - move or turn - and in which direction
		moveBCK();
		stop();
		enable();
		turnRGT();
		stop();
		enable();
		turnLFT();
		stop();
		enable();
		turnRGTsmall();
		stop();
		enable();
		turnLFTsmall();
		stopFNL();							// This function includes a CPU trap so that the robot doesn't continue to move after executing all commands
    }


} // end main

void enable(void){							// This function enables both motors so that they can be used in another function
	P2OUT |= BIT0;							// P2.0 enables L Motor
	P2OUT |= BIT5;							// P2.5 enables R Motor
}

void stop(void){							// This function clears the enable pins and includes a delay for a couple seconds
	int i=0;								// Clearing the enable pins stops the robot as neither motor can function
	while(i<0xDFFF){
		P2OUT &= ~BIT0;
		P2OUT &= ~BIT5;
		i++;
	}
}

void moveFWD(void){							// This function moves the robot forward for a couple seconds
	int i=0;

	while(i<0xFFFF){
		P2OUT |= BIT0;						// Redundant motor enables
	    P2OUT |= BIT5;

	    P2OUT |= BIT1;						// Confirm correct motor direction
	    P2OUT &= ~BIT3;

	    TA1CCTL1 = OUTMOD_7;				// To be thorough - not needed for duty cylce of 50% but otherwise is helpful

	    TA1CCTL2 = OUTMOD_3;

	    i++;
	}
}

void moveBCK(void){							// This function moves the robot backward for a few seconds
	int i=0;

	while(i<0xFFFF){
		P2OUT |= BIT0;						// Redundant motor enables
	    P2OUT |= BIT5;

	    P2OUT &= ~BIT1;						// Set appropriate directions for backward motion
	    P2OUT |= BIT3;

	    TA1CCTL1 = OUTMOD_3;				// Only actually needed for duty cycle != 50%

	    TA1CCTL2 = OUTMOD_7;

	    i++;
	}
}

void turnLFT(void){							// This function turns the robot left
	int i=0;								// On the tile floor of the lab, the turn here is about 450 degrees

    while(i<0xCFFF){
    	P2OUT |= BIT5;						// Enable R Motor
    	P2OUT &= ~BIT0;						// Disable L Motor

    	P2OUT &= ~BIT3;						// Set direction for R motor to FWD

    	TA1CCTL2 = OUTMOD_3;

    	i++;
    }
}

void turnRGT(void){							// This function turns the robot right
	int i=0;								// Similar to the turnLFT() function, this is about 450 degrees of turn

    while(i<0xCFFF){
    	P2OUT &= ~BIT5;						// Disable right motor
    	P2OUT |= BIT0;						// Enable left motor

    	P2OUT |= BIT1;						// Confirm direction for L motor is FWD

    	TA1CCTL1 = OUTMOD_7;

    	i++;
    }
}

void turnLFTsmall(void){					// This function turns the robot about 45 degrees left
	int i=0;

    while(i<0x2FFF){
    	P2OUT |= BIT5;						// Enable right motor
    	P2OUT &= ~BIT0;						// Disable left motor

    	P2OUT &= ~BIT3;						// Set direction for R motor to FWD

    	TA1CCTL2 = OUTMOD_3;

    	i++;
    }
}

void turnRGTsmall(void){					// This function turns the robot about 45 degrees right
	int i=0;

    while(i<0x2FFF){

    	P2OUT &= ~BIT5;						// Disable right motor
    	P2OUT |= BIT0;						// Enable left motor

    	P2OUT |= BIT1;						// Set direction for L motor to FWD

    	TA1CCTL1 = OUTMOD_7;

    	i++;
    }
}

void stopFNL(void){							// This function disables motors and traps CPU to stop robot until a reset
	while(1){
		P2OUT &= ~BIT0;
		P2OUT &= ~BIT5;

	}

}
