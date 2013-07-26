//Interrupt Handler
void setLaser()
{
	TCCR0 = 0b00000100;
	TCNT0 = 0x00;
	TIMSK = 0x01;
	sei();
}

volatile int count=0;

ISR(TIMER0_OVF_vect)
{
	int laserState=1;
	/*
	if(laserState)
		{
			PORTB &= ~_BV(PB3);	//turn off laser
			laserState=0;
		}
		else
		{	
			PORTB |= _BV(PB3);	//turn on laser
			laserState=1;
		}
	*/
	
	count++;
	if(count==1000)
	{
		if(laserState)
		{
			PORTB &= ~_BV(PB3);	//turn off laser
			count=0;
			laserState=0;
		}
		else
		{	
			PORTB |= _BV(PB3);	//turn on laser
			count=0;
			laserState=1;
		}
	}
	
}
