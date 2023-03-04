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
 */

#include <msp430.h>

void gpioInit();
void timerInit();

int time;


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

          // Configure Button on P4.1 as input with pullup resistor
           P4OUT |= BIT1;                          // Configure P2.3 as pulled-up
           P4REN |= BIT1;                          // P2.3 pull-up register enable
           P4IES &= ~BIT1;                         // P2.3 Low --> High edge
           P4IE |= BIT1;                           // P2.3 interrupt enabled
}

void timerInit(){

    TB1CCTL0 |= CCIE;                          // TBCCR0 interrupt enabled
    TB1CCR0 = 25000;                            //interrupt happens when it hits 50000
    TB1CTL = TBSSEL_1 | MC_1 | ID_3 | TBCLR;     // ACLK, up mode (Timer counts up to TBxCL0), input divider (/8)
}

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    if(P2IN & BIT3){                     //on posedge


        while(1){
            time++;                         //keep track of the time the button is being pushed

        }
        //button is pressed - light is on timer is on
        //button is released - light is off timer off
    }


    P2IFG &= ~BIT3;                         // Clear P2.3 IFG
}


// Timer B1 interrupt service routine
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void)
{
    P6OUT ^= BIT6;
    TB1CCR0 = time;
 }




