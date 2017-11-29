#include "msp430g2553.h"

#include "uart.h"

void gsm();

int main(void)

{
    WDTCTL = WDTPW + WDTHOLD;// STOP WATCHDOG TIMER
    BCSCTL1 = CALBC1_8MHZ;
    DCOCTL = CALDCO_8MHZ;
    uart_init();
    __enable_interrupt();// ENABLE INTERRUPT
    __delay_cycles(100000);
    gsm();
}



void gsm()

{
      uart_puts((char *)"AT"); // COMMAND FOR INITIALIZING GSm
      uart_putc(0x0A);//ENTER
      uart_putc(0x0D);//CARRIAGE RETURN
      __delay_cycles(10000000);//DELAY...WAIT FOR OK FROM GSM
      uart_puts((char *)"AT+CMGF=1");//COMMUNICATION
      uart_putc(0x0A);
      uart_putc(0x0D);
     __delay_cycles(10000000);//WAIT FOR OK
      uart_puts((char *)"AT+CMGS=\"061995523211\"");//SEND A MESSAGE TO PARTICULAR NUMBER
      uart_putc(0x0A);
      uart_putc(0x0D);
      uart_puts((char *)"hello");//SEND HELLO
         uart_putc(0x1A);//CTRL Z
}
