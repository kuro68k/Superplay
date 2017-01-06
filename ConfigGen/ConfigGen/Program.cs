using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;

namespace ConfigGen
{
	class Program
	{
		static void Main(string[] args)
		{
			var types = GetCfgTypes();
			foreach (var t in types)
			{
				Console.WriteLine(t.identifier + " (" + t.ToString() + ")");
			}
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
