/*
 *	Filename: max7221spi.c
 *	Current Rev: 2.0
 *	ShortDesc: PIC18 MAX7221 SPI Communication Routines
 *	Author: mas
 *	
 *	FullDesc: Provides communication routines for the Maxim MAX7221 7-segment display driver
 *				over SPI.  Contains some functions specifically written for PrISUm Dash.
 *
 *	Revision History:	Rev		User	Date		Desc
 *						2.0		mas		6/6/2010	Functions revised and polished for P10
 *						1.x		mas		lots		A bazillion undocumented revisions
 *						1.0		mas		6/2008		Original Release
 */

#include "max7221spi.h"
#include <spi.h>
#include <p18cxxx.h>
#include <delays.h>

unsigned char cNumber;
unsigned long powTen[8] = {1,10,100,1000,10000,100000,1000000,10000000};
unsigned char BarValL[9] = {0b00000000,0b01000000,0b01100000,0b01110000,0b01111000,0b01111100,0b01111110,0b01111111,0b11111111};
unsigned char BarValR[9] = {0b00000000,0b10000000,0b10000001,0b10000011,0b10000111,0b10001111,0b10011111,0b10111111,0b11111111};

//Initialize given number of 7221s in cascade
void Init7221SPI(unsigned char cNum)
{
	char i;

	//Store number of 7221's in memory
	cNumber = cNum;

	//Set SPI pin modes
	CSpin	= 0;
	SCKpin	= 0;
	SDIpin	= 1;
	SDOpin	= 0;

	//Initialize SPI connection and CS state
	OpenSPI(SPI_FOSC_64, MODE_00, SMPEND);
	CS = 1;

	for(i=0;i<cNumber;i++)
	{
		WriteReg7221(i,0x0C,0x01);  //Bring out of shutdown
		WriteReg7221(i,0x0B,0x07);  //Set number of digits: 8
		WriteReg7221(i,0x0A,0x0F);	//Set intensity: Full
		WriteReg7221(i,0x09,0x00);  //Set decode mode: no decode
		WriteReg7221(i,0x0F,0x00);	//Exit Display Test mode

		//Blank all digit registers
		WriteReg7221(i,0x01,0x00);
		WriteReg7221(i,0x02,0x00);
		WriteReg7221(i,0x03,0x00);
		WriteReg7221(i,0x04,0x00);
		WriteReg7221(i,0x05,0x00);
		WriteReg7221(i,0x06,0x00);
		WriteReg7221(i,0x07,0x00);
		WriteReg7221(i,0x08,0x00);
	}
}

//Write data to a given register on a specified chip
void WriteReg7221(unsigned char cSel,unsigned char cReg,unsigned char cData)
{

	char j;

	CS = 0;
	for(j=0;j<((cNumber-1)-cSel);j++)
	{
		putcSPI(0x00);
		putcSPI(0x00);
	}
	putcSPI(cReg);
	putcSPI(cData);
	for(j=0;j<cSel;j++)
	{
		putcSPI(0x00);
		putcSPI(0x00);
	}
	CS = 1;
}

//Set output intensity of specified chip
void SetIntensity7221(unsigned char cSel, unsigned char cValue)
{
	if(cValue > 0x0F)
		WriteReg7221(cSel,0x0A,0x0F);
	else
		WriteReg7221(cSel,0x0A,cValue);
}

