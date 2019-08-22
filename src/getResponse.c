#include "getResponse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>  
#include <netdb.h>

#include "requestTypes.h"

#define PORT 80

#define HEADER_SIZE 256
#define HEADER_HTTP "HTTP/1.1"
#define HEADER_HOST "Host: "
#define HEADER_BREAK "\r\n"
#define HEADER_ACCEPT "Accept: */*"

int tcpRequest(requestHint_t *hint);
int printResponse(int *sock, requestHint_t *hint);
int sendHeader(int *sock, requestHint_t *hint);
int domainFormURL(char* buff, size_t size, char *url);
int connectToServer(int *socket, char *URL, int port);
char* ipToString(struct sockaddr_in *list);

int tcpRequest(requestHint_t *hint){

	int sock = 0;
	// Connect to server
	if (connectToServer(&sock, hint->url, hint->port) != 1){
		fprintf(stderr, "Failed to connect to server\n");
		close(sock);
		return -1;
	}

	// Send the header
	if (sendHeader(&sock, hint) != 1){
		fprintf(stderr, "Failed to send data to server\n");
		close(sock);
		return -1;
	}

	// Print Response
	if (printResponse(&sock, hint) != 1){
		fprintf(stderr, "Failed to read server response\n");
		close(sock);
		return -1;
	}

	close(sock);
	return 1;
}

int printResponse(int *sock, requestHint_t *hint){
	int msgLen = 0;
	bool msgCompleate = false;
	char buff[128];

	// Get message
	while (!msgCompleate){
		msgLen = recv(*sock, buff, sizeof(buff), 0);
		if (msgLen ==-1){
			fprintf(stderr, "Somthing went wrong, no message\n");
			return -1;
		} 

		// Check if hole message was recived 
		if (msgLen > sizeof(buff)){
			msgCompleate = false;
		} else {
			msgCompleate = true;
		}

		printf("%s", buff);
	}
	printf("\n");
	return 1;
}

int sendHeader(int *sock, requestHint_t *hint){

	char * header;
	char *p;
	char host[64];
	header =  malloc(HEADER_SIZE);
	memset(header, 0, HEADER_SIZE);
	size_t headerSize = HEADER_SIZE;
	size_t headerPos = 0;
	size_t lineLen = 0;

	// Create header from hints
	// Add Type
	lineLen = strlen(hint->type) + 1;
	while (lineLen>= headerSize){
		header = (char *)realloc(header, headerSize + HEADER_SIZE);
		headerSize += HEADER_SIZE;
	}
	memcpy(header, hint->type, strlen(hint->type));
	strcat(header, " ");
	headerPos += lineLen;

	// Add location
	// Find where it starts in URL 
	p = strchr(hint->url, '/');
	if (p == NULL){
		fprintf(stderr, "URL does not meet expected format!\n");
		return -1;
	}

	lineLen = strlen(p) + 1 + strlen(HEADER_HTTP) + strlen(HEADER_BREAK);
	while (lineLen>= headerSize){
		header = (char *)realloc(header, headerSize + HEADER_SIZE);
		headerSize += HEADER_SIZE;
	}
	strcat(header, p); // MIGHT NEED TO ADD A / BEFOR 
	strcat(header, " ");
	strcat(header, HEADER_HTTP);
	strcat(header, HEADER_BREAK);
	headerPos += lineLen;

	// Add host
	if (domainFormURL(host, sizeof(host), hint->url) != 1)
		return -1; // Some failer that should have been caught eariler
	lineLen = strlen(HEADER_HOST) + strlen(HEADER_BREAK) + strlen(host);
	while (lineLen >= headerSize){
		header = (char *)realloc(header, headerSize + HEADER_SIZE);
		headerSize += HEADER_SIZE;
	}
	// THIS MIGHT BREAK HARD
	strcat(header, HEADER_HOST);
	strcat(header, host);
	strcat(header, HEADER_BREAK);
	headerPos += lineLen;

	// Add arguments 
	for (int i = 0; i < hint->numArg; i++){
		lineLen = strlen(hint->arguments[i]) + strlen(HEADER_BREAK);
		while (lineLen >= headerSize){
			header = (char *)realloc(header, headerSize + HEADER_SIZE);
			headerSize += HEADER_SIZE;
		}
		strcat(header, hint->arguments[i]); 
		strcat(header, HEADER_BREAK);
		headerPos += lineLen;
	}

	// Add Accept (if needed)
	if (hint->argAccept == false){
		lineLen = strlen(HEADER_ACCEPT) + strlen(HEADER_BREAK);
		while (lineLen >= headerSize){
			header = (char *)realloc(header, headerSize + HEADER_SIZE);
			headerSize += HEADER_SIZE;
		}
		// THIS MIGHT BREAK HARD
		strcat(header, HEADER_ACCEPT);
		strcat(header, HEADER_BREAK);
		headerPos += lineLen;		
	}

	lineLen = 2*strlen(HEADER_BREAK);
	while (lineLen >= headerSize){
		header = (char *)realloc(header, headerSize + HEADER_SIZE);
		headerSize += HEADER_SIZE;
	}
	strcat(header, HEADER_BREAK);
	strcat(header, HEADER_BREAK);
	headerPos += lineLen;

	// Add message (if there)
	if (hint->paylaod != NULL){
		lineLen = strlen(hint->paylaod);
		while (lineLen >= headerSize){
			header = (char *)realloc(header, headerSize + HEADER_SIZE);
			headerSize += HEADER_SIZE;
		}
		strcat(header, hint->paylaod);
		headerPos += lineLen;
	}

	// Show sender header if not silenced
	if (!hint->silence){
		printf("%s",header);
	}

	send(*sock, header, strlen(header), 0);

	free(header);
	return 1;
}

