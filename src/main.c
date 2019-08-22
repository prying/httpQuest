#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "getResponse.h"
#include "requestTypes.h"

#define MAX_PORT 65535

// default port
#define PORT 80
// default request type
#define TYPE "GET"

void usage(char * name);

int main(int argc, char ** argv){
	int opt;
	bool needAddress = true;
	requestHint_t request;
	memset(&request, 0,sizeof(requestHint_t));

	// Set up default request
	request.port 	= PORT;
	request.type 	= TYPE;
	request.allFile	= false;
	request.numArg 	= 0;
	request.argAccept = false;
	request.paylaod = NULL;
	request.silence = false;
	request.immutable = false;


	while((opt = getopt(argc, argv, "p:t:f:si"))!=-1){
		switch (opt)
		{
		case 'p': // Specified port
			if (1<=atoi(optarg) && atoi(optarg)<=MAX_PORT){
				request.port = atoi(optarg);
			} else {
				fprintf(stderr, "Port no in valid range\n");
				exit(EXIT_FAILURE);
			}
			break;
		case 't': // Specify request type 
			request.type = optarg;
			break;
		case 'f': // Specify file
			request.file = optarg;
			request.allFile = true;
		case 's': // Run silently TODO
			request.silence = true;
			break;
		case 'i': // Make input immutable
			request.immutable = true;
			break;
		default: 
		    usage(argv[0]);
            exit(EXIT_FAILURE);
		}
	}

	// Get address
	if (needAddress && (optind < argc)){
		request.url = argv[optind];
	} else {
		fprintf(stderr, "Expected URL\n");
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	tcpRequest(&request);

	return 0;
}

void usage(char * name){
	fprintf(stderr, "Usage: %s -[s|i] [-p port] [-f file] [-t type] [URL]\n", name);
	fprintf(stderr, "\t-s\tSilence the output\n");
	fprintf(stderr, "\t-i\tMake input immutable\n");
}