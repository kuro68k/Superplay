using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Reflection;
using System.IO;

namespace ConfigGen
{
	class cfgSettings : Config
	{
		public static string _identifier = "SETTINGS";

		public cfgSettings()
		{
			identifier = _identifier;
			id_number = 1;
			allows_multiple = false;
			binary_struct = new BinaryFormat();
			binary_struct.led = new sbyte[16];

			configs = new List<ConfigParameter>();

			configs.Add(new ConfigParameter("af_high_hz", 15, 1, 120));
			configs.Add(new ConfigParameter("af_high_duty_pc", 50, 0, 100));
			configs.Add(new ConfigParameter("af_low_hz", 5, 1, 120));
			configs.Add(new ConfigParameter("af_low_duty_pc", 50, 0, 100));
			configs.Add(new ConfigParameter("af_remember_state", 0, 0, 1));

			for (int i = 1; i <= 16; i++)
				configs.Add(new ConfigParameter("led" + i.ToString(), 0, 0, 255));

			configs.Add(new ConfigParameter("led_display_mapping", 0, 0, 2));
			configs.Add(new ConfigParameter("led_display_timeout_ms", 0, 0, 65535));

			configs.Add(new ConfigParameter("meta_af_toggle", 0, 0, 1));
			configs.Add(new ConfigParameter("meta_af_select_with_stick", 0, 0, 1));
			configs.Add(new ConfigParameter("meta_mapping_select_with_stick", 0, 0, 1));

			configs.Add(new ConfigParameter("rotary_num_positions", 0, 0, 16));
			configs.Add(new ConfigParameter("rotary_enable_pov", 0, 0, 1));
			configs.Add(new ConfigParameter("rotary_enable_buttons", 0, 0, 1));

		}

		[StructLayout(LayoutKind.Sequential, Pack = 1)]
		private struct BinaryFormat
		{
			public UInt16 _config_length;
			public sbyte _id;

			public sbyte af_high_hz;
			public sbyte af_high_duty_pc;
			public sbyte af_low_hz;
			public sbyte af_low_duty_pc;
			public sbyte af_remember_state;

			[MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
			public sbyte[] led;

			public sbyte led_display_mapping;
			public UInt16 led_display_timeout_ms;

			public sbyte meta_af_toggle;
			public sbyte meta_af_select_with_stick;
			public sbyte meta_mapping_select_with_stick;

			public sbyte rotary_num_positions;
			public sbyte rotary_enable_pov;
			public sbyte rotary_enable_buttons;

			[MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
			public sbyte[] _padding;
		}

		public override void CustomByteSteamFormat(MemoryStream ms)
		{
			while (ms.Length < (64 - 4))	// padding
				ms.WriteByte(0xFF);
		}
	}
}