void WriteDigit7221(unsigned char cSel, unsigned char cDigit, unsigned char cData, unsigned char decPt)
{
	switch(cData)
	{
		case 0x00:
			if(!decPt)
				WriteReg7221(cSel,cDigit,FONT21ZERO);
			else
				WriteReg7221(cSel,cDigit,FONT21ZERO|0b10000000);
			break;
		case 0x01:
			if(!decPt)
				WriteReg7221(cSel,cDigit,FONT21ONE);
			else
				WriteReg7221(cSel,cDigit,FONT21ONE|0b10000000);
			break;
		case 0x02:
			if(!decPt)
				WriteReg7221(cSel,cDigit,FONT21TWO);
			else
				WriteReg7221(cSel,cDigit,FONT21TWO|0b10000000);
			break;
		case 0x03:
			if(!decPt)
				WriteReg7221(cSel,cDigit,FONT21THREE);
			else
				WriteReg7221(cSel,cDigit,FONT21THREE|0b10000000);
			break;
		case 0x04:
			if(!decPt)
				WriteReg7221(cSel,cDigit,FONT21FOUR);
			else
				WriteReg7221(cSel,cDigit,FONT21FOUR|0b10000000);
			break;
		case 0x05:
			if(!decPt)
				WriteReg7221(cSel,cDigit,FONT21FIVE);
			else
				WriteReg7221(cSel,cDigit,FONT21FIVE|0b10000000);
			break;
		case 0x06:
			if(!decPt)
				WriteReg7221(cSel,cDigit,FONT21SIX);
			else
				WriteReg7221(cSel,cDigit,FONT21SIX|0b10000000);
			break;
		case 0x07:
			if(!decPt)
				WriteReg7221(cSel,cDigit,FONT21SEVEN);
			else
				WriteReg7221(cSel,cDigit,FONT21SEVEN|0b10000000);
			break;
		case 0x08:
			if(!decPt)
				WriteReg7221(cSel,cDigit,FONT21EIGHT);
			else
				WriteReg7221(cSel,cDigit,FONT21EIGHT|0b10000000);
			break;
		case 0x09:
			if(!decPt)
				WriteReg7221(cSel,cDigit,FONT21NINE);
			else
				WriteReg7221(cSel,cDigit,FONT21NINE|0b10000000);
			break;
		case 0x0A:
			WriteReg7221(cSel,cDigit,FONT21DIG_A);
			break;
		case 0x0B:
			WriteReg7221(cSel,cDigit,FONT21DIG_B);
			break;
		case 0x0C:
			WriteReg7221(cSel,cDigit,FONT21DIG_C);
			break;
		case 0x0D:
			WriteReg7221(cSel,cDigit,FONT21DIG_D);
			break;
		case 0x0E:
			WriteReg7221(cSel,cDigit,FONT21DIG_E);
			break;
		case 0x0F:
			WriteReg7221(cSel,cDigit,FONT21DIG_F);
			break;
		default:
			break;
	}
}
//Ranges: 1-8
//cSel: 0-x
void WriteInt7221(unsigned char cSel, unsigned char cRangeStart, unsigned char cRangeEnd, int iData)
{
	//NOTE:  THIS FUNCTION ONLY WORKS WHEN THE RANGE OF DIGITS IS ACROSS A SINGLE CHIP!
	int i;
	int intData = iData;
	char cAvailDig = cRangeEnd - cRangeStart + 1;
	if(intData < 0)
	{
		intData = -intData;
		cAvailDig--;
		WriteReg7221(cSel,cRangeEnd+1,0b00000001);
	}
	if(intData>=powTen[cAvailDig])
	{
		for(i=cRangeStart;i<=(cRangeStart+cAvailDig-1);i++)
		{
			WriteReg7221(cSel,i+1,0b10000000);
		}
	}
	else
	{
		for(i=cRangeStart;i<=(cRangeStart+cAvailDig-1);i++)
		{
			WriteDigit7221(cSel,i+1,((intData%powTen[i-cRangeStart+1])/powTen[i-cRangeStart]),0);
			//intData /= powTen[i-cRangeStart+1];
			//intData *= powTen[i-cRangeStart+1];
		}
	}
}

void SetBarGraphL(unsigned char cVal)
{
	unsigned char cBars = cVal;
	//unsigned char cBars = (unsigned char)((cVal/255.0)*24);
	if(cBars > 16)
	{
		WriteReg7221(0,0x05,0xFF);
		WriteReg7221(0,0x06,0xFF);
		WriteReg7221(0,0x07,BarValL[cBars - 16]);
	}
	else if(cBars > 8)
	{
		WriteReg7221(0,0x05,0xFF);
		WriteReg7221(0,0x06,BarValL[cBars - 8]);
		WriteReg7221(0,0x07,0);
	}
	else
	{
		WriteReg7221(0,0x05,BarValL[cBars]);
		WriteReg7221(0,0x06,0);
		WriteReg7221(0,0x07,0);
	}
}

void SetBarGraphR(unsigned char cVal)
{
	unsigned char cBars = cVal;
	//unsigned char cBars = (unsigned char)((cVal/255.0)*24);
	if(cBars > 16)
	{
		WriteReg7221(0,0x05,BarValR[cBars - 16]);
		WriteReg7221(0,0x06,0xFF);
		WriteReg7221(0,0x07,0xFF);
	}
	else if(cBars > 8)
	{
		WriteReg7221(0,0x05,0);
		WriteReg7221(0,0x06,BarValR[cBars - 8]);
		WriteReg7221(0,0x07,0xFF);
	}
	else
	{
		WriteReg7221(0,0x05,0);
		WriteReg7221(0,0x06,0);
		WriteReg7221(0,0x07,BarValR[cBars]);
	}
}

