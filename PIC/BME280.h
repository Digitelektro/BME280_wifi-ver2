/* 
 * File:   BME280.h
 * Author: Viktor
 *
 * Created on 27 July 2016, 21:43
 */

#ifndef BME280_H
#define	BME280_H

#ifdef	__cplusplus
extern "C" {
#endif

    
#define uint16_t unsigned int
#define int16_t int
#define uint8_t unsigned char
#define uint32_t unsigned long    


#define BME280_ADDRESS					0xEC

#define BME280_REGISTER_DIG_T1 			0x88
#define BME280_REGISTER_DIG_T2 			0x8A
#define BME280_REGISTER_DIG_T3 			0x8C

#define BME280_REGISTER_DIG_P1 			0x8E
#define BME280_REGISTER_DIG_P2 			0x90
#define BME280_REGISTER_DIG_P3 			0x92
#define BME280_REGISTER_DIG_P4 			0x94
#define BME280_REGISTER_DIG_P5 			0x96
#define BME280_REGISTER_DIG_P6 			0x98
#define BME280_REGISTER_DIG_P7 			0x9A
#define BME280_REGISTER_DIG_P8 			0x9C
#define BME280_REGISTER_DIG_P9 			0x9E

#define BME280_REGISTER_DIG_H1 			0xA1
#define BME280_REGISTER_DIG_H2 			0xE1
#define BME280_REGISTER_DIG_H3 			0xE3
#define BME280_REGISTER_DIG_H4 			0xE4
#define BME280_REGISTER_DIG_H5 			0xE5
#define BME280_REGISTER_DIG_H6 			0xE7

#define BME280_REGISTER_CHIPID 			0xD0
#define BME280_REGISTER_VERSION 		0xD1
#define BME280_REGISTER_SOFTRESET 		0xE0

#define BME280_REGISTER_CAL26 			0xE1  

#define BME280_REGISTER_CONTROLHUMID	0xF2
#define BME280_REGISTER_STATUS			0XF3
#define BME280_REGISTER_CONTROL			0xF4
#define BME280_REGISTER_CONFIG			0xF5
#define BME280_REGISTER_PRESSUREDATA	0xF7
#define BME280_REGISTER_TEMPDATA 		0xFA
#define BME280_REGISTER_HUMIDDATA 		0xFD   
    
extern float Temperature;
extern float Pressure;
extern float Humidity;
    
    
void BMP280_Init();
void BMP280_Read_ID();
void BMP280_Read_AllData();
void BMP280_StartForcedMode();
char BMP280_IsMeasuring();





typedef struct _compParams_ts {
  uint16_t dig_T1;
  int16_t  dig_T2;
  int16_t  dig_T3;

  uint16_t dig_P1;
  int16_t  dig_P2;
  int16_t  dig_P3;
  int16_t  dig_P4;
  int16_t  dig_P5;  
  int16_t  dig_P6;  
  int16_t  dig_P7; 
  int16_t  dig_P8;
  int16_t  dig_P9;

  uint8_t  dig_H1;
  int16_t  dig_H2;
  uint8_t  dig_H3;
  int16_t  dig_H4;
  int16_t  dig_H5;
  uint8_t  dig_H6;
  
} compParams_ts; 


typedef struct
{
  uint16_t dig_T1;
  int16_t  dig_T2;
  int16_t  dig_T3;
} TempCompParams;

typedef struct
{
  uint16_t dig_P1;
  int16_t  dig_P2;
  int16_t  dig_P3;
  int16_t  dig_P4;
  int16_t  dig_P5;  
  int16_t  dig_P6;  
  int16_t  dig_P7; 
  int16_t  dig_P8;
  int16_t  dig_P9;
} PressCompParams;

typedef struct
{
    uint8_t  dig_H1;
    int16_t  dig_H2;
    uint8_t  dig_H3;
    int16_t  dig_H4;
    int16_t  dig_H5;
    uint8_t  dig_H6;
} HumCompParams;


#ifdef	__cplusplus
}
#endif

#endif	/* BME280_H */

