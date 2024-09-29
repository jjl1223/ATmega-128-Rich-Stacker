#include <stdio.h>
#include <avr/io.h>

#define F_CPU 16000000UL

#include "uart.h"

#define UART_BAUD 9600

int uart_putchar_stream (char c, FILE* stream);

int uart_getchar_stream (FILE *stream );

FILE uart_str = FDEV_SETUP_STREAM(uart_putchar_stream, uart_getchar_stream, _FDEV_SETUP_RW);

void uart_init (void){

// 보드레이트 설정  DEFINE 한 F_CPU 변수로 값이 결정됨.

UBRR0L = ((F_CPU/(16UL* UART_BAUD))- 1);

// Tx,Rx 초기화 

UCSR0B = (1<<TXEN0) | (1<<RXEN0); // 송신부가 동작하거나, 수신부가 동작할때 비트 shift 연산으로 1 의 값을 송수신 될때 shgit 시킴.

// 8N1

UCSR0C = 3<<UCSZ00;

// stdio set to uart

stdout = stdin = stderr= &uart_str;}

// uart 에서 데이터를 전송하는 코드 
int uart_putchar (char c){

loop_until_bit_is_set(UCSR0A, UDRE0 );

UDR0=c;

return 0;}

// uart 에서 데이터를 입력받는 코드
int uart_getchar (void ){

loop_until_bit_is_set(UCSR0A, RXC0);

return UDR0;}

uint8_t uart_kbhit (void )
{ uint8_t hit = 0;

if(UCSR0A & (1<<RXC0)) // RXC0 에서 쉬프트와 동시에 UCSROA 가 set 될때 hit 를 set 시키고 return 함.
hit=1;
return hit;}

int uart_putchar_stream (char c, FILE * stream){

if(c == '\n') // c 의 배열이 끝나면. (개행으로 구분함.) 값을 출력함 

uart_putchar_stream('\r', stream);

loop_until_bit_is_set(UCSR0A, UDRE0);

UDR0=c;

return 0;}

int uart_getchar_stream (FILE *stream){

uint8_t c;

char *cp;

static char rxb[RX_BUFSIZE];

static char * bp;

// 버퍼에 데이터가 들어 올때까지 대기
if(bp == 0){
	cp=rxb ;
while(1){

loop_until_bit_is_set(UCSR0A, RXC0 );

if (UCSR0A & (1<<FE0))
	return _FDEV_EOF;

if (UCSR0A & (1<<DOR0))
	return _FDEV_ERR;
// 데이터를 얻음 
	c=UDR0;
// 새로운 라인으로 개행함
if(c == '\r')
	c = '\n';
// 줄바꿈이 시작되면 새로운 입력으로 인식하여 버퍼를 비움.
if(c == '\n'){
	*cp=c;
	uart_putchar_stream(c, stream);
	bp = rxb;//rxp=b;
	break;}

// 0x20 과 0x73 번지에서 출력할수있는 문자들을 저장함 . 그때의 값이 공백과 ~ 임. 
if(( c >= (uint8_t)' ') && (c <= (uint8_t) '~')) 
{
if( cp < (rxb + RX_BUFSIZE-1)){
	*cp ++ = c;
	uart_putchar_stream(c, stream);}	
  }
 }
}
	c= *bp++;
if ( c == '\n'){
	bp = 0;}
return c;
}
