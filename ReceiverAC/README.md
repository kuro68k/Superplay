# ReceiverACPS #

KBUS Receiever for:

- Atari/Commodore style 9 pin joystick ports  
  - Amiga (1-3 buttons)  
  - C64  
  - Atari 2600 / 7800 / ST etc.  
  - Many other 8/16 bit machines
- PC-Engine (Turbografx-16)
- Sega Master System
- Sega Megadrive (Genesis)

Any other simple switch/multiplex based system can also be supported, with the caveat that de-multiplexing is not as fast as real logic. Sega specify a 2uS delay after changing the multiplexed outputs, which this receiver exceeds.

The generic XMEGA E5 receiver hardware is used. An external 5V power input is provided because many computers and consoles can't supply the 250mA that KBUS requires. Of course not all KBUS devices need it, so you might be able to get away without it.

Lag is sub 1ms ("true zero", or at least 10x better than USB).
