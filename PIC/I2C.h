//I2C.h


void I2CInit();

void I2CRead(unsigned char Address, unsigned char Register, void *Data, unsigned int len);
void I2CWriteBlock(unsigned char Address, unsigned char Register, void *Data, unsigned int len);
void I2CWrite(unsigned char Address, unsigned char Register, unsigned char Data);
