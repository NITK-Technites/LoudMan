
#include  "msp430g2231.h"

#define SIN BIT7
#define SCLK BIT5
#define LTCH BIT4
#define THRESH 400

int temp=0;
long int a1=0,a2=0;
long int a3=0,a4=0;

long int count1=0, count2=0, count3=0;

/*
void send_num(unsigned int num)
{
	  while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
	  UCA0TXBUF = '$';
	while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
	  	  UCA0TXBUF = num;
	  	  while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
	  	  UCA0TXBUF = num>>8;
;;
}
*/
void send_data(int count)
{
	int i=0;
	if(count>15) count=16;



	if (count<3) count=0;
	if(count)
	{
		P1OUT&=~(SIN);

		for(i=0;i<16-count;i++)
		{

			P1OUT|=SCLK;
			P1OUT&=~SCLK;
			P1OUT|=LTCH;
			P1OUT&=~LTCH;
		}


		P1OUT|=(SIN);
		for(i=0;i<count;i++)
		{

			P1OUT|=SCLK;
			P1OUT&=~SCLK;
			P1OUT|=LTCH;
			P1OUT&=~LTCH;
			_delay_cycles(60000);
			_delay_cycles(60000);
			_delay_cycles(60000);
			_delay_cycles(60000);
		}

		_delay_cycles(60000);
		_delay_cycles(60000);
		_delay_cycles(60000);
		_delay_cycles(60000);
		_delay_cycles(60000);
		_delay_cycles(60000);
		_delay_cycles(60000);
		_delay_cycles(60000);
	}

}

void flush()
{
	P1OUT&=~(SIN+SCLK+LTCH);
	int i=0;
	for(i=0;i<16;i++)
	{
		P1OUT&=~SIN;
		P1OUT|=SCLK;
		P1OUT&=~SCLK;
	}
	P1OUT|=LTCH;
	P1OUT&=~LTCH;
}


void main(void)
{

WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  P1DIR = (SIN+SCLK+LTCH);

  ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
  ADC10CTL1 = INCH_0;                       // input A1
  ADC10AE0 |= BIT0;                         // PA.1 ADC option select

  BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
  DCOCTL = CALDCO_1MHZ;
/*  P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
  P1SEL2 = BIT1 + BIT2 ;                    // P1.1 = RXD, P1.2=TXD
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 = 104;                             // 1MHz 19200
  UCA0BR1 = 0;                              // 1MHz 19200
  UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
//  IE2 |= UCA0RXIE;                     // Enable USCI_A0 RX interrupt
*/
  flush();
  while(1)
  {    ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start

  	  __bis_SR_register(LPM0_bits + GIE);

  	  a1 = a1+ temp;
  	  count1++;
  	  count3++;
  	  if(count1==500)
  	  {

  		  a2 = a2+(a1/10);
  		  count1=0;
  		  count2++;
  		  a1=0;
  	  }
  	  if(count2==20)
  	  {
  		  count2=0;
  		  if(a3<a2) a3=a2;
  		  a2=0;

  	  }
  	  if(count3==70000)
  	  {
  		  if(a3<7187) a4 = 0;
  		  else if(a3>7187 && a3<8609) a4 = 1;
  		else if(a3>8609 && a3<10313) a4 = 2;
  		else if(a3>10313 && a3<12353) a4 = 3;
  		else if(a3>12353 && a3<14797) a4 = 4;
  		else if(a3>14797 && a3<17725) a4 = 5;
  		else if(a3>17725 && a3<21232) a4 = 6;
  		else if(a3>21232 && a3<25433) a4 = 7;
  		else if(a3>25433 && a3<30465) a4 = 8;
  		else if(a3>30465 && a3<36493) a4 = 9;
  		else if(a3>36493 && a3<43714) a4 = 10;
  		else if(a3>43714 && a3<52363) a4 = 11;
  		else if(a3>52363 && a3<62724) a4 = 12;
  		else if(a3>62724 && a3<75134) a4 = 13;
  		else if(a3>75134 && a3<90000) a4 = 14;
  		else if(a3>90000) a4 = 15;
  		  else a4=0;

  		  send_data(a4);
  		  a3=0;
  		  count3=0;
  	  }


    	flush();
/*
  	  int max,conv1, conv;

  	  max=temp;
  	  conv1 = 900-max;
  	  conv = conv1/50;
  	  if(conv>3){
  	  send_data(conv);
  	  temp=0;
  	  ADC10CTL0&=~ENC;
      //flush();
  	_delay_cycles(50000);
  	_delay_cycles(50000);
  	_delay_cycles(50000);
  	_delay_cycles(50000);
  	_delay_cycles(50000);
  	_delay_cycles(50000);
  	  }
*/
//  	   flush();


  }
}

#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
	  temp =ADC10MEM;
	  __bic_SR_register_on_exit(LPM0_bits);        // Clear CPUOFF bit from 0(SR)
}


