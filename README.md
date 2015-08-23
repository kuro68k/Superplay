# SUPERPLAY

SUPERPLAY is an arcade joystick controller system. It consists of serveral parts.

The main SUPERPLAY PCB is fitted to the joystick and can either connect via USB or a custom KBUS. Both use the same USB cable. Configuration is via a Windows app called SPConfig.

SUPERPLAY supports up to 16 buttons, multiple autofire configurations with selectable fire rate, multiple options for switching modes and rotary joysticks (Seimitsu LS-30, used by SNK) for games like Midnight Resistance. Rotary joystick support is compatible with MAME, and configurable. SUPERPLAY also supports expansion boards via an expansion header, similar to Arduino sheilds.

Aside from USB, SUPERPLAY can be connected to KBUS devices. Currently under development are a supergun with KBUS support, and a number of "recievers". Recievers connect to games consoles. The first is planned to support Playstation 1/2, Saturn and Dreamcast. Unfortunately due to licencing agreements Dreamcast support will not be published.

A HID based USB bootloader will be provided for firmware and configuration updates. See my other projects for code.

The main SUPERPLAY PCB uses an XMEGA 128A3U microcontroller. This might be revised down to a 64KB in future. The project opens with Atmel Studio 6.2. Schematic and gerbers created in Eagle. V1.0 hardware has been built, V1.1 corrects a few issues.

SPConfig is built in Visual Studio 2013 Community Edition. I uses MahApps.Metro to create a "modern" UI.

Everything is licenced under GPL v3.
