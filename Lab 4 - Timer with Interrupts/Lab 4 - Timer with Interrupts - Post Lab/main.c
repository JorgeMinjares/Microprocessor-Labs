#include "msp.h"
#include "lcdLib_432.h"

/**
 * main.c
 */
uint8_t stre = 0;
char boomFor = '>';
char boomBack = '<';
char buffer[16];
uint8_t pos = 5;
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	//Initiate and clear LCD screen
	lcdInit();
	lcdClear();

	//Set P2.1 and P2.2 as inputs using pull-down
	P5->DIR &= ~(BIT0|BIT1); //0
	P5->REN |= (BIT0|BIT1); //1
	P5->OUT &= ~(BIT0|BIT1);//0
	//Display Jorge on LCD screen
	lcdSetText("Jorge",pos,0);
	sprintf(buffer,"%2u",stre);
	lcdSetText(buffer,8,1);

	while(1){
	    if(P5->IN & BIT1){
	       //Debounce button and read button again
	       __delay_cycles(1000000); //delay for 0.25 s
	       while(P5->IN & BIT1){
	           stre++;
	           __delay_cycles(1000000);
	           sprintf(buffer,"%u",stre);
	           lcdSetText(buffer,9,1);
	           if(stre >= 15){
	               lcdClear();
	               lcdSetText("Jorge",pos,0);
	               stre = 15;
	               sprintf(buffer,"%u",stre);
	               lcdSetText(buffer,9,1);
	           }
	       }
	   }
	    //Start throw
	   if(P5->IN & BIT0){
	       uint8_t i = 0;
	       while(stre-- > 0){
	           __delay_cycles(1000000);
	           lcdClear();
	           lcdSetText("Jorge",pos,0);
	           sprintf(buffer,"%c",boomFor);
	           lcdSetText(buffer,i,0);
	           i++;
	           if(i > 15 /* && pos > 15*/){
	               lcdClear();
	               i = 15;
	           }
	           if(i == pos){
	               pos += 1;
	           }
	        }
	        if(i > 0){
	            while(i > 0){
	                if(pos > 15){
	                    lcdClear();
	                    sprintf(buffer,"%c",boomBack);
	                    lcdSetText(buffer,i-1,1);
	                    i--;
	                    stre = 0;
	                    sprintf(buffer,"%u",stre);
	                    lcdSetText(buffer,9,1);
	                }
	                else{
	                    __delay_cycles(1000000);
	                    lcdClear();
	                    lcdSetText("Jorge",pos,0);
	                    sprintf(buffer,"%c",boomBack);
	                    lcdSetText(buffer,i-1,1);
	                    i--;
	                }
	            }
	            lcdClear();
	            stre = 0;
	            if(pos > 15){
	                sprintf(buffer,"%u",stre);
	                lcdSetText(buffer,9,1);
	            }
	            else{
	                lcdSetText("Jorge",pos,0);
	                sprintf(buffer,"%u",stre);
	                lcdSetText(buffer,9,1);
	            }

	        }
	    }
	}
}

