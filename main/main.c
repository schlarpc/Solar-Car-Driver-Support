#include <delays.h>
#include "Can.h"
#include "UARTIntC.h"
#include "max7221spi.h"
#include "mcc.h"
#include "evc402.h"
#include "dash.h"
#include <stdarg.h>

#pragma config OSC=HSPLL,WDT=OFF,LVP=OFF

void HighIsr(void);
void LowIsr(void);

#pragma code HighIntVector = 0x08

void HighInt(void)
{
	_asm
		goto HighIsr
	_endasm
}

#pragma code LowIntVector = 0x18

void LowInt(void)
{
	_asm
		goto LowIsr
	_endasm
}

#pragma code

#pragma interrupt HighIsr

void HighIsr(void)
{
	CANISR();
}

#pragma interruptlow LowIsr

void LowIsr(void)
{
	UARTIntISR();
}

//only allow new button presses every this many loops
#define BUTTONPRESCALE		5

void ParseCANMessage(struct CANMessage RX_Message);
void SendCANPacketRaw(unsigned short addr, unsigned char args, ...);
void SetStatusLED(unsigned char led, unsigned char data);
void CruiseEnable(void);
void CruiseDisable(void);
void CruiseAccel(unsigned char numMPH);
void CruiseDecel(unsigned char numMPH);

struct CANMessage RX_Message;
struct CANMessage TX_Message;
//cStatusLED 0b76543210: unassigned PACK MOTOR CCTL D4 D3 D2 D1
unsigned char cStatusLED, cruiseState;
unsigned char buttonDelay[9], r;
int desiredSpeed;

void main(void)
{

	ADCON1 |= 0b00001111;
	ADCON1 &= 0b11001111;
	TRISA |=  0b00000111;
	TRISB = 0xDF;
	//TRISC = 0x07;

	LATB = 0;
	
	INTCON = 0;
	PIE1 = 0;
	PIE2 = 0;
	PIE3 = 0;

	for(r=0;r<9;r++)
		buttonDelay[r] = 0;

	cStatusLED = 0;
	cruiseState = CRUISEOFF;

	//Initialize CAN and serial interfaces
	CANInit();
	UARTIntInit();

	//Initialize display driver
	Init7221SPI(2);
	SetIntensity7221(1,0x04);

	DisplayTest7221();

	RCONbits.IPEN = 1;
	INTCONbits.GIEH = 1;
	INTCONbits.GIEL = 1;

	WriteInt7221(1,0,4,0);
	WriteInt7221(0,0,1,0);

	while(1)
	{
		WriteInt7221(0,0,1,NGMGetMPH());

		if(CANRXMessageIsPending())
		{
			RX_Message = CANGet();
			ParseCANMessage(RX_Message);
		}

		if(NGMGetRegen() > 30)
		{
			LATBbits.LATB5 = 1;
			CruiseDisable();
		}
		else
			LATBbits.LATB5 = 0;

		if(!PORTBbits.RB4)			//Brake switch
			CruiseDisable();

		if(!PORTAbits.RA0 && buttonDelay[0] == 0)	//Cruise On/Off
		{
			if(cruiseState == CRUISEOFF)
				CruiseEnable();
			else
				CruiseDisable();
		}
		else if(!PORTAbits.RA1 && buttonDelay[1] == 0)	//Accel
		{
			CruiseAccel(1);
		}

		else if(!PORTAbits.RA2 && buttonDelay[2] == 0)	//Coast
		{
			CruiseDecel(1);
		}

		for(r=0;r<9;r++)
			if(buttonDelay[r] > 0)
				buttonDelay[r]--;

		Delay10KTCYx(50);
		if(UARTIntGetRxBufferDataSize() > 0) //MC RX error
			UARTIntInit();
	}

}

