#include "msp.h"
#include "lcdLib_432.h"
int tic = 0;
int firstPattern[] = {30000,39000,47400,54000,58800,60000,58800,54000,47400,39000,30000,21000,12600,6000,1200,1,1200,6000,12600,21000}; //size of 20
int secondPattern[20];
int thirdPattern[] = {30000, 47400,39000,54000,60000,58800,47400,54000,39000,21000,30000,12600,1200,6000,1,6000,1200,12600,30000,21000}; // size of 20
void PWMINIT(void);
void GPIOINIT(void);
void patternOne(void);
void patternTwo(void);
void patternThree(void);
void fillPattern(void);
void clearPWM(void);
#define INCREASE BIT0
#define STOP BIT1
void main(void){
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	GPIOINIT();
	PWMINIT();
	lcdInit();
	lcdClear();
	while(1){
	    if(tic > 3){
	        tic = 1;
	    }
	    if(tic == 0){
	        lcdClear();
	        lcdSetText("Blink",0,0);
	        while(tic == 0){
	            TIMER_A0->CCR[1] = 1 - 1;
	            TIMER_A0->CCR[2] = 1 - 1;
	            TIMER_A0->CCR[3] = 1 - 1;
	            TIMER_A0->CCR[4] = 1 - 1;
	            TIMER_A2->CCR[1] = 1 - 1;
	            __delay_cycles(1000000);
	            TIMER_A0->CCR[1] = 30000 - 1;
	            TIMER_A0->CCR[2] = 30000 - 1;
	            TIMER_A0->CCR[3] = 30000 - 1;
	            TIMER_A0->CCR[4] = 30000 - 1;
	            TIMER_A2->CCR[1] = 30000 - 1;
	            __delay_cycles(1000000);
	        }
	    }
	    if(tic == 1){
	        lcdClear();
	        lcdSetText("Pattern 1",0,0);
	        patternOne();
	    }
	    if(tic == 2){
	        lcdClear();
	        lcdSetText("Pattern 2",0,0);
	        patternTwo();
	    }
	    if(tic == 3){
	        lcdClear();
	        lcdSetText("Pattern 3",0,0);
	        patternThree();
	    }
	    clearPWM();
	}
}
void PORT5_IRQHandler(void){
    uint8_t result = P5->IFG;
    if(result & INCREASE){
        tic++;
    }
    if(result & STOP){
        lcdClear();
        lcdSetText("STOP",0,0);
        clearPWM();
        tic = 0;
    }
    P5->IFG &= ~(result);
}
void patternOne(void){
    int i = 0;
    while(i++ <= 20){
        __delay_cycles(500000);
        if(i == 0){
            TIMER_A0->CCR[1] = firstPattern[i]; // P2.4
        }
        if(i == 1){
            TIMER_A0->CCR[2] = firstPattern[i]; // P2.5
        }
        if(i == 2){
            TIMER_A0->CCR[3] = firstPattern[i]; // P2.6
        }
        if(i == 3){
            TIMER_A0->CCR[4] = firstPattern[i]; // P2.7
        }
        if(i == 4){
            TIMER_A2->CCR[1] = firstPattern[i]; // P5.6
        }
        if(i >= 5){
            if((i % 5) == 0){
                TIMER_A0->CCR[1] = firstPattern[i]; // P2.4
            }
            if((i % 5) == 1){
                TIMER_A0->CCR[2] = firstPattern[i]; // P2.5
            }
            if((i % 5) == 2){
                TIMER_A0->CCR[3] = firstPattern[i]; // P2.6
            }
            if((i % 5) == 3){
                TIMER_A0->CCR[4] = firstPattern[i]; // P2.7
            }
            if((i % 5) == 4){
                TIMER_A2->CCR[1] = firstPattern[i]; // P5.6
            }
        }
    }
}
void fillPattern(void){
    int i = 0;
    for(; i < 20; i++){
        secondPattern[i] = rand();
    }
}
void patternTwo(void){
    fillPattern();
    int i = 0;
    while(i++ < (sizeof(thirdPattern)/sizeof(int))){
        __delay_cycles(500000);
        if(i == 0){
            //TIMER_A0->CCR[1] = secondPattern[i]; // P2.4
            TIMER_A0->CCR[1] = secondPattern[i];
        }
        if(i == 1){
            //TIMER_A0->CCR[2] = secondPattern[i]; // P2.5
            TIMER_A0->CCR[2] = secondPattern[i];
        }
        if(i == 2){
            //TIMER_A0->CCR[3] = secondPattern[i]; // P2.6
            TIMER_A0->CCR[3] = secondPattern[i];
        }
        if(i == 3){
            //TIMER_A0->CCR[4] = secondPattern[i]; // P2.7
            TIMER_A0->CCR[4] = secondPattern[i];
        }
        if(i == 4){
            TIMER_A2->CCR[1] = secondPattern[i]; // P5.6
        }
        if(i >= 5){
            if((i % 5) == 0){
                TIMER_A0->CCR[1] = secondPattern[i]; // P2.4
            }
            if((i % 5) == 1){
                TIMER_A0->CCR[2] = secondPattern[i]; // P2.5
            }
            if((i % 5) == 2){
                TIMER_A0->CCR[3] = secondPattern[i]; // P2.6
            }
            if((i % 5) == 3){
                TIMER_A0->CCR[4] = secondPattern[i]; // P2.7
            }
            if((i % 5) == 4){
                TIMER_A2->CCR[1] = secondPattern[i]; // P5.6
            }
        }
    }
}
void patternThree(void){
    int i = 0;
    while(i++ < 20){
        __delay_cycles(500000);
        if(i == 0){
            TIMER_A0->CCR[1] = thirdPattern[i]; // P2.4
        }
        if(i == 1){
            TIMER_A0->CCR[2] = thirdPattern[i]; // P2.5
        }
        if(i == 2){
            TIMER_A0->CCR[3] = thirdPattern[i]; // P2.6
        }
        if(i == 3){
            TIMER_A0->CCR[4] = thirdPattern[i]; // P2.7
        }
        if(i == 4){
            TIMER_A2->CCR[1] = thirdPattern[i]; // P5.6
        }
        if(i >= 5){
            if((i % 5) == 0){
                TIMER_A0->CCR[1] = thirdPattern[i]; // P2.4
            }
            if((i % 5) == 1){
                TIMER_A0->CCR[2] = thirdPattern[i]; // P2.5
            }
            if((i % 5) == 2){
                TIMER_A0->CCR[3] = thirdPattern[i]; // P2.6
            }
            if((i % 5) == 3){
                TIMER_A0->CCR[4] = thirdPattern[i]; // P2.7
            }
            if((i % 5) == 4){
                TIMER_A2->CCR[1] = thirdPattern[i]; // P5.6
            }
        }
    }
}
void clearPWM(void){
    TIMER_A0->CCR[1] = 1 - 1;//set pwm value for P2.4 as 0
    TIMER_A0->CCR[2] = 1 - 1;
    TIMER_A0->CCR[3] = 1 - 1;
    TIMER_A0->CCR[4] = 1 - 1;
    TIMER_A2->CCR[1] = 1 - 1;// P5.6
}
void GPIOINIT(void){
    //Pull-down setup for P5.0 and P5.1
    P5->DIR &= ~(BIT0 | BIT1);
    P5->OUT &= ~(BIT0 | BIT1);
    P5->REN |= (BIT0 | BIT1);
    P5->SEL0 &= ~(BIT0 | BIT1);
    P5->SEL1 &= ~(BIT0 | BIT1);

    P5->IE |= (BIT0|BIT1);
    P5->IES &= ~(BIT0|BIT1);
    P5->IFG &= ~(BIT0|BIT1);

    NVIC->ISER[1] = 1 << ((PORT5_IRQn) & 31);
    __enable_irq();
}
void PWMINIT(void){
    //Enable P2.4 - P2.7 as PWM channels
    P2->DIR |= (BIT4|BIT5|BIT6|BIT7);
    P2->SEL0 |= (BIT4|BIT5|BIT6|BIT7);
    P2->SEL1 &= ~(BIT4|BIT5|BIT6|BIT7);
    //Enable P5.6 as PWM Channel
    P5->DIR |= (BIT6);
    P5->SEL0 |= (BIT6);
    P5->SEL1 &= ~(BIT6);
    //Set timer as a System clock, mode as up, and clear previous configuration
    TIMER_A0->CTL |= TIMER_A_CTL_SSEL__SMCLK |
                     TIMER_A_CTL_MC__UP|
                     TIMER_A_CTL_CLR;
    //frequency for each PWM channel
    TIMER_A0->CCR[0] = 60000 - 1; // Set max frequency as 50 Hz
    TIMER_A0->CCR[1] = 1 - 1; //2.4
    TIMER_A0->CCR[2] = 1 - 1; //2.5
    TIMER_A0->CCR[3] = 1 - 1;//2.6
    TIMER_A0->CCR[4] = 1 - 1;//2.7
    //Output for each PWM Channel
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7; //enable reset/set mode
    TIMER_A0->CCTL[2] = TIMER_A_CCTLN_OUTMOD_7;
    TIMER_A0->CCTL[3] = TIMER_A_CCTLN_OUTMOD_7;
    TIMER_A0->CCTL[4] = TIMER_A_CCTLN_OUTMOD_7;
    //P5.6 PWM Set up
    TIMER_A2->CCR[0] = 60000 - 1;
    //PWM channel frequency
    TIMER_A2->CCR[1] = 1 - 1;
    TIMER_A2->CTL |= TIMER_A_CTL_SSEL__SMCLK |
                     TIMER_A_CTL_MC__UP|
                     TIMER_A_CTL_CLR;
    TIMER_A2->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;
}
