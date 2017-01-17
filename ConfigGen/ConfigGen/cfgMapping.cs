using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace ConfigGen
{
	class cfgMapping : Config
	{
		public static string _identifier = "MAPPING";

		public cfgMapping()
		{
			identifier = _identifier;
			id_number = 4;
			allows_multiple = true;
			binary_struct = new BinaryFormat();
			configs = new List<ConfigParameter>();

			SuperIO.AddLogicalMappings(ref configs);
			//SuperIO.AddPhysicalMappings(ref configs);
		}

		[StructLayout(LayoutKind.Sequential, Pack = 1)]
		private struct BinaryFormat
		{
			public UInt16 _config_length;
			public sbyte _id;
			public sbyte _count;
			public sbyte[,] _mapping;
		}
	}
}
