**UART transmission speed on Arduino Nano (ATMega689P)**
This program recieves and transmits a data set from PC to an avr based MCU and measure the time required for each process.
    Python send a 1000 charater (or byte) message from PC to the MCU using pyserial commands.
    The MCU recieves data and stores the data in EEPROM memory.
    MCU keeps note of the time measured to send a 10 bytes.
    Let T be the time required the baud rate for a recieving data is calculated as 1/(T*10*10) (a single UART byte contains two extra bit, the start and stop bit).
    Similarly is calculated for transmission as well.
    Both data and the passage from EEPROM is then sent back to the PC and received using Python code.
