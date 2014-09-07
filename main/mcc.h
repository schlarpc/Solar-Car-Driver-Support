#ifndef MCC_H
#define MCC_H

//Dia. of wheel (inches) * pi * 60 / 12 / 5280
#define RPM2MPH			0.05528
//Because calculations take time and memory is cheap, ~1/RPM2MPH
#define MPH2RPM			18

void NGMPut(unsigned char data);
int NGMQuery(unsigned int cReg, unsigned char cBit);
int NGMGetRPM(void);
unsigned char NGMGetMPH(void);
void NGMCommand(unsigned int command);
void NGMWriteReg(unsigned int cReg, unsigned char cBit, int cData);
unsigned int NGMGetRegen(void);
#endif