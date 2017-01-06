using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConfigGen
{
	class ConfigItem
	{
		public string identifier;
		public int value;

		public ConfigItem(string _identifier, int _value)
		{
			identifier = _identifier;
			value = _value;
		}
	}

	class Config
	{
		public string identifier;			// string identifier in config file
		public int index_number;			// logical index number used by firmware
		public List<ConfigItem> configs;	// configurable items
	}
}
