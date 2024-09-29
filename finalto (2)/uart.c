#include <stdio.h>
#include <avr/io.h>

#define F_CPU 16000000UL

#include "uart.h"

#define UART_BAUD 9600

int uart_putchar_stream (char c, FILE* stream);

int uart_getchar_stream (FILE *stream );

FILE uart_str = FDEV_SETUP_STREAM(uart_putchar_stream, uart_getchar_stream, _FDEV_SETUP_RW);

void uart_init (void){

// ���巹��Ʈ ����  DEFINE �� F_CPU ������ ���� ������.

UBRR0L = ((F_CPU/(16UL* UART_BAUD))- 1);

// Tx,Rx �ʱ�ȭ 

UCSR0B = (1<<TXEN0) | (1<<RXEN0); // �۽źΰ� �����ϰų�, ���źΰ� �����Ҷ� ��Ʈ shift �������� 1 �� ���� �ۼ��� �ɶ� shgit ��Ŵ.

// 8N1

UCSR0C = 3<<UCSZ00;

// stdio set to uart

stdout = stdin = stderr= &uart_str;}

// uart ���� �����͸� �����ϴ� �ڵ� 
int uart_putchar (char c){

loop_until_bit_is_set(UCSR0A, UDRE0 );

UDR0=c;

return 0;}

// uart ���� �����͸� �Է¹޴� �ڵ�
int uart_getchar (void ){

loop_until_bit_is_set(UCSR0A, RXC0);

return UDR0;}

uint8_t uart_kbhit (void )
{ uint8_t hit = 0;

if(UCSR0A & (1<<RXC0)) // RXC0 ���� ����Ʈ�� ���ÿ� UCSROA �� set �ɶ� hit �� set ��Ű�� return ��.
hit=1;
return hit;}

int uart_putchar_stream (char c, FILE * stream){

if(c == '\n') // c �� �迭�� ������. (�������� ������.) ���� ����� 

uart_putchar_stream('\r', stream);

loop_until_bit_is_set(UCSR0A, UDRE0);

UDR0=c;

return 0;}

int uart_getchar_stream (FILE *stream){

uint8_t c;

char *cp;

static char rxb[RX_BUFSIZE];

static char * bp;

// ���ۿ� �����Ͱ� ��� �ö����� ���
if(bp == 0){
	cp=rxb ;
while(1){

loop_until_bit_is_set(UCSR0A, RXC0 );

if (UCSR0A & (1<<FE0))
	return _FDEV_EOF;

if (UCSR0A & (1<<DOR0))
	return _FDEV_ERR;
// �����͸� ���� 
	c=UDR0;
// ���ο� �������� ������
if(c == '\r')
	c = '\n';
// �ٹٲ��� ���۵Ǹ� ���ο� �Է����� �ν��Ͽ� ���۸� ���.
if(c == '\n'){
	*cp=c;
	uart_putchar_stream(c, stream);
	bp = rxb;//rxp=b;
	break;}

// 0x20 �� 0x73 �������� ����Ҽ��ִ� ���ڵ��� ������ . �׶��� ���� ����� ~ ��. 
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
