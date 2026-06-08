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
#define GRAVEDAD 1
#define MAXPANTALLAS 50
#define MAXHITBOX 50


//==========Estructuras==========//

//==Structs Assets==//

typedef struct 
{
    ALLEGRO_BITMAP* fondo_base;

} s_AssetsPantalla;


typedef struct{
    ALLEGRO_BITMAP* levi_parado;

} s_LeviSprites;

typedef struct{

    ALLEGRO_FONT* shingekiFont;
    ALLEGRO_FONT* minimalistTemplateFont;    
    s_AssetsPantalla assetsPantalla[MAXPANTALLAS];
    s_LeviSprites levi;

} s_Assets;

//==============================//

typedef struct 
{
    float x;
    float y;
    float ancho;
    float alto;
} s_Hitbox;

typedef struct
{
    s_Hitbox hitbox[MAXHITBOX];
    int num_hitbox;
    
} s_Pantalla;

typedef struct
{
    float x;
    float y;
    float velocidadX;
    float velocidadY;
    float doble_salto;
    bool levi_suelo;
    s_Hitbox hitbox;
} s_Levi;

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
} s_EstadoPantalla;

typedef struct { //input.c actualiza a través de la variable s_GameState, update.c lo lee y reacciona.
    bool keyW;
    bool keyS;
    bool keyD;
    bool keyA;
    bool keySpace;
    bool keyLShift;
    bool keyL; //Para saber posicion de levi
} s_InputState;


//====s_GameState====//
typedef struct {
    s_Temporizador tiempoJugado;
    s_EstadoPantalla estadoPantalla;      
    s_InputState input;  
    s_Levi levi;    
    s_Pantalla pantalla[MAXPANTALLAS];
    int pantalla_actual;
    int nivel; 
    float escala;

    bool ejecutando; 
} s_GameState;


//==========Prototipos de funciones==========//
void game_init(s_GameState *gs);
void input_update(s_InputState *input, ALLEGRO_EVENT* evento);
void update(s_GameState *gs, s_InputState *input, s_Assets *assets);
void render_gameview(s_GameState *gs, s_Assets *assets);
void render_ui(s_GameState *gs, s_Assets *assets);
void assets_load(s_Assets *assets);


#endif