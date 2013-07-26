void setPWM(int pwmA, int pwmB)
{
	//set pwm parameters 
	TCCR1A = 0b10100010; //a2
	TCCR1B = 0b00001100; //19
	
	//Phase and Frequency Correct
	int phaseCorr=1;
	int phaseDebug=0;
	int max = 4000;
	int min = 3500;
	if(phaseCorr==1)
	{
		TCCR1A = (1 << COM1A1) | (1 << COM1B1);
		TCCR1B = (1 << WGM13) | (1 << CS10);
		ICR1 = 40000; 
		if(phaseDebug==1)
		{
			OCR1A = 3000;	//higher OCR = negative angle
			OCR1B = 4500;	//
			while(1)
			{
				OCR1A+=5;
				if(OCR1A>max)
					OCR1A = min;
				_delay_ms(10);
			}	
		}
	}
	
	TCNT1 = 0; 
	OCR1A = pwmA;//PWM Duty "optional"
	OCR1B = pwmB;
}


void checkOCRa(int min, int max)
{
	if(OCR1A>max)
		OCR1A=max;
	else if(OCR1A<min)
		OCR1A=min;
	//outputOCR(OCR1A);
}

void checkOCRb(int min, int max)
{
	if(OCR1B>max)
		OCR1B=max;
	else if(OCR1B<min)
		OCR1B=min;
	//outputOCR(OCR1B);
}

void outputOCR(unsigned char value)
{
	PORTC &= ~0b00111111;
	PORTB &= ~0b11000000;
	PORTC |= (value & 0b00111111);
	PORTB |= (value & 0b11000000)>>2;
}
