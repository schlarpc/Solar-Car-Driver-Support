#ifndef MAX7221SPI_H
#define MAX7221SPI_H

/*
 *	Filename: max7221spi.h
 *	Current Rev: 2.0
 *	ShortDesc: PIC18 MAX7221 SPI Communication Routines Header
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

//###### Define SPI/Chip Select pins on PIC & No. of 7221s in use ######
//################ MODIFY THESE FOR THE HARDWARE IN USE ################

#define CS			PORTBbits.RB0
#define CSpin		TRISBbits.TRISB0
#define SCKpin		TRISCbits.TRISC3
#define SDIpin		TRISCbits.TRISC4
#define SDOpin		TRISCbits.TRISC5
#define NUM_DEVICES	2

//######################################################################

//This can be left at 8, or set lower to disable scanning of
// unused digits (doesn't affect processing time).
#define NUM_DIGITS 8

//Define common cathode 7-segment display font for
// standard Arabic numerals.
#define FONT21ZERO	0b01111110
#define FONT21ONE	0b00110000
#define FONT21TWO	0b01101101
#define FONT21THREE	0b01111001
#define FONT21FOUR	0b00110011
#define FONT21FIVE	0b01011011
#define FONT21SIX	0b01011111
#define FONT21SEVEN	0b01110000
#define FONT21EIGHT	0b01111111
#define FONT21NINE	0b01111011
#define FONT21DIG_A	0b01110111
#define FONT21DIG_B	0b00011111
#define FONT21DIG_C	0b01001110
#define FONT21DIG_D	0b00111101
#define FONT21DIG_E	0b01001111
#define FONT21DIG_F	0b01000111

/*
 *	Function: Init7221SPI
 *	Description: Initializes the PIC SPI bus, then initializes a given number of
 *					MAX7221 display drivers in cascade.
 *
 *	Parameters:		Param		Type				Desc
 *					cNum		unsigned char		Number of 7221's in cascade
 */
void Init7221SPI(unsigned char cNum);

/*
 *	Function: WriteReg7221
 *	Description: Writes data to a given register on a given chip.
 *
 *	Parameters:		Param		Type				Desc
 *					cSel		unsigned char		Chip position in cascade (0 to n)
 *					cReg		unsigned char		Register to set.  Digit registers are 1-8!
 *					cData		unsigned char		Data to write
 */
void WriteReg7221(unsigned char cSel, unsigned char cReg, unsigned char cData);

/*
 *	Function: SetIntensity7221
 *	Description: Metafunction for WriteReg7221.  Sets display brightness.
 *
 *	Parameters:		Param		Type				Desc
 *					cSel		unsigned char		Chip position in cascade (0 to n)
 *					cValue		unsigned char		Brightness value (0x0 to 0xF)
 */
void SetIntensity7221(unsigned char cSel, unsigned char cValue);

/*
 *	Function: WriteDigit7221
 *	Description: Writes an Arabic numeral to a given display.  Uses previously-defined font.
 *					Quite useful for displaying numeric data, since there is otherwise no
 *					correlation between 7221 register data and displayed character.
 *
 *	Parameters:		Param		Type				Desc
 *					cSel		unsigned char		Chip position in cascade (0 to n)
 *					cDigit		unsigned char		Digit on chip (1 to 8)
 *					cData		unsigned char		Numeral to display (0x0 to 0xF)
 *					decPt		unsigned char		Should there be a decimal point on this digit? (0 or n)
 */
void WriteDigit7221(unsigned char cSel, unsigned char cDigit, unsigned char cData, unsigned char decPt);

/*
 *	Function: WriteInt7221
 *	Description: Autoformats an int for a given number of displays.  Can handle signed ints.
 *					Writes leading zeroes for numbers with less than the no. of avail. digits,
 *					writes decimal points for overflowed numbers.
 *					Will only work when digit range is across a single chip!
 *
 *	Parameters:		Param		Type				Desc
 *					cSel		unsigned char		Chip position in cascade (0 to n)
 *					cRangeStart	unsigned char		First digit to utilize (0 to 7)
 *					cRangeEnd	unsigned char		Last digit to utilize (0 to 7)
 *					iData		int					Signed data to display.
 */
void WriteInt7221(unsigned char cSel, unsigned char cRangeStart, unsigned char cRangeEnd, int iData);

/*
 *	Function: SetBarGraphL
 *	Description: Sets bar graph of P9/P10 Dash to a given value.  Left justified.
 *
 *	Parameters:		Param		Type				Desc
 *					cVal		unsigned char		Number of bars to display (0 to 24)
 */
void SetBarGraphL(unsigned char cVal);

/*
 *	Function: SetBarGraphL
 *	Description: Sets bar graph of P9/P10 Dash to a given value.  Right justified.
 *
 *	Parameters:		Param		Type				Desc
 *					cVal		unsigned char		Number of bars to display (0 to 24)
 */
void SetBarGraphR(unsigned char cVal);

/*
 *	Function: DisplayTest7221
 *	Description: Wastes program memory and power, if you happen to have a surplus of either.
 */
void DisplayTest7221(void);
#endif