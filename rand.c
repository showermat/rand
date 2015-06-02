#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <getopt.h>

char *usage = "rand:  simple random integer generator using /dev/random\nrand [-hdxrun] [[min] max]\n-h\tThis help\n-d/-x\tOutput in decimal (default) or hexadecimal\n-r/-u\tUse /dev/random (default) or /dev/urandom\n-n num\tOutput num numbers (default 1)\nmin\tMinimum number (default 0)\nmax\tMaximum number (default UINT_MAX)\n";

void die(char *msg)
{
	fprintf(stderr, "%s", msg);
	exit(1);
}

int main(int argc, char **argv)
{
	// Important variables
	unsigned int min = 0, max = UINT_MAX;
	int n = 1;
	int base = 10, range = 0;
	char *randpath = "/dev/urandom";
	
	// Read arguments
	int opt;
	while ((opt = getopt(argc, argv, "dhn:rux")) != -1)
	{
		if (opt == 'h') die(usage);
		else if (opt == 'r') randpath = "/dev/random";
		else if (opt == 'u') randpath = "/dev/urandom";
		else if (opt == 'n')
		{
			errno = 0;
			n = strtol(optarg, NULL, 10);
			if (errno) die(usage);
		}
		else if (opt == 'd') base = 10;
		else if (opt == 'x') base = 16;
		else die(usage);
	}
	int optcnt = optind;
	if (optcnt < argc - 2)
	{
		fprintf(stderr, "Bad argument list.\n");
		die(usage);
	}
	else if (optcnt == argc - 1)
	{
		range = 1;
		errno = 0;
		max = strtol(argv[optcnt], NULL, 10);
		if (errno) die(usage);
	}
	else if (optcnt == argc - 2)
	{
		range = 1;
		errno = 0;
		min = strtol(argv[optcnt], NULL, 10);
		max = strtol(argv[optcnt + 1], NULL, 10);
		if (errno) die(usage);
	}
	else if (optcnt == argc);
	else die("Internal error 1");
	if (min > max)
	{
		int t = min;
		min = max;
		max = t;
	}
	
	// The trivial case
	if (min == max)
	{
		for (int i = 0; i < n; i++) printf("%u\n", min);
		return 0;
	}
	
	// Open the random file
	errno = 0;
	FILE *randfile = fopen(randpath, "r");
	if (randfile == NULL)
	{
		perror("Could not open random file for reading");
		return 1;
	}
	
	// Get and print random data
	int nchars = sizeof(unsigned int) / sizeof(char);
	unsigned char c[nchars];
	unsigned int rand;
	int cnt = 0;
	while (cnt < n)
	{
		rand = 0;
		for (int j = 0; j < nchars; j++)
		{
			int t = fgetc(randfile);
			if (t == -1) die("EOF");
			c[j] = (unsigned char) (t & 0xFF);
		}
		
		for (int j = 0; j < nchars; j++) rand |= c[j] << (j * sizeof(char) * 8);
		if (range) rand = rand % (max - min + 1) + min;
		if (base == 10) printf("%u\n", rand);
		else if (base == 16) printf("%x\n", rand);
		if (n > 0) cnt++;
	}
	
	return 0;
}
