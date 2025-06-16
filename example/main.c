#include "adt7320.h"


ADT7320_ConfigTypeDef adt7320_handler;
ADT7320_StatusTypeDef status[5U];
float temperature = 0.0;


int main(void)
{
    /* Initialize all configured peripherals (HAL, GPIO, SPI, SystemClock, etc.) */
    // ...
    // ...
    // ...
	
   
    adt7320_handler.SPIx = &hspi1;
    adt7320_handler.csPort = GPIOA;
    adt7320_handler.csPin = GPIO_PIN_4; 
    
    status[0U] = ADT7320_Init(&adt7320_handler);
    status[1U] = ADT7320_WriteRegister(&adt7320_handler, ADT7320_CONFIG, 1U, 0x80U);   // Enable 16-bit temperature resolution mode
    status[2U] = ADT7320_WriteRegister(&adt7320_handler, ADT7320_TLOW, 2U, 0xF600U);   // Set low temperature threshold to -20°C
    status[3U] = ADT7320_WriteRegister(&adt7320_handler, ADT7320_THIGH, 2U, 0x2300U);  // Set high temperature threshold to +70°C
        
 
    while (1)
    {
        status[4U] = ADT7320_ReadTemperature(&adt7320_handler, &temperature);  // Read the Temperature   
    }
}
