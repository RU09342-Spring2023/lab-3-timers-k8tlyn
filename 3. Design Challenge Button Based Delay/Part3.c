/*
 * Part3.c
 *
 *  Created on: Feb 12, 2023
 *      Author: Kaitlyn Pounds
 *
 *     You will need to build a system which has the LED blinking speed controlled by how long you hold down a button.
 *
 *     The system should boot up and begin blinking at a specific speed (maybe like 4Hz).
 *
 *     When the user presses the button, you should begin timing how long that button is being held down for.
 *
 *     Once the button has been released, the LED should begin blinking using the amount of time the button was held down at the period of it blinking.
 *
 *     For example, if I hold it down for 3 seconds, the LED should then blink every 3 seconds.
 *
 *     If the user presses the other button, you can then reset the system back to a default speed.
 *
 *
 *
 *     increment while button pushed
 *     timer to blink led with period of
 *
 *
 *       blinking period = CCR0 * 1/8192
 *
 *       blinking period = button press period
 *
 *       button press period = variable incremented while button is pushed
 *
 *
 *
 */

#include <msp430.h>

void gpioInit();
void timerInit();

int time = 0;           //used to keep track of how long button is pressed
int count = 0;          //1 when button is pressed, 0 when button released





void main(){

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

       gpioInit();
       timerInit();                               //set how fast timer should be going

    if(count) time++;

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

          // Configure Button on P4.1 as input with pullup resistor
           P4OUT |= BIT1;                          // Configure P2.3 as pulled-up
           P4REN |= BIT1;                          // P2.3 pull-up register enable
           P4IES &= ~BIT1;                         // P2.3 Low --> High edge
           P4IE |= BIT1;                           // P2.3 interrupt enabled
}

void timerInit(){

    TB1CCTL0 |= CCIE;                          // TBCCR0 interrupt enabled
    TB1CCR0 = 7500;                        //timer counts up to this value initially and then stops
    TB1CTL = TBSSEL_1 | MC_2 | ID_3;          // ACLK, continuous mode , input divider (/4)
}

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    if(count){
        count = 0;

        P2IES |= BIT3;                 // P2.3 High --> Low edge

    }

    else{
        count = 1;

        P2IES &= ~BIT3;                 // P2.3 Low --> High edge

    }


    P2IFG &= ~BIT3;                         // Clear P2.3 IFG
}

// Port 4 interrupt service routine
#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    if(!(P4IN & BIT1)){
        P1OUT ^= BIT0;
    }

    P4IFG &= ~BIT1;                         // Clear P4.1 IFG
}

// Timer B0 interrupt service routine
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void)        //blinks LED at period of button push
{

    P6OUT ^= BIT6;
    TB1CCR0 += time;
 }

