
//******************************************************************************
//  MSP-FET430P140 Demo - Timer_A, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK
//
//  Description: Toggle P1.0 using software and TA_0 ISR. Timer_A is
//  configured for up mode, thus the timer overflows when TAR counts
//  to CCR0. In this example, CCR0 is loaded with 20000.
//  ACLK = n/a, MCLK = SMCLK = TACLK = default DCO ~800kHz
//
//           MSP430F149
//         ---------------
//     /|\|            XIN|-
//      | |               |
//      --|RST        XOUT|-
//        |               |
//        |           P1.0|-->LED
//
//  M. Buccini
//  Texas Instruments Inc.
//  Feb 2005
//  Built with IAR Embedded Workbench Version: 3.21A
//******************************************************************************

#include  <msp430x14x.h>

unsigned int timer;
unsigned char jishu1,IO_now,i;
unsigned char code[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

void Clock_init()
{
 unsigned char iq0;
   BCSCTL1 = RSEL0 + RSEL1 + RSEL2;          // XT2on, max RSEL
   BCSCTL1&=~XT2OFF; //打开XT2振荡器
   do
   {
      IFG1 &= ~OFIFG; // 清除振荡器失效标志
      for (iq0 = 255; iq0 > 0; iq0--); // 延时，等待XT2起振
   }
    while ((IFG1 & OFIFG) != 0); // 判断XT2是否起振
   
   BCSCTL2 |= SELS+SELM_2;                          // SMCLK = XT2
}



void timerA_init()
{
   CCTL0 = CCIE;                              // CCR0 interrupt enabled
   CCR0 = 8000;                               // 晶振8M   
   TACTL = TASSEL_2 + MC_1+TACLR;              // SMCLK, upmode
}
void main(void)
{
   WDTCTL = WDTPW + WDTHOLD;                  // Stop WDT
   
  
   Clock_init();

   P4DIR |= 0xff;                             // P1.0 output
   timerA_init();
   _BIS_SR(LPM0_bits + GIE);                   // Enter LPM0 w/ interrupt

}


#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
   timer++;
   if(timer==100)
     {
         if(i<8)
         {
           P4OUT=~(code[jishu1++]|IO_now); 
           if((jishu1+i)==8)
           {
             IO_now=P4OUT^0xff;
             jishu1=0;
             i++;  
           }
         }  
    }

  if(timer==200)
     {
          timer=0;
          if(i<8)
          {
             P4OUT=~(code[jishu1++]|IO_now); 
             if((jishu1+i)==8)
             {
               IO_now=P4OUT^0xff;
               jishu1=0;
               i++; 
             }
          }
         else
          {
            i=0;    
            jishu1=0;
            P4OUT=0xff;
            IO_now=P4OUT^0xff;
          }
     }

}    


