using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace ConfigGen
{
	class cfgPCE6 : Config
	{
		public static string _identifier = "PCE6";

		public cfgPCE6()
		{
			identifier = _identifier;
			id_number = 18;
			allows_multiple = false;
			binary_struct = new BinaryFormat();
			configs = new List<ConfigParameter>();

			configs.Add(new ConfigParameter("up", 0));
			configs.Add(new ConfigParameter("down", 0));
			configs.Add(new ConfigParameter("left", 0));
			configs.Add(new ConfigParameter("right", 0));

			configs.Add(new ConfigParameter("button1", 0));
			configs.Add(new ConfigParameter("button2", 0));
			configs.Add(new ConfigParameter("button3", 0));
			configs.Add(new ConfigParameter("button4", 0));
			configs.Add(new ConfigParameter("button5", 0));
			configs.Add(new ConfigParameter("button6", 0));
			configs.Add(new ConfigParameter("select", 0));
			configs.Add(new ConfigParameter("run", 0));
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
