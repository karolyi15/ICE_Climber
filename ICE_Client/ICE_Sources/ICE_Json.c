//
// Created by gunther on 16/11/19.
//

#include "../ICE_Headers/ICE_Json.h"
#include <json-c/json.h>

//jsonParser function reads take a char array as argument
//it reads and convert it to a Json Object

 struct json_object * jsonParser(char jsonString[]) {

    //INIT STRUCTS TO SAVE JSON DATA
    struct json_object *parsed_json;
    struct json_object *posX;
    struct json_object *posY;
    struct json_object *score;
    struct json_object *lives;

    //PARSE JSON STRING
    parsed_json=json_tokener_parse(jsonString);

    //GET DATA BY JSON KEYS
    json_object_object_get_ex(parsed_json,"posX",&posX);
    json_object_object_get_ex(parsed_json,"posY",&posY);
    json_object_object_get_ex(parsed_json,"score",&score);
    json_object_object_get_ex(parsed_json,"lives",&lives);

    return parsed_json;

}



const char *jsonWriter(int id,int posX, int posY, int score, int lives){

    //CREATE A NEW JSON OBJECT
    json_object *jsonObject=json_object_new_object();
    json_object *jsonObjectPopo=json_object_new_object();

    //CREATE JSON DATA
    json_object *jsonId=json_object_new_int(id);
    json_object *jsonPosX=json_object_new_int(posX);
    json_object *jsonPosY=json_object_new_int(posY);
    json_object *jsonScore=json_object_new_int(score);
    json_object *jsonLives=json_object_new_int(lives);

    //ADDING DATA TO JSON OBJECT
    json_object_object_add(jsonObjectPopo,"posX",jsonPosX);
    json_object_object_add(jsonObjectPopo,"posY",jsonPosY);
    json_object_object_add(jsonObjectPopo,"score",jsonScore);
    json_object_object_add(jsonObjectPopo,"lives",jsonLives);

    json_object_object_add(jsonObject,"id",jsonId);
    json_object_object_add(jsonObject,"Popo",jsonObjectPopo);

    //printf("%s\n",json_object_to_json_string(jsonObject));
    return json_object_to_json_string(jsonObject);

}