//******************************************************************************
// MSP432P401 Demo – SysTick timer used in interval mode
//
// Description: SysTick is configured to count down from 0x60000 to 0. At 0,
// the SysTick count gets reloaded to the original 0x60000 value to repeat the
// process. An interrupt is also configured to trigger when the SysTick count
// gets down to 0. P1.0 LED is toggled in the SysTick interrupt service routine.
//
// MSP432P401x
// ---------------
// /|\| |
// | | |
// --|RST |
// | P1.0|-->LED
//
// William Goh
// Texas Instruments Inc.
// Oct 2016 (updated) | November 2013 (created)
// Built with CCSv6.1, IAR, Keil, GCC
//******************************************************************************

int sec = 0;            //Global Variables
int min = 0;
#include "msp.h"
void startTimer(void){
    SysTick->CTRL |= 0x02;           //Disable Systick interrupt aka start timer or start systick handler
    P2->OUT &= ~(0x07);                 //Clear RGB LEDs
    int i;
    for(i = 0; i < 4; i++){         //Toggle Blue Led twice
        P2->OUT ^= BIT2;
    }
}
void stopTimer(void){
    SysTick->CTRL &= ~(0x02);              //Enable Systick interrupt aka stop timer or stop Systick handler
    P2->OUT &= ~(0x07);                 //Clear RGB LEDs
    int i;
    for(i = 0; i < 4; i++){         //Toggle Red LED twice
        P2->OUT ^= BIT0;
    }
}
void setTimer(void){
    P2->OUT &= ~(0x07);                 //Clear RGB LEDs
    int i;
    for(i = 0; i < 4; i++){             //Toggle Red LED twice
        P2->OUT ^= BIT0;
    }
    sec += 5;                           //Increase seconds by 5 and if sec greater than 15 reset value and increase minute
    if(sec > 60){
        sec = 0;
        min++;
        if(min > 3 && sec > 60){
            sec = 60;
            min = 3;
        }
    }

}
void clearTimer(void){
    sec = 0;
    min = 0;
    SysTick->CTRL |= 0x02;
}
void countDown(void){
    P4->OUT = sec;
    P6->OUT = min;
    if(sec > 0){
       sec = sec - 1;
       if(sec == 0 && min > 0){
          min = min - 1;
          sec = 60;
        }
    }
    if(sec == 0 && min == 0){
        SysTick->CTRL &= ~(0x02);
        int i;
        for(i = 0; i < 4; i++){             //Toggle Yellow LED twice
            P2->OUT ^= BIT0|BIT1;
        }
    }
}
void main(void){
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    P4->DIR |= (0x3F);                      //Enable P4.0 - P4.5 as output for seconds
    P4->OUT &= ~(0x3F);
    P4->SEL0 &= ~(0x3F);
    P4->SEL1 &= ~(0x3F);

    P6->DIR |= (BIT4 | BIT5);               //Enable P6.4 and P6.5 as output minutes
    P6->OUT &= ~(BIT4 | BIT5);
    P6->SEL0 &= ~(BIT4 | BIT5);              //Disable Special functions to not use ADC or PWM
    P6->SEL1 &= ~(BIT4 | BIT5);

    P2->DIR |= 0x07;                        //Enable onboard LEDs for button identifiers
    P2->OUT &= ~(0x07);

    P2->DIR &= ~(0x78);                     //Enable P2.3 - P2.6 as pull-down buttons
    P2->OUT &= ~(0x78);
    P2->REN |= (0x78);
    P2->SEL0 &= ~(0x78);                    //Disable Special functions to not use ADC or PWM
    P2->SEL1 &= ~(0x78);

   SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk| SysTick_CTRL_ENABLE_Msk;        //Enable SysTick Timer
   SysTick->VAL = 0x01;                                                         //Start Systick Timer at value 0
   SysTick->LOAD = 3000000 - 1;                                                 // Set frequency to 1s = 1Hz

   SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;
   __enable_irq();

   while(1){
       int result = P2->IN;
           if(result & BIT3){                      //Start Timer
               startTimer();
           }
           if(result & BIT4){//Stop Timer
               stopTimer();
           }
           if(result & BIT5){//Set Timer
               setTimer();
           }
           if(result & BIT6){//Clear Timer
               clearTimer();
           }
           P2->OUT &= ~(0x07);
           P2->OUT |= 0x02;

       __sleep();
   }
}
void SysTick_Handler(void){
    countDown();
}
