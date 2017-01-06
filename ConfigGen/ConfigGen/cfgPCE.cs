using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConfigGen
{
	class cfgPCE : Config
	{
		public cfgPCE()
		{
			identifier = "PCE";
			index_number = 2;
			configs = new List<ConfigItem>();

			configs.Add(new ConfigItem("up", 0));
			configs.Add(new ConfigItem("down", 0));
			configs.Add(new ConfigItem("left", 0));
			configs.Add(new ConfigItem("right", 0));

			configs.Add(new ConfigItem("button1", 0));
			configs.Add(new ConfigItem("button2", 0));
			configs.Add(new ConfigItem("select", 0));
			configs.Add(new ConfigItem("run", 0));
		}
	}
}
