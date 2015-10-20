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

#define	DEBUG	1
#define DEBUG_PRINTF(fmt, ...) \
		do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)

char *infile = NULL;
char *outfile = NULL;
bool optd = false;


/**************************************************************************************************
* Hand command line args
*
* -o <outfile>		save output to file
* -d				modify default mapping
*/
int parse_args(int argc, char* argv[])
{
	int c;

	if (argc == 1)
	{
		printf("Usage: [-d] [-o output.eep] map.txt\n");
		printf("\t-d\tModify default mapping\n");
		return 1;
	}

	while ((c = getopt(argc, argv, "do:")) != -1)
	{
		switch (c)
		{
		case 'o':
			outfile = optarg;
			break;

		case 'd':
			optd = true;
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
* main()
*/
int main(int argc, char* argv[])
{
	MAPPING_CONFIG_t cfg;
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
		error = false;
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
}
