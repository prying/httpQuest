#ifndef REQUESTTYPES_H
#define REQUESTTYPES_H

#include <stdbool.h>

typedef struct
{
	// URL form argv as a string
	char *url;
	int port;
	char *type;
    int numArg;
	char **arguments;
	bool argAccept;
	// Take header as is and only header (NO URL)
	bool allFile;
	char *file;
	char *paylaod;
	bool silence;
	// No edit
	bool immutable;
}requestHint_t;

#endif