//******************************************************************************
// MSP432P401 Demo - Software Poll Switches S1 and S2
//
// Description:
// ACLK = n/a, MCLK = SMCLK = default DCO
//
// MSP432P401x
// -----------------
// /|\| XIN|-
// | | |
// --|RST XOUT|-
// /|\ | |
// --o--|P1.0 P1.0|-->LED
// \|/
//
// Dung Dang
// Texas Instruments Inc.
// June 2016 (updated) | November 2013 (created)
// Built with CCSv6.1, IAR, Keil, GCC
//******************************************************************************
#include "msp.h"
int main(void){
     WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // Stop watchdog timer

     /*        /* Configure GPIO */
     /* RGB LEDs at P2.0, P2.1, and P2.2, respectively */

       P2->DIR |= 0x07; // All set as outputs
       P2->OUT &= ~0x07; //Clear previous state of P2.0 P2.1, P2.2
       P2->OUT |= BIT2; //BITSet P2.2 as active
      //Enable Switch 1 and Switch 2
       P1->DIR &= ~(BIT1 | BIT4); // SET P1.0 and P1.4 as Inputs
       P1->OUT |= (BIT1 | BIT4); //BIT SET P1.0 and P1.4
       P1->REN |= (BIT1 | BIT4); // Bit SET P1.0 and P1.4 Internal Resistors

       P1->DIR |= BIT0; // Red Led at P1.0 set for output direction
       P1->OUT &= ~BIT0; //Clear previous state for P1.0
     */
     /* Switch S1 */
     /*
        P1->DIR &= ~BIT1; // Switch 1 at P1.1 set for input direction
        P1->REN |= BIT1; // Enable Input Resistor
        P1->OUT |= BIT1; // Set resistor to pull up
     */
     /* Switch S2 */
     /*
     P1->DIR &= ~BIT4; // Switch 1 at P1.4 set for input Direction
     P1->REN |= BIT4; // Enable input Resistor
     P1->OUT |= BIT4; //pull up setup
    */
     P2->OUT &= ~BIT2; //Bit clear P2.2

     while (1) {
         P2->OUT |= BIT0; //Active Red LED for P2.0
         if (P1->IN & BIT1){ //If Switch 1 is pressed
             P2->OUT &=~BIT0; //Bit clear the Red LED for P2.0
        }else {
            P2->OUT |= BIT0; // Turn on LED if switch is pressed
        }
         P2->OUT |= BIT1; // Turn on Green LED
         if(P1->IN & BIT4) //If Switch 2 is not Equal to BIT4
             P2->OUT &= ~BIT1; //Bit clear P2.1 turn off LED
         else
             P2->OUT |= BIT1; //Turn on GREEN LED

         P2->OUT &= ~BIT1;// Bit Clear P2.1 Turn off GREEN LED
         */
     }
}
