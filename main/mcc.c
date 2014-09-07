#include <delays.h>
#include "mcc.h"
#include "evc402.h"
#include "UARTIntC.h"

void NGMPut(unsigned char data)
{
	char chData;
	
	if(!vUARTIntStatus.UARTIntTxBufferFull)
	{	
		UARTIntPutChar(data);
	
		Delay100TCYx(50);
	
		if( !(vUARTIntStatus.UARTIntRxError) && 
			    !(vUARTIntStatus.UARTIntRxOverFlow) &&
			    !(vUARTIntStatus.UARTIntRxBufferEmpty))
			UARTIntGetChar(&chData);
	}
}

int NGMQuery(unsigned int cReg, unsigned char cBit)
{
	char chData;
	int result = 0;
	unsigned char negative = 0;

	//Make sure the motor controller is ready to receive.
	//Also clear buffer of any error code.
	NGMPut('\r');
	NGMPut('\n');
	Delay100TCYx(50);
	while( !(vUARTIntStatus.UARTIntRxError) && 
		    !(vUARTIntStatus.UARTIntRxOverFlow) &&
		    !(vUARTIntStatus.UARTIntRxBufferEmpty))
	{
		UARTIntGetChar(&chData);
		Delay100TCYx(50);
	}

	//Start with a hexadecimal representation of a register
	// as stored in the EVC402 comms library, such as
	// AM_motorT, 0x105.  Separate and send each digit
	// of the register.
	if(cReg/0x100 > 9)
		NGMPut((cReg/0x100) + '7');
	else
		NGMPut((cReg/0x100) + '0');
	cReg = cReg - ((cReg/0x100)*0x100);
	if(cReg/0x10 > 9)
		NGMPut((cReg/0x10) + '7');
	else
		NGMPut((cReg/0x10) + '0');
	cReg = cReg - ((cReg/0x10)*0x10);
	if(cReg > 9)
		NGMPut(cReg + '7');
	else
		NGMPut(cReg + '0');

	//If the given bit is not between 0 and 15, the user has
	// requested all bits from the given register.  If the
	// given bit is within that range, relay that bit to the
	// controller.
	// The syntax is to follow the register with a dot, then
	// the bit number (0-15), such as 339.3
	if(cBit < 16)
	{
		NGMPut('.');
		if(cBit > 9)
			NGMPut(cBit + '7');
		else
			NGMPut(cBit + '0');
	}

	//NGM queries can end either with a '>' (requesting a
	// hexadecimal reply) or a '?' (requesting a decimal
	// reply), and a carriage return.
	NGMPut('?');
	NGMPut('\r');
	NGMPut('\n');

	//At some point, the NGM should return a (decimal) value.
	Delay100TCYx(50);
	while( !(vUARTIntStatus.UARTIntRxError) && 
		    !(vUARTIntStatus.UARTIntRxOverFlow) &&
		    !(vUARTIntStatus.UARTIntRxBufferEmpty))
	{
		chData = 0;
		UARTIntGetChar(&chData);

		if(chData >= '0' && chData <= '9')
			result = (result * 10) + (chData - '0');
		else if(chData == '-')
			negative = 1;
		Delay100TCYx(50);
	}
	if(negative == 1)
		result = -result;
	return result;
}

int NGMGetRPM(void)
{
	return NGMQuery(AM_velocity,0xFF);
}

unsigned char NGMGetMPH(void)
{
	int rpm = NGMGetRPM();
	if(rpm >= 0)
		return ((unsigned char)(RPM2MPH*rpm));
	else
		return ((unsigned char)(RPM2MPH*-rpm));
}

void NGMCommand(unsigned int command)
{
	char chData;
//	unsigned char cmdResponse;

	//Make sure the motor controller is ready to receive.
	//Also clear buffer of any error code.
	NGMPut('\r');
	NGMPut('\n');
	Delay100TCYx(50);
	while( !(vUARTIntStatus.UARTIntRxError) && 
		    !(vUARTIntStatus.UARTIntRxOverFlow) &&
		    !(vUARTIntStatus.UARTIntRxBufferEmpty))
	{
		UARTIntGetChar(&chData);
		Delay100TCYx(50);
	}
	//Start with a hexadecimal representation of a register
	// as stored in the EVC402 comms library, such as
	// AM_motorT, 0x105.  Separate and send each digit
	// of the register.
	if(command/0x100 > 9)
		NGMPut((command/0x100) + '7');
	else
		NGMPut((command/0x100) + '0');
	command -=((command/0x100)*0x100);
	if(command/0x10 > 9)
		NGMPut((command/0x10) + '7');
	else
		NGMPut((command/0x10) + '0');
	command -= ((command/0x10)*0x10);
	if(command > 9)
		NGMPut(command + '7');
	else
		NGMPut(command + '0');

	//NGM commands end with a ! and a carriage return.
	NGMPut('!');
	NGMPut('\r');
	NGMPut('\n');

	//The NGM will return a three-digit response, 
	// '#' and two numbers indicating any fault with the command.
	Delay100TCYx(50);
	while( !(vUARTIntStatus.UARTIntRxError) && 
		    !(vUARTIntStatus.UARTIntRxOverFlow) &&
		    !(vUARTIntStatus.UARTIntRxBufferEmpty))
	{
		chData = 0;
		UARTIntGetChar(&chData);

//		if(chData >= '0' && chData <= '9')
//			cmdResponse = (cmdResponse * 10) + (chData - '0');
		Delay100TCYx(50);
	}
//	return (unsigned char)(cmdResponse%0x10);
}

