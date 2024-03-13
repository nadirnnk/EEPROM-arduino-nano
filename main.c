/*
 * uart.c
 *
 * Created: 11-03-2024 00:06:20
 * Author : nadir
 */ 

#define F_CPU 16000000UL			/* Define frequency here its 16MHz */
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/eeprom.h>	
#include <avr/interrupt.h>


#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)
unsigned int time_start, time_end, time_elapsed;
unsigned int k[150],l[150];
int n=0,m=0;


void timer1_init()
{
	// Set up timer with prescaler = 64
	TCCR1B |= (1 << CS11) | (1 << CS10);
	
	// Initialize counter
	TCNT1 = 0;
}

void UART_init(long USART_BAUDRATE)
{
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);	/* Turn on transmission and reception */
	UCSR0C |= (1 <<USBS0) | (1 << UCSZ00);	/* Use 8-bit char size */
	UBRR0L = BAUD_PRESCALE;					/* Load lower 8-bits of the baud rate value */
	UBRR0H = (BAUD_PRESCALE >> 8);			/* Load upper 8-bits*/
}

unsigned char UART_RxChar()
{	

	while ((UCSR0A & (1 << RXC0)) == 0);	/* Wait till data is received */
	return(UDR0);							/* Return the byte*/
	
}

void UART_TxChar(char ch)
{

	while (! (UCSR0A & (1<<UDRE0)));		/* Wait for empty transmit buffer*/
	UDR0 = ch ;
}

void UART_SendString(char *tr)				/*Function to send a string of data*/
{
	
	unsigned char j=0;
	
		while (tr[j]!=0)					/* Send string till null */
			{
				UART_TxChar(tr[j]);
				j++;
			}

}

void store_time_elapsed(unsigned int time_elapsed, int flag) {	
	
	if (flag == 0){				
		k[n]=time_elapsed;					/*Storing values of time taken in array*/
		n++;}
	if (flag == 1)
	{
		l[m]=time_elapsed;
		m++;
	}
	
}

unsigned long int calculate(int c, int flag){
	
	unsigned long int sumavg = 0;
	unsigned int d;
	if (flag == 0){
		for(unsigned int p=1; p < (c/10); p++){					//loop to read array of time_elapsed
		d=(100000000/(k[p]*4));									//calculating the baud rate for each of 10 bites where k[p]*4 is time to recieve 10 bytes
		sumavg = sumavg + d;									//calculation, 16*1000000*10*10/k[p]*64
		//sprintf(buffer,"\n%u Rtime %u\n",p ,d);				//where 16Hz is the frequency, 64 is the prescaler, 1000000 to convert us to s.
		// 					UART_SendString(buffer);
		}
	}
	if (flag == 1){
		for(unsigned int p=1; p < (c/10); p++){						//loop to read array of time_elapsed
		d=(100000000/(l[p]*4));										//calculating the baud rate for each of 10 bites where k[p]*4 is time to recieve 10 bytes
		sumavg = sumavg + d;										//calculation, 16*1000000*10*10/k[p]*64
		//sprintf(buffer,"\n%u Rtime %u\n",p ,d);					//where 16Hz is the frequency, 64 is the prescaler, 1000000 to convert us to s.
		//UART_SendString(buffer);
		}
	}
	return (sumavg);
}


int main()
{	
	UART_init(2400);
	timer1_init();
	char receivedData;
	sei();
	unsigned int eepromAddress = 0;
	unsigned int c=0, a=0, b=0, flag;
	char buffer[15];
	unsigned long int sumavg;



															// Loop to receive data and store it in EEPROM then read and transmit
while (1){
	while (eepromAddress < 1024) {							//Loop to recieve and store data in EEPROM
		
		if(a==0)
		{
			time_start = TCNT1;								//starting timer 
		}
		receivedData = UART_RxChar();						//saving received data in char receivedData 
		c++;
 		if(receivedData == '#'){
			 time_end = TCNT1;
			 break; 
			 }						
		a++;
		if (a==10)													//Stoping the time after sending 10 bytes
		{	
			flag = 0;
			time_end = TCNT1;
			time_elapsed = time_end - time_start;					//calcualting the time elapsed
			store_time_elapsed(time_elapsed, flag);					//storing the time elapsed in a arrat
			a=0;
		}
 		eeprom_write_byte((uint8_t*)eepromAddress++, receivedData);		//writing byte in EEPROM	
	}
	
	sumavg = calculate(c, flag);
	sprintf(buffer,"RxAvg %lu \n",sumavg/((c/10)-1));				//printing the average baudrate to recieve
	UART_SendString(buffer);
		
		
		
	for (unsigned int i = 0; i < eepromAddress; i++)			//loop to read from EEPROM and transmit data
	{		
		if(b==0)
			{
				time_start = TCNT1;								//timer start
			}
		UART_TxChar(eeprom_read_byte((uint8_t*)i));				//read and sent the EEPROM message in UART
		b++;
		if (b==10)
		{	
			flag = 1;
			time_end = TCNT1;
			time_elapsed = time_end - time_start;
			store_time_elapsed(time_elapsed, flag);				//stored in a array
			b=0;
		}
		if((eepromAddress-1)==c)
		{
			time_end = TCNT1;									//timer is stopped
		}
	}
		
	sumavg = calculate(c, flag);
	sprintf(buffer,"\n TxAvg %lu \n",sumavg/((c/10)-1));		//printing the average baudrate to transmit
	UART_SendString(buffer);
	
	return 0;
	}
}