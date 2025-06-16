# ADT7320 STM32 HAL Driver

A STM32 HAL-based driver for the ADT7320 high-accuracy digital temperature sensor via SPI.

## 🔧 Features
- 🔗 **Fully compatible with STM32 HAL** — supports **all STM32 MCU series**
- 🛡️ **MISRA-C-style design** — clean, safe, and portable for embedded and safety-critical applications
- Supports read/write to all ADT7320 registers
- Temperature conversion to °C
- Easily portable across STM32CubeIDE or IAR Embedded Workbench

## ⚙️ How to use this library

1. **Configure SPI in STM32CubeMX**
   - Enable an SPI peripheral (e.g., SPI1)
   - Set **I²C Speed Mode** to **Standard Mode (100 kHz)** or **Fast Mode (400 kHz)**

2. **Configure GPIO (Optional)**
   - Set up a **GPIO Output** to control the chip select pin

3. **Add the Library to Your Project**
   - **Include** `adt7320.h` in your application code
   - **Add** `adt7320.c` to your compiler
   - **Set the STM32 MCU series macro** in `adt7320_config.h`
   - **Add the library folder** to your compiler’s include paths

4. **Define EEPROM Configuration**
   - Create one or more `I2C_MemoryTypeDef` instances
   - Set the I²C handle and the device address (using A0/A1/A2 pin configuration)

5. **Build and Flash**  
   - Use the example in [`example/main.c`](./example/main.c) to guide your implementation

## 🧪 API Reference
Each function returns an `I2C_Memory_StatusTypeDef` status code.

### `I2C_Memory_Init(...)`  
Initializes an EEPROM instance, calculates the full device address, and prepares WP pin (if used).

### `I2C_Memory_SingleWrite(...)`  
Writes a single byte to the specified EEPROM memory address.

### `I2C_Memory_BurstWrite(...)`  
Writes multiple bytes (burst/page mode) starting from a target memory address.

### `I2C_Memory_SingleRead(...)`  
Reads one byte from a specified memory address.

### `I2C_Memory_BurstRead(...)`  
Reads multiple bytes from the EEPROM starting at a given address.

## 💡 Example
A complete working example is available in [`example/main.c`](./example/main.c).

## 📜 License
This project is released under the [MIT License](./LICENSE).

## 👤 Author
**Amirhossein Askari**  
📧 theamiraskarii@gmail.com  
🔗 [GitHub Profile](https://github.com/AmirhoseinAskari)

