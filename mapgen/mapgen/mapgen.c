/* mapping config generator for SUPERPLAY
*/

#include "stdio.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "names.h"
#include "getopt.h"
#include "config.h"
#include "crc.h"
#include "hidapi.h"

#define	DEBUG	1
#define DEBUG_PRINTF(fmt, ...) \
		do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)


#define	BUFFER_SIZE			(64+1)		// +1 for mandatory HID report ID
#define	IDX_REPORT_ID		0
#define	IDX_COMMAND			1
#define	IDX_ADDR			2
#define	IDX_DATA			4


char *infile = NULL;
char *outfile = NULL;
bool optd = false;
bool optu = false;
MAPPING_CONFIG_t cfg;


/**************************************************************************************************
* Hand command line args
*
* -o <outfile>		save output to file
* -d				modify default mapping
* -u				upload to joystick
*/
int parse_args(int argc, char* argv[])
{
	int c;

	if (argc == 1)
	{
		printf("Usage: [-u] [-d] [-o output.eep] map.txt\n");
		printf("\t-d\tModify default mapping\n");
		printf("\t-u\tUpload to joystick\n");
		return 1;
	}

	while ((c = getopt(argc, argv, "udo:")) != -1)
	{
		switch (c)
		{
		case 'o':
			outfile = optarg;
			break;

		case 'd':
			optd = true;
			break;

		case 'u':
			optu = true;
			break;

		case '?':
			if (optopt == 'o')
				printf("-%c requres an argument.\n", optopt);
			else if (isprint(optopt))
				printf("Unknown option -%c.\n", optopt);
			else
				printf("Unknown option character.\n");
			return 1;
		}
	}

	/*
	printf("optind: %d\n", optind);
	printf("argc: %d\n", argc);
	for (int i = optind; i < argc; i++)
		printf("Non-option argument %d: %s\n", i, argv[i]);
	*/

	//if ((argc - optind) != 1)
	if (optind == 0)
	{
		printf("One input file required.\n");
		return 1;
	}
	infile = argv[optind];

	return 0;
}

/**************************************************************************************************
* Remove spaces other crud
*/
int clean_line(const char *line, char *clean)
{
	int len = strnlen(line, 254);
	int p = 0;
	
	for (int i = 0; i < len; i++)
	{
		if ((line[i] != ' ') &&
			(line[i] != '\t') &&
			(line[i] != '\n') &&
			(line[i] != '\r'))
			clean[p++] = line[i];
	}
	clean[p] = '\0';

	return p;
}

/**************************************************************************************************
* Tokenize a string into an index to the logical_names[] table. If the string is not found return
* -1.
*/
int tokenize(char *str)
{
	for (int i = 0; i < NUM_NAMES; i++)
	{
		if (strcmp(str, logical_names[i]) == 0)
			return i;
	}
	return -1;
}

/**************************************************************************************************
* Process input file
*/
int read_input_file(void)
{
	FILE *fin;
	fin = fopen(infile, "r");
	if (fin == NULL)
	{
		printf("Error opening %s.\n", infile);
		return 1;
	}

	int map_num = 0;
	int line_num = 0;
	bool error = false;
	while (!feof(fin))
	{
		line_num++;
		char line[255];
		if (fgets(line, 255, fin) == NULL)	// EOF
		{
			//error = true;
			//printf("Error reading line %d.\n", line_num);
			break;
		}

		int len = strnlen(line, 255);
		if (len == 255)
		{
			error = true;
			printf("Line %d too long.\n", line_num);
			printf("%s\n", line);
			break;
		}

		char clean[255];
		len = clean_line(line, clean);
		DEBUG_PRINTF("Cleaned line: \"%s\" (%d)\n", clean, len);

		if (len == 0)
			continue;
		if (line[0] == ';')		// comment
			continue;

		if (clean[0] == '=')
		{
			error = true;
			printf("Missing first parameter on line %d.\n", line_num);
			printf("%s\n", line);
			break;
		}

		char *eq = strchr(clean, '=');
		if (eq == NULL)
		{
			error = true;
			printf("Line %d does not contain an equals (=) sign.\n", line_num);
			printf("%s\n", line);
			break;
		}

		*eq = '\0';		// create a null terminated string of the first parameter
		eq++;			// no points to the second parameter

		bool output = false;
		if (*eq == '/')	// output
		{
			output = true;
			eq++;
		}

		if ((eq - clean) >= len)
		{
			error = true;
			printf("Missing second parameter on line %d.\n", line_num);
			printf("%s\n", line);
			break;
		}
		DEBUG_PRINTF("eq:  %d\nlen: %d\n", eq - clean, len);

		int map_logical = tokenize(clean);
		if (map_logical == -1)
		{
			error = true;
			printf("First parameter does not match any known mapping on line %d.\n", line_num);
			printf("%s\n", line);
			break;
		}
		if (map_logical >= NUM_LOGICAL_INPUTS)
		{
			error = true;
			printf("Logical input out of range on line %d.\n", line_num);
			printf("%s\n", line);
		}

		int map_physical;
		if (*eq == '!')	// forced
			map_physical = 127;
		else
			map_physical = tokenize(eq);
		if (map_physical == -1)
		{
			error = true;
			printf("Second parameter does not match any known mapping on line %d.\n", line_num);
			printf("%s\n", line);
			break;
		}

		if (output)
			map_physical |= 0x80;
		DEBUG_PRINTF("%d -> %d\n", map_logical, map_physical);
		//cfg.ltop[map_logical] = map_physical;

		if (map_num >= NUM_MAPPINGS)
		{
			error = true;
			printf("Exceeded the maximum number of mappings (%d) on line %d.\n", NUM_MAPPINGS, line_num);
			printf("%s\n", line);
		}

		cfg.logical[map_num] = map_logical;
		cfg.physical[map_num] = map_physical;
		map_num++;
	}

	fclose(fin);
	if (error)
	{
		printf("Parsing failed.\n");
		return 1;
	}
	printf("Parsing OK.\n");

	return 0;
}

