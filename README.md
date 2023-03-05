# AT24C Series EEPROM Library

#### Hardware dependency

- You must make the settings for the I2C peripheral.
- 1 millisecond Timer is required for the use of this library. This Timer is used for timeout and waiting time.
- This library was made for the AT24C32D. But according to the EEPROM model you will use, you should edit the parts specified with EEPROM_PAGE_SIZE and EEPROM_TOTAL_SIZE in the EEPROM_AT24Series.h file.

If you want to port to a different MCU, you can modify the "***EEPROM_24Series_Hardware.h***" file.



#### Example

The example made with the Renesas RX series processor in the library is below.

Example usage in a project is as follows.

```c
#include "EEPROM_AT24Series.h"

#define SIZE_TEST_ARRAY 		( 1024 )
#define START_ADDRESS			( 0 ) 

int main ()
{
    uint8_t rxData[SIZE_TEST_ARRAY];
    uint8_t txData[SIZE_TEST_ARRAY];
	
    for (int i = 0; i < SIZE_TEST_ARRAY; i++)
        txData[i] = i % 256;
	
    vEEPROM_Write(START_ADDRESS, txData, SIZE_TEST_ARRAY);
    vEEPROM_Read (START_ADDRESS, rxData, SIZE_TEST_ARRAY);

}



/* RENESAS MCU 1 ms Timer Interrupt Example
 */
void r_Config_CMT1_cmi1_interrupt(void)
{
    vEEPROM_TimerProcess();
}

/* RENESAS MCU Interrupt Example
 * Transmit End and Receive End Handlers
 */
volatile uint8_t g_ucFlagI2CTransmitEnd;
volatile uint8_t g_ucFlagI2CReceiveEnd;

static void r_Config_SCI5_callback_transmitend(void)
{
    g_ucFlagI2CTransmitEnd = 1;
}

static void r_Config_SCI5_callback_receiveend(void)
{
    g_ucFlagI2CReceiveEnd = 1;
}

```

