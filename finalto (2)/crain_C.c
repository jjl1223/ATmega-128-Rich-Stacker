#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

void dcmoter_port(void)
{
DDRB=0b11111011; //PB7,PB4,PB3,PC7,PB1,PB0사용  PB5는 주행서브 PB6은 크레인 서브모터  PB4는 주행dc모터 PC7PB3사용PB2고장  PB7은 크레인 DC모터 PB0PB1사용
PORTB=0b10011011; // 8비트 모터 전부 break
//DDRB= 0b00011000; //PB4,PB3사용  확인용
//PORTB= 0b00011000; //정지 확인용
DDRC=0b10000000;   // PC7사용 
PORTC=0b10000000; // 주행 dc모터 break;
TCCR2= 0b01101100;;  // 크레인모터 PWM  64분주 
TCCR0= 0b01101111;  // 차량모터 PWM  
OCR2=0; //크레인모터 속도
OCR0=0; // 차량 모터 속도
}


void dcmoter2_move(void) // PB7,PB1,PB0사용
{


PORTB= 0b10000010;//내려가기

_delay_ms(1000);
PORTB= 0b10010001; //올라가기
_delay_ms(1000);
}


void dcmoter0_move(void) // PB4,PB3,PB2사용
{

PORTB= 0b00010100;//정회전 

_delay_ms(1000);
PORTB= 0b00011000; //역회전 
_delay_ms(1000);
}
