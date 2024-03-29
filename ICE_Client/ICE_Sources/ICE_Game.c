
#include "../ICE_Headers/ICE_Game.h"
#include <allegro5/allegro.h>
#include "allegro5/allegro_image.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_native_dialog.h>
#include "allegro5/allegro_primitives.h"
#include <stdio.h>
#include "../ICE_Headers/ICE_Json.h"
#include "../ICE_Headers/ICE_Socket.h"



//************************************************************************************************************************************//
//**************************************************** GLOBAL GAME FIELDS ************************************************************//

//********************* WINDOW *********************//

const  int screenW=1000;//DETERMINES WINDOW WIDTH
const  int screenH=600;//DETERMINES WINDOW HEIGHT

//****************** GAME FIELDS *******************//

const float FPS=60;//DETERMINES FRAMES PER SECOND IN THE GAME
const float AnimationFPS=10.0;//DETERMINES ANIMATION SPEED

float movementSpeed[2]={4.0, 7.0};//CHARACTERS MOVEMENT SPEED
float acceleration[2]={1,9.8};//CHARACTER MOVEMENT ACCELERATION

int currentScene=0;//Determines WHICH SCENE THE USER IS GONNA SEE
int gameMode;
int playerID;

//******************** PLAYERS *********************//

struct Character{
    bool jump;
    float position[2];
    float animation[2];
    float dimentions[2];
    int score;
    int lives;
    int *direction;

};



//******************** ENEMIES *********************//

int dinoDirection;

struct  Block{

    bool state;
    float dimentions[2];
    float position[2];
    float animation[2];
};

struct Floor{

    bool state;
    int type;
    float position[2];
    struct Block blocks[23];
};


//******************** CAMARA **********************//

float camaraPosition[2]={0,-2850};



//************************************************************************************************************************************//
//******************************************************* GAME COMPONENTS ************************************************************//

//********************* BLOCKS *********************//

