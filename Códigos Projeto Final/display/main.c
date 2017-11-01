#include "msp430g2553.h"
#include "PCD8544.h"

// NOKIA 5510           MSP430g2453
// 1. VCC               VCC
// 2. GND               GND
// 3. SCE               P1.0
// 4. RST               VCC -- 47KOhm -- GND
// 5. D/C               P1.1
// 6. DN<MOSI>          P1.7
// 7. SCLK              P1.5
// 8. LED
#define LCD5110_SCLK_PIN BIT5
#define LCD5110_DN_PIN BIT7
#define LCD5110_SCE_PIN BIT0
#define LCD5110_DC_PIN BIT1
#define LCD5110_SELECT P1OUT &= ~LCD5110_SCE_PIN
#define LCD5110_DESELECT P1OUT |= LCD5110_SCE_PIN
#define LCD5110_SET_COMMAND P1OUT &= ~LCD5110_DC_PIN
#define LCD5110_SET_DATA P1OUT |= LCD5110_DC_PIN
#define LCD5110_COMMAND 0
#define LCD5110_DATA 1

#define BTN BIT3  //Interrupção que virá do GPS ou GPRS

//#define ALARME BIT6 // Dispara alarme



#define SPI_MSB_FIRST UCB0CTL0 |= UCMSB // or UCA0CTL0 |= UCMSB (USCIA) or USICTL0 &= ~USILSB (USI)
#define SPI_LSB_FIRST UCB0CTL0 &= ~UCMSB // or UCA0CTL0 &= ~UCMSB or USICTL0 |= USILSB (USI)

void writeStringToLCD(const char *string);
void writeCharToLCD(char c);
//void writeBlockToLCD(char *byte, unsigned char length);
//void writeGraphicToLCD(char *byte, unsigned char transform);
void writeToLCD(unsigned char dataCommand, unsigned char data);
void clearLCD();
void clearBank(unsigned char bank);
void setAddr(unsigned char xAddr, unsigned char yAddr);
void initLCD();

unsigned char currXAddr = 0; //TODO this will be used for tracking current addr
unsigned char currYAddr = 0; //not implemented

//char testBlock[8] = {0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF};
char testBlock[8] = {0x00, 0x7F, 0x7F, 0x33, 0x33, 0x03, 0x03, 0x03};
char testBlock2[8] = {0x00, 0x18, 0x18, 0x18, 0x7E, 0x3C, 0x18, 0x00};


void main(void) {

    WDTCTL = WDTPW + WDTHOLD; // disable WDT
    BCSCTL1 = CALBC1_1MHZ; // 1MHz clock
    DCOCTL = CALDCO_1MHZ;

    P1OUT |= LCD5110_SCE_PIN + LCD5110_DC_PIN + BTN; // + ALARME;
    P1DIR |= LCD5110_SCE_PIN + LCD5110_DC_PIN;// + ALARME;
    P1DIR &= ~BTN;
    P1REN |= BTN;
    P1IES |= BTN;
    P1IE |= BTN;
    _BIS_SR(GIE);



    // setup USIB
    P1SEL |= LCD5110_SCLK_PIN + LCD5110_DN_PIN;
    P1SEL2 |= LCD5110_SCLK_PIN + LCD5110_DN_PIN;

    UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC; // 3-pin, 8-bit SPI master
    UCB0CTL1 |= UCSSEL_2; // SMCLK
    UCB0BR0 |= 0x01; // 1:1
    UCB0BR1 = 0;
    UCB0CTL1 &= ~UCSWRST; // clear SW

    _delay_cycles(500000);
    initLCD();
    clearLCD();

    // LCD test

  for(;;){
   clearBank(0);
   writeStringToLCD("Nome:");
   clearBank(1);
   writeStringToLCD("Vinicius");
   clearBank(2);
   clearBank(3);
   writeStringToLCD("Idade:");
   clearBank(4);
   writeStringToLCD("21 anos");
   _delay_cycles(2500000);

   clearLCD();
   clearBank(0);
   writeStringToLCD("Responsavel:");
   clearBank(1);
   writeStringToLCD("Vinicius");
   clearBank(2);
   clearBank(3);
   writeStringToLCD("Contato:");
   clearBank(4);
   writeStringToLCD("(61)99884-1011");
   _delay_cycles(2500000);

  }


}

