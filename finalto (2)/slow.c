#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "slow.h"
#include "sonic.h"
#include <avr/interrupt.h>
#include "uart.h"
#define SOUND_VELOCITY 340UL
unsigned int u;
void slowsubport(void) //공용 그냥 서브모터 설정이랑 같음
{
 TCCR1A = 0b10100010; // 1010OCRA OCRB만 사용 00 OCRC사용안함 FAST PWM 

 TCCR1B=0x1b; //FAST PWM

 ICR1=4999;     //TOP

 OCR1A=385;   //0도
 OCR1B=385;   //0도
}

void movefind1(void)
{
uart_init();
unsigned short find = sonic1();
_delay_ms(1);
unsigned int A_P=375;
while(1)

{ 
printf("find is = %d \r\n" );
          OCR1B=A_P;
_delay_ms(500);
    if (find< 1200)
	{
          A_P=A_P-20;
          OCR1B=A_P;
		  break;
		   _delay_ms(500);

	}


 	if (A_P<150)
	{     

	  break;


 	}	
		A_P=A_P-5;
		  

}


}

void movefind2(void)
{
unsigned int A_P;
while(1)

{ 

          OCR1B=A_P;

 	if (A_P>375)
	{     

	  break;


 	}	
		A_P=A_P+5;
		  _delay_ms(500);

}


}


void moveslow1(void)
{

unsigned int u=385;
while(1)

{ 

          OCR1B=u;
 	if (u<160)
	{     

	  break;


 	}	
		u=u-5;
		  _delay_ms(500);

}

}

void moveslow2(void)
{

unsigned int u=160;
while(1)

{ 

          OCR1B=u;
 	if (u>385)
	{     

	  break;


 	}	
		u=u+15;
		  _delay_ms(500);

}

}

