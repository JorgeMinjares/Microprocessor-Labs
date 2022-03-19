#include "msp.h"
#include "lcdLib_432.h"
#include <string.h>

void sendString(char *str);
void sendChar(char s);
char buffer[16];
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer

    lcdInit();
    lcdClear();
    // Enable UART0 Pins
    // P1.2->RX
    // P1.3->TX
    P1->SEL0 |= BIT2 | BIT3;
    P1->SEL1 &= ~(BIT2 | BIT3);

    // UART0 Configuration
    // Enhanced Universal Serial Control Interface = EUSCI
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST;                               // Clear previous configuration of UART
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK;                        // Select SMClock, no parity, 1 stop bit, 8 bits, LSB
    EUSCI_A0->BRW = 312;                                                 // Baudrate width, SMClock/16/DR -> 3000000/9600 = 312
    EUSCI_A0->MCTLW = 0;                                                 // disable oversampling
    EUSCI_A0->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);                           // clear reset bit
    // enable UART interrupt
    EUSCI_A0->IE &= ~(EUSCI_A_IE_RXIE|EUSCI_A_IE_TXIE);     // enable receiver interrupt
    EUSCI_A0->IFG &= ~(EUSCI_A_IE_RXIE|EUSCI_A_IE_TXIE);    // clear interrupt flag

    // enable NVIC for UART0
    NVIC->ISER[0] = 1 << (EUSCIA0_IRQn & 31);
    //enable UART2 Pins
    P3->SEL0 |= BIT2 | BIT3;
    P3->SEL1 &= ~(BIT2 | BIT3);

    // UART2 Configuration
    // Enhanced Universal Serial Control Interface = EUSCI
    EUSCI_A2->CTLW0 = EUSCI_A_CTLW0_SWRST;                               // Clear previous configuration of UART
    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK;                        // Select SMClock, no parity, 1 stop bit, 8 bits, LSB
    EUSCI_A2->BRW = 312;                                                 // Baudrate width, SMClock/16/DR -> 3000000/16/9600 = 19.53125
    EUSCI_A2->MCTLW = 0;                                                 // Disable oversampling
    EUSCI_A2->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);                           // clear reset bit

    // enable UART interrupt
    EUSCI_A2->IE |= EUSCI_A_IE_RXIE;     // enable receiver interrupt
    EUSCI_A2->IFG &= ~(EUSCI_A_IE_RXIE); // clear interrupt flag

    // enable NVIC for UART0
    NVIC->ISER[0] = 1 << (EUSCIA2_IRQn & 31);

    // enable global interrupts
    __enable_irq();
    lcdSetText("Receiving:",0,0);

    while (1){}
}

void EUSCIA2_IRQHandler(void)
{
    if (EUSCI_A2->IFG & EUSCI_A_IFG_RXIFG){ // recieve interrupt
         char str[16] = EUSCI_A2->RXBUF; // store data into character buffer, and clear flag
         sendString(&str);

                      // display character through the SERIAL port
                      //sendString("\r\n");
                      // send carriage return and new line
    }
}

void sendString(char *str){
    int i;                           // create variable
    for(i = 0 ; str[i] != '\0'; i++){
        __delay_cycles(5000000);
        sprintf(buffer, "%c",str[i]);
        lcdSetText(buffer,i,1);
        __delay_cycles(5000000);
    }
    for(i = 0; str[i] != '\0'; i++){ // iterate over the end of the string
        while (!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG)){ // wait until is ready to transmit
            EUSCI_A0->TXBUF = str[i]; // send character through buffer
        }
    }
}

void sendChar(char s){
    while (!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG)){ // wait until is ready to transmit
        EUSCI_A0->TXBUF = s; // send character through buffer
        //sprintf(buffer, "%c", s);
        lcdSetText(s,0,1);
    }
}

