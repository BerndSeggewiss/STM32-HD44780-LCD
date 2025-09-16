# STM32-HD44780-LCD

This project provides a C library for controlling an HD44780-based LCD display with an STM32 microcontroller.
It was created and tested with the L476RG type.

## Features

- Supports 4-bit mode
- Data and command transmission
- Cursor positioning
- Text display and custom characters
- Configurable pin mapping

## Requirements

- STM32 microcontroller
- Development environment such as STM32CubeIDE or Keil
- HD44780-compatible LCD display
- Basic C programming knowledge

## Installation & Usage

1. Clone the repository:
   ```bash
   git clone https://github.com/BerndSeggewiss/STM32-HD44780-LCD.git
   ```
2. Add the source files (`lcd.c`, `lcd.h`) to your STM32 project.
3. **Make sure to add a reference to the library in your STM32 project.**
4. Adjust the pin configuration in `lcd.h` to match your hardware setup.

## Example

```c
#include "lcd.h"

int main(void) {
    lcd_init();
    lcd_set_cursor(0, 0);
    lcd_print("Hello STM32!");
    while (1) {}
}
```

## Documentation

Detailed documentation and usage instructions can be found in the folder 'docu'.

## Project Structure

- `lcd.c`, `lcd.h`: LCD driver functions
- `docu/`: Additional documentation
- `README.md`: Project overview

## License

This project is licensed under the MIT License.
