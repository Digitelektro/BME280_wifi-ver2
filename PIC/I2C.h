//I2C.h


void I2CWait ();
void initI2C();
void I2CStart(unsigned char Address);
void I2CReStart(unsigned char Address);
void I2CSend(unsigned char Data);
void I2CStop (void);

void I2CRead(unsigned char Address, unsigned char Register, void *Data, unsigned int len);
void I2CWriteBlock(unsigned char Address, unsigned char Register, void *Data, unsigned int len);
void I2CWrite(unsigned char Address, unsigned char Register, unsigned char Data);