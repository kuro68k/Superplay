using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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
			configs = new List<ConfigParameter>();

			SuperIO.AddPhysicalMappings(ref configs);
		}
	}
}
