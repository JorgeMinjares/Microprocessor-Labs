#include "msp.h"
#include "lcdLib_432.h"
uint8_t tic;
typedef struct{
    uint8_t sec; //uint8_t as it will range from 0-60, which is perfect for 8bit or 256 bits
    uint8_t min; //^
}StopWatch_t;

StopWatch_t myWatch = {.sec = 0, .min = 0}; // Set values for secs and minutes to zero
char buffer[16]; //make it global to avoid constantly intializing variable

//Create macros to allow easier reading for Port interrupts Handler
#define START_TIMER     BIT0
#define STOP_TIMER      BIT1
#define RESET_TIMER     BIT2

//main.c
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	//Initialize and clear LCD
	lcdInit();
	lcdClear();

	//Enable P5.0 - P5.2 as inputs, Pull down
	P5->DIR &= ~(START_TIMER | STOP_TIMER | RESET_TIMER);   // START_TIMER | BIT1 |BIT2
	P5->OUT &= ~(START_TIMER | STOP_TIMER | RESET_TIMER);
	P5->REN |= (START_TIMER | STOP_TIMER | RESET_TIMER);

	P5->IE |= (START_TIMER | STOP_TIMER | RESET_TIMER);     //Enable P5.0 - P5.2 as interrupts
	P5->IES &= ~(START_TIMER | STOP_TIMER | RESET_TIMER);   //Set interrupts as raising edge
	P5->IFG &= ~(START_TIMER | STOP_TIMER | RESET_TIMER);   //Clear interrupt flags

	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; //Enable SysTick to begin Counting down and use System Clock as clocksource
	SysTick->VAL = 1 - 1; // Value for starting timer = 0
	SysTick->LOAD = 300000 - 1; // 100 ms = 10 Hz

	//Enable port 5 interrupt on NVIC Table
	NVIC->ISER[1] = 1 << ((PORT5_IRQn) & 31);
	__enable_irq();
	//Set text for LCD screen
	lcdSetText("Stopwatch     ",0,0);
	sprintf(buffer, "%2u:%2u", myWatch.min, myWatch.sec);
	lcdSetText(buffer, 0, 1);

	SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk; //Sleep on exit from ISR
	__DSB();                             //Ensure SLEEPONEXIT takes effect immediately

	while(1){
	    __sleep();
	}
}
void SysTick_Handler(void){
    if(tic++ > 10){ //use ten clock cycles for one second
          if(myWatch.sec++ >= 60){ //verify 0-59
              myWatch.sec = 0;
              if(myWatch.min++ >= 60){
                  myWatch.min = 0;
              }
          }
      //Set text for LCD Screen
      sprintf(buffer, "%2u:%2u", myWatch.min, myWatch.sec);
      lcdSetText(buffer, 0, 1);
      tic = 0;
    }
}
void PORT5_IRQHandler(void){ //make sure you spell it correctly, check startup_msp432p401r_ccs.c
    uint8_t result = P5->IFG; //store in uint8_t to avoid data corruption

    if(result & START_TIMER){//Start Timer
        SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; //Enter the systick interrupt
    }

    if(result & STOP_TIMER){//Stop Timer
        SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk); //Exit the systick interrupt
    }

    if(result & RESET_TIMER){//Reset Timer
        myWatch.sec = 0, myWatch.min = 0; //clear
        lcdClear(); //good
        lcdSetText("Stopwatch       ",0,0);
        sprintf(buffer, "%2u:%2u", myWatch.min, myWatch.sec);
        lcdSetText(buffer, 0, 1);
    }
    P5->IFG &= ~(result); //Clear port flag
}