void NGMWriteReg(unsigned int cReg, unsigned char cBit, int cData)
{
	char chData;
//	unsigned char writeResponse;

	//Make sure the motor controller is ready to receive.
	//Also clear buffer of any error code.
	NGMPut('\r');
	NGMPut('\n');
	Delay100TCYx(50);
	while( !(vUARTIntStatus.UARTIntRxError) && 
		    !(vUARTIntStatus.UARTIntRxOverFlow) &&
		    !(vUARTIntStatus.UARTIntRxBufferEmpty))
	{
		UARTIntGetChar(&chData);
		Delay100TCYx(50);
	}

	//Start with a hexadecimal representation of a register
	// as stored in the EVC402 comms library, such as
	// AM_motorT, 0x105.  Separate and send each digit
	// of the register.
	if(cReg/0x100 > 9)
		NGMPut((cReg/0x100) + '7');
	else
		NGMPut((cReg/0x100) + '0');
	cReg = cReg - ((cReg/0x100)*0x100);
	if(cReg/0x10 > 9)
		NGMPut((cReg/0x10) + '7');
	else
		NGMPut((cReg/0x10) + '0');
	cReg = cReg - ((cReg/0x10)*0x10);
	if(cReg > 9)
		NGMPut(cReg + '7');
	else
		NGMPut(cReg + '0');

	//If the given bit is not between 0 and 15, the user has
	// requested all bits from the given register.  If the
	// given bit is within that range, relay that bit to the
	// controller.
	// The syntax is to follow the register with a dot, then
	// the bit number (0-15), such as 339.3
	if(cBit < 16)
	{
		NGMPut('.');
		if(cBit > 0x09)
			NGMPut(cBit + '7');
		else
			NGMPut(cBit + '0');
	}

	//Assignment statements have a '=', then send the desired value.
	NGMPut('=');
	
	if(cData < 0)
		NGMPut('-');

	if(cData/10000)
	{
		NGMPut((cData/10000) + '0');
		NGMPut(((cData%10000)/1000) + '0');
		NGMPut(((cData%1000)/100) + '0');
		NGMPut(((cData%100)/10) + '0');
		NGMPut((cData%10) + '0');
	}
	else if(cData/1000)
	{
		NGMPut((cData/1000) + '0');
		NGMPut(((cData%1000)/100) + '0');
		NGMPut(((cData%100)/10) + '0');
		NGMPut((cData%10) + '0');
	}
	else if(cData/100)
	{
		NGMPut((cData/100) + '0');
		NGMPut(((cData%100)/10) + '0');
		NGMPut((cData%10) + '0');
	}
	else if(cData/10)
	{
		NGMPut((cData/10) + '0');
		NGMPut((cData%10) + '0');
	}
	else
	{
		NGMPut(cData + '0');
	}
	
	NGMPut('\r');
	NGMPut('\n');

	Delay100TCYx(50);

//	writeResponse = 0;
	chData = 0;
	//The NGM will return a three-digit response, 
	// '#' and two numbers indicating any fault with the command.
	Delay100TCYx(50);
	while( !(vUARTIntStatus.UARTIntRxError) && 
		    !(vUARTIntStatus.UARTIntRxOverFlow) &&
		    !(vUARTIntStatus.UARTIntRxBufferEmpty))
	{
		chData = 0;
		UARTIntGetChar(&chData);

//		if(chData >= '0' && chData <= '9')
//			writeResponse = (writeResponse * 10) + (chData - '0');
		Delay100TCYx(50);
	}
//	return (unsigned char)(writeResponse%0x10);
}

unsigned int NGMGetRegen(void)
{
	return NGMQuery(AM_rgn,0xFF);
}