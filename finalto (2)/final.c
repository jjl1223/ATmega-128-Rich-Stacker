#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "sonic.h"
#include <avr/interrupt.h>
#define SOUND_VELOCITY 340UL
#include "uart.h"
#include "crain_C.h"
// ��ũȦ ���� ���� ����
//#define TRIGSF 4   // Trigger ��ȣ (��� = PF4)
//#define ECHOSF 5    // Echo ��ȣ (�Է� = PF5)

// ��ֹ� ȸ�� ����
#define TRIGFF 0   // Trigger ��ȣ (��� = PD0)
#define ECHOFF 1    // Echo ��ȣ (�Է� = PD1)

// ��ֹ� ȸ�� ���� ����
#define TRIGFL 2   // Trigger ��ȣ (��� = PD2)
#define ECHOFL 3    // Echo ��ȣ (�Է� = PD3)

// ��ֹ� ȸ�� ���� ������
#define TRIGFR 4   // Trigger ��ȣ (��� = PD4)
#define ECHOFR 5    // Echo ��ȣ (�Է� = PD5)

int main(){
uart_init();
unsigned int k=0; // �󸶳� ���������� ���� 
unsigned int i=0; // ������ ��ŭ �ö�����ϱ�
unsigned int s=0; // ���� ��� �ִ��� Ȯ�� 1�̸� ����� �ְ� 0�̸� �� �ȵ�� ����
unsigned int r=0; // ���ٽ� ������
unsigned int f=0; // ���������� �ø���
unsigned int a=0; // if �� �Ȱ��� 
unsigned distanceFRONT(int TRIG, int ECHO);
unsigned distanceAFTER(int TRIG, int ECHO);



  //  unsigned int SINKF; // ��ũȦ ����
    unsigned int FRONT; // ��ֹ� ȸ�� ����
    unsigned int LEFT; // ��ֹ� ȸ�� ���� ����
    unsigned int RIGHT; // ��ֹ� ȸ�� ���� ������
  //  unsigned int AFTERRIGHT; // �� �޸� ������
  //  unsigned int AFTERLEFT; // �� �޸� ����
	unsigned int prevSINKF= 100;
unsigned int speed = 40;  // �⺻ �ӵ�
    unsigned int turnspeed = 27;  // ȸ���� �ӵ�

while(1){

dcmoter_port(); // dc ���� ����
slowsubport(); // ������� ����
PORTB = 0b00110000; // 01 ���� �⺻ ����
PORTC = 0b10000000;  //PC7��� 
DDRA = 0b00000001; // A��Ʈ�� �Է��� �ް� pa0�� ���ڼ�
DDRD = 0b01010101; // TRIGL = ��� , ECHOL = �Է� setting //
 DDRF = 0b01010101; // TRIGL = ��� , ECHOL = �Է� setting //
//PORTA = 0b10000000; // ���ܼ� ���� �νĿ� ��� �� �ν��ϸ� PA7=0;
OCR0=50; // ���� ���� �ӵ� ����
 //    SINKF = distanceAFTER(TRIGSF, ECHOSF); // ��ũȦ ���� ����
    //    _delay_ms(10);
        FRONT = distanceFRONT(TRIGFF, ECHOFF);
        _delay_ms(10);
        LEFT = distanceFRONT(TRIGFL, ECHOFL);
        _delay_ms(10);
        RIGHT = distanceFRONT(TRIGFR, ECHOFR);
        _delay_ms(10);

if (s == 0)
{
PORTA = 0b10000000; // ���ܼ� ���� �νĿ� ��� �� �ν��ϸ� PA7=0; �ʿ���µ� Ȥ�ó��ؼ� ���� �ڵ� �������� ���ڼ� �ڵ� off
}




if ( (!(PINA & (1 << PINA7)))  && s == 0)// �� ������ ��� �ν��ϸ� ���� 0���� �ٲ�� ���⿡ and�� s=0�̶�°� �߰� and�� ������ ���� �߰��ϱ� 
{

DDRE=0b00000001;
PORTE=0b00000000;
OCR0=0; // ������� ����
PORTB=0b11011111; // ������� �� ��� ���� ����
// �ν��ϸ鼭 ȸ�� 
unsigned int A_P=385;
while(1)

{ 
unsigned short find = sonic1();
_delay_ms(1);
printf("find is = %d \r\n" );
          OCR1B=A_P;
_delay_ms(500);
    if (find< 120)
	{
          A_P=A_P-65;
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

// �ν��ϸ鼭 ȸ�� ��


//moveslow1(); // ũ���� -90���� õõ�� ������ (�ν��ؼ� ã�� ���ؼ� ����)
_delay_ms(2000);
//ũ���� ������ 
while(1){
unsigned short object = sonic1();
OCR2=200;
PORTB= 0b10000001;//�������� 

printf("sonic is = %d \r\n" , object);

k=k+1;
i=k;
if (object<45)
	{
 _delay_ms(3000);
	break;
	}
 _delay_ms(1);
}

PORTB=0b11011111; // ������� �� ��� ���� ����
PORTA= 0b00000001;// ���ڼ� on pE0 ��� 
_delay_ms(5000);
while(1)//�ø���
{
OCR2=200;
PORTB= 0b10000010; //�ö󰡱� 

printf("i is = %d \r\n" , i);
i=i-1;

if(i==0){
 _delay_ms(4000);
 _delay_ms(4000);
	break;

	}
_delay_ms(10);
}


PORTB=0b11011111; // ������� �� ��� ���� ����
printf("k is = %d \r\n" , k);
// ũ���� ���󺹱� ����
while(1)

{ 

          OCR1B=A_P;

 	if (A_P>385)
	{     

	  break;


 	}	
		A_P=A_P+5;
		  _delay_ms(500);

}

// ũ���� ���󺹱� ��

//moveslow2(); // ũ���� õõ�� ���󺹱� �ϱ�
 _delay_ms(2000);
s=1; // �� ����


PORTB = 0b00110000; // 01 ���� �⺻ ����
PORTC = 0b10000000;  //PC7���
OCR0=52; // ���� ��� 
_delay_ms(1000);
a=1;

}  // ���ø��� ��

if( (!(PINA & (1 << PINA7)))  && s == 1)// �� ������
{

DDRA=0b00000001; // ���ڼ� ��� �ֱ�
PORTA=0b00000001; // ���ڼ� ��� �ֱ� 
OCR0=0; // ������� ����
PORTB=0b11011111; // ������� �� ��� ���� ����
f=k;
r=k;
moveslow1(); // ũ���� -90���� õõ�� ������
_delay_ms(2000);

//ũ���� ������ 

while(1)//������
{
OCR2=200;
PORTB= 0b10000001;//�������� 

printf("r is = %d \r\n" , r);
r=r-1;

if(r==0){
 _delay_ms(6000);
	break;

	}
_delay_ms(10);
}



PORTB=0b11011111; // ������� �� ��� ���� ����
PORTA= 0b00000000;// ���ڼ� off pE0 ��� 
_delay_ms(3000);

while(1)//�ø���
{
OCR2=200;
PORTB= 0b10000010; //�ö󰡱�

printf("f is = %d \r\n" , f);
f=f-1;

if(f==0){
 _delay_ms(5000);
 _delay_ms(3000);
	break;

	}
_delay_ms(10);
}


PORTB=0b11011111; // ������� �� ��� ���� ����
printf("k is = %d \r\n" , k);

moveslow2(); // ũ���� õõ�� ���󺹱� �ϱ�

 _delay_ms(2000);
s=0; // �� ���� 

//a=1;
PORTB = 0b00110000; // 01 ���� �⺻ ����
PORTC = 0b10000000;  //PC7���
OCR0=52; // ���� ��� 
_delay_ms(1000);

 // ���ø��� ��



} // if �� ���������� ��

if ( (PINA & (1 << PINA6)) && (!(PINA & (1 << PINA5)))     ) //5 ���� 6 ������ 7�߾�
               
{ // �������� ȸ��
               	PORTB = 0b00110000; // 01 ���� �⺻ ����
	   			PORTC = 0b10000000;  //B2,3,4,5 ��
                OCR1A = 295; // �������� ���� ���� (�������� ȸ��)
                OCR0 = speed+35; // ���� �ӵ� ����
		_delay_ms(100);



}

if ((PINA & (1 << PINA5))  &&  (!(PINA & (1 << PINA6)))  ) 
{
                // ���������� ȸ��
                PORTB = 0b00110000; // 01 ���� �⺻ ����
	   			PORTC = 0b10000000;  //B2,3,4,5 ��
                OCR1A = 440; // �������� ���� ���� (���������� ȸ��)
                OCR0 = speed+35; // ���� �ӵ� ����
	        _delay_ms(100);




            } 



 if (FRONT < 300 && (PINA & (1 << PINA6)) && (PINA & (1 << PINA5)) ) {
            LEFT = distanceFRONT(TRIGFL, ECHOFL);
            RIGHT = distanceFRONT(TRIGFR, ECHOFR);
	    

            if (LEFT > RIGHT) {
                // �������� ȸ��
               	PORTB = 0b00110000; // 01 ���� �⺻ ����
	   			PORTC = 0b10000000;  //B2,3,4,5 ��
                OCR1A = 295; // �������� ���� ���� (�������� ȸ��)
                OCR0 = speed+40; // ���� �ӵ� ����
		_delay_ms(100);
            } else if((LEFT < RIGHT)) {
                // ���������� ȸ��
                PORTB = 0b00110000; // 01 ���� �⺻ ����
	   			PORTC = 0b10000000;  //B2,3,4,5 ��
                OCR1A = 450; // �������� ���� ���� (���������� ȸ��)
                OCR0 = speed+40; // ���� �ӵ� ����
	        _delay_ms(100);
            }
        } 
		else {//���� ��ֹ� ���� ���

		 if (RIGHT < 250) {
                // �������� ȸ��
               	PORTB = 0b00110000; // 01 ���� �⺻ ����
	   			PORTC = 0b10000000;  //B2,3,4,5 ��
                OCR1A = 290; // �������� ���� ���� (�������� ȸ��)
                OCR0 = speed+55; // ���� �ӵ� ����
		_delay_ms(100);
            } 
		if  (LEFT < 250){
                // ���������� ȸ��
                PORTB = 0b00110000; // 01 ���� �⺻ ����
	   			PORTC = 0b10000000;  //B2,3,4,5 ��
                OCR1A = 445; // �������� ���� ���� (���������� ȸ��)
                OCR0 = speed+55; // ���� �ӵ� ����
	        _delay_ms(100);
            }
		
        }
        
} // ��ü while�� �� 


}// main �� �� 



// ũ���� DC���� OCR2�� PB0,PB1��� ���� DC���� 0CR0�� PB2,PB3��� 
// ũ���� ������� OC1B��� ���� ������� 0C1A��� 
// 

            









unsigned distanceFRONT(int TRIG, int ECHO){

   
   TCCR3B=0x03;     // Timer/Counter3 Ŭ�� 4us(64����)

   PORTD &= ~(1<<TRIG);  // TrigL=LOW����
   _delay_us(10);         // 10us���� ����

   PORTD |= (1<<TRIG);   // TrigL=HIGH -> �Ÿ� ���� ��� ����
   _delay_us(10);         // 10us���� ����

   PORTD &= ~(1<<TRIG);  // TrigL=LOW -> �Ÿ� ���� ��� ��

   while(!(PIND & (1<<ECHO)));   // EchoL=HIGH�� �� ������ ���
   TCNT3=0x0000;     //Timer/Counter1 �� �ʱ�ȭ

   while(PIND & (1<<ECHO));     //Echo=LOW�� �� ������ ���
   TCCR3B=0x00;     //Timer/Counter1 Ŭ�� ����(Ŭ�� �Է� ����,CS11~CS10=000)

   return (unsigned int)(SOUND_VELOCITY * (TCNT3*4/2)/1000);   // �Ÿ�=�ӵ�x�ð�, �Ÿ� ������ 1mm
}

unsigned distanceAFTER(int TRIG, int ECHO){

   
   TCCR3B=0x03;     // Timer/Counter3 Ŭ�� 4us(64����)

   PORTF &= ~(1<<TRIG);  // TrigL=LOW����
   _delay_us(10);         // 10us���� ����

   PORTF |= (1<<TRIG);   // TrigL=HIGH -> �Ÿ� ���� ��� ����
   _delay_us(10);         // 10us���� ����

   PORTF &= ~(1<<TRIG);  // TrigL=LOW -> �Ÿ� ���� ��� ��

   while(!(PINF & (1<<ECHO)));   // EchoL=HIGH�� �� ������ ���
   TCNT3=0x0000;     //Timer/Counter1 �� �ʱ�ȭ

   while(PINF & (1<<ECHO));     //Echo=LOW�� �� ������ ���
   TCCR3B=0x00;     //Timer/Counter1 Ŭ�� ����(Ŭ�� �Է� ����,CS11~CS10=000)

   return (unsigned int)(SOUND_VELOCITY * (TCNT3*4/2)/1000);   // �Ÿ�=�ӵ�x�ð�, �Ÿ� ������ 1mm
}
