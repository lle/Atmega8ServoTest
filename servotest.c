#define u08 unsigned char
#define u16 unsigned int
#define s08 char
#define s16 int
#define F_CPU 8000000 

#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <util/delay.h>
#include "servoControl.h"
#include "debug.h"
#include "calibration.h"
#include "laser.h"

static u16 minA=3000;		//Up Down	-	higher OCR = negative angle
static u16 maxA=3800;
static u16 minB=4300;		//Left Right	-	higher OCR = right
static u16 maxB=5000;
volatile u08 laserFlag=0;
static u16 tiltAmount=30;
volatile u16 counter=0;

static u16 A1=0x0C12; 
static u16 B1=0x119E;
	
static u16 A2=0x0C12; 
static u16 B2=0x1324;
	
static u16 A3=0x0C8A;
static u16 B3=0x1234;
	
static u16 A4=0x0C8A; 
static u16 B4=0x12AC;

static u16 A5=0x0D20; 
static u16 B5=0x1234;

static u16 A6=0x0D20; 
static u16 B6=0x12AC;

static u16 A7=0x0D98;
static u16 B7=0x119E;

static u16 A8=0x0DB6;
static u16 B8=0x1342;

unsigned char read_from_internal_eeprom(unsigned int addr) 
  { 
   while (EECR & (1<<EEWE)); //wait for write to finish 
   EEAR = addr; //load the address to read from 
   EECR |= (1<<EERE);//set the read flag 
   //the clock is stopped for 4 cyles 
   EEAR =0; //leave address at 0 so if power down corrupts location 0 
   return  EEDR; 
  } 
  
unsigned char write_internal_eeprom(unsigned int addr,char data) 
 { 
  char cnt; 
  for(cnt=0;cnt<10;cnt++) 
     { 
      if(data==(read_from_internal_eeprom(addr))) //will check in the read routine for write flag being cleared 
         break; 
      EEAR = addr;//load up address 
      EEDR = data; //load up data 
      EECR |= (1<<EEMWE); //enable write 
      EECR |= (1<<EEWE); //start write 
     } 
    return cnt; 
  } 

void record()
{
   write_internal_eeprom(counter, OCR1AH);
   write_internal_eeprom(counter+1, OCR1AL);
   write_internal_eeprom(counter+2, 0x00);
   write_internal_eeprom(counter+3, OCR1BH);
   write_internal_eeprom(counter+4, OCR1BL);
   counter+=32;
}

void calibrateAll()
{
	//Calibration ALL 8 targets
	//setLaser();
	u16 flexA = 30;		//up down (higher = negative angle)
	u16 flexB = 00;		//right left (higher = right)

	OCR1A=A1+flexA;
	OCR1B=B1+flexB;
	_delay_ms(400);
	OCR1A=A2+flexA;
	OCR1B=B2+flexB;
	_delay_ms(400);
	OCR1A=A3+flexA;
	OCR1B=B3+flexB;
	_delay_ms(400);
	OCR1A=A4+flexA;
	OCR1B=B4+flexB;
	_delay_ms(400);
	OCR1A=A5+flexA;
	OCR1B=B5+flexB;
	_delay_ms(400);
	OCR1A=A6+flexA;
	OCR1B=B6+flexB;
	_delay_ms(400);
	OCR1A=A7+flexA;
	OCR1B=B7+flexB;
	_delay_ms(400);
	OCR1A=A8+flexA;
	OCR1B=B8+flexB;
	_delay_ms(400);
}

//Main Program
int main (void) 
{
	//set pins
	DDRB=0xFF;
	DDRC=0b00111111;
	DDRD=0x00;	

	setPWM(minA,minB);

	//debug();
	
	//Go to max angle (these angle are not the mecanical maximum of the servo. There are just limits so the laser does shot in people's eye
	/*		
	while(1)
	{
		//calibrate16();
		testMaxA(minA, maxA);
		testMaxB(minB, maxB);
	}
	*/

	while(1)
	{
		while(!(PIND & (1<<PD5)))
		{
			calibrateAll();
		}
		while(!(PIND & (1<<PD6)))
		{

		}
		while(!(PIND & (1<<PD7)))
		{

		}
			
		//Acticate Laser
		if(!(PIND & (1<<PD4)))
		{	

			PORTB |= _BV(PB3);
			//laserFlag=1;

			//Recording the x-y value of the servo when button press
			/*
			_delay_ms(10);
			if( !(PIND & (1<<PD4)))
			{
				record();
				while(!(PIND & (1<<PD4)))
				{
					_delay_ms(10);
				}
			}
			*/
		}
		else
		{	
			PORTB &= ~_BV(PB3);
			//laserFlag = 0;
		}

		//Set Servo 1
		if(!(PIND & (1<<PD0)))
		{
			OCR1B+=tiltAmount;
			checkOCRb(minB, maxB);
		}
		else if(!(PIND & (1<<PD1)))
		{
			OCR1B-=tiltAmount;
			checkOCRb(minB, maxB);
		}

		//Set Servo 2
		if(!(PIND & (1<<PD2)))
		{
			OCR1A+=tiltAmount;
			checkOCRa(minA, maxA);
		}
		else if(!(PIND & (1<<PD3)))
		{
			OCR1A-=tiltAmount;
			checkOCRa(minA, maxA);
		}

		_delay_ms(30);
		PORTB &= ~_BV(PB3);	//turn off laser
		_delay_ms(30);
	}	
}
