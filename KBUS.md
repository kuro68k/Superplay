# KBUS specification V0.1

KBUS is a high speed bus for video game controllers. It provides a low latency link and standard data format that is relatively easy to implement on common 8 bit microcontrollers and above. KBUS is a point-to-point bus, so there is always exactly one receiver and one device.


## Terminolgy

**Receiver**

The receiver is the bus master, issuing polling requests and other commands to devices. It's usually something like a PC or converter for a game console, which takes the player's input from devices.

**Device**

Devices respond to requests from the receiver by supplying data about user inputs. They are usually things like arcade joysticks, flight controls or converters for game console controllers. Devices are usually responsible for handling features like autofire, although receivers can optionally implement them as well.


## Electrical

KBUS uses RS232 signalling at 3.3V and 1M baud, 8 bits, no parity, 1 stop bit (8n1). This is easily implemented on most microcontrollers with a UART. Optionally, USB HID can be supported on the same cable with auto-detection.

Packets are separated by a gap of 2 or more characters.

The standard KBUS cable is a standard USB 2.0 cable. The type of connector on either end (A, B or C, micro, mini etc.) is not specified, but the standard full size A to B cable is cheap, readily available and fairly robust. D- is used for TX, and D+ for RX, from the receiver's point of view. Obviously, devices must be reversed.

Power supplied to KBUS devices is 4.5-5.25V at 250mA. This allows for both standard USB 5V ±5% and for diode ORing of multiple supplies. Some receivers can't supply 250mA so it may be necessary to provide an external power. Since signalling is at 3.3V it is expected that the supply voltage will be regulated down to that level anyway.


## Protocol

KBUS packets are 64 bytes long, for compatibility with USB HID. Each packet contains a command and up to 63 data bytes.

**KBUS over RS232 packet**

| Bytes | Description |
| :--- | :--- |
| 1 | Command code |
| 1 | Data length |
| 0-63 | Data |
| 2 | CCITT CRC-16 Checksum |

**KBUS over USB packet**

| Bytes | Description |
| :--- | :--- |
| 1 | Command code |
| 0-63 | Data |

USB handles packet length and checksums as part of its own protocol, so there is no need to duplicate them.

Commands are as follows:

| Command | Code | Description |
| :--- | :--- | :--- |
| ECHO | 0x50 | Echo data back, for testing/NOP |
| READ_STRING | 0x51 | Data byte indicates string ID<br>`0 - DEVICE_NAME`<br>`1 - MANUFACTURER`<br>`2 - SERIAL_NUMBER` |
| READ_VID_PID | 0x52 | Read 16 bit vendor/product IDs |
| | |
| START_REPORTING | 0x54 | Start sending continuous report packets |
| STOP_REPORTING | 0x55 | Stop sending continuous report packets |
| | |
| READ_REPORT | 0x5A | Request sending a report |
| | |
| ENTER_BOOTLOADER | 0x5B | Start bootloader (only on supported connections, e.g. USB) |

Strings are in Unicode UTF16-LE format, the same as USB. While Unicode has some severe limitations, compatibility makes it easy to implement dual protocol devices. To convert basic ISO 8859-1 / ASCII to UTF16-LE, just extend every byte to 16 bits and esure you have little-endian order. Strings do not need to be null terminated. Due to the packet length limit, the maximum string length is 31 characters.

Report packets must contain at least two data bytes, but can be up to 63 bytes. Currently only the following bytes are defined:

| Byte | Name | Description |
| :--- | :--- | :--- |
| 0 | UDLR_SSCC | (LSB) Up/down/left/right - start/select/coin/control (MSB) |
| 1 | BUTTONS1_8 | Buttons 1 - 8 (1 = LSB) |
| 2 | BUTTONS9_16 | Buttons 9 - 16 |
| 3 | ROT_MODE | Bits 0-3 - Rotary position 0-11<br>Bits 4-5 - Button mode<br>Bits 6-7 - Unused |

Receivers can ignore inputs they don't need / understand.


## Polling and activation

Receivers often need to poll KBUS ports to see if a device is attached. Polling also gives devices an opportunity to calibrate internal oscillators/UARTs and signal to the receiver that they are ready.

Polling starts by the receiver sending 0xFF bytes continuiously. 0xFF produces just one low pulse per byte that is 1uS wide. The receiver can measure the width of this bit for calibration / auto-bauding. When ready, the device should respond with 0x0F, which again provides useful timing data and an opportunity to test the receiver's UART. The device should keep transmitting until the receiver stops sending 0xFF bytes.

Once both receiver and device are quiet, the receiver should send an ECHO command to test the link, followed optionally by polling IDs and strings, before starting to request reports.
