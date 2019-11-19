//
// Created by gunther on 16/11/19.
//

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

//Global Fields
const float FPS=60;//Determines the frames per second in game
const  int screenW=1000;//Determines window width
const  int screenH=600;//Determiner window height
float popoAnimation[2]={0,0};
float nanaAnimation[2]={0,0};
float dinoAnimation[2]={0,0};
//Hold keys values / Determines which keys are able to be recognize
enum KEYS {
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_W, KEY_S, KEY_A, KEY_D
};



//********************************************************

//PAYERS VALUES
float playerMovementSpeed[2]={4.0,7.0};
float acceleration[2]={0,9.8};
//PLAYER 1 POSITION
float  popoPosition[2]={718,506-81};
//PLAYER 2 POSITION
float  nanaPosition[2]={250,506-81};
//CAMARA POSITION
float camaraPosition[2]={0,-2850};
//ENEMY POSITION
float dinoPosition[2]={0,400};


int dir=1;//1=derecha
bool jump=false;
//********************************************************

bool Collision(float x, float y, float ex, float ey,int width, int height){


    if(x+width < ex || x > ex+width || y+height < ey || y > ey+height){
        return false;
    }
    return true;
}

void moveEnemy(ALLEGRO_BITMAP *dino){

    dinoAnimation[0]+=al_get_bitmap_width(dino)/3;
    if(dinoAnimation[0]>al_get_bitmap_width(dino)){
        dinoAnimation[0]=0;
    }
    if(dir==0 ){
        dinoAnimation[1]=0;
        if(dinoPosition[0]>=4.0){
            dinoPosition[0]-=3;
        }else{
            dir=1;
        }

    }
    if (dir==1 ){
        dinoAnimation[1]=al_get_bitmap_height(dino)/2;
        if(dinoPosition[0]<=(996-al_get_bitmap_width(dino)/3)){
            dinoPosition[0]+=3;
        }else{

            dir=0;
        }

    }




}

//SCROLLING BACKGROUND
void refreshCamara(){
    if (popoPosition[1]<=200 || nanaPosition[1]<=200){
        camaraPosition[1]+=20;
    }

    if(camaraPosition[1]>=0){
        camaraPosition[1]=0;
    }
}



