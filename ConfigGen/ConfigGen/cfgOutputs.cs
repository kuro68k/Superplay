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
			configs = new List<ConfigParameter>();

			SuperIO.AddPhysicalMappings(ref configs);
		}

		[StructLayout(LayoutKind.Sequential, Pack = 1)]
		private struct BinaryFormat
		{
			public UInt16 _config_length;
			public sbyte _count;
			public sbyte[] _mapping;
		}
	}
}
