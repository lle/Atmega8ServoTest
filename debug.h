void debug()
{
	PORTB |= _BV(PB5);
	PORTB |= _BV(PB4);
	_delay_ms(500);
	PORTB=0x00;
}