//Starts Game Interface
int startGame() {

    //CREATING INTERFACE FIELDS
    ALLEGRO_DISPLAY *display=NULL;
    ALLEGRO_EVENT_QUEUE *eventQueue=NULL;
    ALLEGRO_TIMER *timer=NULL;
    ALLEGRO_SAMPLE *titleMusic=NULL;
    ALLEGRO_SAMPLE *jumpSound=NULL;
    ALLEGRO_SAMPLE *fieldSound=NULL;
    ALLEGRO_BITMAP *background=NULL;
    ALLEGRO_BITMAP *popoMove=NULL;
    ALLEGRO_BITMAP *nanaMove=NULL;
    ALLEGRO_BITMAP *dino=NULL;
    ALLEGRO_BITMAP *bouncer3=NULL;
    bool key[8] = { false, false, false, false, false, false, false, false };
    bool redraw=true;
    bool exit=false;


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

    //CREATE TIMER
    timer = al_create_timer(1.0 / FPS);
    if(!timer){
        al_show_native_message_box(display, "Error", "Error", "Fail to create timer",
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
    if (!al_reserve_samples(3)){
        al_show_native_message_box(display, "Error", "Error", "Failed to reserve samples",
                                   NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    //LOAD IMAGE FILES
    popoMove=al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/Popo.png");
    nanaMove=al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/Nana.png");
    background = al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/Backgrounds.png");
    dino=al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/Dino.png");
    if(!popoMove || !nanaMove || !background || !dino) {
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

    //LOADING AUDIO FILES
    titleMusic = al_load_sample("/home/gunther/CLionProjects/ICE_Client/_sounds/_TitleScreen.wav" );
    jumpSound = al_load_sample("/home/gunther/CLionProjects/ICE_Client/_sounds/Jump.wav" );
    fieldSound = al_load_sample("/home/gunther/CLionProjects/ICE_Client/_sounds/DestroyField.wav" );
    if (!titleMusic || !jumpSound || !fieldSound) {
        al_show_native_message_box(display, "Error", "Error", "Failed to load audio clip",
                                   NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_bitmap(background);
        al_destroy_bitmap(dino);
        al_destroy_bitmap(nanaMove);
        al_destroy_bitmap(popoMove);

        return 0;
    }

    //CREATE BITMAP
    bouncer3=al_create_bitmap(1000,20);
    if(!bouncer3){
        al_show_native_message_box(display, "Error", "Error", "Fail to create bitmap",
                                   NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_bitmap(background);
        al_destroy_timer(timer);
        al_destroy_sample(titleMusic);
        al_destroy_sample(jumpSound);
        al_destroy_sample(fieldSound);
        al_destroy_display(display);
        al_destroy_bitmap(dino);
        al_destroy_bitmap(nanaMove);
        al_destroy_bitmap(popoMove);
        return 0;
    }

    //DRAWING BITMAPS

    al_set_target_bitmap(bouncer3);
    al_clear_to_color(al_map_rgb(0, 255, 0));
    al_set_target_bitmap(al_get_backbuffer(display));


    //CREATE AND MANAGE EVENT QUEUE
    eventQueue=al_create_event_queue();
    if(!eventQueue){
        al_show_native_message_box(display, "Error", "Error", "Fail to create Event Queue",
                                   NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_sample(titleMusic);
        al_destroy_bitmap(bouncer3);
        al_destroy_bitmap(nanaMove);
        al_destroy_bitmap(popoMove);
        al_destroy_sample(jumpSound);
        al_destroy_sample(fieldSound);
        al_destroy_bitmap(background);
        al_destroy_bitmap(dino);
        return 0;
    }

    //REGISTER EVENTS TO EVENT QUEUE (LINK THEM)
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_clear_to_color(al_map_rgb(0,0,0));

    //FLIP IMAGE BUFFERED
    al_flip_display();
    //START TIMER
    al_start_timer(timer);

    //gain=volume-pan=side of sound
    al_play_sample(titleMusic, 0.5, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);




    //GAME LOOP
    while(!exit){
        ALLEGRO_EVENT event;
        al_wait_for_event(eventQueue,&event);


        if(event.type==ALLEGRO_EVENT_TIMER) {

            //SET SCREEN LIMITS TO BOUNCER
            if (key[KEY_UP] && popoPosition[1] >= 4.0 ) {

                popoPosition[1] -= playerMovementSpeed[1];
                
                //key[KEY_UP] = false;
            }
            if(!key[KEY_UP] && popoPosition[1] < 506-32){
                popoPosition[1]+=acceleration[1];
            }
            if (key[KEY_DOWN] &&popoPosition[1] <= screenH - 81 - 4.0-94) {
                //player1_y += 4.0;
            }
            if (key[KEY_LEFT]) {

                popoAnimation[0]+=al_get_bitmap_width(popoMove)/5;
                popoAnimation[1]=0;

                if (popoAnimation[0]>(al_get_bitmap_width(popoMove)/5)*3){
                    popoAnimation[0]=0;
                }

                if (popoPosition[0]<-32){
                    popoPosition[0]=1000;
                }else {
                    popoPosition[0]-= playerMovementSpeed[0];
                }
            }
            if (key[KEY_RIGHT]) {

                popoAnimation[0]+=al_get_bitmap_width(popoMove)/5;
                popoAnimation[1]=al_get_bitmap_height(popoMove)/2;

                if (popoAnimation[0]>(al_get_bitmap_width(popoMove)/5)*3){
                    popoAnimation[0]=0;
                }
                if (popoPosition[0]>1000){
                    popoPosition[0]=0;
                }else {
                    popoPosition[0] += playerMovementSpeed[0];
                }
            }


            if (key[KEY_W] && nanaPosition[1] >= 4.0) {
                nanaPosition[1]-= playerMovementSpeed[1];
            }
            if(!key[KEY_W] && nanaPosition[1]<506-32){
                nanaPosition[1]+=acceleration[1];
            }
            if (key[KEY_S] && nanaPosition[1] <= screenH - 81 - 4.0-94) {
                //player2_y += 4.0;
            }
            if (key[KEY_A]) {

                nanaAnimation[0]+=al_get_bitmap_width(nanaMove)/5;
                nanaAnimation[1]=0;

                if (nanaAnimation[0]>(al_get_bitmap_width(nanaMove)/5)*3){
                    nanaAnimation[0]=0;
                }


                if (nanaPosition[0]<-32){
                    nanaPosition[0]=1000;
                }else {
                    nanaPosition[0]-= playerMovementSpeed[0];
                }
            }
            if (key[KEY_D]) {

                nanaAnimation[0]+=al_get_bitmap_width(nanaMove)/5;
                nanaAnimation[1]=al_get_bitmap_height(nanaMove)/2;

                if (nanaAnimation[0]>(al_get_bitmap_width(nanaMove)/5)*3){
                    nanaAnimation[0]=0;
                }

                if (nanaPosition[0]>1000){
                    nanaPosition[0]=0;
                }else {
                    nanaPosition[0] += playerMovementSpeed[0];
                }
            }

            //initClient(jsonWriter(1,player1_x, player1_y, 0, 3));
            moveEnemy(dino);
            refreshCamara();
            redraw = true;
        }
        else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
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
            //al_draw_bitmap(bouncer3, 0, 506, 0);
            al_draw_bitmap_region(background,0,0,1000,3450,camaraPosition[0],camaraPosition[1],0);
            al_draw_bitmap_region(popoMove, popoAnimation[0], popoAnimation[1], al_get_bitmap_width(popoMove)/5,al_get_bitmap_height(popoMove)/2,popoPosition[0],popoPosition[1]-60,0);
            al_draw_bitmap_region(nanaMove, nanaAnimation[0], nanaAnimation[1], al_get_bitmap_width(nanaMove)/5,al_get_bitmap_height(nanaMove)/2,nanaPosition[0],nanaPosition[1]-60,0);
            al_draw_bitmap_region(dino, dinoAnimation[0], dinoAnimation[1], al_get_bitmap_width(dino)/3,al_get_bitmap_height(dino)/2,dinoPosition[0],dinoPosition[1],0);
            //al_draw_bitmap(bouncer3, 0, 506, 0);
            al_flip_display();
        }

    }

    //FREE MEMORY AFTER CLOSE PROGRAM
    al_destroy_bitmap(background);
    al_destroy_sample(titleMusic);
    al_destroy_sample(jumpSound);
    al_destroy_sample(fieldSound);
    al_destroy_bitmap(popoMove);
    al_destroy_bitmap(nanaMove);
    al_destroy_bitmap(bouncer3);
    al_destroy_bitmap(dino);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(eventQueue);

    return 0;
}





