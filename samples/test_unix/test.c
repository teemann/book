#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include "http.h"

int main(int argc, char* argv[]){
    printf("You have successfully set up your development environment.\nCongratulations!\n\n");
    printf("Try starting your browser and going to http://127.0.0.1:12345.\n");
    printf("You can exit this program by holding Ctrl + C\n\n");
    struct sockaddr_in sinServer, sinClient;
    int len;
    int server, client;
    char buffer[8192];
    if((server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1){
        printf("Oops, something went wrong when creating the socket\n");
        return 1;
    }
    if(inet_pton(AF_INET, "127.0.0.1", &sinServer.sin_addr) == 0)
        sinServer.sin_addr.s_addr = INADDR_ANY;
    sinServer.sin_port = htons(12345);
    sinServer.sin_family = AF_INET;

    int ena = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &ena, sizeof(int));

    if(bind(server, (struct sockaddr*)&sinServer, sizeof(sinServer)) == -1){
        close(server);
        printf("Could not bind the socket to port 12345.\n");
        return 1;
    }
    listen(server, 1);
    while(1){
        unsigned int slen = sizeof(sinClient);
        if((client = accept(server, (struct sockaddr*)&sinClient, &slen)) == -1){
            close(server);
            printf("Something went wrong when accepting a connection.\n");
            return 1;
        }
        if((len = recv(client, buffer, 8192, 0)) == -1){
            close(client);
            printf("Warning: Client disconnected before sending any data\n");
            continue;
        }
        //			fwrite(buffer, len, 1, stdout);
        //			puts("\n");
        char dest[1024];
        int res = http_get_requested_file(buffer, dest, 1024);
        if(res > 0){
            printf("Req: %s\n", dest);
            if(strcmp(dest, "/") != 0){
                char buf[1024];
                char buf2[1024];
                int flen = sprintf(buf2, "<html><head><title>Test</title></head><body><h1>You have requested %s.</h1></body></html>", dest);
                printf("%d\n", flen);
                len = http_get_response_header(200, strlen(buf2), buf, 1024);
                send(client, buf, len, 0);
                send(client, buf2, flen, 0);
            } else{
                FILE* file = fopen("index.html", "rb");
                if(file != 0){
                    fseek(file, 0, SEEK_END);
                    int flen = ftell(file);
                    rewind(file);
                    len = http_get_response_header(200, flen, dest, 1024);
                    send(client, dest, len, 0);
                    do{
                        len = fread(dest, 1, 1024, file);
                        send(client, dest, len, 0);
                    } while (len == 1024);
                    fclose(file);
                }
            }
        }
        close(client);
    }
    return 0;
}
