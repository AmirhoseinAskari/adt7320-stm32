# ADT7320 STM32 HAL Driver

STM32 HAL-based driver for the **ADT7320** high-accuracy digital temperature sensor over **SPI**.

## 🔧 Features
- ✅ **Full STM32 HAL compatibility** — supports all STM32 MCU series (F0–F7, G0/G4, H7, L0–L5, U0/U5)
- 🛡️ **MISRA-C-style design** — clean, safe, and portable for embedded and safety-critical applications
- 🔄 **Modular and portable** — works with STM32CubeIDE, Keil, IAR, or Makefile-based environments
- 📥 Supports read/write to all ADT7320 registers
- 🌡️ Accurate temperature conversion to degrees Celsius

## ⚙️ Getting Started

### 1. Configure SPI in STM32CubeMX
- Mode: **Full-Duplex Master**
- Data Size: **8-bit**
- First Bit: **MSB First**
- Clock Polarity (CPOL): **High**
- Clock Phase (CPHA): **2nd Edge**

### 2. Configure Chip Select (CS) Pin (Optional)
- Add a **GPIO Output** pin for CS (Chip Select)

### 3. Add the Driver to Your Project
- **Include** `adt7320.h` in your application code
- **Add** `adt7320.c` to your compiler
- **Set the STM32 MCU series macro** in `adt7320_config.h`
- **Add the library folder** to your compiler’s include paths

## 🧪 API Reference
Each function returns an `ADT7320_StatusTypeDef` status code.

### `ADT7320_Init(...)`  
Sends a reset sequence to initialize sensor.

### `ADT7320_ReadRegister(...)`  
Reads data from any ADT7320 register.

### `ADT7320_WriteRegister(...)`  
Writes data to any ADT7320 register.

### `ADT7320_ReadTemperature(...)`  
Reads and converts the temperature to °C.

## 💡 Example
A complete working example is available in [`example/main.c`](./example/main.c).

## 📜 License
This project is released under the [MIT License](./LICENSE).

## 👤 Author
**Amirhossein Askari**  
📧 theamiraskarii@gmail.com  
🔗 [GitHub Profile](https://github.com/AmirhoseinAskari)

