/**
  ******************************************************************************
  * @file           : EEPROM_24Series_Hardware.h
  *                   This library is made for I2C Communication with EEPROM
  *                   AT24Cxx Series
  * @author         : Muhammed YILDIRIM
  * @version        : 1.0
  * @date           : January 2023
  ******************************************************************************
  *
  ******************************************************************************
  */

#ifndef __EEPROM_24_SERIES_HARDWARE__H_
#define __EEPROM_24_SERIES_HARDWARE__H_

#define RENESAS

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#ifdef RENESAS
#include "Config_SCI5.h"
#endif

/* Defines -------------------------------------------------------------------*/
#define SLAVE_ADDRESS                                                (0x50)


/* Enum definitions ----------------------------------------------------------*/

/* Structure definitions -----------------------------------------------------*/

/* External variables --------------------------------------------------------*/
#ifdef RENESAS
extern volatile uint8_t g_ucFlagI2CTransmitEnd;
extern volatile uint8_t g_ucFlagI2CReceiveEnd;
#endif


/* Function prototypes -------------------------------------------------------*/
/**
  * @brief  I2C Transmit function
  * @param  ucSlaveAddress: Slave Address
  * @param  pucTxData: Address of Data
  * @param  usTxDataNumber: Data number
  */
static inline void vI2C_Transmit ( const uint8_t *pucTxData, uint16_t usTxDataNumber )
{

	// I2C Hardware Transmit Function
#ifdef RENESAS
    R_Config_SCI5_IIC_Master_Send(SLAVE_ADDRESS << 1, (uint8_t * const)pucTxData, usTxDataNumber);
    while ( g_ucFlagI2CTransmitEnd == 0 ) {}
    g_ucFlagI2CTransmitEnd = 0;
#endif

}


/**
  * @brief  I2C Receive function
  * @param  ucSlaveAddress: Slave address
  * @param  pucRxData: Address of Data
  * @param  usTxDataNumber: Data number
  */
static inline void vI2C_Receive ( uint8_t *pucRxData, uint16_t usRxDataNumber )
{

    // I2C Hardware Receive Function
#ifdef RENESAS
    const uint8_t READ_COMMAND = 0x01;
    R_Config_SCI5_IIC_Master_Receive((SLAVE_ADDRESS << 1) | READ_COMMAND, pucRxData, usRxDataNumber);
    while ( g_ucFlagI2CReceiveEnd == 0 ) {}
    g_ucFlagI2CReceiveEnd = 0;
#endif

}


/**
  * @brief  I2C Start Condition
  */
static inline void vI2C_StartCondition ( void )
{
    // I2C Hardware Start Condition Function
}

/**
  * @brief  I2C Stop Condition
  */
static inline void vI2C_StopCondition ( void )
{
    // I2C Hardware Stop Condition Function
}

#endif /* __EEPROM_24_SERIES_HARDWARE__H_ */
