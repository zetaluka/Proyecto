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
#define MAXPANTALLAS 4
#define MAXHITBOX 50
#define TAM_CELDA 32
#define MAXFIL 22
#define MAXCOL 40
#define MAXENTIDADES 50
#define LEVI_HB_RECORTE 37
#define BLANCO al_map_rgb(220, 220, 220)


//==========Estructuras==========//

//==Structs Assets==//

typedef struct 
{
    ALLEGRO_BITMAP* fondo_base;
    ALLEGRO_BITMAP* cubo;

} s_AssetsPantalla;


typedef struct{
    ALLEGRO_BITMAP* levi_parado;

} s_LeviSprites;

typedef struct 
{
    ALLEGRO_BITMAP* titan_bizarro;

} s_TitanesSprites;


typedef struct{

    ALLEGRO_FONT* shingekiFont;
    ALLEGRO_FONT* minimalistTemplateFont;    
    s_AssetsPantalla assetsPantalla[MAXPANTALLAS];
    s_LeviSprites levi;
    s_TitanesSprites titanes;

} s_Assets;

//==============================//

typedef struct 
{
    float x;
    float y;
    float ancho;
    float alto;
    ALLEGRO_COLOR color;
} s_Hitbox;

typedef struct 
{
    int x;
    int y;
    int velocidadX;
    int velocidadY;
    int vida;
    int ataque;
    s_Hitbox hitboxAtaque;
    s_Hitbox hitboxTitan;
    s_Hitbox hitboxDeteccion;
    bool activo;
} s_Entidades;

typedef struct
{
    s_Hitbox hitbox[MAXHITBOX];
    s_Entidades entidades[MAXENTIDADES];
    int num_entidades;
    int num_hitbox;
    int num_pantallas;
    
} s_Pantalla;

typedef struct
{
    int viendoDerecha;
    float cooldownAtaque;
    float x;
    float y;
    float velocidadX;
    float velocidadY;
    bool doble_salto;
    bool levi_suelo;
    s_Hitbox hitbox;
    s_Hitbox hitboxAtaque;
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
    bool keyH; //Para visualizar hitbox de levi
    bool keyG; //Detener entidades
    bool ClickIzq;
    float mouseX;
    float mouseY;
} s_InputState;

typedef struct 
{
    int carga_pantalla;
    FILE *fdata;
    bool cambioSentido;
    bool detenerEntidades;
    s_Hitbox titan1;

} s_Variables;

typedef struct 
{
    char mapa1[MAXFIL][MAXCOL];
    int num_pantallas;

} s_Mapas;

//====s_GameState====//
typedef struct {
    s_Temporizador tiempoJugado;
    s_EstadoPantalla estadoPantalla;      
    s_InputState input;  
    s_Levi levi;    
    s_Pantalla pantalla[MAXPANTALLAS];
    s_Variables variables;
    s_Mapas mapas;
    int pantalla_actual;
    int nivel; 
    int lado_colision;
    float escala;

    bool ejecutando; 
} s_GameState;


//==========Prototipos de funciones==========//
void game_init(s_GameState *gs, s_Assets *assets);
void input_update(s_GameState *gs, ALLEGRO_EVENT* evento);
void update(s_GameState *gs, s_Assets *assets);
void render_gameview(s_GameState *gs, s_Assets *assets);
void render_ui(s_GameState *gs, s_Assets *assets);
void assets_load(s_Assets *assets);
void genera_entidades(s_GameState *gs, s_Assets *assets);

//==========Prototipos de funciones no principales======//
void mapa1(s_GameState *gs, s_Assets *assets);
void hitbox_init(s_GameState *gs);


#endif