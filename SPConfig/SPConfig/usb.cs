using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using HidSharp;
using System.Runtime.InteropServices;

namespace SPConfig
{
	class usb
	{
		enum BootloaderCommands
		{
			NOP = 0x00,
			RESET_POINTER = 0x01,
			READ_BUFFER = 0x02,
			ERASE_APP_SECTION = 0x03,
			READ_FLASH_CRCS = 0x04,
			READ_MCU_IDS = 0x05,
			READ_FUSES = 0x06,
			WRITE_PAGE = 0x07,
			READ_PAGE = 0x08,
			ERASE_USER_SIG_ROW = 0x09,
			WRITE_USER_SIG_ROW = 0x0A,
			READ_USER_SIG_ROW = 0x0B,
			READ_SERIAL = 0x0C,
			READ_BOOTLAODER_VERSION = 0x0D,
			RESET_MCU = 0x0E,
			READ_EEPROM = 0xF,
			WRITE_EEPROM = 0x10
		}

		private const int MinBootloaderVersion = 1;


		/* execute a HID bootloader command */
		private byte[] ExecuteHIDCommand(HidStream stream, int command, int address = 0, byte[] data = null)
		{
			var buffer = new byte[64];
			buffer[0] = 0;
			buffer[1] = (byte)command;
			buffer[2] = (byte)(address & 0xFF);
			buffer[3] = (byte)((address >> 8) & 0xFF);

			if (data != null)
			{
				if (data.Length > 60)
					//return null;
					throw new System.ArgumentException("Data length cannot be >60 bytes", "original");
				for (int i = 0; i < data.Length; i++)
					buffer[4 + i] = data[i];
			}

			try
			{
				stream.Write(buffer);
			
				stream.ReadTimeout = 100;
				byte[] result = stream.Read();
				if (result.Length < 4)
					return null;
				if (result[1] != (byte)(command | 0x80))
					return null;
				return result;
			}
			catch (Exception e)
			{
				Console.WriteLine(e.ToString());
				return null;
			}
		}

		/* check the bootloader before use */
		private bool CheckBootloader(HidStream stream)
		{
			// check bootloader version
			var version_res = ExecuteHIDCommand(stream, (int)BootloaderCommands.READ_BOOTLAODER_VERSION);
			if (version_res == null)
				return false;
			Console.WriteLine("Bootloader version: " + version_res[3].ToString());
			if (version_res[3] < MinBootloaderVersion)
			{
				Console.WriteLine("Bootloader too old.\n");
				return false;
			}

			// get serial number
			var serial_res = ExecuteHIDCommand(stream, (int)BootloaderCommands.READ_SERIAL);
			if (serial_res == null)
				return false;
			if (serial_res.Length <= 5)
				return false;
			Console.Write("Serial: ");
			for (int i = 5; i < serial_res.Length; i++)
				Console.Write((char)serial_res[i]);
			Console.WriteLine("");

			return true;
		}
		
		/* write config to connected devices */
		public bool Program(eeprom.eep_config config)
		{
			var loader = new HidDeviceLoader();
			var device = loader.GetDevices(0x1234, 0x9876).First();

			HidStream stream;
			if (!device.TryOpen(out stream))
				return false;
			if (!CheckBootloader(stream))
				return false;

			// convert config to a byte array buffer
			int size = Marshal.SizeOf(config);
			byte[] buffer = new byte[size];
			IntPtr ptr = Marshal.AllocHGlobal(size);
			Marshal.StructureToPtr(config, ptr, true);
			Marshal.Copy(ptr, buffer, 0, size);
			Marshal.FreeHGlobal(ptr);
			
			// write EEPROM
			//if (ExecuteHIDCommand(stream, (int)BootloaderCommands.RESET_POINTER) == null)
			//	return false;
			if (ExecuteHIDCommand(stream, (int)BootloaderCommands.WRITE_EEPROM, 0, buffer) == null)
				return false;

			return true;
		}

		/* read config from first connected device */
		/* returns null on failure */
		public config Read()
		{
			var loader = new HidDeviceLoader();
			var device = loader.GetDevices(0x1234, 0x9876).First();

			HidStream stream;
			if (!device.TryOpen(out stream))
				return null;
			if (!CheckBootloader(stream))
				return null;

			// read EEPROM
			var result = ExecuteHIDCommand(stream, (int)BootloaderCommands.READ_EEPROM, 0);
			if (result == null)
				return null;

			if (result.Length != (4 + 32))	// header + one EEPROM page
			{
				Console.WriteLine("Incorrect size response from bootloader.");
				return null;
			}

			// marshall the bytes from result into a new eep_config object
			var eep = new eeprom.eep_config();
			int size = Marshal.SizeOf(eep);
			IntPtr ptr = Marshal.AllocHGlobal(size);
			Marshal.Copy(result, 4, ptr, size);
			eep = (eeprom.eep_config)Marshal.PtrToStructure(ptr, typeof(eeprom.eep_config));
			Marshal.FreeHGlobal(ptr);

			// test EEPROM config and convert to config
			string error_message;
			var cfg = eeprom.DecodeEEPROMConfig(eep, out error_message);
			if (cfg == null)
			{
				Console.WriteLine(error_message);
				return null;
			}

			return cfg;
		}
	}
}