#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void){

          /*    clearBank(0);
              writeStringToLCD("PERDIDO!!!");
              clearBank(1);
              writeStringToLCD("Nome");
              clearBank(2);
              writeStringToLCD("Vinicius");
              clearBank(3);
              writeStringToLCD("Idade:");
              clearBank(4);
              writeStringToLCD("21 anos");
              _delay_cycles(2500000); */

              clearLCD();
              clearBank(0);
              writeStringToLCD("  PERDIDO!!!");
              clearBank(1);
              writeStringToLCD("Responsavel:");
              clearBank(2);
              writeStringToLCD("Vinicius");
              clearBank(3);
              writeStringToLCD("Contato:");
              clearBank(4);
              writeStringToLCD("(61)99884-1011");
              _delay_cycles(2500000);

        while((P1IN&BTN)== 0);
        while((P1IN&BTN)!= 0);
        P1IFG &= ~BTN;

}


void writeStringToLCD(const char *string) {
    while(*string) {
        writeCharToLCD(*string++);
    }
}

void writeCharToLCD(char c) {
    unsigned char i;
    for(i = 0; i < 5; i++) {
        writeToLCD(LCD5110_DATA, font[c - 0x20][i]);
    }
    writeToLCD(LCD5110_DATA, 0);
}

//void writeBlockToLCD(char *byte, unsigned char length) {
  //  unsigned char c = 0;
    //while(c < length) {
      //  writeToLCD(LCD5110_DATA, *byte++);
        //c++;
    //}
//}

//void writeGraphicToLCD(char *byte, unsigned char transform) {
  //  int c = 0;
    //char block[8];

  //  if(transform & FLIP_V) {
    //    SPI_LSB_FIRST;
   // }
    //if(transform & ROTATE) {
      //  c = 1;
        //while(c != 0) {
          //  (*byte & 0x01) ? (block[7] |= c) : (block[7] &= ~c);
            //(*byte & 0x02) ? (block[6] |= c) : (block[6] &= ~c);
            //(*byte & 0x04) ? (block[5] |= c) : (block[5] &= ~c);
            //(*byte & 0x08) ? (block[4] |= c) : (block[4] &= ~c);
            //(*byte & 0x10) ? (block[3] |= c) : (block[3] &= ~c);
            //(*byte & 0x20) ? (block[2] |= c) : (block[2] &= ~c);
            //(*byte & 0x40) ? (block[1] |= c) : (block[1] &= ~c);
            //(*byte & 0x80) ? (block[0] |= c) : (block[0] &= ~c);
            //*byte++;
            //c <<= 1;
      //  }
    //} else {
        //while(c < 8) {
          //  block[c++] = *byte++;
        //}
    //}

    //if(transform & FLIP_H) {
      //  c = 7;
        //while(c > -1) {
          //  writeToLCD(LCD5110_DATA, block[c--]);
        //}
    //} else {
      //  c = 0;
        //while(c < 8) {
          //  writeToLCD(LCD5110_DATA, block[c++]);
        //}
    //}
    //SPI_MSB_FIRST;
//}

void writeToLCD(unsigned char dataCommand, unsigned char data) {
    LCD5110_SELECT;
    if(dataCommand) {
        LCD5110_SET_DATA;
    } else {
        LCD5110_SET_COMMAND;
    }
    UCB0TXBUF = data;
    while(!(IFG2 & UCB0TXIFG));
    LCD5110_DESELECT;
}

void clearLCD() {
    setAddr(0, 0);
    int c = 0;
    while(c < PCD8544_MAXBYTES) {
        writeToLCD(LCD5110_DATA, 0);
        c++;
    }
    setAddr(0, 0);
}

void clearBank(unsigned char bank) {
    setAddr(0, bank);
    int c = 0;
    while(c < PCD8544_HPIXELS) {
        writeToLCD(LCD5110_DATA, 0);
        c++;
    }
    setAddr(0, bank);
}

void setAddr(unsigned char xAddr, unsigned char yAddr) {
    writeToLCD(LCD5110_COMMAND, PCD8544_SETXADDR | xAddr);
    writeToLCD(LCD5110_COMMAND, PCD8544_SETYADDR | yAddr);
}

void initLCD() {
    writeToLCD(LCD5110_COMMAND, PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION);
    writeToLCD(LCD5110_COMMAND, PCD8544_SETVOP | 0x3F);
    writeToLCD(LCD5110_COMMAND, PCD8544_SETTEMP | 0x02);
    writeToLCD(LCD5110_COMMAND, PCD8544_SETBIAS | 0x03);
    writeToLCD(LCD5110_COMMAND, PCD8544_FUNCTIONSET);
    writeToLCD(LCD5110_COMMAND, PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);
}
