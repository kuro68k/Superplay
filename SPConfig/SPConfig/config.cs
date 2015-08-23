using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Serialization;
using System.IO;

namespace SPConfig
{
	public class config
	{
		enum ButtonModes {	Switched = 0,
							Eight = 1,
							Four = 2,
							Four_AF = 3,
							Sixteen = 4 };

		enum AFModes {		High_Low = 0,
							High_with_LEDs = 1,
							Fixed = 2 };

		enum RotaryModes {	Disabled = 0,
							Buttons = 1,
							POV = 2 };


		// data to be saved in config save files
		public int buttons_mode;
		public int af_mode;
		public float af_high_hz;
		public float af_low_hz;
		public bool[] af_mask = new bool[16];
		public bool af_save_state;
		public int rotary_mode;
		public bool start_af_toggle_mode;



		// load default settings
		public void LoadDefaults()
		{
			buttons_mode = (int)ButtonModes.Eight;
			af_mode = (int)AFModes.High_Low;
			af_high_hz = 15;
			af_low_hz = 5;
			for (int i = 0; i < 16; i++)
				af_mask[i] = false;
			rotary_mode = (int)RotaryModes.Disabled;
		}

		static public bool ValidateConfig(config cfg, out string failure_message)
		{
			if (!Enum.IsDefined(typeof(ButtonModes), cfg.buttons_mode))
			{
				failure_message = "buttons_mode incorrect.";
				return false;
			}
			if (!Enum.IsDefined(typeof(AFModes), cfg.af_mode))
			{
				failure_message = "af_mode incorrect.";
				return false;
			}
			if (!Enum.IsDefined(typeof(RotaryModes), cfg.rotary_mode))
			{
				failure_message = "rotary_mode incorrect.";
				return false;
			}
			if ((cfg.af_high_hz > 120) ||
				(cfg.af_high_hz < 0.5))
			{
				failure_message = "af_high_hz out of range.";
				return false;
			}
			if ((cfg.af_low_hz > 120) ||
				(cfg.af_low_hz < 0.5))
			{
				failure_message = "af_low_hz out of range.";
				return false;
			}

			failure_message = "No failures.";
			return true;
		}

		// save to XML file
		public bool SaveToFile(string filename)
		{
			bool result = true;

			XmlSerializer serializer = new XmlSerializer(typeof(config));
			TextWriter writer = new StreamWriter(filename);
			try
			{
				serializer.Serialize(writer, this);
			}
			catch(Exception e)
			{
				Console.WriteLine(e.ToString());
				result = false;
			}
			writer.Close();
			
			return result;
		}

		// load from XML file
		static public config ReadFromFile(string filename, out string failure_message)
		{
			XmlSerializer serializer = new XmlSerializer(typeof(config));
			serializer.UnknownNode += new XmlNodeEventHandler(serializer_UnknownNode);
			serializer.UnknownAttribute += new XmlAttributeEventHandler(serializer_UnknownAttribute);

			FileStream fs = new FileStream(filename, FileMode.Open);
			config new_config;
			try
			{
				new_config = (config)serializer.Deserialize(fs);
			}
			catch(Exception e)
			{
				Console.WriteLine(e.ToString());
				failure_message = "Problem parsing file.";
				return null;
			}

			// validate loaded data
			if (!ValidateConfig(new_config, out failure_message))
				return null;
			return new_config;
		}

		// handlers for problems loading XML files
		static private void serializer_UnknownNode (object sender, XmlNodeEventArgs e)
		{
			Console.WriteLine("Unknown node:\t" + e.Name + "\t" + e.Text);
		}

		static private void serializer_UnknownAttribute(object sender, XmlAttributeEventArgs e)
		{
			System.Xml.XmlAttribute attr = e.Attr;
			Console.WriteLine("Unknown attribute " + attr.Name + "='" + attr.Value + "'");
		}
	}
}
