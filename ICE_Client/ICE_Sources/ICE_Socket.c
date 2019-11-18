//
// Created by gunther on 16/11/19.
//

#include "../ICE_Headers/ICE_Socket.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

//Define the port where the server is listening
#define PORT 9090

//Init Client connects to the server port
//Sends the message that is passed by parameter (jsonString)
//and receives an answer from the server
const char *initClient(char jsonString[]) {

    int sock = 0;//Define a socket
    struct sockaddr_in serv_addr;

    //The buffer stores the char array that receive from the server
    const char *buffer[1024] = {0};

    //Validate if socket is created
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return NULL;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "192.168.0.134", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return NULL;
    }

    //Validate connection failed
    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return NULL;
    }

    //Sends message to server
    write(sock, jsonString, strlen(jsonString));
    //Clear content of the buffer
    bzero(buffer, sizeof(buffer));
    //Receive message from server
    read(sock, buffer, 1024);

    //Close the socket to free memory
    close(sock);

    //Translate the message of the server
    return buffer;

}