void SendCANPacketRaw(unsigned short addr, unsigned char args, ...)
{
	unsigned char i = 0;
	unsigned char size; 

    va_list ap;

    va_start(ap, args);
	
	size = args;
	if(size > 8)
		size=8;
    while (i <= size)
    {
        TX_Message.Data[i++] = va_arg(ap, unsigned char);
    }
	
	TX_Message.Address		= addr;
	TX_Message.Remote		= 0;				//Clear remote flag to signal outgoing data
	TX_Message.Ext 			= 0;
	TX_Message.Priority 	= 0;
	TX_Message.NoOfBytes 	= size;

    va_end(ap);
    
	i=0;
	while (CANPut(TX_Message))					//Send CAN message
	{
	//	if(i++ == 1)
			break;

	//	Delay10KTCYx(20);
	}
	//Delay1KTCYx(20);
}

void ParseCANMessage(struct CANMessage RX_Message)
{
	//unsigned char data[8];

	switch(RX_Message.Address)
	{
		case 0x122:		//Receive BPAM pack data
			SetBarGraphR((unsigned char)(((((unsigned int)(RX_Message.Data[0]) << 8) | (unsigned int)(RX_Message.Data[1]))-PACKEMPTY)/(float)(PACKFULL-PACKEMPTY)*24));
			WriteInt7221(1,0,4,(int)(((unsigned int)(RX_Message.Data[2]) << 8) | (unsigned int)(RX_Message.Data[3])));
			SetStatusLED(PACKLED,(RX_Message.Data[6]>>1)%2);
			break;
		case 0x123:		//Receive BPAM motor relay data
		//	motorStatus = RX_Message.Data[6];
			SetStatusLED(MOTORLED,(RX_Message.Data[6]>>1)%2);
			break;
		case 0x22B:		//Reset motor controller
			NGMCommand(CMD_RESET);
			break;
	}
	
}

void SetStatusLED(unsigned char led, unsigned char data)
{
	if(data)
	{
		cStatusLED |= 1<<led;
	}
	else
	{
		cStatusLED &= ~(1<<led);
	}
	WriteReg7221(1,0x06,cStatusLED);
}

void CruiseDisable(void)
{
	SetStatusLED(CCLED,0);
	WriteReg7221(0,3,0);
	WriteReg7221(0,4,0);

	NGMCommand(CMD_DISCTORQUE);

	cruiseState = CRUISEOFF;

}

void CruiseEnable(void)
{
	
	desiredSpeed = NGMGetRPM();
	if(desiredSpeed > MINCCRPM && cruiseState == CRUISEOFF)
	{
		SetStatusLED(CCLED,1);
		NGMCommand(CMD_WRENABLE);
		NGMWriteReg(VC_SCI,0xFF,255);
		NGMWriteReg(SI_desiredspd,0xFF,desiredSpeed);
		NGMCommand(CMD_SERIALSPEED);
		NGMCommand(CMD_WRDISABLE);
		cruiseState = CRUISEON;
		WriteInt7221(0,2,3,(unsigned char)(desiredSpeed*RPM2MPH));
	}
	else
		CruiseDisable();
}

void CruiseAccel(unsigned char numMPH)
{
	if((desiredSpeed + (MPH2RPM*numMPH))<1650 && cruiseState == CRUISEON)
	{
		desiredSpeed += (MPH2RPM*numMPH);
		NGMWriteReg(SI_desiredspd,0xFF,desiredSpeed);
		WriteInt7221(0,2,3,(unsigned char)(desiredSpeed*RPM2MPH));
	}
}

void CruiseDecel(unsigned char numMPH)
{
	if((desiredSpeed - (MPH2RPM*numMPH))>MINCCRPM && cruiseState == CRUISEON)
	{
		desiredSpeed -= (MPH2RPM*numMPH);
		NGMWriteReg(SI_desiredspd,0xFF,desiredSpeed);
		WriteInt7221(0,2,3,(unsigned char)(desiredSpeed*RPM2MPH));
	}
	else
		CruiseDisable();
}