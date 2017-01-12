using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;

namespace ConfigGen
{
	class ConfigParameter
	{
		public string identifier;
		public int value;
		public int min;
		public int max;
		
		public ConfigParameter(string _identifier, int _value)
		{
			identifier = _identifier;
			value = _value;
			min = 0;
			max = 255;
		}

		public ConfigParameter(string _identifier, int _value, int _min, int _max)
		{
			identifier = _identifier;
			value = _value;
			min = _min;
			max = _max;
		}
	}



	class Config
	{
		public string identifier;			// string identifier in config file
		public int index_number;			// logical index number used by firmware
		public bool allows_multiple;		// more than one of this config is allowed
		public dynamic binary_struct;
		public List<ConfigParameter> configs;	// configurable items

		// compile parameters to binary format
		public byte[] CompileToBinary()
		{
			FieldInfo[] fieldinfo = Type.GetType(GetType().FullName + "+BinaryFormat").GetFields(BindingFlags.Instance | BindingFlags.Public);
			if (fieldinfo == null)
				throw new Exception("Config " + identifier + " does not have a BinaryFormat struct.");
			UInt16 size = (UInt16)Marshal.SizeOf(binary_struct);

			foreach (FieldInfo field in fieldinfo)
			{
				string field_id = field.Name;
				if (field_id == "_config_length")
				{
					field.SetValue(binary_struct, size);
					continue;
				}

				if (field.FieldType.IsArray)
				{
					Array array = field.GetValue(binary_struct) as Array;
					for (int i = 0; i < array.Length; i++)
					{
						ConfigParameter paramA = FindParameterByIdentifier(field_id + (i + 1).ToString());
						if (paramA == null)
							throw new Exception("Struct field with no matching parameter (" + field_id + i.ToString() + ")");
						array.SetValue(Convert.ChangeType(paramA.value, field.FieldType.GetElementType()), i);
					}
					field.SetValue(binary_struct, array);
					continue;
				}

				ConfigParameter param = FindParameterByIdentifier(field_id);
				if (param == null)
					throw new Exception("Struct field with no matching parameter (" + field_id + ")");
				field.SetValue(binary_struct, Convert.ChangeType(param.value, field.FieldType));
			}

			byte[] buffer = new byte[size];
			IntPtr ptr = Marshal.AllocHGlobal(size);
			Marshal.StructureToPtr(binary_struct, ptr, true);
			Marshal.Copy(ptr, buffer, 0, size);
			Marshal.FreeHGlobal(ptr);

			return buffer;
		}

		// dump all config items in this config
		public void DumpToConsole()
		{
			Console.WriteLine(identifier + " (" + index_number.ToString() + ")");
			foreach (ConfigParameter item in configs)
			{
				Console.WriteLine("\t" + item.identifier + " = " + item.value.ToString() + " (" + item.min.ToString() + ".." + item.max.ToString() + ")");
			}
		}

		// load config file
		public static List<Config> LoadConfigFromFile(string filename)
		{
			StreamReader sr;
			try
			{
				sr = new StreamReader(filename);
			}
			catch (Exception e)
			{
				Console.WriteLine("Unable to open " + filename);
				Console.WriteLine(e.ToString());
				return null;
			}

			bool failed = false;
			int line_num = 1;
			Config cfg = null;
			List<Config> config_list = new List<Config>();

			while (!sr.EndOfStream)
			{
				string line = sr.ReadLine();
				if (!ParseLine(line, line_num, ref cfg, ref config_list))
				{
					failed = true;
					break;
				}
				line_num++;
			}

			sr.Close();
			
			if (failed)
				return null;
			return config_list;
		}

