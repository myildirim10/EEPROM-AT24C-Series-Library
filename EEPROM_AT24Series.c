/**
  ******************************************************************************
  * @file           : EEPROM_AT24Series.c
  * @brief          : EEPROM AT24 Series EEPROM (AT24CXX) Library
  *                   This library based on I2C Communication
  ******************************************************************************
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "EEPROM_AT24Series.h"
#include "EEPROM_AT24Series_Hardware.h"
#include <stdint.h>
#include <string.h>

/* Defines -------------------------------------------------------------------*/
/**
 *  EEPROM INSTRUCTION SET
 */
#define EEPROM_PAGE_WRITE_WAIT              ( 5 )       // 5 Milisecond
#define EEPROM_BYTE_WRITE_WAIT              ( 1 )       // 1 Milisecond


/* Enum definitions ----------------------------------------------------------*/

/* Structure definitions -----------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static volatile uint32_t g_timerCounter1MS;


/* Private function prototypes -----------------------------------------------*/
static uint8_t ucTimeout ( uint32_t ulMilisecond );


/* Functions ----------------------------------------------------------------*/
/**
  * @brief  Initialize EEPROM
  * @retval Status
  */
EEPROM_Status_t xEEPROM_Init ( void )
{
	return EEPROM_STATUS_SUCCESS;
}


/**
 * @brief EEPROM Timer Process
 *        This function must be in 1ms Timer
 */
void vEEPROM_TimerProcess ( void )
{
    g_timerCounter1MS++;
}


/**
  * @brief  Read byte given EEPROM address
  * @param  usAddress: Address
  */
uint8_t ucEEPROM_ReadByte ( uint16_t usAddress )
{
    uint8_t ucDummyData[2];
    uint8_t ucReceivedData;

    ucDummyData[0] = usAddress >> 8;
    ucDummyData[1] = usAddress & 0xFF;

    vI2C_Transmit(ucDummyData, 2);
	vI2C_Receive(&ucReceivedData, 1);
    vI2C_StopCondition();

	return ucReceivedData;
}


/**
  * @brief  Read Sequential from EEPROM address
  * @param  pucRxData: Address of Data
  * @param  usRxDataNumber: Number of Data
  */
void vEEPROM_Read ( uint16_t usAddress, uint8_t *pucRxData, uint16_t usRxDataNumber )
{
    uint8_t ucAddress[2];

    ucAddress[0] = usAddress >> 8;
    ucAddress[1] = usAddress & 0xFF;

    vI2C_Transmit(ucAddress, 2);
    vI2C_Receive(pucRxData, usRxDataNumber);
    vI2C_StopCondition();

}


/**
  * @brief  Write a byte to EEPROM address
  * @param  usAddress: Address
  * @param  ucValue: Value
  */
void vEEPROM_WriteByte ( uint16_t usAddress, uint8_t ucValue )
{
    uint8_t ucData[3];

    ucData[0] = usAddress >> 8;
    ucData[1] = usAddress & 0xFF;
    ucData[2] = ucValue;

    vI2C_Transmit(ucData, 3);
    vI2C_StopCondition();
    while ( ucTimeout(EEPROM_BYTE_WRITE_WAIT) != 0 ) {}
}


/**
  * @brief  Write to EEPROM address (AT24cxx; page write operation allows up to "EEPROM_PAGE_SIZE" bytes to be written in the same write cycle)
  * @param  usAddress: Address
  * @param  pucWriteData: Address Data
  * @param  usDataSize: Number of Data
  * @return Success or Failure
  */
EEPROM_Status_t vEEPROM_Write ( uint16_t usAddress, const uint8_t *pucWriteData, uint16_t usDataSize )
{
    EEPROM_Status_t status = EEPROM_STATUS_ERROR;

    static uint8_t ucData[EEPROM_PAGE_SIZE+2];
    uint16_t usDataToWriteSize = usDataSize;
    uint16_t usDataWrittenSize;
    uint16_t usPageOffset;
    uint16_t usTempAddress = usAddress;
    uint16_t usPacketIndex = 0;

    if ( usTempAddress + usDataToWriteSize >= EEPROM_TOTAL_SIZE )
        return status;

    while ( usDataToWriteSize > 0 )
    {
        usPageOffset = EEPROM_PAGE_SIZE - (usTempAddress % EEPROM_PAGE_SIZE);
        usDataWrittenSize = usDataToWriteSize > usPageOffset ? usPageOffset : usDataToWriteSize;
        ucData[0] = usTempAddress >> 8;
        ucData[1] = usTempAddress & 0xFF;
        memcpy(&ucData[2], &pucWriteData[usPacketIndex], usDataWrittenSize);

        vI2C_Transmit(ucData, usDataWrittenSize+2);
        vI2C_StopCondition();

        while ( ucTimeout(EEPROM_PAGE_WRITE_WAIT) != 0 ) {}

        usDataToWriteSize = usDataToWriteSize - usDataWrittenSize;
        usPacketIndex = usPacketIndex + usDataWrittenSize;
        usTempAddress = usTempAddress + usDataWrittenSize;
    }

    status = EEPROM_STATUS_SUCCESS;

    return status;
}


/**
  * @brief  Update byte to EEPROM address
  * @param  usAddress: Address
  * @param  usValue: Value
  */
void vEEPROM_UpdateByte ( uint16_t usAddress, uint8_t ucValue )
{
	if ( ucEEPROM_ReadByte(usAddress) != ucValue )
	{
		vEEPROM_WriteByte(usAddress, ucValue);
	}
}


/**
  * @brief  Timeout function for page write
  * @retval 0: Success (Exceeded timeout)
  *         1: Fail (Timeout continues)
  */
static uint8_t ucTimeout ( uint32_t ulMilisecond )
{
    static volatile uint32_t ulTimerCounter1MS;
    static volatile uint32_t ulDifferenceTime;
    static volatile uint8_t ucFlagFirstTime;

    if (!ucFlagFirstTime)
    {
        ulTimerCounter1MS = g_timerCounter1MS;
        ucFlagFirstTime = 1;
    }

    if ( (g_timerCounter1MS - ulTimerCounter1MS) <= ulMilisecond )
    {
        return 1;
    }

    ucFlagFirstTime = 0;

    return 0;
}
