#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "getResponse.h"

static const char* HTTPheader = 
"POST /t/986b9-1565274094/post/post.txt HTTP/1.1\r\n"
"Host: www.ptsv2.com\r\n"
"Content-Type: text/plain\r\n"
"Content-Length: 9\r\n"
"Accept: */*\r\n"
"\r\n"
"apitester";

int main(int argc, char ** argv){
    int opt;
    for(int i = 1;(opt = getopt(argc, argv, "nf:"))!=1; i++){

        switch (opt)
        {
        case 'n':
            printf("option: %c\n", opt);
            break;
        case 'f':
            printf("filename: %s\n", optarg);
            break;
        case ':':
            printf("option needs a value\n");
            break;
        default: 
            fprintf(stderr,"Usage: %s \n",argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}