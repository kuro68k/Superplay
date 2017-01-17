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
		public int index;
		public int value;
		public int min;
		public int max;
		
		public ConfigParameter(string _identifier, int _value)
		{
			identifier = _identifier;
			value = _value;
			min = 0;
			max = 255;
			index = 0;
		}

		public ConfigParameter(string _identifier, int _value, int _index)
		{
			identifier = _identifier;
			value = _value;
			min = 0;
			max = 255;
			index = _index;
		}

		public ConfigParameter(string _identifier, int _value, int _min, int _max)
		{
			identifier = _identifier;
			value = _value;
			min = _min;
			max = _max;
			index = 0;
		}
	}



	class Config
	{
		public string identifier;			// string identifier in config file
		public int id_number;				// logical ID number used by firmware
		public bool allows_multiple;		// more than one of this config is allowed
		public dynamic binary_struct;
		public List<ConfigParameter> configs;	// configurable items

		// compile parameters to binary format
		public byte[] CompileToBinary()
		{
			PopulateBinaryStruct();

			MemoryStream ms = new MemoryStream();
			FieldInfo[] fieldinfo = Type.GetType(GetType().FullName + "+BinaryFormat").GetFields(BindingFlags.Instance | BindingFlags.Public);
			foreach (FieldInfo field in fieldinfo)
			{
				dynamic d;
				int size;
				Type t = field.FieldType;
				if (t.IsArray)
				{
					Array array = field.GetValue(binary_struct) as Array;
					// convert to a 1D array for easy processing
					d = Array.CreateInstance(t.GetElementType(), array.Length);
					Buffer.BlockCopy(array, 0, d, 0, array.Length);

					size = Marshal.SizeOf(t.GetElementType());
					byte[] buffer = new byte[size];

					for (int i = 0; i < d.Length; i++)
					{
						IntPtr ptr = Marshal.AllocHGlobal(size);
						Marshal.StructureToPtr(d[i], ptr, false);
						Marshal.Copy(ptr, buffer, 0, size);
						Marshal.FreeHGlobal(ptr);
						ms.Write(buffer, 0, buffer.Length);
					}
				}
				else
				{
					d = Convert.ChangeType(field.GetValue(binary_struct), t);
					size = Marshal.SizeOf(d);

					byte[] buffer = new byte[size];
					IntPtr ptr = Marshal.AllocHGlobal(size);
					Marshal.StructureToPtr(d, ptr, false);
					Marshal.Copy(ptr, buffer, 0, size);
					Marshal.FreeHGlobal(ptr);
					ms.Write(buffer, 0, buffer.Length);
				}
			}

			return ms.ToArray();
		}

		// fill out BinaryFormat struct with data from parameters and class
		private void PopulateBinaryStruct()
		{
			int byte_size = 0;

			// fill out all non-custom fields
			FieldInfo[] fieldinfo = Type.GetType(GetType().FullName + "+BinaryFormat").GetFields(BindingFlags.Instance | BindingFlags.Public);
			if (fieldinfo == null)
				throw new Exception("Config " + identifier + " does not have a BinaryFormat struct.");

			foreach (FieldInfo field in fieldinfo)
			{
				string field_id = field.Name;

				// mapping array
				if (field_id == "_mapping")
				{
					int count = CountNonZeroParams();
					foreach (FieldInfo fi in fieldinfo)
					{
						if (fi.Name == "_count")
							fi.SetValue(binary_struct, (sbyte)count);
					}

					sbyte[,] map = new sbyte[count, 2];
					byte_size += count * 2;

					count = 0;
					for (int i = 0; i < configs.Count; i++)
					{
						if (configs[i].value != 0)
						{
							map[count, 0] = (sbyte)configs[i].index;
							map[count, 1] = (sbyte)configs[i].value;
							count++;
						}
					}

					field.SetValue(binary_struct, map);
					continue;
				}

				// config index number
				if (field_id == "_id")
				{
					field.SetValue(binary_struct, Convert.ChangeType(id_number, field.FieldType));
					continue;
				}

				// unknown custom field
				if (field_id.StartsWith("_"))
				{
					byte_size += Marshal.SizeOf(field.FieldType);
					continue;
				}

				// array fields need to be split into individual numbered parameters
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
					byte_size += Marshal.SizeOf(field.FieldType.GetElementType()) * array.Length;
					continue;
				}

				// standard field
				ConfigParameter param = FindParameterByIdentifier(field_id);
				if (param == null)
					throw new Exception("Struct field with no matching parameter (" + field_id + ")");
				field.SetValue(binary_struct, Convert.ChangeType(param.value, field.FieldType));
				byte_size += Marshal.SizeOf(field.FieldType);
			}

			// custom fields
			//Type type = GetType();
			//MethodInfo method = type.GetMethod("CustomBinaryFormat");
			//if (method != null)
			//	method.Invoke(this, null);

			fieldinfo = Type.GetType(GetType().FullName + "+BinaryFormat").GetFields(BindingFlags.Instance | BindingFlags.Public);
			foreach (FieldInfo field in fieldinfo)
			{
				if (field.Name == "_config_length")
				{
					field.SetValue(binary_struct, (UInt16)byte_size);
					continue;
				}
			}
		}

		// dump all config items in this config
		public void DumpToConsole()
		{
			Console.WriteLine(identifier + " (" + id_number.ToString() + ")");
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

					if (Program.opt_verbose)
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

		// count the number of non-zero value parameters
		public int CountNonZeroParams()
		{
			int count = 0;
			for (int i = 0; i < configs.Count; i++)
			{
				if (configs[i].value != 0)
					count++;
			}
			return count;
		}
	}
}
