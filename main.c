/* 
 * FunCube Dongle command line interface
 * David Pello EA1IDZ 2011
 *
 * This code is licensed under a GNU GPL licensed
 * See LICENSE for information
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "fcd.h"

const char* program_name;

void print_help()
{
	printf("USAGE: %s options [arguments]\n", program_name);
	printf("     -s   --status			Gets FCD current status\n");
	printf("     -f   --frequency <frequency>	Sets FCD frequency in Hz\n");
	printf("     -c   --correction <correction>	Sets frequency correction in ppm\n");
	printf("     -h   --help       			Shows this help\n");
}

void print_status()
{
	int stat;
	char version[20];

	stat = fcdGetMode();

	if (stat == FCD_MODE_NONE)
	{
		printf("No FCD Detected.\n");
		return;
	}
	else if (stat == FCD_MODE_BL)
	{
		printf("FCD present in bootloader mode.\n");
		return;
	}
	else	
	{
		printf("FCD present in application mode.\n");
		stat = fcdGetFwVerStr(version);
		printf("FCD firmware version: %s.\n", version);
		return;
	}
}

int main(int argc, char* argv[])
{
	int stat;
	int freq = 0;
	int corr = 0;

	/* getopt infrastructure */
	int next_option;
	const char* const short_options = "sf:c:h";
	const struct option long_options[] =
	{
		{ "status", 0, NULL, 's' },
		{ "frequency", 1, NULL, 'f' },
		{ "correction", 1, NULL, 'c' },
		{ "help", 0, NULL, 'h' }
	};


	/* save program name */
	program_name = argv[0];

	if (argc == 1)
	{
		print_help();
		exit(EXIT_SUCCESS);
	}

	while(1)
	{
		/* call getopt */
		next_option = getopt_long(argc, argv, short_options, long_options, NULL);

		/* end of the options */
		if (next_option == -1)
			break;

		switch (next_option)
		{
			case 'h' :
				print_help();
				exit(EXIT_SUCCESS);
			case 's' :
				print_status();
				exit(EXIT_SUCCESS);
			case 'f' :
				freq = atoi(optarg);
				break;
			case 'c' :
				corr = atoi(optarg);
				break;
			case '?' :
				print_help();
				exit(1);
			default :
				abort();
		}	
	}

	/* calculate frequency */
	freq *= 1.0 + corr / 1000000.0;
	
	/* set it */
	stat = fcdAppSetFreq(freq);
	if (stat == FCD_MODE_NONE)
	{
		printf("No FCD Detected.\n");
		return 1;
	}
	else if (stat == FCD_MODE_BL)
	{
		printf("FCD in bootloader mode.\n");
		return 1;
	}
	else	
	{
		printf("Freq set to %d.\n", freq);
		return EXIT_SUCCESS;
	}

	return EXIT_SUCCESS;


}
