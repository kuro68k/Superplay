using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace ConfigGen
{
	class IntelHEX
	{
		// convert a stream to a HEX file stream of characters
		public static TextWriter GenerateHEX(Stream input)
		{
			TextWriter output = new StringWriter();

			input.Seek(0, SeekOrigin.Begin);
			sbyte checksum = 0;
			for (int address = 0; address < input.Length; address++)
			{
				if (address % 0x10 == 0)	// new line
				{
					checksum = (sbyte)(~checksum);
					checksum++;
					if (address != 0)
						output.WriteLine(checksum.ToString("X2"));

					output.Write(":");
					int available_bytes = (int)input.Length - address;
					if (available_bytes > 0x10)
						available_bytes = 0x10;
					output.Write(available_bytes.ToString("X2"));
					output.Write(address.ToString("X4"));
					output.Write("00");

					checksum = (sbyte)(available_bytes);
					checksum += (sbyte)(address >> 8);
					checksum += (sbyte)(address & 0xFF);
				}

				sbyte b = (sbyte)input.ReadByte();
				output.Write(b.ToString("X2"));
				checksum += b;
			}

			checksum = (sbyte)(~checksum);
			checksum++;
			output.WriteLine(checksum.ToString("X2"));
			output.WriteLine(":00000001FF");

			return output;
		}
	}
}
