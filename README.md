# SUPERPLAY

SUPERPLAY is an arcade joystick controller system. It consists of serveral parts.

The main SUPERPLAY PCB is fitted to the joystick and can either connect via USB or a custom KBUS. Both use the same USB cable. Configuration is via a Windows app called SPConfig.

SUPERPLAY supports up to 16 buttons, multiple autofire configurations with selectable fire rate, multiple options for switching modes and rotary joysticks (Seimitsu LS-30, used by SNK) for games like Midnight Resistance. Rotary joystick support is compatible with MAME, and configurable. SUPERPLAY also supports expansion boards via an expansion header, similar to Arduino sheilds.


####True zero lag

Many joystick controllers claim to be zero lag, but as FPS players know USB introduces some delay. USB HID devices are polled at 120Hz maximum, resulting in up to 8ms of lag (1/2 a frame at 60Hz). FPS players hack the Windows mouse driver to increase the poll rate, but it doesn't work for joysticks.

SUPERPLAY uses the excellent vJoy (vjoystick.sourceforge.net) and a custom USB interface to increase the polling rate to 1000Hz, reducing lag down to 1ms maximum. This gives true zero, arcade/console levels of input lag (i.e. none).


####Autofire

SUPERPLAY has a special autofire implementation that allows the player to set a precise frequency, and ensures that a button press is always registered when the button is first pushed. Simple autofire systems switch in a square wave, so the button press is sometimes not registered until the "on" part of the square wave comes around.


####KBUS

Aside from USB, SUPERPLAY can be connected to KBUS devices. Currently under development are a supergun with KBUS support, and a number of "recievers". Recievers connect to games consoles. The first is planned to support Playstation 1/2, Saturn and Dreamcast. Unfortunately due to licencing agreements Dreamcast support will not be published.

KBUS uses a simple 3.3V "RS232" compatible protocol, that can be easily implemented on low cost microcontrollers.


####Other stuff

A HID based USB bootloader will be provided for firmware and configuration updates. See my other projects for code.

The main SUPERPLAY PCB uses an XMEGA 128A3U microcontroller. This might be revised down to a 64KB in future. The project opens with Atmel Studio 6.2. Schematic and gerbers created in Eagle. V1.0 hardware has been built, V1.1 corrects a few issues.

SPConfig is built in Visual Studio 2013 Community Edition. I uses MahApps.Metro to create a "modern" UI.

Everything is licenced under GPL v3.
