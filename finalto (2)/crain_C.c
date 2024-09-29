#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

void dcmoter_port(void)
{
DDRB=0b11111011; //PB7,PB4,PB3,PC7,PB1,PB0���  PB5�� ���༭�� PB6�� ũ���� �������  PB4�� ����dc���� PC7PB3���PB2����  PB7�� ũ���� DC���� PB0PB1���
PORTB=0b10011011; // 8��Ʈ ���� ���� break
//DDRB= 0b00011000; //PB4,PB3���  Ȯ�ο�
//PORTB= 0b00011000; //���� Ȯ�ο�
DDRC=0b10000000;   // PC7��� 
PORTC=0b10000000; // ���� dc���� break;
TCCR2= 0b01101100;;  // ũ���θ��� PWM  64���� 
TCCR0= 0b01101111;  // �������� PWM  
OCR2=0; //ũ���θ��� �ӵ�
OCR0=0; // ���� ���� �ӵ�
}


void dcmoter2_move(void) // PB7,PB1,PB0���
{


PORTB= 0b10000010;//��������

_delay_ms(1000);
PORTB= 0b10010001; //�ö󰡱�
_delay_ms(1000);
}


void dcmoter0_move(void) // PB4,PB3,PB2���
{

PORTB= 0b00010100;//��ȸ�� 

_delay_ms(1000);
PORTB= 0b00011000; //��ȸ�� 
_delay_ms(1000);
}
