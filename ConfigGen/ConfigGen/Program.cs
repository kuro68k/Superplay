﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;
using System.IO;

namespace ConfigGen
{
	class Program
	{
		static bool opt_generate_hex = false;
		static bool opt_generate_bin = false;
		static string opt_input_file;
		static string opt_output_file;
		public static bool opt_verbose = false;

		static void Main(string[] args)
		{
			if (!ParseArgs(args))
				return;

			//var types = GetCfgTypes();
			//foreach (var t in types)
			//{
			//	//Console.WriteLine(t.identifier + " (" + t.ToString() + ")");
			//	t.DumpToConsole();
			//}

			List<Config> config_list = Config.LoadConfigFromFile("config.txt");
			if (config_list == null)
			{
				Console.WriteLine("Failed to load config.txt.");
				return;
			}

			List<byte[]> buffer_list = new List<byte[]>();
			int byte_size = 0;
			foreach (Config cfg in config_list)
			{
				byte[] buffer = cfg.CompileToBinary();
				byte_size += buffer.Length;
				buffer_list.Add(buffer);
				Console.WriteLine(cfg.identifier + " = " + buffer.Length + " bytes");
			}

			Console.WriteLine(buffer_list.Count.ToString() + " config(s) generated, totaling " + byte_size.ToString() + " bytes.");
			if (opt_generate_bin)
				GenerateBinFile(buffer_list);
			if (opt_generate_hex)
				GenerateHexFile(buffer_list);
		}

		private static bool ParseArgs(string[] args)
		{
			if (args.Length < 2)
			{
				PrintUsage();
				return false;
			}

			int non_option_count = 0;
			for (int i = 0; i < args.Length; i++)
			{
				// option
				if ((args[i].StartsWith("-")) || (args[i].StartsWith("/")))
				{
					string a = args[i].Substring(1);
					switch(a)
					{
						case "h":
						case "hex":
							opt_generate_hex = true;
							break;

						case "b":
						case "bin":
							opt_generate_bin = true;
							break;

						case "v":
						case "verbose":
							opt_verbose = true;
							break;

						default:
							Console.WriteLine("Unknown option \"" + a + "\"");
							PrintUsage();
							return false;
					}
				}
				// file name
				else
				{
					switch(non_option_count)
					{
						case 0:
							opt_input_file = args[i];
							break;
						case 1:
							opt_output_file = args[i];
							break;
						default:
							Console.WriteLine("Extra filename \"" + args[i] + "\"");
							return false;
					}
					non_option_count++;
				}
			}

			if (non_option_count != 2)
			{
				PrintUsage();
				return false;
			}
			return true;
		}

		private static void PrintUsage()
		{
			Console.WriteLine("ConfigGen [options] <config file> <output file>");
			Console.WriteLine("  -b,bin\tProduce binary file");
			Console.WriteLine("  -h,hex\tProduce hex file");
			Console.WriteLine("  -v,verbose\tVerbose output");
		}

		private static bool GenerateBinFile(List<byte[]> buffer_list)
		{
			BinaryWriter bw;
			try
			{
				bw = new BinaryWriter(File.Open(opt_output_file, FileMode.Create));
			}
			catch
			{
				Console.WriteLine("Unable to open \"" + opt_output_file + "\" for writing.");
				return false;
			}

			foreach(byte[] buffer in buffer_list)
				bw.Write(buffer);

			bw.Close();
			Console.WriteLine("Wrote \"" + opt_output_file + "\".");
			return true;
		}

		private static bool GenerateHexFile(List<byte[]> buffer_list)
		{
			StreamWriter sw;
			try
			{
				sw = new StreamWriter(opt_output_file);
			}
			catch
			{
				Console.WriteLine("Unable to open \"" + opt_output_file + "\" for writing.");
				return false;
			}

			MemoryStream ms = new MemoryStream();
			foreach (byte[] buffer in buffer_list)
				ms.Write(buffer, 0, buffer.Length);
			byte[] terminator = new byte[2] { 0, 0 };
			ms.Write(terminator, 0, 2);
			//byte[] buffer = new byte[512];
			//for (int i = 0; i < buffer.Length; i++)
			//	buffer[i] = 0xFF;
			//ms.Write(buffer, 0, buffer.Length);
			
			sw.Write(IntelHEX.GenerateHEX(ms));

			sw.Close();
			Console.WriteLine("Wrote \"" + opt_output_file + "\".");
			return true;
		}

		// find all available config types using reflection
		private static List<Config> GetCfgTypes()
		{
			var types = Assembly
				.GetExecutingAssembly()
				.GetTypes()
				.Where(t => t.Namespace.StartsWith("ConfigGen"));

			List<Config> cfgs = new List<Config>();

			foreach (var t in types)
			{
				if (t.Name.StartsWith("cfg"))
				{
					Config obj = (Config)Activator.CreateInstance("ConfigGen", "ConfigGen." + t.Name).Unwrap();
					cfgs.Add(obj);
				}
			}

			return cfgs;
		}
	}
}
