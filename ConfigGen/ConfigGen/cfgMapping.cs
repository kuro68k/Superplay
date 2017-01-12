using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConfigGen
{
	class cfgMapping : Config
	{
		public static string _identifier = "MAPPING";

		public cfgMapping()
		{
			identifier = _identifier;
			index_number = 4;
			allows_multiple = true;
			configs = new List<ConfigParameter>();

			SuperIO.AddLogicalMappings(ref configs);
			SuperIO.AddPhysicalMappings(ref configs);
		}
	}
}
