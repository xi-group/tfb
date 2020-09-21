#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statvfs.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <errno.h>

#include <fcntl.h>

#ifndef	FALLOC_FL_COLLAPSE_RANGE
#define	FALLOC_FL_COLLAPSE_RANGE	0x08
#endif

#define	SIZE_ARG_MAX	32

void print_help(char *progname)
{
	printf("\n");
	printf("\t%s : Trim Files from Beginning\n", progname);
	printf("\t\t-h : Print this help message\n");
	printf("\t\t-s : Size to trim\n");
	printf("\n");
	printf("\tNotes:\n");
	printf("\t\tSize is an integer representing number of bytes to trim and optional unit.\n");
	printf("\t\tUnits can be B,K,M,G. 1K equals 1024 bytes, 1M equals 1024K, etc.\n");
	printf("\n");
	printf("\tExample:\n");
	printf("\t\t%s -h\n", progname);
	printf("\t\t%s -s 10M FILE\n", progname);
	printf("\n");

	return;
}

unsigned int parse_size_arg(char *size_arg)
{
	char		units = -1, *base = NULL;
	int		i = -1;
	long int	multiplier = -1, len = -1;

	/* Invalid input */	
	if (size_arg == NULL)
		return 0;

	/* Validate nnn[B|K|M|G] format */
	for (i = 0; i < strlen(size_arg) - 1; i++)
		if ((size_arg[i] < '0') || (size_arg[i] > '9'))
			return 0;

	/* Get the units identifier */
	units = size_arg[strlen(size_arg) - 1];

	switch (units) {
	case 'B':
	case 'b':
		multiplier = 1;
		break;
	case 'K':
	case 'k':
		multiplier = 1024;
		break;
	case 'M':
	case 'm':
		multiplier = 1024 * 1024;
		break;
	case 'G':
	case 'g':
		multiplier = 1024 * 1024 * 1024;
		break;
	default:
		return 0;
	}

	/* Calculate total number of bytes */
	if ((base = strndup(size_arg, strlen(size_arg) - 1)) == NULL)
		return 0;

	len = strtol(base, NULL, 10);
	if ((len == LONG_MIN) || (len == LONG_MAX) || (len == -1))
		return 0;

	/* Cleanup */
	free(base);

	return len * multiplier;
}

int main(int argc, char * argv[])
{
	int 			ret = -1, fd = 1, ch = -1;
	char			*filename = NULL;
	char			*size_arg = NULL;
	unsigned long int	trim_bytes = 0, adjust = 0;
	struct stat		file_stat;
	struct statvfs		fs_stat;

	/* Check minimum number of arguments */
	if (argc < 3) {
		print_help(argv[0]);
		exit(-1);
	}

	filename = strndup(argv[argc], PATH_MAX);

	/* Parse command line parameters */
	while ((ch = getopt(argc, argv, "s:h")) != -1) {
		switch (ch) {
		case 's':
			size_arg = strndup(optarg, SIZE_ARG_MAX);
			break;
		case 'h':
		default:
			print_help(argv[0]);
			exit(-1);
		}
	}
	argc -= optind;
	argv += optind;

	/* Check for required arguments */
	if ((filename == NULL) || (size_arg == NULL)) {
		print_help(argv[0]);
		exit(-1);
	}

	/* Parse size argument */
	if ((trim_bytes = parse_size_arg(size_arg)) == 0) {
		fprintf(stderr, "Requested invalid trim size! Exiting ...\n");
		exit(-1);
	}

	/* Obtain filesystem stats */
	if(statvfs(filename, &fs_stat) == -1) {
		perror("statvfs():");
		exit(-1);
	}

	/* Calculate trim_bytes as multiple of logical block size */
	adjust = trim_bytes / fs_stat.f_bsize;
	trim_bytes = adjust * fs_stat.f_bsize;

	/* Obtain file stats */
	if(stat(filename, &file_stat) == -1) {
		perror("stat():");
		exit(-1);
	}
	
	/* Check for regular file */
	if (!(file_stat.st_mode & S_IFREG)) {
		fprintf(stderr, "Not a regular file! Exiting ...\n");
		exit(-1);
	}
	
	/* Check if file size allows trimming */
	if (file_stat.st_size <= trim_bytes) {
		fprintf(stderr, "File size less than requested trim size! Exiting ...\n");
		exit(-1);
	}

	/* Trim the file from the beginning */
	if ((fd = open(filename, O_RDWR)) == -1) {
		perror("open():");
		exit(-1);
	}

	if((ret = fallocate(fd, FALLOC_FL_COLLAPSE_RANGE, 0, trim_bytes)) == -1) {
		perror("fallocate():");
		exit(-1);
	}

	/* Cleanup */
	close(fd);
	free(filename);
	free(size_arg);

	return 0;
}

