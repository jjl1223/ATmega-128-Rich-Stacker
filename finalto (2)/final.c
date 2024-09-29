#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "sonic.h"
#include <avr/interrupt.h>
#define SOUND_VELOCITY 340UL
#include "uart.h"
#include "crain_C.h"
// 싱크홀 감지 정면 센서
//#define TRIGSF 4   // Trigger 신호 (출력 = PF4)
//#define ECHOSF 5    // Echo 신호 (입력 = PF5)

// 장애물 회피 정면
#define TRIGFF 0   // Trigger 신호 (출력 = PD0)
#define ECHOFF 1    // Echo 신호 (입력 = PD1)

// 장애물 회피 정면 왼쪽
#define TRIGFL 2   // Trigger 신호 (출력 = PD2)
#define ECHOFL 3    // Echo 신호 (입력 = PD3)

// 장애물 회피 정면 오른쪽
#define TRIGFR 4   // Trigger 신호 (출력 = PD4)
#define ECHOFR 5    // Echo 신호 (입력 = PD5)

int main(){
uart_init();
unsigned int k=0; // 얼마나 내려갔는지 측정 
unsigned int i=0; // 내려간 만큼 올라오게하기
unsigned int s=0; // 짐을 들고 있는지 확인 1이면 짐들고 있고 0이면 짐 안들고 있음
unsigned int r=0; // 짐다시 내리기
unsigned int f=0; // 마지막으로 올리기
unsigned int a=0; // if 문 안가게 
unsigned distanceFRONT(int TRIG, int ECHO);
unsigned distanceAFTER(int TRIG, int ECHO);



  //  unsigned int SINKF; // 씽크홀 정면
    unsigned int FRONT; // 장애물 회피 정면
    unsigned int LEFT; // 장애물 회피 정면 왼쪽
    unsigned int RIGHT; // 장애물 회피 정면 오른쪽
  //  unsigned int AFTERRIGHT; // 차 뒷면 오른쪽
  //  unsigned int AFTERLEFT; // 차 뒷면 왼쪽
	unsigned int prevSINKF= 100;
unsigned int speed = 40;  // 기본 속도
    unsigned int turnspeed = 27;  // 회전시 속도

while(1){

dcmoter_port(); // dc 모터 설정
slowsubport(); // 서브모터 설정
PORTB = 0b00110000; // 01 전진 기본 설정
PORTC = 0b10000000;  //PC7사용 
DDRA = 0b00000001; // A포트는 입력을 받게 pa0는 전자석
DDRD = 0b01010101; // TRIGL = 출력 , ECHOL = 입력 setting //
 DDRF = 0b01010101; // TRIGL = 출력 , ECHOL = 입력 setting //
//PORTA = 0b10000000; // 적외선 센서 인식용 흰색 선 인식하면 PA7=0;
OCR0=50; // 차량 모터 속도 설정
 //    SINKF = distanceAFTER(TRIGSF, ECHOSF); // 씽크홀 전면 변수
    //    _delay_ms(10);
        FRONT = distanceFRONT(TRIGFF, ECHOFF);
        _delay_ms(10);
        LEFT = distanceFRONT(TRIGFL, ECHOFL);
        _delay_ms(10);
        RIGHT = distanceFRONT(TRIGFR, ECHOFR);
        _delay_ms(10);

if (s == 0)
{
PORTA = 0b10000000; // 적외선 센서 인식용 흰색 선 인식하면 PA7=0; 필요없는데 혹시나해서 넣은 코드 짐없으면 전자석 자동 off
}




if ( (!(PINA & (1 << PINA7)))  && s == 0)// 짐 없을때 흰색 인식하면 실행 0으로 바뀌면 여기에 and로 s=0이라는것 추가 and로 초음파 구문 추가하기 
{

DDRE=0b00000001;
PORTE=0b00000000;
OCR0=0; // 주행모터 정지
PORTB=0b11011111; // 주행모터 및 모든 모터 정지
// 인식하면서 회전 
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

// 인식하면서 회전 끝


//moveslow1(); // 크레인 -90도로 천천히 돌리기 (인식해서 찾기 위해서 제외)
_delay_ms(2000);
//크레인 내리기 
while(1){
unsigned short object = sonic1();
OCR2=200;
PORTB= 0b10000001;//내려가기 

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

PORTB=0b11011111; // 주행모터 및 모든 모터 정지
PORTA= 0b00000001;// 전자석 on pE0 사용 
_delay_ms(5000);
while(1)//올리기
{
OCR2=200;
PORTB= 0b10000010; //올라가기 

printf("i is = %d \r\n" , i);
i=i-1;

if(i==0){
 _delay_ms(4000);
 _delay_ms(4000);
	break;

	}
_delay_ms(10);
}


PORTB=0b11011111; // 주행모터 및 모든 모터 정지
printf("k is = %d \r\n" , k);
// 크레인 원상복귀 시작
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

// 크레인 원상복귀 끝

//moveslow2(); // 크레인 천천히 원상복귀 하기
 _delay_ms(2000);
s=1; // 짐 있음


PORTB = 0b00110000; // 01 전진 기본 설정
PORTC = 0b10000000;  //PC7사용
OCR0=52; // 차량 출발 
_delay_ms(1000);
a=1;

}  // 짐올리기 끝

if( (!(PINA & (1 << PINA7)))  && s == 1)// 짐 있을때
{

DDRA=0b00000001; // 전자석 잡고 있기
PORTA=0b00000001; // 전자석 잡고 있기 
OCR0=0; // 주행모터 정지
PORTB=0b11011111; // 주행모터 및 모든 모터 정지
f=k;
r=k;
moveslow1(); // 크레인 -90도로 천천히 돌리기
_delay_ms(2000);

//크레인 내리기 

while(1)//내리기
{
OCR2=200;
PORTB= 0b10000001;//내려가기 

printf("r is = %d \r\n" , r);
r=r-1;

if(r==0){
 _delay_ms(6000);
	break;

	}
_delay_ms(10);
}



PORTB=0b11011111; // 주행모터 및 모든 모터 정지
PORTA= 0b00000000;// 전자석 off pE0 사용 
_delay_ms(3000);

while(1)//올리기
{
OCR2=200;
PORTB= 0b10000010; //올라가기

printf("f is = %d \r\n" , f);
f=f-1;

if(f==0){
 _delay_ms(5000);
 _delay_ms(3000);
	break;

	}
_delay_ms(10);
}


PORTB=0b11011111; // 주행모터 및 모든 모터 정지
printf("k is = %d \r\n" , k);

moveslow2(); // 크레인 천천히 원상복귀 하기

 _delay_ms(2000);
s=0; // 짐 없음 

//a=1;
PORTB = 0b00110000; // 01 전진 기본 설정
PORTC = 0b10000000;  //PC7사용
OCR0=52; // 차량 출발 
_delay_ms(1000);

 // 짐올리기 끝



} // if 문 끝짐내리기 끝

if ( (PINA & (1 << PINA6)) && (!(PINA & (1 << PINA5)))     ) //5 왼쪽 6 오른쪽 7중앙
               
{ // 왼쪽으로 회전
               	PORTB = 0b00110000; // 01 전진 기본 설정
	   			PORTC = 0b10000000;  //B2,3,4,5 씀
                OCR1A = 295; // 서보모터 각도 설정 (왼쪽으로 회전)
                OCR0 = speed+35; // 모터 속도 설정
		_delay_ms(100);



}

if ((PINA & (1 << PINA5))  &&  (!(PINA & (1 << PINA6)))  ) 
{
                // 오른쪽으로 회전
                PORTB = 0b00110000; // 01 전진 기본 설정
	   			PORTC = 0b10000000;  //B2,3,4,5 씀
                OCR1A = 440; // 서보모터 각도 설정 (오른쪽으로 회전)
                OCR0 = speed+35; // 모터 속도 설정
	        _delay_ms(100);




            } 



 if (FRONT < 300 && (PINA & (1 << PINA6)) && (PINA & (1 << PINA5)) ) {
            LEFT = distanceFRONT(TRIGFL, ECHOFL);
            RIGHT = distanceFRONT(TRIGFR, ECHOFR);
	    

            if (LEFT > RIGHT) {
                // 왼쪽으로 회전
               	PORTB = 0b00110000; // 01 전진 기본 설정
	   			PORTC = 0b10000000;  //B2,3,4,5 씀
                OCR1A = 295; // 서보모터 각도 설정 (왼쪽으로 회전)
                OCR0 = speed+40; // 모터 속도 설정
		_delay_ms(100);
            } else if((LEFT < RIGHT)) {
                // 오른쪽으로 회전
                PORTB = 0b00110000; // 01 전진 기본 설정
	   			PORTC = 0b10000000;  //B2,3,4,5 씀
                OCR1A = 450; // 서보모터 각도 설정 (오른쪽으로 회전)
                OCR0 = speed+40; // 모터 속도 설정
	        _delay_ms(100);
            }
        } 
		else {//정면 장애물 없는 경우

		 if (RIGHT < 250) {
                // 왼쪽으로 회전
               	PORTB = 0b00110000; // 01 전진 기본 설정
	   			PORTC = 0b10000000;  //B2,3,4,5 씀
                OCR1A = 290; // 서보모터 각도 설정 (왼쪽으로 회전)
                OCR0 = speed+55; // 모터 속도 설정
		_delay_ms(100);
            } 
		if  (LEFT < 250){
                // 오른쪽으로 회전
                PORTB = 0b00110000; // 01 전진 기본 설정
	   			PORTC = 0b10000000;  //B2,3,4,5 씀
                OCR1A = 445; // 서보모터 각도 설정 (오른쪽으로 회전)
                OCR0 = speed+55; // 모터 속도 설정
	        _delay_ms(100);
            }
		
        }
        
} // 전체 while문 끝 


}// main 문 끝 



// 크레인 DC모터 OCR2에 PB0,PB1사용 주행 DC모터 0CR0에 PB2,PB3사용 
// 크레인 서브모터 OC1B사용 주행 서브모터 0C1A사용 
// 

            









unsigned distanceFRONT(int TRIG, int ECHO){

   
   TCCR3B=0x03;     // Timer/Counter3 클록 4us(64분주)

   PORTD &= ~(1<<TRIG);  // TrigL=LOW상태
   _delay_us(10);         // 10us동안 유지

   PORTD |= (1<<TRIG);   // TrigL=HIGH -> 거리 측정 명령 시작
   _delay_us(10);         // 10us동안 유지

   PORTD &= ~(1<<TRIG);  // TrigL=LOW -> 거리 측정 명령 끝

   while(!(PIND & (1<<ECHO)));   // EchoL=HIGH가 될 때까지 대기
   TCNT3=0x0000;     //Timer/Counter1 값 초기화

   while(PIND & (1<<ECHO));     //Echo=LOW가 될 때까지 대기
   TCCR3B=0x00;     //Timer/Counter1 클록 정지(클록 입력 차단,CS11~CS10=000)

   return (unsigned int)(SOUND_VELOCITY * (TCNT3*4/2)/1000);   // 거리=속도x시간, 거리 단위는 1mm
}

unsigned distanceAFTER(int TRIG, int ECHO){

   
   TCCR3B=0x03;     // Timer/Counter3 클록 4us(64분주)

   PORTF &= ~(1<<TRIG);  // TrigL=LOW상태
   _delay_us(10);         // 10us동안 유지

   PORTF |= (1<<TRIG);   // TrigL=HIGH -> 거리 측정 명령 시작
   _delay_us(10);         // 10us동안 유지

   PORTF &= ~(1<<TRIG);  // TrigL=LOW -> 거리 측정 명령 끝

   while(!(PINF & (1<<ECHO)));   // EchoL=HIGH가 될 때까지 대기
   TCNT3=0x0000;     //Timer/Counter1 값 초기화

   while(PINF & (1<<ECHO));     //Echo=LOW가 될 때까지 대기
   TCCR3B=0x00;     //Timer/Counter1 클록 정지(클록 입력 차단,CS11~CS10=000)

   return (unsigned int)(SOUND_VELOCITY * (TCNT3*4/2)/1000);   // 거리=속도x시간, 거리 단위는 1mm
}
