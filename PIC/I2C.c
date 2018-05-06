#include"p24Exxxx.h"
#include"I2C.h"




void I2CWait ()
{
	while (I2C1CONbits.SEN ==1)
	{
		Nop ();
	}
	while (I2C1CONbits.RSEN ==1)
	{
		Nop ();
	}
	while (I2C1CONbits.PEN ==1)
	{
		Nop ();
	}
	while (I2C1CONbits.ACKEN ==1)
	{
		Nop ();
	}
	while (I2C1CONbits.RCEN ==1)
	{
		Nop ();
	}
	while (I2C1STATbits.TRSTAT ==1)
	{
		Nop ();
	}
	if(I2C1STATbits.BCL == 1)
	{
		//Error handling
	}
	
}

void I2CInit()	// I2C1 master mode enable
{
	I2C1RCV = 0;
	I2C1TRN = 0;
	I2C1STAT = 0;
	I2C1CON = 0b1000001000000000;
	I2C1BRG = 400;
	char dummy = I2C1RCV;
}

void I2CStart(unsigned char Address)
{
	I2C1CONbits.SEN = 1;	 
	I2CWait();

	I2C1TRN = Address;
	I2CWait();
}

void I2CReStart(unsigned char Address)
{
	I2C1CONbits.RSEN = 1;			 
	I2CWait();
	I2C1TRN = Address;
	I2CWait();
}

void I2CSend(unsigned char Data)
{
	I2C1TRN = Data;				
	I2CWait();
}


void I2CStop (void)
{
	I2C1CONbits.PEN = 1;			
	I2CWait();
}


void I2CRead(unsigned char Address, unsigned char Register, void *Data, unsigned int len)
{
	char *dest = (char *)Data;
	I2CStart(Address); 	
	I2CSend(Register);
	I2CReStart(Address | 0b00000001);

	int count;
	for (count = 0; count < len-1; count++)
	{
	   I2C1CONbits.RCEN = 1;
	   I2CWait();
	   dest[count]= I2C1RCV;
	   I2C1CONbits.ACKDT = 0;	//ACK
	   I2C1CONbits.ACKEN = 1;
	   I2CWait();
	}

	I2C1CONbits.RCEN = 1;
	I2CWait();
	dest[len-1]= I2C1RCV;
	I2C1CONbits.ACKDT = 1;	//NACK
	I2C1CONbits.ACKEN = 1;
	I2CWait();
	I2C1CONbits.ACKDT = 0;
	I2CStop();
}

void I2CWriteBlock(unsigned char Address, unsigned char Register, void *Data, unsigned int len)
{
	char *data = (char *)Data;
	I2CStart(Address);
	I2CSend(Register);
	int count;
	for (count = 0; count < len; count++)
	{
		I2CSend(data[count]);
	} 	
	I2CStop();    
}

void I2CWrite(unsigned char Address, unsigned char Register, unsigned char Data)
{
	I2CStart(Address); 	
    I2CSend(Register);
    I2CSend(Data);
    I2CStop();    
}