		// parse a single line from config file, returing false on error
		private static bool ParseLine(string line, int line_num, ref Config cfg, ref List<Config> config_list)
		{
			string t = line;

			// remove comments
			if (t.Contains(';'))
				t = t.Substring(0, t.IndexOf(';'));
			if (t.Contains("//"))
				t = t.Substring(0, t.IndexOf("//"));

			t = t.Trim();

			if (t == "")		// blank / comment only line
				return true;

			t = t.ToLower();

			try
			{
				// begin a new config
				if (t.StartsWith("begin"))
				{
					if (cfg != null)	// missing end
						throw new Exception("Unexpected \"begin\" at line " + line_num.ToString());

					string identifier = t.Substring(t.IndexOf(' ')).Trim();
					Config new_cfg = FindCfgByIdentifier(identifier);
					if (new_cfg == null)
						throw new Exception("Unknown identifier \"" + identifier + "\" at line " + line_num.ToString());

					cfg = new_cfg;

					// check for repeats
					if (cfg.allows_multiple == false)
					{
						foreach (Config c in config_list)
						{
							if (c.GetType() == cfg.GetType())
								throw new Exception("Multiple configs of type \"" + identifier + "\" not allowed at line " + line_num.ToString());
						}
					}
				}

				// end current config
				else if (t.StartsWith("end"))
				{
					if (cfg == null)
						throw new Exception("End without matching begin at line " + line_num.ToString());

					if (t.Length > 3)	// must have the config identifier following it or nothing
					{
						string identifier = t.Substring(3).Trim();
						if (identifier != cfg.identifier.ToLower())
							throw new Exception("End identifier \"" + identifier + "\" does not match previous begin at line " + line_num.ToString());
					}

					config_list.Add(cfg);
					cfg = null;
				}

				// setting
				else
				{
					if (cfg == null)
						throw new Exception("No matching begin for setting at line " + line_num.ToString());

					int eq = t.IndexOf('=');
				
					if (eq == -1)
						throw new Exception("Missing \"=\" at line " + line_num.ToString());

					string identifier;
					string value;

					try
					{
						identifier = t.Substring(0, eq - 1).Trim();
						value = t.Substring(eq + 1).Trim();
					}
					catch
					{
						throw new Exception("Syntax error at line " + line_num.ToString());
					}

					ConfigParameter item = cfg.FindParameterByIdentifier(identifier);
					if (item == null)
						throw new Exception("Unknown identifier \"" + identifier + "\" at line " + line_num.ToString());
					
					int i;
					if (!cfg.ParseValue(value, out i))
						throw new Exception("Unable to parse value \"" + value + "\" at line " + line_num.ToString());
					item.value = i;

					Console.WriteLine(cfg.identifier + "->" + item.identifier + " = " + item.value.ToString());
				}

			}
			catch (Exception e)
			{
				Console.WriteLine(e.Message);
				Console.WriteLine(line);
				return false;
			}

			return true;
		}

		// find a config type matching the given identifier, returns null if not found
		private static Config FindCfgByIdentifier(string identifier)
		{
			var types = Assembly
				.GetExecutingAssembly()
				.GetTypes()
				.Where(t => t.Namespace.StartsWith("ConfigGen"));

			foreach (var t in types)
			{
				if (t.Name.StartsWith("cfg"))
				{
					var field = t.GetField("_identifier");
					if (field != null)
					{
						string id = (string)field.GetValue(null);
						id = id.ToLower();
						if (id == identifier)
						{
							Config obj = (Config)Activator.CreateInstance("ConfigGen", "ConfigGen." + t.Name).Unwrap();
							return obj;
						}
					}
				}
			}

			return null;
		}

		// find a config item by identifier, returns null if not found
		public ConfigParameter FindParameterByIdentifier(string identifier)
		{
			identifier = identifier.ToLower();
			foreach (ConfigParameter item in configs)
			{
				if (item.identifier.ToLower() == identifier)
					return item;
			}
			return null;
		}

		// parse a value, either as a number, SuperIO mapping or reflection
		private bool ParseValue(string value, out int i)
		{
			i = 0;
			value = value.ToLower();
			if (TextUtils.IsCNumeric(value))	// parse as number
			{
				if (TextUtils.ParseInt(value, out i))
					return true;
				return false;
			}

			if (value.StartsWith("@"))			// copy another setting
			{
				ConfigParameter item = FindParameterByIdentifier(value.Substring(1));
				if (item != null)
				{
					i = item.value;
					return true;
				}
			}

			// parse as identifier
			i = SuperIO.IdentifierToIndex(value);
			if (i == -1)
				return false;
			return true;
		}
	}
}
