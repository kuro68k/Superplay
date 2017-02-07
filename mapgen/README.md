# mapgen

Generate button mappings for the Superplay joystick controller. Produces an EEPROM hex file that can be loaded onto the controller via the bootloader.

The source file format is as follows, see names.c for a list of available input names:

; - comment
/ - Makes input an output for this signal, e.g. to drive an LED
! - Forces an input to always be on (as if the button was permanently held)
