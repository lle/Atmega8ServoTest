void calibrate16()
{
	//up right corner = 27,45 (a,b)
	//up left corner = 27,39
	//down right = 32,39
	//down left = 32,45
	OCR1A = 27;	//up down (greater ocr = lower angle)
	OCR1B = 45;	//left right (greater ocr = right)
	for(int i=0; i<4; i++)
	{
		OCR1B=OCR1B-4;
		OCR1A++;
		for(int j=0; j<4; j++)
		{
			OCR1B++;
			_delay_ms(750);
		}
	}
}

void testMaxA(int min, int max)
{
	while(OCR1A<max)
	{
		OCR1A+=10;
		_delay_ms(5);
	}
	while(OCR1A>min)
	{
		OCR1A-=10;
		_delay_ms(5);
	}
}

void testMaxB(int min, int max)
{
	while(OCR1B<max)
	{
		OCR1B+=10;
		_delay_ms(5);
	}
	while(OCR1B>min)
	{
		OCR1B-=10;
		_delay_ms(5);
	}	
}
