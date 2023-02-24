/**
  ******************************************************************************
  * @file           : EEPROM_AT24Series.h
  *                   This library is made for I2C Communication with EEPROM
  *                   AT24Cxx Series
  * @author         : Muhammed YILDIRIM
  * @version        : 1.0
  * @date           : January 2023
  ******************************************************************************
  * 
  ******************************************************************************
  * @HowToUse
  * 1.  EEPROMTimerProcess Function must be in 1ms Timer Interrupt
  */

#ifndef __EEPROM_AT24Series_H_
#define __EEPROM_AT24Series_H_


/* Includes ------------------------------------------------------------------*/
#include "stdint.h"


/* Defines -------------------------------------------------------------------*/
#define EEPROM_PAGE_SIZE					        ( 32 )
#define EEPROM_TOTAL_SIZE					        ( 0x1000 )			// for 32-Kbit ( 4 Kbyte ) EEPROMs


/* Enum definitions ----------------------------------------------------------*/
typedef enum
{
	EEPROM_STATUS_SUCCESS,
	EEPROM_STATUS_ERROR

} EEPROM_Status_t;


/* Structure definitions -----------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
/* API for Settings EEPROM */
EEPROM_Status_t xEEPROM_Init ( void );
void vEEPROM_TimerProcess ( void );

/* API for Read EEPROM */
uint8_t ucEEPROM_ReadByte ( uint16_t usAddress );
void vEEPROM_Read ( uint16_t usAddress, uint8_t *pucRxData, uint16_t usRxDataNumber );

/* API for Write EEPROM */
void vEEPROM_WriteByte ( uint16_t usAddress, uint8_t ucValue );
void vEEPROM_UpdateByte ( uint16_t usAddress, uint8_t ucValue );
EEPROM_Status_t vEEPROM_Write ( uint16_t usAddress, const uint8_t *pucWriteData, uint16_t usDataSize );


#endif /* __EEPROM_AT24Series_H_ */
