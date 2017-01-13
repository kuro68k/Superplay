using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace ConfigGen
{
	class cfgPCE : Config
	{
		public static string _identifier = "PCE";

		public cfgPCE()
		{
			identifier = _identifier;
			index_number = 17;
			allows_multiple = false;
			binary_struct = new BinaryFormat();
			configs = new List<ConfigParameter>();

			configs.Add(new ConfigParameter("up", 0));
			configs.Add(new ConfigParameter("down", 0));
			configs.Add(new ConfigParameter("left", 0));
			configs.Add(new ConfigParameter("right", 0));

			configs.Add(new ConfigParameter("button1", 0));
			configs.Add(new ConfigParameter("button2", 0));
			configs.Add(new ConfigParameter("select", 0));
			configs.Add(new ConfigParameter("run", 0));
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
