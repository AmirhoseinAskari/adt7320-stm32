/**
 * @file    adt7320.c
 * @author  Amirhossein Askari
 * @version 1.0.0
 * @date    2025-06-17
 * @email   theamiraskarii@gmail.com
 * @see     https://github.com/AmirhoseinAskari
 * @brief   Driver for the ADT7320 digital temperature sensor for use with STM32 microcontrollers.
 *
 * @details
 * This driver enables communication with the Analog Devices ADT7320 digital temperature sensor via SPI.
 * It provides functions to configure the sensor, read temperature data, and access device registers.
 *
 * Configure the SPI peripheral with the following settings:
 * - Clock polarity (CPOL) = 1 (idle high)
 * - Clock phase    (CPHA) = 1 (data captured on the trailing edge)
 *
 * @note
 * The SPI peripheral and GPIO (chip select) must be initialized externally
 * before using the functions provided in this driver.
 *
 * @warning
 * Ensure that the SPI peripheral is correctly configured (CPOL=1, CPHA=1).
 * Improper setup may lead to incorrect data reads or communication failure.
 *
 * @section supported_platforms Supported Platforms
 * - STM32 microcontrollers using HAL drivers.
 *
 * @copyright
 * MIT License. See LICENSE file for details.
 */


/* ------------------------------------ Includes ------------------------------------ */

#include "adt7320.h"  /**< Include the ADT7320 driver interface and hardware configuration */


/* ------------------------------------ Functions ----------------------------------- */

/**
 * @brief  Initializes the ADT7320 temperature sensor by sending a reset sequence.
 *
 * This function resets the ADT7320 sensor by transmitting the required SPI reset sequence.
 * It ensures the sensor starts in a known state before configuration.
 *
 * @param[in] pConfig  Pointer to the ADT7320 configuration structure.
 *
 * @retval ADT7320_OK     Initialization successful
 * @retval ADT7320_ERROR  SPI communication failure or sensor not responding.
 */
ADT7320_StatusTypeDef ADT7320_Init(const ADT7320_ConfigTypeDef *pConfig)
{
    ADT7320_StatusTypeDef status = ADT7320_OK;
    uint8_t data[4U] = {0xFFU, 0xFFU, 0xFFU, 0xFFU};
    
    if (pConfig == NULL)
    {
        status = ADT7320_ERROR;
    }
    else
    {       
        HAL_GPIO_WritePin(pConfig->csPort, pConfig->csPin, GPIO_PIN_RESET);
        status = (ADT7320_StatusTypeDef) HAL_SPI_Transmit(pConfig->SPIx, data, 4U, ADT7320_MAX_DELAY);
        HAL_GPIO_WritePin(pConfig->csPort, pConfig->csPin, GPIO_PIN_SET);
    } 
    
    return status;
}

/**
 * @brief  Reads data from a specified register of the ADT7320 sensor.
 *
 * This function retrieves data from the specified register of the ADT7320 temperature sensor
 * via SPI communication. The function supports reading up to 2 bytes of data.
 *
 * @param[in]   pConfig   Pointer to the ADT7320 configuration structure.
 * @param[in]   reg       Register address to read from.
 * @param[in]   dataSize  Number of bytes to read (valid range: 1 to 2 bytes).
 * @param[out]  pData     Pointer to the variable where the read data will be stored.
 *
 * @retval ADT7320_OK     Operation successful
 * @retval ADT7320_ERROR  SPI communication failure or invalid parameters
 */