/**************************************************************************************************
* Execute a HID "command". Set timeout_ms to zero if no response is required.
*/
bool ExecuteHIDCommand(hid_device *handle, uint8_t command, uint16_t addr, int timeout_ms, uint8_t *buffer)
{
	int res;

	// send command
	buffer[IDX_REPORT_ID] = 0;
	buffer[IDX_COMMAND] = command;
	buffer[IDX_ADDR] = addr & 0xFF;
	buffer[IDX_ADDR + 1] = (addr >> 8) & 0xFF;
	res = hid_send_feature_report(handle, buffer, BUFFER_SIZE);
	if (res == -1)
	{
		printf("hid_send_feature_report failed.\n");
		printf("%ls\n", hid_error(handle));
		return false;
	}

	// wait for response
	if (timeout_ms == 0)	// no response required
		return true;

	res = hid_read_timeout(handle, buffer, BUFFER_SIZE, timeout_ms);
	if (res == -1)
	{
		printf("hid_read failed.\n");
		printf("%ls\n", hid_error(handle));
		return false;
	}

	// validate response
	if (buffer[IDX_COMMAND - 1] != (command | 0x80))
		return false;
	if ((buffer[IDX_ADDR - 1] != (addr & 0xFF)) || (buffer[IDX_ADDR] != ((addr >> 8) & 0xFF)))
		return false;

	return true;
}

/**************************************************************************************************
* Upload config
*/
int upload_config(void)
{
	uint8_t buffer[BUFFER_SIZE];

	hid_device *handle;
	handle = hid_open(0x8282, 0x6099, NULL);
	if (handle != NULL)
	{
		//printf("Unable to find any devices to upload to.\n");
		printf("Found SUPERPLAY joystick.\n");
		if (!ExecuteHIDCommand(handle, 0x5B, 0, 0, buffer))
		{
			printf("KBUS command failed.\n");
			return 1;
		}
		printf("Waiting for bootloader...\n");
	}

	return 0;
}

/**************************************************************************************************
* main()
*/
int main(int argc, char* argv[])
{
	memset(&cfg, 0, sizeof(cfg));
	cfg.config_id = MAPPING_CONFIG_ID;
	cfg.config_size = sizeof(cfg);

	int res;
	if ((res = parse_args(argc, argv)) != 0)
		return res;

	if (optd)
	{
		for (int i = 0; i < NUM_LOGICAL_INPUTS; i++)
		{
			cfg.logical[i] = i;
			cfg.physical[i] = i;
		}
	}

	res = read_input_file();
	if (res != 0)
		return res;

	cfg.crc32 = crc32(&cfg, sizeof(cfg) - 4);
	DEBUG_PRINTF("Config CRC: %08X\n", cfg.crc32);

	// try to save if required
	if (outfile != NULL)
	{
		FILE *fout;
		fout = fopen(outfile, "wb");
		if (fout == NULL)
		{
			printf("Error opening %s.\n", outfile);
			return 1;
		}
		bool error = false;
		if (fwrite(&cfg, 1, sizeof(cfg), fout) != sizeof(cfg))
		{
			error = true;
			printf("Unable to write to %s.\n", outfile);
		}
		fclose(fout);
		if (error)
			return false;
		printf("Wrote config to %s.\n", outfile);
	}

	// try to upload if required
	if (optu)
	{
		res = upload_config();
		if (res != 0)
			return res;
	}

	return 0;
}