void blockCollide(struct Floor floors[], struct Character *character,ALLEGRO_SAMPLE *sound){


    for(int floor=0;floor<7;floor++){

        if(floors[floor].type==1) {

            for (int block = 0; block < 23; block++) {

                struct Block tempBlock = floors[floor].blocks[block];

                if(character->position[1]>tempBlock.position[1]+(tempBlock.dimentions[1]/2)+7 &&character->position[1]<tempBlock.position[1]+tempBlock.dimentions[1]+camaraPosition[1]+2850 && character->position[0]+(character->dimentions[0]/2)>=tempBlock.position[0] && character->position[0]+(character->dimentions[0]/2)<=tempBlock.position[0]+tempBlock.dimentions[0] &&tempBlock.state==true){
                    character->position[1]=tempBlock.position[1]+tempBlock.dimentions[1];
                    floors[floor].blocks[block].state=false;
                    al_play_sample(sound, 0.5, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                    character->score+=10;
                    printf("collide");
                }

                else if( character->position[1]+character->dimentions[1]<=tempBlock.position[1] && character->position[1]+character->dimentions[1]>=tempBlock.position[1]+camaraPosition[1]+2850&& character->position[0]+(character->dimentions[0]/2)>=tempBlock.position[0] && character->position[0]+(character->dimentions[0]/2)<=tempBlock.position[0]+tempBlock.dimentions[0] &&tempBlock.state==true){

                    character->position[1]-=10;
                }



            }


        }
        else if(floors[floor].type==2){

            for (int block = 0; block < 23; block++) {

                struct Block tempBlock = floors[floor].blocks[block];

                if(character->position[1]<tempBlock.position[1]+tempBlock.dimentions[1]+camaraPosition[1]+2850 && character->position[0]+(character->dimentions[0]/2)>=tempBlock.position[0] && character->position[0]+(character->dimentions[0]/2)<=tempBlock.position[0]+tempBlock.dimentions[0] &&tempBlock.state==true){
                    character->position[1]=tempBlock.position[1]+tempBlock.dimentions[1]+25;
                    floors[floor].blocks[block].state=false;
                    printf("collide");
                }



            }

        }
        else if(floors[floor].type==3){

            for (int block = 0; block < 23; block++) {

                struct Block tempBlock = floors[floor].blocks[block];

                if(character->position[1]<tempBlock.position[1]+tempBlock.dimentions[1]+camaraPosition[1]+2850 && character->position[0]+(character->dimentions[0]/2)>=tempBlock.position[0] && character->position[0]+(character->dimentions[0]/2)<=tempBlock.position[0]+tempBlock.dimentions[0] &&tempBlock.state==true){
                    character->position[1]=tempBlock.position[1]+tempBlock.dimentions[1]+25;
                    floors[floor].blocks[block].state=false;
                    printf("collide");
                }



            }
        }
    }



}


int collide(float xPlayer, float yPlayer, float xObstacle, float yObstacle,int playerWidth, int playerHeight,int ObstacleWidth, int ObstacleHeight){

    if(xPlayer+playerWidth <= xObstacle || xPlayer >= xObstacle+ObstacleWidth || yPlayer+playerHeight <= yObstacle+68 || yPlayer >= yObstacle+ObstacleHeight){
        return false;
    }
    return true;
}


void createBlocks(struct Block blocks[],ALLEGRO_BITMAP *image,int blockX,int blockY, int type){

    //DETERMINE NUMBER OF BLOCKS PER FLOOR
    //DETERMINE BLOCK COLOR
    int len;
    float section;
    if(type==1){
        len=23;
        section=0;
    }
    else if(type==2){
        len=21;
        section=al_get_bitmap_width(image)/3;
    }
    else{
        len=19;
        section=(al_get_bitmap_width(image)/3)*2;
    }

    //CREATE BLOCKS AND PUT IT INTO BLOCK ARRAY
    for(int counter=0;counter<len;counter++){

        struct Block tempBlock;
        tempBlock.position[0]=blockX;
        tempBlock.position[1]=blockY;
        tempBlock.animation[0]=section;
        tempBlock.animation[1]=0;
        tempBlock.dimentions[0]=al_get_bitmap_width(image)/3;
        tempBlock.dimentions[1]=al_get_bitmap_height(image);
        tempBlock.state=true;

        blocks[counter]=tempBlock;
        blockX+=al_get_bitmap_width(image)/3;
    }

}


void createFloors(struct Floor floors[], ALLEGRO_BITMAP *image){

    //CREATE FIRST FLOOR
    struct Floor floor1;
    floor1.state=true;
    floor1.type=1;
    floor1.position[0]=125;
    floor1.position[1]=320;

    //CREATE BLOCKS FOR FIRST FLOOR
    createBlocks(floor1.blocks,image,floor1.position[0],floor1.position[1],floor1.type);

    //SET FLOOR 1 INTO FLOORS ARRAY
    floors[0]=floor1;

    //***********************************************************

    //CREATE SECOND FLOOR
    struct Floor floor2;

    floor2.state=true;
    floor2.type=2;
    floor2.position[0]=157;
    floor2.position[1]=132;

    //CREATE BLOCKS FOR SECOND FLOOR
    createBlocks(floor2.blocks,image,floor2.position[0],floor2.position[1],floor2.type);

    //SET FLOOR 2 INTO FLOORS ARRAY
    floors[1]=floor2;

    //***********************************************************

    //CREATE THIRD FLOOR
    struct Floor floor3;

    floor3.state=true;
    floor3.type=2;
    floor3.position[0]=157;
    floor3.position[1]=-59;

    //CREATE BLOCKS FOR THIRD FLOOR
    createBlocks(floor3.blocks,image,floor3.position[0],floor3.position[1],floor3.type);

    //SET FLOOR 3 INTO FLOORS ARRAY
    floors[2]=floor3;

    //***********************************************************

    //CREATE FOURTH FLOOR
    struct Floor floor4;

    floor4.state=true;
    floor4.type=2;
    floor4.position[0]=157;
    floor4.position[1]=-247;

    //CREATE BLOCKS FOR FOURTH FLOOR
    createBlocks(floor4.blocks,image,floor4.position[0],floor4.position[1],floor4.type);

    //SET FLOOR 4 INTO FLOORS ARRAY
    floors[3]=floor4;

    //***********************************************************

    //CREATE FIVE FLOOR
    struct Floor floor5;

    floor5.state=true;
    floor5.type=3;
    floor5.position[0]=190;
    floor5.position[1]=-435;

    //CREATE BLOCKS FOR FIVE FLOOR
    createBlocks(floor5.blocks,image,floor5.position[0],floor5.position[1],floor5.type);

    //SET FLOOR 5 INTO FLOORS ARRAY
    floors[4]=floor5;

    //***********************************************************

    //CREATE SIX FLOOR
    struct Floor floor6;

    floor6.state=true;
    floor6.type=3;
    floor6.position[0]=190;
    floor6.position[1]=-623;

    //CREATE BLOCKS FOR FOURTH FLOOR
    createBlocks(floor6.blocks,image,floor6.position[0],floor6.position[1],floor6.type);

    //SET FLOOR 6 INTO FLOORS ARRAY
    floors[5]=floor6;

    //***********************************************************

    //CREATE SEVEN FLOOR
    struct Floor floor7;

    floor7.state=true;
    floor7.type=3;
    floor7.position[0]=190;
    floor7.position[1]=-811;

    //CREATE BLOCKS FOR SEVEN FLOOR
    createBlocks(floor7.blocks,image,floor7.position[0],floor7.position[1],floor7.type);

    //SET FLOOR 7 INTO FLOORS ARRAY
    floors[6]=floor7;

}



void displayFloors(struct Floor floors[], ALLEGRO_BITMAP *image){

    for(int floor=0;floor<7;floor++){

        if(floors[floor].type==1) {

            for (int block = 0; block < 23; block++) {

                struct Block tempBlock = floors[floor].blocks[block];

                if(tempBlock.state==true) {

                    al_draw_bitmap_region(image, tempBlock.animation[0], tempBlock.animation[1],
                                          tempBlock.dimentions[0],
                                          tempBlock.dimentions[1], tempBlock.position[0],
                                          tempBlock.position[1] + camaraPosition[1] + 2850, 0);
                }
            }

        }
        else if(floors[floor].type==2){

            for (int block = 0; block < 21; block++) {

                struct Block tempBlock = floors[floor].blocks[block];

                if(tempBlock.state==true) {
                    al_draw_bitmap_region(image, tempBlock.animation[0], tempBlock.animation[1],
                                          tempBlock.dimentions[0],
                                          tempBlock.dimentions[1], tempBlock.position[0],
                                          tempBlock.position[1] + camaraPosition[1] + 2850, 0);
                }
            }


        }
        else if(floors[floor].type==3){

            for (int block = 0; block < 19; block++) {

                struct Block tempBlock = floors[floor].blocks[block];

                if(tempBlock.state==true) {
                    al_draw_bitmap_region(image, tempBlock.animation[0], tempBlock.animation[1],
                                          tempBlock.dimentions[0],
                                          tempBlock.dimentions[1], tempBlock.position[0],
                                          tempBlock.position[1] + camaraPosition[1] + 2850, 0);
                }
            }



        }


    }

}












//********************* CAMARA *********************//

void refreshCamara(struct Character character){
    if (character.position[1]<=250){
        camaraPosition[1]+=6.5;
    }

    if(camaraPosition[1]>=0){
        camaraPosition[1]=0;
    }
}

void restartCamara(){
    camaraPosition[0]=0;
    camaraPosition[1]=-2850;
}

//******************* ANIMATIONS *******************//

void animateEnemy(ALLEGRO_BITMAP *dino, float dinoPosition[] ,float dinoAnimation[]){


    //Animation
    dinoAnimation[0]+=al_get_bitmap_width(dino)/3;
    if(dinoAnimation[0]>=al_get_bitmap_width(dino)){
        dinoAnimation[0]=0;
    }
    //Direction
    if(dinoDirection==0 ){
        dinoAnimation[1]=0;
        if(dinoPosition[0] >= 4.0){
            dinoPosition[0]-=25*acceleration[0];
        }else{
            dinoDirection=1;
        }
    }
    else if (dinoDirection==1 ){
        dinoAnimation[1]=(al_get_bitmap_height(dino)/2);
        if(dinoPosition[0]<=(996-al_get_bitmap_width(dino)/3)){
            dinoPosition[0]+=25*acceleration[0];
        }else{

            dinoDirection=0;
        }
    }
}


//************************************************************************************************************************************//
//********************************************************* GAME SCENES **************************************************************//

//****************** TITLE SCENE *******************//

int titleScene(ALLEGRO_DISPLAY *display){

    //INITIALIZE EVENT QUEUE
    ALLEGRO_EVENT_QUEUE *eventQueue=NULL;

    //INITIALIZE TIMERS
    ALLEGRO_TIMER  *timer=NULL;

    //INITIALIZE IMAGES
    ALLEGRO_BITMAP *background=NULL;

    //INITIALIZE SOUNDS
    ALLEGRO_SAMPLE *titleMusic=NULL;

    //INITIALIZE FLAGS
    bool selecting=true;
    bool redraw=true;

    //INITIALIZE MOUSE POSITION
    float mouseX;
    float mouseY;

    //DECLARE TIMERS
    timer = al_create_timer(1.0 / FPS);

    //VALIDATE TIMERS
    if(!timer){
        al_show_native_message_box(display, "Error", "Error", "Fail to create timer",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return 0;
    }

    //LOAD IMAGES
    background = al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/TitleScreen.jpeg");

    //VALIDATE IMAGES
    if(!background) {
        al_show_native_message_box(display, "Error", "Error", "Failed to load image file",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_timer(timer);
        al_destroy_display(display);
        return 0;
    }

    //LOAD AUDIO FILES
    titleMusic = al_load_sample("/home/gunther/CLionProjects/ICE_Client/_sounds/_TitleScreen.wav" );

    //VALIDATE AUDIO FILES
    if (!titleMusic) {
        al_show_native_message_box(display, "Error", "Error", "Failed to load audio clip",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_bitmap(background);
        return 0;
    }

    //DECLARE EVENT QUEUE
    eventQueue=al_create_event_queue();

    //VALIDATE EVENT QUEUE
    if(!eventQueue){
        al_show_native_message_box(display, "Error", "Error", "Fail to create Event Queue",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_timer(timer);
        al_destroy_sample(titleMusic);
        al_destroy_bitmap(background);
        al_destroy_display(display);
        return 0;
    }

    //REGISTER EVENTS TO EVENT QUEUE (LINK THEM)
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_register_event_source(eventQueue, al_get_mouse_event_source());

    //SET DISPLAY BACKGROUND COLOR
    al_clear_to_color(al_map_rgb(0,0,0));

    //PLAY TITLE MUSIC
    al_play_sample(titleMusic, 0.5, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);

    //START TIMERS
    al_start_timer(timer);

    while(selecting){

        //ADD EVENTS TO QUEUE
        ALLEGRO_EVENT event;
        al_wait_for_event(eventQueue,&event);

        //SET REDRAW TRUE
        if(event.type == ALLEGRO_EVENT_TIMER){
            redraw=true;
        }

        //CLOSE WINDOW
        else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            currentScene=4;
            break;
        }

        //ASSIGN MOUSE POSITION VALUES
        else if(event.type==ALLEGRO_EVENT_MOUSE_AXES)
        {
            mouseX=event.mouse.x;
            mouseY=event.mouse.y;
        }

        //VALIDATE MOUSE CLICK
        else if(event.type==ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){

            //SELECT ONE PLAYER MODE
            if(mouseX>309 & mouseX<716 & mouseY>352 & mouseY<380){
                gameMode=0;
                playerID=1;
                currentScene=2;
                break;
            }
            //SELECT TWO PLAYERS MODE
            else if(mouseX>309 & mouseX<716 & mouseY>439 & mouseY<466){
                currentScene=1;
                break;
            }
        }

        //DRAWING TO DISPLAY
        if(redraw && al_is_event_queue_empty(eventQueue)){
            redraw=false;
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_bitmap(background,0,0,0);
            al_flip_display();
        }

    }

    //FREE MEMORY
    al_destroy_timer(timer);
    al_destroy_event_queue(eventQueue);
    al_destroy_sample(titleMusic);
    al_destroy_bitmap(background);

    return 0;
}

//***************** GAME MODE SCENE ****************//

int gameModeScene(ALLEGRO_DISPLAY *display){

    //INITIALIZE EVENT QUEUE
    ALLEGRO_EVENT_QUEUE *eventQueue=NULL;

    //INITIALIZE TIMERS
    ALLEGRO_TIMER  *timer=NULL;

    //INITIALIZE IMAGES
    ALLEGRO_BITMAP *background=NULL;
    ALLEGRO_BITMAP *screenMode=NULL;
    ALLEGRO_BITMAP *localMode=NULL;
    ALLEGRO_BITMAP *onlineMode=NULL;

    //INITIALIZE SOUNDS
    ALLEGRO_SAMPLE *titleMusic=NULL;

    //INITIALIZE FLAGS
    bool selecting=true;
    bool redraw=true;

    //INITIALIZE MOUSE POSITION
    float mouseX;
    float mouseY;

    //DECLARE TIMERS
    timer = al_create_timer(1.0 / FPS);

    //VALIDATE TIMERS
    if(!timer){
        al_show_native_message_box(display, "Error", "Error", "Fail to create timer",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return 0;
    }

    //LOAD IMAGES
    background = al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/Backgrounds.png");
    screenMode = al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/ScreenMode.png");
    localMode = al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/LocalMode.png");
    onlineMode = al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/OnlineMode.png");

    //VALIDATE IMAGES
    if(!background || !screenMode || !localMode || !onlineMode) {
        al_show_native_message_box(display, "Error", "Error", "Failed to load image file",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_timer(timer);
        al_destroy_display(display);
        return 0;
    }

    //LOAD AUDIO FILES
    titleMusic = al_load_sample("/home/gunther/CLionProjects/ICE_Client/_sounds/_TitleScreen.wav" );

    //VALIDATE AUDIO FILES
    if (!titleMusic) {
        al_show_native_message_box(display, "Error", "Error", "Failed to load audio clip",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_bitmap(background);
        al_destroy_bitmap(screenMode);
        al_destroy_bitmap(localMode);
        al_destroy_bitmap(onlineMode);

        return 0;
    }

    //DECLARE EVENT QUEUE
    eventQueue=al_create_event_queue();

    //VALIDATE EVENT QUEUE
    if(!eventQueue){
        al_show_native_message_box(display, "Error", "Error", "Fail to create Event Queue",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_timer(timer);
        al_destroy_sample(titleMusic);
        al_destroy_bitmap(background);
        al_destroy_bitmap(screenMode);
        al_destroy_bitmap(localMode);
        al_destroy_bitmap(onlineMode);
        al_destroy_display(display);
        return 0;
    }

    //REGISTER EVENTS TO EVENT QUEUE (LINK THEM)
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_register_event_source(eventQueue, al_get_mouse_event_source());

    //SET DISPLAY BACKGROUND COLOR
    al_clear_to_color(al_map_rgb(0,0,0));

    //PLAY TITLE MUSIC
    al_play_sample(titleMusic, 0.5, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);

    //START TIMERS
    al_start_timer(timer);

    while(selecting){

        //ADD EVENTS TO QUEUE
        ALLEGRO_EVENT event;
        al_wait_for_event(eventQueue,&event);

        //SET REDRAW TRUE
        if(event.type == ALLEGRO_EVENT_TIMER){
            redraw=true;
        }

            //CLOSE WINDOW
        else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            currentScene=4;
            break;
        }

            //ASSIGN MOUSE POSITION VALUES
        else if(event.type==ALLEGRO_EVENT_MOUSE_AXES)
        {
            mouseX=event.mouse.x;
            mouseY=event.mouse.y;
        }
            //VALIDATE MOUSE CLICK
        else if(event.type==ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){

            //SELECT SAME SCREEN MODE
            if(mouseX>177.5 & mouseX<822.5 & mouseY>150 & mouseY<150+al_get_bitmap_height(screenMode)){
                gameMode=1;
                currentScene=2;
                break;
            }
            //SELECT ONLINE MODE
            else if(mouseX>283 & mouseX<717 & mouseY>250 & mouseY<250+al_get_bitmap_height(onlineMode)){
                gameMode=2;
                playerID=2;
                currentScene=2;
                break;
            }
            //SELECT LOCAL MODE
            else if(mouseX>290.5 & mouseX<709.5 & mouseY>350 & mouseY<350+al_get_bitmap_height(localMode)){
                gameMode=2;
                playerID=1;
                currentScene=2;
                break;
            }
        }

        //DRAWING TO DISPLAY
        if(redraw && al_is_event_queue_empty(eventQueue)){
            redraw=false;
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_bitmap_region(background,1035,0,1000,3450,0,-2850,0);
            al_draw_bitmap(screenMode,screenW/2-al_get_bitmap_width(screenMode)/2,150,0);
            al_draw_bitmap(onlineMode,screenW/2-al_get_bitmap_width(onlineMode)/2,250,0);
            al_draw_bitmap(localMode,screenW/2-al_get_bitmap_width(localMode)/2,350,0);
            al_flip_display();
        }

    }

    //FREE MEMORY
    al_destroy_timer(timer);
    al_destroy_event_queue(eventQueue);
    al_destroy_sample(titleMusic);
    al_destroy_bitmap(background);
    al_destroy_bitmap(screenMode);
    al_destroy_bitmap(localMode);
    al_destroy_bitmap(onlineMode);

    return 0;

}

//************* ENEMY SELECTION SCENE **************//

int enemySelectionScene(ALLEGRO_DISPLAY *display){

    //INITIALIZE EVENT QUEUE
    ALLEGRO_EVENT_QUEUE *eventQueue=NULL;

    //INITIALIZE TIMERS
    ALLEGRO_TIMER  *timer=NULL;

    //INITIALIZE IMAGES
    ALLEGRO_BITMAP *background=NULL;
    ALLEGRO_BITMAP *titleText=NULL;
    ALLEGRO_BITMAP *playText=NULL;
    ALLEGRO_BITMAP *Oso=NULL;
    ALLEGRO_BITMAP *Yeti=NULL;
    ALLEGRO_BITMAP *Foca=NULL;
    ALLEGRO_BITMAP *Pajaro=NULL;

    //INITIALIZE SOUNDS
    ALLEGRO_SAMPLE *titleMusic=NULL;

    //INITIALIZE FLAGS
    bool osoSelected=false;
    bool yetiSelected=false;
    bool focaSelected=false;
    bool pajaroSelected=false;
    bool selecting=true;
    bool redraw=true;


    //INITIALIZE RECTANGLE VALUES
    float rectangleW=100;
    float rectangleH=100;

    //INITIALIZE MOUSE POSITION
    float mouseX;
    float mouseY;

    //DECLARE TIMERS
    timer = al_create_timer(1.0 / FPS);

    //VALIDATE TIMERS
    if(!timer){
        al_show_native_message_box(display, "Error", "Error", "Fail to create timer",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return 0;
    }

    //DECLARE  COLORS
    ALLEGRO_COLOR enemyAvailable= al_map_rgb(0,43.9,92.5);
    ALLEGRO_COLOR enemySelected= al_map_rgb(89.4, 0, 34.5);
    ALLEGRO_COLOR osoColor=enemyAvailable;
    ALLEGRO_COLOR yetiColor=enemyAvailable;
    ALLEGRO_COLOR focaColor=enemyAvailable;
    ALLEGRO_COLOR pajaroColor=enemyAvailable;

    //LOAD IMAGES
    background = al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/Backgrounds.png");
    titleText = al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/SelectEnemy.png");
    playText = al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/PLAY.png");
    Oso = al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/Oso.png");
    Yeti = al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/Yeti.png");
    Foca = al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/Foca.png");
    Pajaro = al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/Pajaro.png");

    //VALIDATE IMAGES
    if(!background || !titleText || !playText || !Oso || !Yeti || !Foca || !Pajaro) {
        al_show_native_message_box(display, "Error", "Error", "Failed to load image file",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_timer(timer);
        al_destroy_display(display);
        return 0;
    }

    //LOAD AUDIO FILES
    titleMusic = al_load_sample("/home/gunther/CLionProjects/ICE_Client/_sounds/_TitleScreen.wav" );

    //VALIDATE AUDIO FILES
    if (!titleMusic) {
        al_show_native_message_box(display, "Error", "Error", "Failed to load audio clip",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_bitmap(background);
        al_destroy_bitmap(titleText);
        al_destroy_bitmap(playText);
        al_destroy_bitmap(Oso);
        al_destroy_bitmap(Yeti);
        al_destroy_bitmap(Foca);
        al_destroy_bitmap(Pajaro);

        return 0;
    }

    //DECLARE EVENT QUEUE
    eventQueue=al_create_event_queue();

    //VALIDATE EVENT QUEUE
    if(!eventQueue){
        al_show_native_message_box(display, "Error", "Error", "Fail to create Event Queue",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_timer(timer);
        al_destroy_sample(titleMusic);
        al_destroy_bitmap(background);
        al_destroy_bitmap(titleText);
        al_destroy_bitmap(playText);
        al_destroy_bitmap(Oso);
        al_destroy_bitmap(Yeti);
        al_destroy_bitmap(Foca);
        al_destroy_bitmap(Pajaro);
        al_destroy_display(display);
        return 0;
    }

    //REGISTER EVENTS TO EVENT QUEUE (LINK THEM)
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_register_event_source(eventQueue, al_get_mouse_event_source());

    //SET DISPLAY BACKGROUND COLOR
    al_clear_to_color(al_map_rgb(0,0,0));

    //PLAY TITLE MUSIC
    al_play_sample(titleMusic, 0.5, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);

    //START TIMERS
    al_start_timer(timer);

    while(selecting){

        //ADD EVENTS TO QUEUE
        ALLEGRO_EVENT event;
        al_wait_for_event(eventQueue,&event);

        //SET REDRAW TRUE
        if(event.type == ALLEGRO_EVENT_TIMER){
            redraw=true;
        }

            //CLOSE WINDOW
        else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            currentScene=4;
            break;
        }

            //ASSIGN MOUSE POSITION VALUES
        else if(event.type==ALLEGRO_EVENT_MOUSE_AXES)
        {
            mouseX=event.mouse.x;
            mouseY=event.mouse.y;
        }
            //VALIDATE MOUSE CLICK
        else if(event.type==ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){

            //SELECT OSO
            if(mouseX>300 & mouseX<300+rectangleW & mouseY>140 & mouseY<140+rectangleH){

                if(osoSelected==false){
                    osoSelected=true;
                    osoColor=enemySelected;
                }else{
                    osoSelected=false;
                    osoColor=enemyAvailable;
                }

            }
            //SELECT YETI
            else if(mouseX>600 & mouseX<600+rectangleW & mouseY>140 & mouseY<140+rectangleH){

                if(yetiSelected==false){
                    yetiSelected=true;
                    yetiColor=enemySelected;
                }else{
                    yetiSelected=false;
                    yetiColor=enemyAvailable;
                }
            }
            //SELECT FOCA
            else if(mouseX>300 & mouseX<300+rectangleW & mouseY>300 & mouseY<300+rectangleH){

                if(focaSelected==false){
                    focaSelected=true;
                    focaColor=enemySelected;
                }else{
                    focaSelected=false;
                    focaColor=enemyAvailable;
                }
            }
            //SELECT PAJARO
            else if(mouseX>600 & mouseX<600+rectangleW & mouseY>300 & mouseY<300+rectangleH){

                if(pajaroSelected==false){
                    pajaroSelected=true;
                    pajaroColor=enemySelected;
                }else{
                    pajaroSelected=false;
                    pajaroColor=enemyAvailable;
                }
            }
            //SELECT START
            else if(mouseX>417 & mouseX<417+al_get_bitmap_width(playText) & mouseY>450 & mouseY<450+al_get_bitmap_height(playText)){
                currentScene=3;
                break;

            }

        }

        //DRAWING TO DISPLAY
        if(redraw && al_is_event_queue_empty(eventQueue)){
            redraw=false;
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_bitmap_region(background,1035,0,1000,3450,0,-2850,0);

            //FIRST ROW OF ENEMIES
            al_draw_bitmap_region(Oso,0,0,al_get_bitmap_width(Oso)/3,al_get_bitmap_height(Oso)/3,325,140,0);
            al_draw_rectangle(300,140,300+rectangleW,140+rectangleH,osoColor,10);
            al_draw_bitmap_region(Yeti,0,0,al_get_bitmap_width(Yeti)/3,al_get_bitmap_height(Yeti)/3,622,156,0);
            al_draw_rectangle(600,140,600+rectangleW,140+rectangleH,yetiColor,10);

            //SECOND ROW OF ENEMIES
            al_draw_bitmap_region(Foca,0,0,al_get_bitmap_width(Foca)/3,al_get_bitmap_height(Foca)/3,322,315,0);
            al_draw_rectangle(300,300,300+rectangleW,300+rectangleH,focaColor,10);
            al_draw_bitmap_region(Pajaro,0,0,al_get_bitmap_width(Pajaro)/2,al_get_bitmap_height(Pajaro)/3,625,325,0);
            al_draw_rectangle(600,300,600+rectangleW,300+rectangleH,pajaroColor,10);

            al_draw_bitmap(titleText,500-al_get_bitmap_width(titleText)/2,40,0);
            al_draw_bitmap(playText,500-al_get_bitmap_width(playText)/2,450,0);
            al_flip_display();
        }

    }

    //FREE MEMORY
    al_destroy_timer(timer);
    al_destroy_event_queue(eventQueue);
    al_destroy_sample(titleMusic);
    al_destroy_bitmap(titleText);
    al_destroy_bitmap(playText);
    al_destroy_bitmap(background);
    al_destroy_bitmap(Oso);
    al_destroy_bitmap(Yeti);
    al_destroy_bitmap(Foca);
    al_destroy_bitmap(Pajaro);

    return 0;



}

//*************** GAME LEVEL INTRO ******************//

int levelIntro(ALLEGRO_DISPLAY *display) {

    ALLEGRO_EVENT_QUEUE *eventQueue = NULL;

    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_TIMER *frameTimer = NULL;

    ALLEGRO_SAMPLE *introMusic = NULL;

    ALLEGRO_BITMAP *background = NULL;
    ALLEGRO_BITMAP *dino = NULL;

    bool running = true;
    bool redraw = true;


    //CREATE TIMER
    timer = al_create_timer(1.0 / FPS);
    frameTimer = al_create_timer(1.0 / AnimationFPS);
    if (!timer || !frameTimer) {
        al_show_native_message_box(display, "Error", "Error", "Fail to create timer", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    background = al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/Backgrounds.png");
    dino = al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/Dino.png");
    if (!background || !dino) {
        al_show_native_message_box(display, "Error", "Error", "Failed to load image file",
                                   NULL, ALLEGRO_MESSAGEBOX_ERROR);

        al_destroy_timer(timer);
        al_destroy_timer(frameTimer);
        al_destroy_display(display);
        return 0;
    }

    al_convert_mask_to_alpha(dino, al_map_rgb(0, 0, 0));

    //LOADING AUDIO FILES
    introMusic = al_load_sample("/home/gunther/CLionProjects/ICE_Client/_sounds/IntroMusic.wav");
    if (!introMusic) {
        al_show_native_message_box(display, "Error", "Error", "Failed to load audio clip", NULL,
                                   ALLEGRO_MESSAGEBOX_ERROR);

        al_destroy_timer(timer);
        al_destroy_timer(frameTimer);
        al_destroy_bitmap(background);
        al_destroy_bitmap(dino);
        al_destroy_display(display);
        return 0;
    }

    //CREATE AND MANAGE EVENT QUEUE
    eventQueue = al_create_event_queue();
    if (!eventQueue) {
        al_show_native_message_box(display, "Error", "Error", "Fail to create Event Queue",
                                   NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_timer(timer);
        al_destroy_timer(frameTimer);
        al_destroy_bitmap(background);
        al_destroy_bitmap(dino);
        al_destroy_sample(introMusic);
        al_destroy_display(display);

        return 0;
    }

    //REGISTER EVENTS TO EVENT QUEUE (LINK THEM)
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_register_event_source(eventQueue, al_get_timer_event_source(frameTimer));


    al_clear_to_color(al_map_rgb(0, 0, 0));

    //CREATE DINO
    struct Character Dino;
    Dino.position[0]=125;
    Dino.position[1]=506;
    Dino.animation[0]=0;
    Dino.animation[1]=0;
    Dino.dimentions[0]=al_get_bitmap_width(dino)/3;
    Dino.dimentions[1]=al_get_bitmap_height(dino)/2;
    Dino.direction=1;


    //FLIP IMAGE BUFFERED
    al_flip_display();

    //START TIMER
    al_start_timer(timer);
    al_start_timer(frameTimer);


    //gain=volume-pan=side of sound
    al_play_sample(introMusic, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);

    while (running) {

        ALLEGRO_EVENT event;
        al_wait_for_event(eventQueue, &event);


        if (event.type == ALLEGRO_EVENT_TIMER) {

            if(event.timer.source==timer){

                if(Dino.position[1]>=300) {
                    Dino.position[1] -= 20;
                }
                else if (camaraPosition[1]==0){
                    al_rest(0.5);
                    Dino.position[1]=506;
                    break;
                }
            }

            if (event.timer.source == frameTimer) {
                Dino.animation[0]+=al_get_bitmap_width(dino)/3;
                if(Dino.animation[0]>=al_get_bitmap_width(dino)){
                    Dino.animation[0]=0;
                }
            }

            refreshCamara(Dino);
            redraw = true;


        }


        if (redraw && al_is_event_queue_empty(eventQueue)) {

            redraw = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));

            al_draw_bitmap_region(background, 0, 0, 1000, 3450, camaraPosition[0], camaraPosition[1], 0);
            al_draw_bitmap_region(dino, Dino.animation[0], Dino.animation[1], al_get_bitmap_width(dino) / 3,al_get_bitmap_height(dino) / 2, 500-(al_get_bitmap_width(dino)/3)/2, Dino.position[1], 0);


            al_flip_display();


        }

    }

    //FREE MEMORY
    al_destroy_timer(timer);
    al_destroy_timer(frameTimer);
    al_destroy_bitmap(background);
    al_destroy_bitmap(dino);
    al_destroy_sample(introMusic);
    al_destroy_event_queue(eventQueue);

    return 0;

}

//*************** GAME LEVEL 1 SCENE ***************//

int level1(ALLEGRO_DISPLAY *display){

    ALLEGRO_EVENT_QUEUE *eventQueue=NULL;

    ALLEGRO_TIMER *timer=NULL;
    ALLEGRO_TIMER *frameTimer=NULL;

    ALLEGRO_SAMPLE *levelMusic=NULL;
    ALLEGRO_SAMPLE *jumpSound=NULL;
    ALLEGRO_SAMPLE *fieldSound=NULL;

    ALLEGRO_BITMAP *background=NULL;
    ALLEGRO_BITMAP *popoMove=NULL;
    ALLEGRO_BITMAP *nanaMove=NULL;
    ALLEGRO_BITMAP *dino=NULL;
    ALLEGRO_BITMAP *foca=NULL;
    ALLEGRO_BITMAP *yeti=NULL;
    ALLEGRO_BITMAP *pajaro=NULL;
    ALLEGRO_BITMAP *oso=NULL;
    ALLEGRO_BITMAP *block=NULL;



    //KEY VALUES (Determines which keys are able to be recognize)
    enum KEYS {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_W, KEY_S, KEY_A, KEY_D};
    bool key[8] = { false, false, false, false, false, false, false, false };
    bool redraw=true;
    bool jump=true;
    bool exit=false;

    //CREATE TIMERS
    timer = al_create_timer(1.0 / FPS);
    frameTimer = al_create_timer(1.0 / AnimationFPS);
    if(!timer || !frameTimer){
        al_show_native_message_box(display, "Error", "Error", "Fail to create timer",
                                   NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    //LOAD IMAGE FILES
    popoMove=al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/Popo.png");
    nanaMove=al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/Nana.png");
    background = al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/Backgrounds.png");
    block=al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/Blocks.png");
    foca=al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/Foca.png");
    yeti=al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/Yeti.png");
    pajaro=al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/Pajaro.png");
    oso=al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/Oso.png");
    dino=al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/Dino.png");
    if(!popoMove || !nanaMove || !background || !dino || !foca || !yeti || !oso ||!pajaro) {
        al_show_native_message_box(display, "Error", "Error", "Failed to load image file",
                                   NULL, ALLEGRO_MESSAGEBOX_ERROR);

        al_destroy_timer(timer);
        al_destroy_display(display);
        return 0;
    }

    //MASK BLACK COLOR FROM IMAGES
    al_convert_mask_to_alpha(popoMove,al_map_rgb(0,0,0));
    al_convert_mask_to_alpha(nanaMove,al_map_rgb(0,0,0));
    al_convert_mask_to_alpha(dino,al_map_rgb(0,0,0));
    al_convert_mask_to_alpha(oso,al_map_rgb(0,0,0));
    al_convert_mask_to_alpha(foca,al_map_rgb(0,0,0));
    al_convert_mask_to_alpha(pajaro,al_map_rgb(0,0,0));
    al_convert_mask_to_alpha(yeti,al_map_rgb(0,0,0));
    al_convert_mask_to_alpha(block,al_map_rgb(0,0,0));

    //LOADING AUDIO FILES
    levelMusic = al_load_sample("/home/gunther/CLionProjects/ICE_Client/_sounds/LevelMusic.wav" );
    jumpSound = al_load_sample("/home/gunther/CLionProjects/ICE_Client/_sounds/Jump.wav" );
    fieldSound = al_load_sample("/home/gunther/CLionProjects/ICE_Client/_sounds/DestroyField.wav" );
    if (!levelMusic || !jumpSound || !fieldSound) {
        al_show_native_message_box(display, "Error", "Error", "Failed to load audio clip",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_bitmap(background);
        al_destroy_bitmap(dino);
        al_destroy_bitmap(foca);
        al_destroy_bitmap(oso);
        al_destroy_bitmap(yeti);
        al_destroy_bitmap(pajaro);
        al_destroy_bitmap(nanaMove);
        al_destroy_timer(frameTimer);
        al_destroy_bitmap(popoMove);

        return 0;
    }




    //CREATE AND MANAGE EVENT QUEUE
    eventQueue=al_create_event_queue();
    if(!eventQueue){
        al_show_native_message_box(display, "Error", "Error", "Fail to create Event Queue",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_timer(frameTimer);
        al_destroy_sample(levelMusic);
        al_destroy_bitmap(nanaMove);
        al_destroy_bitmap(popoMove);
        al_destroy_bitmap(oso);
        al_destroy_bitmap(yeti);
        al_destroy_bitmap(pajaro);
        al_destroy_sample(jumpSound);
        al_destroy_bitmap(foca);
        al_destroy_sample(fieldSound);
        al_destroy_bitmap(background);
        al_destroy_bitmap(dino);
        return 0;
    }

    //REGISTER EVENTS TO EVENT QUEUE (LINK THEM)
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_register_event_source(eventQueue, al_get_timer_event_source(frameTimer));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());

    //CLEAR BITMAP
    al_clear_to_color(al_map_rgb(0,0,0));
    //***********************************************************************************
    //CREATE FLOORS

    struct Floor floors[7];

    createFloors(floors,block);

    //CREATE PLAYER CHARACTERS

    struct Character Popo;
    Popo.position[0]=718;
    Popo.position[1]=425;
    Popo.animation[0]=0;
    Popo.animation[1]=0;
    Popo.dimentions[0]=al_get_bitmap_width(popoMove)/5;
    Popo.dimentions[1]=al_get_bitmap_height(popoMove)/2;
    Popo.lives=3;
    Popo.score=0;
    Popo.jump=true;

    struct Character Nana;
    Nana.position[0]=250;
    Nana.position[1]=425;
    Nana.animation[0]=0;
    Nana.animation[1]=0;
    Nana.dimentions[0]=al_get_bitmap_width(nanaMove)/5;
    Nana.dimentions[1]=al_get_bitmap_height(nanaMove)/2;
    Nana.lives=3;
    Nana.score=0;
    Nana.jump=true;

    struct Character Dino;
    Dino.position[0]=125;
    Dino.position[1]=-2747;
    Dino.animation[0]=0;
    Dino.animation[1]=0;
    Dino.dimentions[0]=al_get_bitmap_width(dino)/3;
    Dino.dimentions[1]=al_get_bitmap_height(dino)/2;
    Dino.direction=1;



    //***********************************************************************************
    //FLIP IMAGE BUFFERED
    al_flip_display();

    //START TIMER
    al_start_timer(timer);
    al_start_timer(frameTimer);


    //gain=volume-pan=side of sound
    al_play_sample(levelMusic, 0.5, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);




    //GAME LOOP
    while(!exit){

        ALLEGRO_EVENT event;
        al_wait_for_event(eventQueue,&event);

        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            currentScene=4;
            break;
        }


        //******************** SINGLE PLAYER MODE **********************//

        if(gameMode==0){

            //TIMER EVENTS
            if(event.type==ALLEGRO_EVENT_TIMER) {

                //WIN GAME
                if(collide(Popo.position[0],Popo.position[1],Dino.position[0],Dino.position[1]+camaraPosition[1]+2850,Popo.dimentions[0],Popo.dimentions[1],Dino.dimentions[0],Dino.dimentions[1])){
                    restartCamara();
                    currentScene=3;
                    acceleration[0]+=1.5;
                    break;
                }

                //SET SCREEN LIMITS TO PLAYER
                if (key[KEY_UP] && Popo.jump ) {
                    Popo.jump=false;
                    Popo.position[1] -= 300;
                    al_play_sample(jumpSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    key[KEY_UP] = false;
                }

                if(!key[KEY_UP] && Popo.position[1] < 506-32+camaraPosition[1]+2850){
                    Popo.position[1]+=acceleration[1];
                }

                if (key[KEY_DOWN] ) {
                    Popo.jump=true;
                    printf("%s","Popo attack");
                }
                if (key[KEY_LEFT]) {
                    if(event.timer.source ==frameTimer) {
                        Popo.animation[0] += al_get_bitmap_width(popoMove) / 5;
                        Popo.animation[1] = 0;
                    }

                    if (Popo.animation[0]>(al_get_bitmap_width(popoMove)/5)*3){
                        Popo.animation[0]=0;
                    }

                    if (Popo.position[0]<-32){
                        Popo.position[0]=1000;
                    }else {
                        Popo.position[0]-= movementSpeed[0];
                    }
                }
                if (key[KEY_RIGHT]) {
                    if(event.timer.source ==frameTimer) {
                        Popo.animation[0] += al_get_bitmap_width(popoMove) / 5;
                        Popo.animation[1] = al_get_bitmap_height(popoMove) / 2;
                    }

                    if (Popo.animation[0]>(al_get_bitmap_width(popoMove)/5)*3){
                        Popo.animation[0]=0;
                    }
                    if (Popo.position[0]>1000){
                        Popo.position[0]=0;
                    }else {
                        Popo.position[0] += movementSpeed[0];
                    }
                }



                //ANIMATE DINO
                if(event.timer.source==frameTimer){
                    animateEnemy(dino,Dino.position,Dino.animation);
                }


                blockCollide(floors,&Popo,fieldSound);
                refreshCamara(Popo);
                redraw=true;

            }

            //KEY DOWN EVENTS
            else if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch (event.keyboard.keycode) {
                    case ALLEGRO_KEY_UP:
                        key[KEY_UP] = true;

                        break;

                    case ALLEGRO_KEY_DOWN:
                        key[KEY_DOWN] = true;

                        break;

                    case ALLEGRO_KEY_LEFT:
                        key[KEY_LEFT] = true;
                        break;

                    case ALLEGRO_KEY_RIGHT:
                        key[KEY_RIGHT] = true;
                        break;
                }
            }


            else if(event.type == ALLEGRO_EVENT_KEY_UP) {
                switch (event.keyboard.keycode) {
                    case ALLEGRO_KEY_UP:
                        key[KEY_UP] = false;
                        break;

                    case ALLEGRO_KEY_DOWN:
                        key[KEY_DOWN] = false;
                        break;

                    case ALLEGRO_KEY_LEFT:
                        key[KEY_LEFT] = false;
                        break;

                    case ALLEGRO_KEY_RIGHT:
                        key[KEY_RIGHT] = false;
                        break;
                }
            }



            if(redraw && al_is_event_queue_empty(eventQueue)){

                redraw=false;
                al_clear_to_color(al_map_rgb(0,0,0));

                al_draw_bitmap_region(background,0,0,1000,3450,camaraPosition[0],camaraPosition[1],0);
                al_draw_bitmap_region(popoMove, Popo.animation[0], Popo.animation[1], al_get_bitmap_width(popoMove)/5,al_get_bitmap_height(popoMove)/2,Popo.position[0],Popo.position[1]-60,0);
                al_draw_bitmap_region(dino, Dino.animation[0], Dino.animation[1], al_get_bitmap_width(dino)/3,al_get_bitmap_height(dino)/2,Dino.position[0],Dino.position[1]+camaraPosition[1]+2850,0);
                displayFloors(floors,block);

                al_flip_display();
            }






        }

            //******************** SCREEN MULTIPLAYER MODE *****************//

        else if(gameMode==1){


            if(event.type==ALLEGRO_EVENT_TIMER) {



                //SET SCREEN LIMITS TO BOUNCER
                if (key[KEY_UP] && Popo.position[1] >= 4.0 ) {

                    Popo.position[1] -= movementSpeed[1];

                }
                if(!key[KEY_UP] && Popo.position[1] < 506-32){
                    Popo.position[1]+=acceleration[1];
                }
                if (key[KEY_DOWN] &&Popo.position[1] ) {

                    printf("%s","POPO ATTACK");
                }
                if (key[KEY_LEFT]) {
                    if(event.timer.source ==frameTimer) {
                        Popo.animation[0] += al_get_bitmap_width(popoMove) / 5;
                        Popo.animation[1] = 0;
                    }

                    if (Popo.animation[0]>(al_get_bitmap_width(popoMove)/5)*3){
                        Popo.animation[0]=0;
                    }

                    if (Popo.position[0]<-32){
                        Popo.position[0]=1000;
                    }else {
                        Popo.position[0]-= movementSpeed[0];
                    }
                }
                if (key[KEY_RIGHT]) {
                    if(event.timer.source ==frameTimer) {
                        Popo.animation[0] += al_get_bitmap_width(popoMove) / 5;
                        Popo.animation[1] = al_get_bitmap_height(popoMove) / 2;
                    }

                    if (Popo.animation[0]>(al_get_bitmap_width(popoMove)/5)*3){
                        Popo.animation[0]=0;
                    }
                    if (Popo.position[0]>1000){
                        Popo.position[0]=0;
                    }else {
                        Popo.position[0] += movementSpeed[0];
                    }
                }


                if (key[KEY_W] && Popo.position[1] >= 4.0) {
                    Nana.position[1]-= movementSpeed[1];
                }
                if(!key[KEY_W] && Nana.position[1]<506-32){
                    Nana.position[1]+=acceleration[1];
                }
                if (key[KEY_S] && Nana.position[1] <= screenH - 81 - 4.0-94) {
                    //player2_y += 4.0;
                }
                if (key[KEY_A]) {

                    Nana.position[0]+=al_get_bitmap_width(nanaMove)/5;
                    Nana.position[1]=0;

                    if (Nana.position[0]>(al_get_bitmap_width(nanaMove)/5)*3){
                        Nana.position[0]=0;
                    }


                    if (Nana.position[0]<-32){
                        Nana.position[0]=1000;
                    }else {
                        Nana.position[0]-= movementSpeed[0];
                    }
                }
                if (key[KEY_D]) {

                    Nana.animation[0]+=al_get_bitmap_width(nanaMove)/5;
                    Nana.animation[1]=al_get_bitmap_height(nanaMove)/2;

                    if (Nana.animation[0]>(al_get_bitmap_width(nanaMove)/5)*3){
                        Nana.animation[0]=0;
                    }

                    if (Nana.position[0]>1000){
                        Nana.position[0]=0;
                    }else {
                        Nana.position[0] += movementSpeed[0];
                    }
                }


                if(event.timer.source ==frameTimer) {
                    //animateEnemy(dino,Dino.position,Dino.animation, Dino.direction);
                }

                refreshCamara(Popo);
                refreshCamara(Nana);
                redraw = true;
            }

            else if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch(event.keyboard.keycode) {
                    case ALLEGRO_KEY_UP:
                        key[KEY_UP] = true;
                        al_play_sample(jumpSound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                        break;

                    case ALLEGRO_KEY_DOWN:
                        key[KEY_DOWN] = true;
                        al_play_sample(fieldSound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                        break;

                    case ALLEGRO_KEY_LEFT:
                        key[KEY_LEFT] = true;
                        break;

                    case ALLEGRO_KEY_RIGHT:
                        key[KEY_RIGHT] = true;
                        break;

                    case ALLEGRO_KEY_W:
                        key[KEY_W] = true;
                        al_play_sample(jumpSound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                        break;

                    case ALLEGRO_KEY_S:
                        key[KEY_S] = true;
                        al_play_sample(fieldSound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                        break;

                    case ALLEGRO_KEY_A:
                        key[KEY_A] = true;
                        break;

                    case ALLEGRO_KEY_D:
                        key[KEY_D] = true;
                        break;

                }

            }
            else if(event.type == ALLEGRO_EVENT_KEY_UP) {
                switch(event.keyboard.keycode) {
                    case ALLEGRO_KEY_UP:
                        key[KEY_UP] = false;
                        break;

                    case ALLEGRO_KEY_DOWN:
                        key[KEY_DOWN] = false;
                        break;

                    case ALLEGRO_KEY_LEFT:
                        key[KEY_LEFT] = false;
                        break;

                    case ALLEGRO_KEY_RIGHT:
                        key[KEY_RIGHT] = false;
                        break;

                    case ALLEGRO_KEY_W:
                        key[KEY_W] = false;
                        break;

                    case ALLEGRO_KEY_S:
                        key[KEY_S] = false;
                        break;

                    case ALLEGRO_KEY_A:
                        key[KEY_A] = false;
                        break;

                    case ALLEGRO_KEY_D:
                        key[KEY_D] = false;
                        break;

                    case ALLEGRO_KEY_ESCAPE:
                        exit = true;
                        break;
                }
            }


            if(redraw && al_is_event_queue_empty(eventQueue)){
                redraw=false;
                al_clear_to_color(al_map_rgb(0,0,0));

                al_draw_bitmap_region(background,0,0,1000,3450,camaraPosition[0],camaraPosition[1],0);
                al_draw_bitmap_region(popoMove, Popo.animation[0], Popo.animation[1], al_get_bitmap_width(popoMove)/5,al_get_bitmap_height(popoMove)/2,Popo.position[0],Popo.position[1]-60,0);
                al_draw_bitmap_region(nanaMove, Nana.animation[0], Nana.animation[1], al_get_bitmap_width(nanaMove)/5,al_get_bitmap_height(nanaMove)/2,Nana.position[0],Nana.position[1]-60,0);
                al_draw_bitmap_region(dino, Dino.animation[0], Dino.animation[1], al_get_bitmap_width(dino)/3,al_get_bitmap_height(dino)/2,Dino.position[0],Dino.position[1]+camaraPosition[1]+2850,0);
                displayFloors(floors,block);

                al_flip_display();
            }









        }


            //******************** LOCAL MULTIPLAYER MODE *****************//

        else if(gameMode==2){



            if (playerID==1){

                if(event.type==ALLEGRO_EVENT_TIMER) {

                    //SET SCREEN LIMITS TO BOUNCER
                    if (key[KEY_UP] && Popo.position[1] >= 4.0 ) {

                        Popo.position[1] -= movementSpeed[1];

                        //key[KEY_UP] = false;
                    }
                    if(!key[KEY_UP] && Popo.position[1] < 506-32){
                        Popo.position[1]+=acceleration[1];
                    }
                    if (key[KEY_DOWN] &&Popo.position[1] <= screenH - 81 - 4.0-94) {
                        //player1_y += 4.0;
                    }
                    if (key[KEY_LEFT]) {
                        if(event.timer.source ==frameTimer) {
                            Popo.animation[0] += al_get_bitmap_width(popoMove) / 5;
                            Popo.animation[1] = 0;
                        }

                        if (Popo.animation[0]>(al_get_bitmap_width(popoMove)/5)*3){
                            Popo.animation[0]=0;
                        }

                        if (Popo.position[0]<-32){
                            Popo.position[0]=1000;
                        }else {
                            Popo.position[0]-= movementSpeed[0];
                        }
                    }
                    if (key[KEY_RIGHT]) {
                        if(event.timer.source ==frameTimer) {
                            Popo.animation[0] += al_get_bitmap_width(popoMove) / 5;
                            Popo.animation[1] = al_get_bitmap_height(popoMove) / 2;
                        }

                        if (Popo.animation[0]>(al_get_bitmap_width(popoMove)/5)*3){
                            Popo.animation[0]=0;
                        }
                        if (Popo.position[0]>1000){
                            Popo.position[0]=0;
                        }else {
                            Popo.position[0] += movementSpeed[0];
                        }
                    }

                    refreshCamara(Popo);
                    refreshCamara(Nana);
                    redraw=true;
                }


                else if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
                    switch (event.keyboard.keycode) {
                        case ALLEGRO_KEY_UP:
                            key[KEY_UP] = true;
                            al_play_sample(jumpSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            break;

                        case ALLEGRO_KEY_DOWN:
                            key[KEY_DOWN] = true;
                            al_play_sample(fieldSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            break;

                        case ALLEGRO_KEY_LEFT:
                            key[KEY_LEFT] = true;
                            break;

                        case ALLEGRO_KEY_RIGHT:
                            key[KEY_RIGHT] = true;
                            break;
                    }
                }


                else if(event.type == ALLEGRO_EVENT_KEY_UP) {
                    switch (event.keyboard.keycode) {
                        case ALLEGRO_KEY_UP:
                            key[KEY_UP] = false;
                            break;

                        case ALLEGRO_KEY_DOWN:
                            key[KEY_DOWN] = false;
                            break;

                        case ALLEGRO_KEY_LEFT:
                            key[KEY_LEFT] = false;
                            break;

                        case ALLEGRO_KEY_RIGHT:
                            key[KEY_RIGHT] = false;
                            break;
                    }
                }





                if(redraw && al_is_event_queue_empty(eventQueue)){
                    redraw=false;
                    al_clear_to_color(al_map_rgb(0,0,0));

                    al_draw_bitmap_region(background,0,0,1000,3450,camaraPosition[0],camaraPosition[1],0);
                    al_draw_bitmap_region(popoMove, Popo.animation[0], Popo.animation[1], al_get_bitmap_width(popoMove)/5,al_get_bitmap_height(popoMove)/2,Popo.position[0],Popo.position[1]-60,0);
                    al_draw_bitmap_region(nanaMove, Nana.animation[0], Nana.animation[1], al_get_bitmap_width(nanaMove)/5,al_get_bitmap_height(nanaMove)/2,Nana.position[0],Nana.position[1]-60,0);
                    al_draw_bitmap_region(dino, Dino.animation[0], Dino.animation[1], al_get_bitmap_width(dino)/3,al_get_bitmap_height(dino)/2,Dino.position[0],Dino.position[1]+camaraPosition[1]+2850,0);
                    displayFloors(floors,block);

                    al_flip_display();
                }






            }
            else if(playerID==2){



                if(event.type==ALLEGRO_EVENT_TIMER) {

                    //SET SCREEN LIMITS TO BOUNCER
                    if (key[KEY_UP] && Nana.position[1] >= 4.0 ) {

                        Nana.position[1] -= movementSpeed[1];

                        //key[KEY_UP] = false;
                    }
                    if(!key[KEY_UP] && Nana.position[1] < 506-32){
                        Nana.position[1]+=acceleration[1];
                    }
                    if (key[KEY_DOWN] ) {

                        printf("%s","NANA ATTACK");
                    }
                    if (key[KEY_LEFT]) {
                        if(event.timer.source ==frameTimer) {
                            Nana.animation[0] += al_get_bitmap_width(nanaMove) / 5;
                            Nana.animation[1] = 0;
                        }

                        if (Nana.animation[0]>(al_get_bitmap_width(nanaMove)/5)*3){
                            Nana.animation[0]=0;
                        }

                        if (Nana.position[0]<-32){
                            Nana.position[0]=1000;
                        }else {
                            Nana.position[0]-= movementSpeed[0];
                        }
                    }
                    if (key[KEY_RIGHT]) {
                        if(event.timer.source ==frameTimer) {
                            Nana.animation[0] += al_get_bitmap_width(nanaMove) / 5;
                            Nana.animation[1] = al_get_bitmap_height(nanaMove) / 2;
                        }

                        if (Nana.animation[0]>(al_get_bitmap_width(nanaMove)/5)*3){
                            Nana.animation[0]=0;
                        }
                        if (Nana.position[0]>1000){
                            Nana.position[0]=0;
                        }else {
                            Nana.position[0] += movementSpeed[0];
                        }
                    }

                    refreshCamara(Popo);
                    refreshCamara(Nana);
                    redraw=true;

                }


                else if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
                    switch (event.keyboard.keycode) {
                        case ALLEGRO_KEY_UP:
                            key[KEY_UP] = true;
                            al_play_sample(jumpSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            break;

                        case ALLEGRO_KEY_DOWN:
                            key[KEY_DOWN] = true;
                            al_play_sample(fieldSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            break;

                        case ALLEGRO_KEY_LEFT:
                            key[KEY_LEFT] = true;
                            break;

                        case ALLEGRO_KEY_RIGHT:
                            key[KEY_RIGHT] = true;
                            break;
                    }
                }


                else if(event.type == ALLEGRO_EVENT_KEY_UP) {
                    switch (event.keyboard.keycode) {
                        case ALLEGRO_KEY_UP:
                            key[KEY_UP] = false;
                            break;

                        case ALLEGRO_KEY_DOWN:
                            key[KEY_DOWN] = false;
                            break;

                        case ALLEGRO_KEY_LEFT:
                            key[KEY_LEFT] = false;
                            break;

                        case ALLEGRO_KEY_RIGHT:
                            key[KEY_RIGHT] = false;
                            break;
                    }
                }





                if(redraw && al_is_event_queue_empty(eventQueue)){
                    redraw=false;
                    al_clear_to_color(al_map_rgb(0,0,0));

                    al_draw_bitmap_region(background,0,0,1000,3450,camaraPosition[0],camaraPosition[1],0);
                    al_draw_bitmap_region(popoMove, Popo.animation[0], Popo.animation[1], al_get_bitmap_width(popoMove)/5,al_get_bitmap_height(popoMove)/2,Popo.position[0],Popo.position[1]-60,0);
                    al_draw_bitmap_region(nanaMove, Nana.animation[0], Nana.animation[1], al_get_bitmap_width(nanaMove)/5,al_get_bitmap_height(nanaMove)/2,Nana.position[0],Nana.position[1]-60,0);
                    al_draw_bitmap_region(dino, Dino.animation[0], Dino.animation[1], al_get_bitmap_width(dino)/3,al_get_bitmap_height(dino)/2,Dino.position[0],Dino.position[1]+camaraPosition[1]+2850,0);
                    displayFloors(floors,block);

                    al_flip_display();
                }







            }
            else if(playerID==3){

                if(event.type==ALLEGRO_EVENT_TIMER){

                    refreshCamara(Popo);
                    refreshCamara(Nana);
                    redraw=true;
                }

                if(redraw && al_is_event_queue_empty(eventQueue)){
                    redraw=false;
                    al_clear_to_color(al_map_rgb(0,0,0));
                    al_draw_bitmap_region(background,0,0,1000,3450,camaraPosition[0],camaraPosition[1],0);
                    al_draw_bitmap_region(popoMove, Popo.animation[0], Popo.animation[1], al_get_bitmap_width(popoMove)/5,al_get_bitmap_height(popoMove)/2,Popo.position[0],Popo.position[1]-60,0);
                    al_draw_bitmap_region(nanaMove, Nana.animation[0], Nana.animation[1], al_get_bitmap_width(nanaMove)/5,al_get_bitmap_height(nanaMove)/2,Nana.position[0],Nana.position[1]-60,0);
                    al_draw_bitmap_region(dino, Dino.animation[0], Dino.animation[1], al_get_bitmap_width(dino)/3,al_get_bitmap_height(dino)/2,Dino.position[0],Dino.position[1]+camaraPosition[1]+2850,0);
                    displayFloors(floors,block);


                    al_flip_display();
                }


            }


        }



    }

    //FREE MEMORY AFTER CLOSE PROGRAM
    al_destroy_bitmap(background);
    al_destroy_sample(levelMusic);
    al_destroy_sample(jumpSound);
    al_destroy_sample(fieldSound);
    al_destroy_bitmap(popoMove);
    al_destroy_bitmap(foca);
    al_destroy_bitmap(yeti);
    al_destroy_bitmap(oso);
    al_destroy_bitmap(pajaro);
    al_destroy_bitmap(nanaMove);
    al_destroy_timer(frameTimer);
    al_destroy_bitmap(dino);
    al_destroy_bitmap(block);
    al_destroy_timer(timer);

    al_destroy_event_queue(eventQueue);

    return 0;
}

//************************************************************************************************************************************//
//****************************************************** GAME SCENE MANAGER **********************************************************//


//******************* START GAME *******************//

int startGame(int players) {

    //CREATING INTERFACE FIELDS
    ALLEGRO_DISPLAY *display=NULL;
    //
    bool running=true;


    //INIT ALLEGRO LIBRARY
    if(!al_init()){
        al_show_native_message_box(display, "Error", "Error", "Fail to init Allegro",
                                   NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    //CREATE IMAGE LOADER
    if(!al_init_image_addon()) {
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize al_init_image_addon",
                                   NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    //HANDLE KEYBOARD EVENTS
    if(!al_install_keyboard()) {
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize the keyboard",
                                   NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    //HANDLE MOUSE EVENTS
    if(!al_install_mouse()) {
        fprintf(stderr, "failed to initialize the mouse!\n");
        return -1;
    }

    //CREATE PRIMITIVE FORMS DRAWER
    if(!al_init_primitives_addon()) {
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize primitive forms drawer!",NULL, ALLEGRO_MESSAGEBOX_ERROR);
    }

    //CREATE SOUND SYSTEM
    if(!al_install_audio()){
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize audio",
                                   NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    //LOAD AUDIO FORMATS
    if(!al_init_acodec_addon()) {
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize audio codecs",
                                   NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    //RESERVE A NUMBER OF AUDIO SAMPLES, AND CREATES A DEFAULT MIXER
    if (!al_reserve_samples(5)){
        al_show_native_message_box(display, "Error", "Error", "Failed to reserve samples",
                                   NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }


    //CREATE DISPLAY WINDOW
    display=al_create_display(screenW,screenH);
    if(!display){
        al_show_native_message_box(display, "Error", "Error", "Fail to init Window",
                                   NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }
    //SET WINDOW TITLE
    al_set_window_title(display,"ICE-CLimber");

    while (running) {

        if(currentScene==0) {
            titleScene(display);
        }
        else if(currentScene==1){
            gameModeScene(display);
        }
        else if(currentScene==2) {
            enemySelectionScene(display);
        }
        else if(currentScene==3){
            //levelIntro(display);
            //restartCamara();
            level1(display);
        }
        else{
            break;
        }

    }


    al_destroy_display(display);


}





