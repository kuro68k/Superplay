using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConfigGen
{
	class cfgSettings : Config
	{
		public cfgSettings()
		{
			identifier = "SETTINGS";
			index_number = 1;
			configs = new List<ConfigItem>();

			configs.Add(new ConfigItem("af_high_hz", 15));
			configs.Add(new ConfigItem("af_high_duty_pc", 50));
			configs.Add(new ConfigItem("af_low_hz", 5));
			configs.Add(new ConfigItem("af_low_duty_pc", 50));

			for (int i = 1; i <= 16; i++)
				configs.Add(new ConfigItem("led" + i.ToString(), 0));

			configs.Add(new ConfigItem("led_display_mapping_pos", 0));
			configs.Add(new ConfigItem("led_display_timeout_s", 0));

			configs.Add(new ConfigItem("meta_af_toggle", 0));
			configs.Add(new ConfigItem("meta_af_select_with_stick", 0));
			configs.Add(new ConfigItem("meta_mapping_select_with_stick", 0));
		}
	}
}
