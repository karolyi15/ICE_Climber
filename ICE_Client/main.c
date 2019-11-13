
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <json-c/json.h>
//#include "/usr/include/json-c/json.h"
#define PORT 9090


void jsonParser() {

    FILE *jsonFile;
    char buffer[1024];

    //INIT STRUCTS TO SAVE JSON DATA
    struct json_object *parsed_json;
    struct json_object *posX;
    struct json_object *posY;
    struct json_object *score;
    struct json_object *lives;

    jsonFile=fopen("test.json","r");
    fread(buffer,1024,1,jsonFile);
    fclose(jsonFile);

    //PARSE JSON STRING
    printf("Buffer Content: %s\n",buffer);
    parsed_json=json_tokener_parse(buffer);

    //GET DATA BY JSON KEYS
    json_object_object_get_ex(parsed_json,"posX",&posX);
    json_object_object_get_ex(parsed_json,"posY",&posY);
    json_object_object_get_ex(parsed_json,"score",&score);
    json_object_object_get_ex(parsed_json,"lives",&lives);

    //PRINTING RESULTS
    printf("posX = %i\n",json_object_get_int(posX));
    printf("posY = %i\n",json_object_get_int(posY));
    printf("score = %i\n",json_object_get_int(score));
    printf("lives = %i\n",json_object_get_int(lives));


}



int initClient() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;

    char *hello = "Hello from client";

    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "172.18.97.181", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    //send(sock , hello , strlen(hello) , 0 );

    printf("se enviara un mensaje\n");

    bzero(buffer, sizeof(buffer));

    write(sock, hello, sizeof(buffer));

    valread = read(sock, buffer, 1024);
    printf("%s", buffer);
    close(sock);
}


int main(int argc, char **argv) {

    jsonParser();

}