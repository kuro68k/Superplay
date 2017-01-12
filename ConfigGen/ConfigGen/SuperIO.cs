using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConfigGen
{
	class SuperIO
	{
		static Dictionary<string, int> _logicalDict = new Dictionary<string, int>
		{
			// logical I/O
			{ "off", 0x00 },
			{ "up", 0x01 },
			{ "down", 0x02 },
			{ "left", 0x03 },
			{ "right", 0x04 },
			{ "meta", 0x0F },

			{ "button1", 0x10 },
			{ "button2", 0x11 },
			{ "button3", 0x12 },
			{ "button4", 0x13 },
			{ "button5", 0x14 },
			{ "button6", 0x15 },
			{ "button7", 0x16 },
			{ "button8", 0x17 },
			{ "button9", 0x18 },
			{ "button10", 0x19 },
			{ "button11", 0x1A },
			{ "button12", 0x1B },
			{ "button13", 0x1C },
			{ "button14", 0x1D },
			{ "button15", 0x1E },
			{ "button16", 0x1F },

			// button aliases
			{ "select", 0x1E },
			{ "start", 0x1F },

			{ "af_low1", 0x20 },
			{ "af_low2", 0x21 },
			{ "af_low3", 0x22 },
			{ "af_low4", 0x23 },
			{ "af_low5", 0x24 },
			{ "af_low6", 0x25 },
			{ "af_low7", 0x26 },
			{ "af_low8", 0x27 },
			{ "af_low9", 0x28 },
			{ "af_low10", 0x29 },
			{ "af_low11", 0x2A },
			{ "af_low12", 0x2B },
			{ "af_low13", 0x2C },
			{ "af_low14", 0x2D },
			{ "af_low15", 0x2E },
			{ "af_low16", 0x2F },

			{ "af_high1", 0x30 },
			{ "af_high2", 0x31 },
			{ "af_high3", 0x32 },
			{ "af_high4", 0x33 },
			{ "af_high5", 0x34 },
			{ "af_high6", 0x35 },
			{ "af_high7", 0x36 },
			{ "af_high8", 0x37 },
			{ "af_high9", 0x38 },
			{ "af_high10", 0x39 },
			{ "af_high11", 0x3A },
			{ "af_high12", 0x3B },
			{ "af_high13", 0x3C },
			{ "af_high14", 0x3D },
			{ "af_high15", 0x3E },
			{ "af_high16", 0x3F },

			{ "rotary1", 0x40 },
			{ "rotary2", 0x41 },
			{ "rotary3", 0x42 },
			{ "rotary4", 0x43 },
			{ "rotary5", 0x44 },
			{ "rotary6", 0x45 },
			{ "rotary7", 0x46 },
			{ "rotary8", 0x47 },
			{ "rotary9", 0x48 },
			{ "rotary10", 0x49 },
			{ "rotary11", 0x4A },
			{ "rotary12", 0x4B },
			{ "rotary13", 0x4C },
			{ "rotary14", 0x4D },
			{ "rotary15", 0x4E },
			{ "rotary16", 0x4F },

			{ "led1", 0x70 },
			{ "led2", 0x71 },
			{ "led3", 0x72 },
			{ "led4", 0x73 },
			{ "led5", 0x74 },
			{ "led6", 0x75 },
			{ "led7", 0x76 },
			{ "led8", 0x77 },
			{ "led9", 0x78 },
			{ "led10", 0x79 },
			{ "led11", 0x7A },
			{ "led12", 0x7B },
			{ "led13", 0x7C },
			{ "led14", 0x7D },
			{ "led15", 0x7E },
			{ "led16", 0x7F },
		};

		static Dictionary<string, int> _physicalDict = new Dictionary<string, int>
		{
			// physical I/O
			{ "_off", 0x80 },
			{ "_up", 0x81 },
			{ "_down", 0x82 },
			{ "_left", 0x83 },
			{ "_right", 0x84 },
			{ "_meta", 0x8F },

			{ "_b1", 0x90 },
			{ "_b2", 0x91 },
			{ "_b3", 0x92 },
			{ "_b4", 0x93 },
			{ "_b5", 0x94 },
			{ "_b6", 0x95 },
			{ "_b7", 0x96 },
			{ "_b8", 0x97 },
			{ "_b9", 0x98 },
			{ "_b10", 0x99 },
			{ "_b11", 0x9A },
			{ "_b12", 0x9B },
			{ "_b13", 0x9C },
			{ "_b14", 0x9D },
			{ "_b15", 0x9E },
			{ "_b16", 0x9F },

			// button aliases
			{ "_coin", 0x9E },
			{ "_start", 0x9F },

			// autofire
			{ "_a1", 0xA0 },
			{ "_a2", 0xA1 },
			{ "_a3", 0xA2 },
			{ "_a4", 0xA3 },
			{ "_a5", 0xA4 },
			{ "_a6", 0xA5 },
			{ "_a7", 0xA6 },
			{ "_a8", 0xA7 },
			{ "_a9", 0xA8 },
			{ "_a10", 0xA9 },
			{ "_a11", 0xAA },
			{ "_a12", 0xAB },
			{ "_a13", 0xAC },
			{ "_a14", 0xAD },
			{ "_a15", 0xAE },
			{ "_a16", 0xAF },

			// rotary
			{ "_r1", 0xB0 },
			{ "_r2", 0xB1 },
			{ "_r3", 0xB2 },
			{ "_r4", 0xB3 },
			{ "_r5", 0xB4 },
			{ "_r6", 0xB5 },
			{ "_r7", 0xB6 },
			{ "_r8", 0xB7 },
			{ "_r9", 0xB8 },
			{ "_r10", 0xB9 },
			{ "_r11", 0xBA },
			{ "_r12", 0xBB },
			{ "_r13", 0xBC },
			{ "_r14", 0xBD },
			{ "_r15", 0xBE },
			{ "_r16", 0xBF },
		};

		public static int IdentifierToIndex(string id)
		{
			int result = -1;
			if (_logicalDict.TryGetValue(id, out result))
				return result;
			if (_physicalDict.TryGetValue(id, out result))
				return result;
			return -1;
		}

		public static void AddLogicalMappings(ref List<ConfigParameter> configs)
		{
			foreach (KeyValuePair<string, int> pair in _logicalDict)
			{
				configs.Add(new ConfigParameter(pair.Key, pair.Value));
			}
		}

		public static void AddPhysicalMappings(ref List<ConfigParameter> configs)
		{
			foreach (KeyValuePair<string, int> pair in _physicalDict)
			{
				configs.Add(new ConfigParameter(pair.Key, pair.Value));
			}
		}
	}
}
