using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace ConfigGen
{
	class cfgOutputs : Config
	{
		public static string _identifier = "OUTPUTS";

		public cfgOutputs()
		{
			identifier = _identifier;
			index_number = 2;
			allows_multiple = false;
			binary_struct = new BinaryFormat();
			binary_struct._mapping = new sbyte[128];
			configs = new List<ConfigParameter>();

			SuperIO.AddPhysicalMappings(ref configs);
		}

		[StructLayout(LayoutKind.Sequential)]
		private struct BinaryFormat
		{
			public UInt16 _config_length;

			[MarshalAs(UnmanagedType.ByValArray, SizeConst = 128)]
			public sbyte[] _mapping;
		}

		public void CustomBinaryFormat()
		{
			for (int i = 0; i < configs.Count; i++)
				binary_struct._mapping[i] = (sbyte)configs[i].value;
		}
	}
}
