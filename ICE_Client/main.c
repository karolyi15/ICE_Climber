
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <json-c/json.h>

#define PORT 9090


void jsonParser(char jsonString[]) {

    //INIT STRUCTS TO SAVE JSON DATA
    struct json_object *parsed_json;
    struct json_object *posX;
    struct json_object *posY;
    struct json_object *score;
    struct json_object *lives;

    //PARSE JSON STRING
    printf("Buffer Content: %s\n",jsonString);
    parsed_json=json_tokener_parse(jsonString);

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

const char *jsonWriter(int posX,int posY,int score, int lives){

    //CREATE JSON OBJECT
    json_object *jsonObject=json_object_new_object();

    //CREATE JSON STRING
    //json_object *posXString=json_object_new_string("posX");
    //json_object *posYString=json_object_new_string("posX");
    //json_object *scoreString=json_object_new_string("posX");
    //json_object *livesString=json_object_new_string("posX");

    //CREATE JSON INT
    json_object *posXInt=json_object_new_int(posX);
    json_object *posYInt=json_object_new_int(posY);
    json_object *scoreInt=json_object_new_int(score);
    json_object *livesInt=json_object_new_int(lives);

    //ADD KEYS AND VALUES TO JSON OBJECT
    json_object_object_add(jsonObject,"posX",posXInt);
    json_object_object_add(jsonObject,"posY",posYInt);
    json_object_object_add(jsonObject,"score",scoreInt);
    json_object_object_add(jsonObject,"lives",livesInt);

    //RETURNS JSON OBJECT
    return json_object_to_json_string(jsonObject);

}

int initClient() {

    int sock = 0, valread;
    struct sockaddr_in serv_addr;

    char *hello = "Hello from client";

    const char *buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "192.168.0.134", &serv_addr.sin_addr) <= 0) {
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

    jsonParser(buffer);
    //printf("%s", buffer);
    close(sock);
}





















int main(int argc, char **argv) {

    //initClient();
    printf("%s",jsonWriter(5,5,100,3));
}