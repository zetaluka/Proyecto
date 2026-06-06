#ifndef COMMONS_H
#define COMMONS_H


//==========Librerias=========//
#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/display.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/events.h>
#include <allegro5/timer.h>
#include <allegro5/color.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>


//==========Defines==========//
#define SCREEN_X 1280
#define SCREEN_Y 720
#define FPS 60


//==========Estructuras==========//

//==Structs Assets==//
typedef struct{

    ALLEGRO_FONT* shingekiFont;
    ALLEGRO_FONT* minimalistTemplateFont;    

} s_Assets;

//==============================//

typedef struct
{
    int milisegundos;
    int segundos;
    int minutos;
    int validacion;
} s_Temporizador;

typedef enum {
    PANTALLA_MENU,
    PANTALLA_JUGANDO,
    PANTALLA_GAME_OVER
} s_Pantalla;

typedef struct { //input.c actualiza a través de la variable s_GameState, update.c lo lee y reacciona.
    bool keyW;
    bool keyS;
    bool keyD;
    bool keyA;
} s_InputState;


//====s_GameState====//
typedef struct {
    s_Temporizador tiempoJugado;
    s_Pantalla pantalla;      /* pantalla activa */
    s_InputState input;    /* estado del input */  
    int nivel; /*nivel actual*/

    bool ejecutando; /* mantiene el juego corriendo */
    /* Agrega los campos que necesite tu juego */
} s_GameState;


//==========Prototipos de funciones==========//
void game_init(s_GameState *gs);
void input_update(s_InputState *input, ALLEGRO_EVENT* evento);
void update(s_GameState *gs, s_InputState *input);
void render_gameview(s_GameState *gs);
void render_ui(s_GameState *gs, s_Assets *assets);
void assets_load(s_Assets *assets);


#endif