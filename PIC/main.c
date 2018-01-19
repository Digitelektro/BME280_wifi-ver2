#include <p24Exxxx.h>
#include "RN171.h"
#include "BME280.h"
#include "I2C.h"
#include "base64.h"
#include <stdlib.h> 
#include <string.h>
#include <stdio.h>

#define BLUETOOTH_TX LATBbits.LATB4	//To Bluetooth RX 
#define STATUS_LED LATBbits.LATB14

#define FCY 60000000LL            //Define clock
#include <libpic30.h>            //órajel függvénye, a define FCY alatt kell lennie, különben nem muködik

_FPOR(ALTI2C1_ON)	//I2C alternativ pin configuration  :RB8, RB9  
_FGS(GWRP_OFF & GCP_OFF)
_FICD(ICS_PGD1 & JTAGEN_OFF)
_FWDT(FWDTEN_ON & WDTPOST_PS1024 & WDTPRE_PR32 & WINDIS_OFF)		//Watchdog 
_FOSCSEL(FNOSC_FRC & IESO_OFF);			// Select Internal FRC at POR
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF  & POSCMD_NONE);  
								// Clock switching is enabled,Fail-safe Clock Monitor is disabled
								// OSC2 Pin Function: OSC2 is Clock Output
								// Primary Oscillator Mode: NONE


void SleepSec(unsigned int Sec);


unsigned char StartConversion = 0;
unsigned char TaskFinished = 0;

 void init()
{
	CLKDIV = 0;
	// Configure PLL prescaler, PLL postscaler, PLL divisor	
	CLKDIVbits.PLLPOST=0b00; // N2=2
	CLKDIVbits.PLLPRE=0b00; // N1=2
	CLKDIVbits.DOZEN = 0;
	PLLFBD=64; // M=65

	// Initiate Clock Switch to FRC oscillator with PLL (NOSC=0b001)
    __builtin_write_OSCCONH(0x01);
    __builtin_write_OSCCONL(OSCCON | 0x01);
    // Wait for Clock switch to occur
    while (OSCCONbits.COSC!= 0b001);
    // Wait for PLL to lock
    while (OSCCONbits.LOCK!= 1);

   	ANSELA = 0;                //Disable analog pin
	ANSELB = 0;
    TRISB = 0;					//All port output
    TRISA = 0;
    
    //Timer init
    T1CONbits.TON = 0; // Disable Timer
    T1CONbits.TCS = 0; // Select internal instruction cycle clock
    T1CONbits.TGATE = 0; // Disable Gated Timer mode
    T1CONbits.TCKPS = 0b11; // Select 1:256 Prescaler
    TMR1 = 0x00; // Clear timer register
    PR1 = 23391; // Load the period value
    IPC0bits.T1IP = 0x01; // Set Timer 1 Interrupt Priority Level
    IFS0bits.T1IF = 0; // Clear Timer 1 Interrupt Flag
    IEC0bits.T1IE = 1; // Enable Timer1 interrupt
    T1CONbits.TON = 1; // Start Timer

	

	UARTInit();
	initI2C();
}

char SensorData[255] = "/0";
char* EncodedSensorData;
int EncodedLen;
char status;


int main()
{
    init();
    STATUS_LED = 1;
    __delay_ms(500);
    STATUS_LED = 0;
    BMP280_Init();

    while(1)
    {
		status = RN171_Join();
		if(status == RN171_OK)
		{
			STATUS_LED = 1;
			__delay_ms(500);
			STATUS_LED = 0;
            StartConversion = 0;
            //Temp/Hum/Press
            BMP280_StartForcedMode();
            BMP280_StartForcedMode();
            while(BMP280_IsMeasuring() == 0)
            {
                __delay_ms(10);  
            }
            BMP280_Read_AllData();
            sprintf(SensorData,"{\"temperature\": %f ,\"pressure\": %f, \"humidity\" : %f, \"location\" : %d }", Temperature, Pressure, Humidity, 2);
            EncodedSensorData = base64_encode(SensorData, strlen(SensorData), &EncodedLen);
            Nop();
            RN171_SendData(EncodedSensorData, EncodedLen);
            free(EncodedSensorData);
			
			//RN171_SendData("asd\r\n", 3);
		}
		else
		{
			STATUS_LED = 1;
			__delay_ms(250);
			STATUS_LED = 0;
			__delay_ms(250);
			STATUS_LED = 1;
			__delay_ms(250);
			STATUS_LED = 0;
		}
		RN171_Sleep();
		TaskFinished = 1;
		SleepSec(60);
		
    }
    return 1;
}

void SleepSec(unsigned int Sec)
{
	static unsigned int SleepCount;
	SleepCount = 0;
	do
	{
		Sleep();
		RCONbits.SLEEP = 0;
		SleepCount++;
	}while(SleepCount < Sec);
}



void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)	
{
	IFS0bits.T1IF = 0;
	ClrWdt();
}

void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void)	
{
	RN171_RxData[RxDataCounter] = U1RXREG;
	if(RN171_RxData[RxDataCounter] == '\n')
	{
		RN171_CommandReceived = 1;
	}
	if(RN171_RxData[RxDataCounter] != 0)	//Drop zeros
		RxDataCounter++;
	IFS0bits.U1RXIF = 0;
}



//*********************************************************//



