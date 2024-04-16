// gcc -o Server server.c -pthread

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_LEN 1024
#define PORT 5656

/* shared variable */


void *handle_new_connection(void *args)
{
    int *arg = (int *)args;
    int newSd = *arg;
    int valread;

    char buffer[MAX_LEN] = {0};

    printf("new connection: %d\n", newSd);

    /* read message */
    memset(buffer, 0x0, MAX_LEN); // reset
    valread = read(newSd, buffer, MAX_LEN - 1);
    printf("Received: %s\n", buffer);

    /* send response */
    char response[5] = "back";
    send(newSd, response, strlen(response), 0);
}

int main(int argc, char *argv[])
{
    int sd, newSd, cliLen;
    struct sockaddr_in cliAddr, servAddr;
    cliLen = sizeof(cliAddr);

    /* create socket */
    sd = socket(AF_INET, SOCK_STREAM, 0);

    /* bind server port */
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(PORT);
    bind(sd, (struct sockaddr *)&servAddr, sizeof(servAddr));

    listen(sd, 5);
    
    // handle new connection
    while (1)
    {
        /* accept connection */
        newSd = accept(sd, (struct sockaddr *)&cliAddr, &cliLen);
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_new_connection, (void *)&newSd);
    }

    pthread_exit(NULL);
}