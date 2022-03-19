#include "msp.h"
#include "lcdLib_432.h"

void buzzer_on(void);
void buzzer_off(void);
void buzzer_toggle(uint8_t times);
void PWMInit(void);
void GPIOInit(void);
int angle(int servo);
#define DELAY 2500000

void updateLCD(int data);
int on = 0;
int tic = 0;

int left = 2000; //increments of 2750
int middle = 4750;
int right = 7500;

char buffer[16];
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    //Initialize GPIO, PWM and LCD
    GPIOInit();
    PWMInit();

    lcdInit();
    lcdClear();

    __enable_irq();

    lcdSetText("State:", 0, 0);
    lcdSetText("Position:", 0, 1);


    while(1){
        if(on){
            if(tic > 3){
               tic = 1;
            }
            if(tic == 1){
                updateLCD(right);
            }
            if(tic == 2){
                updateLCD(middle);
            }
            if(tic == 3){
                updateLCD(left);
            }
        }
        else{
            __delay_cycles(100000);
        }
    }
}

void updateLCD(int data){
    TIMER_A1->CCR[1] = data;
    sprintf(buffer, "Position: %d          ", angle(data));
    lcdSetText(buffer, 0, 1);
    //buzzer_toggle(2);
    __delay_cycles(DELAY);
}

int angle(int servo){
    if(servo == 2000){
        return 0;
    }
    else if(servo == 4750){
        return 90;
    }
    else if(servo == 7500){
        return 180;
    }
    else{
        return 0;
    }
}
//Interrupt invoked when switch is pressed
void PORT5_IRQHandler(void){
    //Stop system if P5.0 is pressed
    if(P5->IFG & BIT0){
        buzzer_off();
        on = 0;
        tic = 1;
        lcdSetText("State: STOP     ", 0, 0);
        updateLCD(right);
    }
    //Start PWM is P5.1 is pressed
    if(P5->IFG & BIT1){
        buzzer_toggle(2);
        on = 1;
        tic++;
        lcdSetText("State: Start    ", 0, 0);
    }
    //Clear flags
    P5->IFG &= ~(P5->IFG) ;
}

void buzzer_on(void){
    P6->OUT |= BIT0;
}
void buzzer_off(void){
    P6->OUT &=~(BIT0);
}
void buzzer_toggle(uint8_t times){
    uint8_t i = 0;
    for( ; i < times; i++){
        P6->OUT ^= BIT0;
        __delay_cycles(100000);
    }
    P6->OUT &= ~(BIT0);
}

void GPIOInit(void){
    //Setup buttons
    P5->DIR &= ~(BIT0 | BIT1);
    P5->OUT &= ~(BIT0 | BIT1);
    P5->REN |= (BIT0 | BIT1);

    P5->IES &= ~(BIT0 | BIT1);
    P5->IE |= (BIT0 | BIT1);
    P5->IFG &= ~(BIT0|BIT1);

    P6->DIR |= BIT0;
    P6->OUT &= ~(BIT0);
    P6->SEL0 &= ~(BIT0);
    P6->SEL1 &= ~(BIT0);
    P6->REN |= BIT0;

    NVIC->ISER[1] = 1 << ((PORT5_IRQn)&31); // see NVIC table, all PORTx go in ISER[1]
}
void PWMInit(void){
    //Setup pin 2.4 as PWM output
    P7->DIR |= BIT7;
    P7->SEL0 |= BIT7;
    P7->SEL1 &= ~BIT7;

    //Set period to 20 ms since (20/1000)*3000000 = 60000
    TIMER_A1->CCR[0] = 60000;

    //Put into reset/set mode
    TIMER_A1->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;

    //Start motor at 0 degrees with a 5% DC (info from datasheet)
    TIMER_A1->CCR[1] = 0;

    //Setup TimerA
    TIMER_A1->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK
                    TIMER_A_CTL_MC__UP |      // Up mode
                    TIMER_A_CTL_CLR;          // Clear TAR
}
