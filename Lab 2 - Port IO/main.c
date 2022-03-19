#include "msp.h"

int co = 0, s1, s2, c2, a1, b1, a2, b2;
void co_mode(void){
    if(P1->IN & BIT1){
        co = 0; // if switch 1 is pressed mode will be set to Low (L)
        P1->OUT &= ~(BIT0);
    }
    if(P1->IN & BIT4){
        co = 1; // if switch 2 is pressed mode will be set to High (H)
        P1->OUT |= BIT0;
    }
}
int adder(int inputs){
    //input == P4->IN P4.1 - P4.4
    a1 = 0;
    a2 = 0;
    b1 = 0;
    b2 = 0;
    s1 = 0;
    s2 = 0;
    c2 = 0;

    if(inputs & BIT1){ // P4.1 is pressed change the value of a1
        a1 = 1;
    }
    if(inputs & BIT2){// P4.2 is pressed change the value of b1
        b1 = 1;
    }
    if(inputs & BIT3){//P4.3 is pressed change the value of a2
        a2 = 1;
    }
    if(inputs & BIT4){//P4.4 is pressed change the value of b2
        b2 = 1;
    }

    //adder logic
    s1 = (a1 ^ b1); //S1 is equal to A1 XOR with B1
    s2 = (a1 & b1) ^ (a2 ^ b2); //S2 is equal to (A1 AND B1) XOR (A2 XOR B2)
    c2 = ((a2 ^ b2) & (a1 & b1)) | (a2 & b2); // C2 (A2 XOR B2) AND (A1 AND B2) | (A2 AND B2)

    return (s1 << 0) | (s2 << 1) | (c2 << 2) | co; //Return the summation value in binary

}

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	//Enable Switch 1 and Switch 2 to change MODE and enable RED LED to represent High co.
	P1->DIR |= (0x13);
	P1->OUT &= ~(0x13);
	P1->REN |= (0x13);

	//P4.1 - 4.4 as pull-down
	P4->DIR &= ~(0x1E); // Bit Clear P4.1 - P4.4
	P4->OUT &= ~(0x1E);// Bit clear previous value of P4.1 - P4.4
	P4->REN |= 0x1E;// Enable internal resistors

	//P5.0 - 5.2 as outputs for LED
	P5->DIR |= (0x07); // Set P5.0 - P5.2 as outputs
	P5->OUT &= ~(0x07); // Bit clear previous values in P5.0 - P5.2
	P5->REN |= (0x07); // Enable internal resistors

	//Activate P6.0 as buzzer output
	P6->DIR |= 0x01; //Set P6.0 as output for buzzer
	P6->OUT &= ~(0x01); // Bit clear previous value of P6.0



	while(1){
	    int inputs = P4->IN; // Store input values in int variable
	    int result = adder(inputs); // Get result from adder function
	    //co is set to low
	    co_mode(); //Check if co is modified
	    P5->OUT = result; // Output results onto leds
	    _delay_cycles(300000);//wait for 1 sec before continuing


	    P6->OUT |= 0x01; //Activate buzzer to signify that program is done
	    _delay_cycles(300000);//Wait for a 1 sec
	    P6->OUT &= ~(0x01);//Disactivate buzzer
	}
}