int domainFormURL(char* buff, size_t size, char *url){
	char * p;

	// Serch for last part of domain name i.e www.google.com/
	p = strchr(url, '/');
	if (p == NULL){
		fprintf(stderr, "Failed to seperate domain name from:\n%s\n", url);
		return -1;
	}
	memset(buff, '\0', size);
	memcpy(buff, url, p-url);
	return 1;
}

int connectToServer(int *sock, char *URL, int port){

	struct sockaddr_in target;
	struct hostent *hostIp;
	char domain[128];

	// Get hosts domain name
	if (domainFormURL(domain, sizeof(domain), URL)!=1){
		return -1;
	}

	// Get server to contact info
	hostIp = gethostbyname(domain);
	if (hostIp == NULL){
		fprintf(stderr,"Failed DNS lookup of %s\n", domain);
		return -1;
	}

	// Create socket 
	*sock = socket(hostIp->h_addrtype, SOCK_STREAM, 0);
	if (*sock == -1){
		fprintf(stderr, "socket() failed\n");
		return -1;
	}

	// Clear target & set the address of outgoing message
	memset(&target, 0, sizeof(struct sockaddr_in));
	target.sin_family = hostIp->h_addrtype;
	target.sin_port = htons(PORT);
	memcpy((char *)&target.sin_addr.s_addr,
			hostIp->h_addr_list[0], hostIp->h_length);

	// Connect to host
	if (connect(*sock, (struct sockaddr *)&target, sizeof(target)) != 0){
		// Failed
		fprintf(stderr, "Failed to connect to: %s\n", ipToString(&target));
		return -1;
	}

	return 1;
}

// Print first ip
char * ipToString(struct sockaddr_in *list){
	if (list->sin_family == AF_INET){
		static char IP[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &list->sin_addr, IP, INET_ADDRSTRLEN);
		return IP;
	}
	if (list->sin_family == AF_INET){
		static char IP[INET6_ADDRSTRLEN];
		inet_ntop(AF_INET6, &list->sin_addr, IP, INET6_ADDRSTRLEN);
		return IP;
	}
	// ToDo
	return NULL;
}
