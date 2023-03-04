/*
 * Part2.c
 *
 *  Created on: Feb 11, 2023
 *      Author: Kaitlyn Pounds
 *
 *      This code will need to change the speed of an LED between 3 different speeds by pressing a button.
 *
 *      You will need to fill in the file attached to this folder to change the blinking speed of the LED between 3 different speeds when you press a button.
 *
 *      Think of it as a controller that cycles from slow, mid, and fast, and back to slow for button presses.

        For the button, you can use either polling or interrupts to determine the button press, but I think you really should be using interrupts for this.

        You will need to look at the examples from part 1 and the example code presented in this part of the lab to accomplish this.

         There are several ways you could approach this, but I would like to point specifically to the TB1CCR0 register which controls
         how often the Timer Interrupt will be going off.

         You may want to consider how your Button Interrupt could possibly change or control that

*/
#include <msp430.h>

void gpioInit();
void timerInit();


unsigned int offset = 50000;

void main(){

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    gpioInit();
    timerInit();                               //set how fast timer should be going





    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(LPM3_bits | GIE);





}


void gpioInit(){
    // Configure RED LED on P1.0 as Output
          P1OUT &= ~BIT0;                         // Clear P1.0 output latch for a defined power-on state
          P1DIR |= BIT0;                          // Set P1.0 to output direction

          // Configure Green LED on P6.6 as Output
          P6OUT &= ~BIT6;                         // Clear P6.6 output latch for a defined power-on state
          P6DIR |= BIT6;                          // Set P6.6 to output direction


          // Configure Button on P2.3 as input with pullup resistor
          P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
          P2REN |= BIT3;                          // P2.3 pull-up register enable
          P2IES &= ~BIT3;                         // P2.3 Low --> High edge
          P2IE |= BIT3;                           // P2.3 interrupt enabled
}

void timerInit(){

    //TBXCTL = ID_0 | TBSSEL_2 | MC_UP;           //
    // @TODO Initialize Timer B1 in Continuous Mode using ACLK as the source CLK with Interrupts turned on
    TB1CCTL0 |= CCIE;                          // TBCCR0 interrupt enabled
    TB1CCR0 = 50000;                            //interrupt happens when it hits 30000 initially
    TB1CTL = TBSSEL_1 | MC_2 | TBCLR;                 // ACLK, continuous mode (first example)
}

/*
 * INTERRUPT ROUTINES
*/

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    // @TODO When the button is pressed, you can change what the CCR0 Register is for the Timer. You will need to track what speed you should be flashing at.

    if(offset == 50000){
        offset = 25000;
    }
    else if(offset == 25000){
            offset = 7500;
    }
    else{
        offset = 50000;
    }

     // @TODO Remember that when you service the GPIO Interrupt, you need to set the interrupt flag to 0.
    P2IFG &= ~BIT3;                         // Clear P2.3 IFG
}


// Timer B1 interrupt service routine
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void)
{
    P1OUT ^= BIT0;
    TB1CCR0 += offset;
        }


    // @TODO You can toggle the LED Pin in this routine and if adjust your count in CCR0.


