/**
 * @file    adt7320.h
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


#ifndef ADT7320_H
#define ADT7320_H


#ifdef __cplusplus
    extern "C" {
#endif  /* __cplusplus */


/* ------------------------------------- Includes ------------------------------------- */

#include <stdint.h>          /**< Standard library for fixed-width integer types */
#include <stddef.h>          /**< Standard library for NULL definition */
#include "adt7320_config.h"  /**< User configuration for ADT7320 driver */


/**
 * @brief Includes the appropriate STM32 HAL header based on the defined MCU series.
 *
 * This conditional block selects the correct HAL driver header for the target STM32 series.
 * One of the `_STM32xx` macros must be defined in @ref adt7320_config.h.
 *
 * @note Only one `_STM32xx` macro should be defined at a time.
 * @see  adt7320_config.h
 */
#if defined (_STM32F0)                 
    #include "stm32f0xx_hal.h"   
#elif defined (_STM32F1)
    #include "stm32f1xx_hal.h"  
#elif defined (_STM32F2)
    #include "stm32f2xx_hal.h" 
#elif defined (_STM32F3)
    #include "stm32f3xx_hal.h"  
#elif defined (_STM32F4)
    #include "stm32f4xx_hal.h"  
#elif defined (_STM32F7)
    #include "stm32f7xx_hal.h"   
#elif defined (_STM32G0)
    #include "stm32g0xx_hal.h" 
#elif defined (_STM32G4)
    #include "stm32g4xx_hal.h"   
#elif defined (_STM32H7)
    #include "stm32h7xx_hal.h" 
#elif defined (_STM32L0)
    #include "stm32l0xx_hal.h" 
#elif defined (_STM32L1)
    #include "stm32l1xx_hal.h" 
#elif defined (_STM32L4)
    #include "stm32l4xx_hal.h"   
#elif defined (_STM32L5)
    #include "stm32l5xx_hal.h" 
#elif defined (_STM32U0)
    #include "stm32u0xx_hal.h" 
#elif defined (_STM32U5)
    #include "stm32u5xx_hal.h" 
#else                             
    #error "STM32 microcontroller not supported. Define one of the _STM32xx macros in adt7320_config.h."
#endif


/**
 * @brief Suppresses 'unused function' warnings for supported compilers.
 *
 * This directive silences warnings about unused static or inline functions, which are
 * common in embedded driver development.
 *
 * Supported compilers:
 * - IAR Embedded Workbench (ICCARM)
 * - GNU Compiler Collection (GCC)
 *
 * @note Compilation will fail if an unsupported compiler is detected.
 */
#if defined (__ICCARM__)             
    #pragma diag_suppress = Pe177                     
#elif defined (__GNUC__)   
    #pragma GCC diagnostic ignored "-Wunused-function"     
#else
    #error "Unsupported compiler for AD7124 driver"
#endif


/* ------------------------------------- Defines -------------------------------------- */

/** @brief Maximum allowable delay for SPI transactions */
#define  ADT7320_MAX_DELAY  (0xFFFFFFFFU)

/** @brief ADT7320 SPI command masks */
#define  ADT7320_READ    (0x40U)  ///< Read command mask (bit 6 = 1)
#define  ADT7320_WRITE   (0x00U)  ///< Write command mask (bit 6 = 0)
#define  ADT7320_DUMMY   (0x00U)  ///< Dummy byte for SPI transactions

/** @brief ADT7320 register addresses */        
#define  ADT7320_STATUS  (0x00U)  ///< Status register
#define  ADT7320_CONFIG  (0x01U)  ///< Configuration register
#define  ADT7320_TEMP    (0x02U)  ///< Temperature value register
#define  ADT7320_ID      (0x03U)  ///< Device ID register
#define  ADT7320_TCRIT   (0x04U)  ///< Critical temperature limit register
#define  ADT7320_THYST   (0x05U)  ///< Hysteresis register
#define  ADT7320_THIGH   (0x06U)  ///< High temperature limit register
#define  ADT7320_TLOW    (0x07U)  ///< Low temperature limit register


/* -------------------------------------- Types -------------------------------------- */

/**
 * @brief Configuration structure for ADT7320 SPI interface.
 *
 * Contains hardware-specific parameters required to perform SPI communication
 * with the ADT7320 digital temperature sensor using STM32 HAL.
 */
typedef struct
{                
    SPI_HandleTypeDef *SPIx;  /**< Pointer to SPI handle used by STM32 HAL SPI driver */      
    GPIO_TypeDef *csPort;     /**< GPIO port for chip select (CS) pin */    
    uint16_t csPin;           /**< GPIO pin number for chip select (CS) */                         
} ADT7320_ConfigTypeDef;


/**
 * @brief Return status codes for ADT7320 driver functions.
 *
 * Describes the result of a function call interacting with the ADT7320 sensor.
 */
typedef enum
{
    ADT7320_OK      = 0U,  /**< Operation completed successfully */   
    ADT7320_ERROR   = 1U,  /**< Operation failed due to an error */ 
    ADT7320_BUSY    = 2U,  /**< Device is currently busy with another operation */ 
    ADT7320_TIMEOUT = 3U   /**< Operation timed out */  
} ADT7320_StatusTypeDef;


/* ------------------------------------ Prototype ------------------------------------ */

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
ADT7320_StatusTypeDef ADT7320_Init(const ADT7320_ConfigTypeDef *pConfig);

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
ADT7320_StatusTypeDef ADT7320_ReadRegister(const ADT7320_ConfigTypeDef *pConfig, uint8_t reg, uint8_t dataSize, uint16_t *pData);

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
ADT7320_StatusTypeDef ADT7320_WriteRegister(const ADT7320_ConfigTypeDef *pConfig, uint8_t reg, uint8_t dataSize, uint16_t data);

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
ADT7320_StatusTypeDef ADT7320_ReadTemperature(const ADT7320_ConfigTypeDef *pConfig, float *pTemperature);


#ifdef __cplusplus
    }
#endif  /* __cplusplus */


#endif  /* ADT7320_H */