ADT7320_StatusTypeDef ADT7320_ReadRegister(const ADT7320_ConfigTypeDef *pConfig, uint8_t reg, uint8_t dataSize, uint16_t *pData)
{
    ADT7320_StatusTypeDef status = ADT7320_OK;
    uint8_t txRxBuf[3U] = {0U};
    uint16_t rxData     = 0U;      
       
    if ( (pConfig == NULL) || (dataSize == 0U) )
    {
        status = ADT7320_ERROR;
    }
    else
    {   
        txRxBuf[0U] = (ADT7320_READ | ((reg & 0x1FU) << 3U)); 
       
        HAL_GPIO_WritePin(pConfig->csPort, pConfig->csPin, GPIO_PIN_RESET);
        status = (ADT7320_StatusTypeDef) HAL_SPI_TransmitReceive(pConfig->SPIx, txRxBuf, txRxBuf, (dataSize + 1U), ADT7320_MAX_DELAY);
        HAL_GPIO_WritePin(pConfig->csPort, pConfig->csPin, GPIO_PIN_SET);

        for (uint8_t i = 1U; i <= dataSize; i++)
        {
            rxData = (rxData << 8U) | txRxBuf[i];
        }
        *pData = rxData;
    }
          
    return status;
}

/**
 * @brief  Writes data to a specified register of the ADT7320 sensor.
 *
 * This function sends data to the specified register of the ADT7320 temperature sensor
 * via SPI communication. It supports writing up to 2 bytes of data.
 *
 * @param[in]  pConfig    Pointer to the ADT7320 configuration structure.
 * @param[in]  reg        Register address to write to.
 * @param[in]  dataSize   Number of bytes to write (valid range: 1 to 2 bytes).
 * @param[in]  data       Data value to write to the register.
 *
 * @retval ADT7320_OK     Operation successful
 * @retval ADT7320_ERROR  SPI communication failure or invalid parameters
 */
ADT7320_StatusTypeDef ADT7320_WriteRegister(const ADT7320_ConfigTypeDef *pConfig, uint8_t reg, uint8_t dataSize, uint16_t data)
{
    ADT7320_StatusTypeDef status = ADT7320_OK;
    uint8_t txBuf[3U] = {0U}; 
    
    if ( (pConfig == NULL) || (dataSize == 0U) )
    {
        status = ADT7320_ERROR;
    }
    else
    {  
        txBuf[0U] = (ADT7320_WRITE | ( (reg & 0x1FU) << 3U) );  

        for (uint8_t i = 0U; i < dataSize; i++)
        {
            txBuf[i + 1U] = (uint8_t)(data >> (8U * (dataSize - i - 1U)));
        }

        HAL_GPIO_WritePin(pConfig->csPort, pConfig->csPin, GPIO_PIN_RESET);
        status = (ADT7320_StatusTypeDef) HAL_SPI_Transmit(pConfig->SPIx, txBuf, (dataSize + 1U), ADT7320_MAX_DELAY);
        HAL_GPIO_WritePin(pConfig->csPort, pConfig->csPin, GPIO_PIN_SET);
    }
    
    return status;
}

/**
 * @brief  Reads and converts the temperature value from the ADT7320 sensor.
 *
 * This function retrieves the raw temperature data from the ADT7320 sensor via SPI,
 * converts it to a Celsius temperature value, and stores the result in the provided variable.
 *
 * The ADT7320 outputs temperature in a 16-bit two's complement format with a resolution
 * of 1/128 °C per LSB.
 *
 * @param[in]   pConfig       Pointer to the ADT7320 configuration structure.
 * @param[out]  pTemperature  Pointer to a float variable where the converted temperature value will be stored.
 *
 * @retval ADT7320_OK     Operation successful.
 * @retval ADT7320_ERROR  Invalid parameters or SPI communication failure.
 */
ADT7320_StatusTypeDef ADT7320_ReadTemperature(const ADT7320_ConfigTypeDef *pConfig, float *pTemperature)
{
    ADT7320_StatusTypeDef status = ADT7320_OK;
    uint16_t data  = 0U;
    float    temp  = 0.0f;
    
    if (pConfig == NULL)
    {
        status = ADT7320_ERROR;
    }
    else
    {   
        status = ADT7320_ReadRegister(pConfig, ADT7320_TEMP, 2U, &data);
        
        if (status == ADT7320_OK)
        {
            if (data & 0x8000U) 
            {
                temp = ((int16_t)data) / 128.0f;
            }
            else                  
            {
                temp = ((float)data) / 128.0f;
            }       
            *pTemperature = temp;       
        }
    }
    
    return status;
}


/* adt7320.c */
