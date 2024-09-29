#include "sonic.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define SOUND_VELOCITY 340UL


unsigned short sonic1(){
unsigned int distance1;
	int TRIG = 6;
 	int ECHO = 7;
//	int SOUND_VELOCITY = 3400;
	DDRE = ((DDRE | (1<<TRIG)) & ~(1<<ECHO));
	

		
		TCCR3B = 0x03;
		PORTE &= ~(1<<TRIG);
		_delay_us(10);
		PORTE |= (1<<TRIG);
		_delay_us(10);
		PORTE &= ~(1<<TRIG);
		while(!(PINE & (1<<ECHO)));
		TCNT3 = 0x0000;
		while(PINE & (1<<ECHO));
		TCCR3B = 0x00;
		distance1 = (unsigned int)(SOUND_VELOCITY * (TCNT3 * 4 / 2) / 1000);

		//음파의 속도는 340m/s 이므로 1cm를 이동하는데 약 29us.
		// 4/2 = 왕복거리
		return distance1;

		
}

unsigned short sonic2(){//우측  
unsigned int distance2;
	int TRIG = 4;
 	int ECHO = 5;
//	int SOUND_VELOCITY = 3400;
	DDRE = ((DDRE | (1<<TRIG)) & ~(1<<ECHO));
	
		
		TCCR3B = 0x03;
		PORTE &= ~(1<<TRIG);
		_delay_us(10);
		PORTE |= (1<<TRIG);
		_delay_us(10);
		PORTE &= ~(1<<TRIG);
		while(!(PINE & (1<<ECHO)));
		TCNT3 = 0x0000;
		while(PINE & (1<<ECHO));
		TCCR3B = 0x00;
		distance2 = (unsigned int)(SOUND_VELOCITY * (TCNT3 * 4 / 2) / 1000);

		//음파의 속도는 340m/s 이므로 1cm를 이동하는데 약 29us.
		// 4/2 = 왕복거리
		return distance2;

		
}

unsigned short sonic3(){ //좌  
unsigned int distance3;
	int TRIG = 2;
 	int ECHO = 3;
//	int SOUND_VELOCITY = 3400;
	DDRE = ((DDRE | (1<<TRIG)) & ~(1<<ECHO));
	
	
		TCCR3B = 0x03;
		PORTE &= ~(1<<TRIG);
		_delay_us(10);
		PORTE |= (1<<TRIG);
		_delay_us(10);
		PORTE &= ~(1<<TRIG);
		while(!(PINE & (1<<ECHO)));
		TCNT3 = 0x0000;
		while(PINE & (1<<ECHO));
		TCCR3B = 0x00;
		distance3 = (unsigned int)(SOUND_VELOCITY * (TCNT3 * 4 / 2) / 1000);

		//음파의 속도는 340m/s 이므로 1cm를 이동하는데 약 29us.
		// 4/2 = 왕복거리
		return distance3;

		
}

// #define TRIG 6
// #define ECHO 7
// #define SOUND_VELOCITY 340UL


