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

	while((opt = getopt(argc, argv, "p:t:f:s"))!=-1){
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
			printf("option s\n");
			break;
		default: 
		    fprintf(stderr, "Usage: %s [-p port] [-s] name\n",argv[0]);
            exit(EXIT_FAILURE);
		}
	}

	// Get address
	if (needAddress && (optind < argc)){
		request.url = argv[optind];
	} else {
		fprintf(stderr, "Expected URL\n");
		fprintf(stderr, "Usage: %s [-p port] [-s] name\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	tcpRequest(&request);

	return 0;
}