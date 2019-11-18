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
//Hold keys values / Determines which keys are able to be recognize
enum KEYS {
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_W, KEY_S, KEY_A, KEY_D
};



//********************************************************
const  int bouncerSize=32;
//PAYERS VALUES
float playerVel=4.0;
float gravity=-9.8;
//PLAYER 1 POSITION
float player1_x = 560;
float player1_y = 392;                                      //Could change in future
//PLAYER 2 POSITION
float player2_x = 48;
float player2_y = 392;
//********************************************************



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
    ALLEGRO_BITMAP *bouncer=NULL;
    ALLEGRO_BITMAP *bouncer2=NULL;
    bool key[8] = { false, false, false, false, false, false, false, false };
    bool redraw=true;
    bool exit=false;
    float camaraPosition[2]={0,0};

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
    background = al_load_bitmap("/home/gunther/CLionProjects/ICE_Client/_imgs/Backgrounds.png");
    if(!background) {
        al_show_native_message_box(display, "Error", "Error", "Failed to load image file",
                                   NULL, ALLEGRO_MESSAGEBOX_ERROR);

        al_destroy_timer(timer);
        al_destroy_display(display);
        return 0;
    }

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
        return 0;
    }

    //CREATE BITMAP
    bouncer=al_create_bitmap(bouncerSize,bouncerSize);
    bouncer2=al_create_bitmap(bouncerSize,bouncerSize);
    if(!bouncer || !bouncer2){
        al_show_native_message_box(display, "Error", "Error", "Fail to create bitmap",
                                   NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_bitmap(background);
        al_destroy_timer(timer);
        al_destroy_sample(titleMusic);
        al_destroy_sample(jumpSound);
        al_destroy_sample(fieldSound);
        al_destroy_display(display);
        //al_destroy_bitmap(image);
        return 0;
    }

    //DRAWING BITMAPS

    al_set_target_bitmap(bouncer);
    al_clear_to_color(al_map_rgb(100, 0, 255));
    al_set_target_bitmap(al_get_backbuffer(display));

    al_set_target_bitmap(bouncer2);
    al_clear_to_color(al_map_rgb(255, 0, 100));
    al_set_target_bitmap(al_get_backbuffer(display));


    //CREATE AND MANAGE EVENT QUEUE
    eventQueue=al_create_event_queue();
    if(!eventQueue){
        al_show_native_message_box(display, "Error", "Error", "Fail to create Event Queue",
                                   NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_sample(titleMusic);
        al_destroy_sample(jumpSound);
        al_destroy_sample(fieldSound);
        al_destroy_bitmap(background);
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
            if (key[KEY_UP] && player1_y >= 4.0) {
                player1_y -= playerVel;
            }
            if (key[KEY_DOWN] && player1_y <= screenH - bouncerSize - 4.0) {
                //player1_y += 4.0;
            }
            if (key[KEY_LEFT] && player1_x >= 4.0) {
                player1_x -= playerVel;
            }
            if (key[KEY_RIGHT] && player1_x <= screenW - bouncerSize - 4.0) {
                player1_x += playerVel;
            }


            if (key[KEY_W] && player2_y >= 4.0) {
                player2_y -= playerVel;
            }
            if (key[KEY_S] && player2_y <= screenH - bouncerSize - 4.0) {
                //player2_y += 4.0;
            }
            if (key[KEY_A] && player2_x >= 4.0) {
                player2_x -= playerVel;
            }
            if (key[KEY_D] && player2_x <= screenW - bouncerSize - 4.0) {
                player2_x += playerVel;
            }

            //initClient(jsonWriter(1,player1_x, player1_y, 0, 3));
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
            al_draw_bitmap_region(background,0,0,1000,3450,0,-2850,0);
            al_draw_bitmap(bouncer, player1_x, player1_y, 0);
            al_draw_bitmap(bouncer2, player2_x, player2_y, 0);
            al_flip_display();
        }

    }

    //FREE MEMORY AFTER CLOSE PROGRAM
    al_destroy_bitmap(background);
    al_destroy_sample(titleMusic);
    al_destroy_sample(jumpSound);
    al_destroy_sample(fieldSound);
    al_destroy_bitmap(bouncer);
    al_destroy_bitmap(bouncer2);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(eventQueue);

    return 0;
}