void DisplayTest7221(void)
{
	//This function wastes enormous quantities of memory.
	//But it looks neato, so there you go.
	//Efficiency!

	WriteReg7221(0,1,0b01000000);
	WriteReg7221(0,2,0b01000000);
	WriteReg7221(0,3,0b01000000);
	WriteReg7221(0,4,0b01000000);
	WriteReg7221(1,1,0b01000000);
	WriteReg7221(1,2,0b01000000);
	WriteReg7221(1,3,0b01000000);
	WriteReg7221(1,4,0b01000000);
	WriteReg7221(1,5,0b01000000);
	WriteReg7221(0,5,0b11111111);
	WriteReg7221(0,6,0b11111111);
	WriteReg7221(0,7,0b11111111);
	WriteReg7221(1,6,0b01000000);
	Delay10KTCYx(50);

	WriteReg7221(0,1,0b00100000);
	WriteReg7221(0,2,0b00100000);
	WriteReg7221(0,3,0b00100000);
	WriteReg7221(0,4,0b00100000);
	WriteReg7221(1,1,0b00100000);
	WriteReg7221(1,2,0b00100000);
	WriteReg7221(1,3,0b00100000);
	WriteReg7221(1,4,0b00100000);
	WriteReg7221(1,5,0b00100000);
	WriteReg7221(0,6,0b11110011);
	Delay10KTCYx(50);

	WriteReg7221(0,1,0b00010000);
	WriteReg7221(0,2,0b00010000);
	WriteReg7221(0,3,0b00010000);
	WriteReg7221(0,4,0b00010000);
	WriteReg7221(1,1,0b00010000);
	WriteReg7221(1,2,0b00010000);
	WriteReg7221(1,3,0b00010000);
	WriteReg7221(1,4,0b00010000);
	WriteReg7221(1,5,0b00010000);
	WriteReg7221(0,6,0b11100001);
	WriteReg7221(1,6,0b00100000);
	Delay10KTCYx(50);

	WriteReg7221(0,1,0b00001000);
	WriteReg7221(0,2,0b00001000);
	WriteReg7221(0,3,0b00001000);
	WriteReg7221(0,4,0b00001000);
	WriteReg7221(1,1,0b00001000);
	WriteReg7221(1,2,0b00001000);
	WriteReg7221(1,3,0b00001000);
	WriteReg7221(1,4,0b00001000);
	WriteReg7221(1,5,0b00001000);
	WriteReg7221(0,6,0b11000000);
	Delay10KTCYx(50);

	WriteReg7221(0,1,0b00000100);
	WriteReg7221(0,2,0b00000100);
	WriteReg7221(0,3,0b00000100);
	WriteReg7221(0,4,0b00000100);
	WriteReg7221(1,1,0b00000100);
	WriteReg7221(1,2,0b00000100);
	WriteReg7221(1,3,0b00000100);
	WriteReg7221(1,4,0b00000100);
	WriteReg7221(1,5,0b00000100);
	WriteReg7221(0,6,0b00000000);
	WriteReg7221(1,6,0b00010000);
	Delay10KTCYx(50);

	WriteReg7221(0,1,0b00000010);
	WriteReg7221(0,2,0b00000010);
	WriteReg7221(0,3,0b00000010);
	WriteReg7221(0,4,0b00000010);
	WriteReg7221(1,1,0b00000010);
	WriteReg7221(1,2,0b00000010);
	WriteReg7221(1,3,0b00000010);
	WriteReg7221(1,4,0b00000010);
	WriteReg7221(1,5,0b00000010);
	WriteReg7221(0,5,0b01111111);
	WriteReg7221(0,7,0b10111111);
	Delay10KTCYx(50);

	WriteReg7221(0,1,0b01000000);
	WriteReg7221(0,2,0b01000000);
	WriteReg7221(0,3,0b01000000);
	WriteReg7221(0,4,0b01000000);
	WriteReg7221(1,1,0b01000000);
	WriteReg7221(1,2,0b01000000);
	WriteReg7221(1,3,0b01000000);
	WriteReg7221(1,4,0b01000000);
	WriteReg7221(1,5,0b01000000);
	WriteReg7221(0,5,0b01111110);
	WriteReg7221(0,7,0b10011111);
	WriteReg7221(1,6,0b00001000);
	Delay10KTCYx(50);

	WriteReg7221(0,1,0b00100000);
	WriteReg7221(0,2,0b00100000);
	WriteReg7221(0,3,0b00100000);
	WriteReg7221(0,4,0b00100000);
	WriteReg7221(1,1,0b00100000);
	WriteReg7221(1,2,0b00100000);
	WriteReg7221(1,3,0b00100000);
	WriteReg7221(1,4,0b00100000);
	WriteReg7221(1,5,0b00100000);
	WriteReg7221(0,5,0b01111100);
	WriteReg7221(0,7,0b10001111);
	Delay10KTCYx(50);

	WriteReg7221(0,1,0b00010000);
	WriteReg7221(0,2,0b00010000);
	WriteReg7221(0,3,0b00010000);
	WriteReg7221(0,4,0b00010000);
	WriteReg7221(1,1,0b00010000);
	WriteReg7221(1,2,0b00010000);
	WriteReg7221(1,3,0b00010000);
	WriteReg7221(1,4,0b00010000);
	WriteReg7221(1,5,0b00010000);
	WriteReg7221(0,5,0b01111000);
	WriteReg7221(0,7,0b10000111);
	WriteReg7221(1,6,0b00000100);
	Delay10KTCYx(50);

	WriteReg7221(0,1,0b00001000);
	WriteReg7221(0,2,0b00001000);
	WriteReg7221(0,3,0b00001000);
	WriteReg7221(0,4,0b00001000);
	WriteReg7221(1,1,0b00001000);
	WriteReg7221(1,2,0b00001000);
	WriteReg7221(1,3,0b00001000);
	WriteReg7221(1,4,0b00001000);
	WriteReg7221(1,5,0b00001000);
	WriteReg7221(0,5,0b01110000);
	WriteReg7221(0,7,0b10000011);
	Delay10KTCYx(50);

	WriteReg7221(0,1,0b00000100);
	WriteReg7221(0,2,0b00000100);
	WriteReg7221(0,3,0b00000100);
	WriteReg7221(0,4,0b00000100);
	WriteReg7221(1,1,0b00000100);
	WriteReg7221(1,2,0b00000100);
	WriteReg7221(1,3,0b00000100);
	WriteReg7221(1,4,0b00000100);
	WriteReg7221(1,5,0b00000100);
	WriteReg7221(0,5,0b01100000);
	WriteReg7221(0,7,0b10000001);
	WriteReg7221(1,6,0b00000010);
	Delay10KTCYx(50);

	WriteReg7221(0,1,0b00000010);
	WriteReg7221(0,2,0b00000010);
	WriteReg7221(0,3,0b00000010);
	WriteReg7221(0,4,0b00000010);
	WriteReg7221(1,1,0b00000010);
	WriteReg7221(1,2,0b00000010);
	WriteReg7221(1,3,0b00000010);
	WriteReg7221(1,4,0b00000010);
	WriteReg7221(1,5,0b00000010);
	WriteReg7221(0,5,0b01000000);
	WriteReg7221(0,7,0b10000000);
	Delay10KTCYx(50);

	WriteReg7221(0,1,0b01000000);
	WriteReg7221(0,2,0b01000000);
	WriteReg7221(0,3,0b01000000);
	WriteReg7221(0,4,0b01000000);
	WriteReg7221(1,1,0b01000000);
	WriteReg7221(1,2,0b01000000);
	WriteReg7221(1,3,0b01000000);
	WriteReg7221(1,4,0b01000000);
	WriteReg7221(1,5,0b01000000);
	WriteReg7221(1,6,0b00000001);
	WriteReg7221(0,5,0);
	WriteReg7221(0,7,0);
	Delay10KTCYx(50);

	WriteReg7221(0,1,0xFF);
	WriteReg7221(0,2,0xFF);
	WriteReg7221(0,3,0xFF);
	WriteReg7221(0,4,0xFF);
	WriteReg7221(1,1,0xFF);
	WriteReg7221(1,2,0xFF);
	WriteReg7221(1,3,0xFF);
	WriteReg7221(1,4,0xFF);
	WriteReg7221(1,5,0xFF);
	WriteReg7221(0,5,0xFF);
	WriteReg7221(0,6,0xFF);
	WriteReg7221(0,7,0xFF);
	WriteReg7221(1,6,0xFF);
	Delay10KTCYx(0);
	Delay10KTCYx(0);

	WriteReg7221(0,1,0);
	WriteReg7221(0,2,0);
	WriteReg7221(0,3,0);
	WriteReg7221(0,4,0);
	WriteReg7221(0,5,0);
	WriteReg7221(0,6,0);
	WriteReg7221(0,7,0);
	WriteReg7221(1,1,0);
	WriteReg7221(1,2,0);
	WriteReg7221(1,3,0);
	WriteReg7221(1,4,0);
	WriteReg7221(1,5,0);
	WriteReg7221(1,6,0);
}