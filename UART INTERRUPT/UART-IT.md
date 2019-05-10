Interrupt for UART using HAL libraries

The UART sent data can be of any size. This is achieved by asserting an interrupt handle request (IRQ) only one character long. Upon receiving each data it stops adding to a buffer when it encounters "\n" character.
