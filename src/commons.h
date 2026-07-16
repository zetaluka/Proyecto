#ifndef COMMONS_H
#define COMMONS_H


//==========Librerias=========//
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
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
#define MAXFIL 30
#define MAXCOL 200
#define MAXENTIDADES 50
#define MAXELEMENTOS 50
#define LEVI_HB_RECORTE 37
#define BORDE_CAM 250
#define BUFFER 200
#define ALTO_SUELO 66
#define LEVI_SS_ANCHO 96
#define LEVI_SS_ALTO 68
#define MAXGAS 100
#define BLANCO al_map_rgb(220, 220, 220)


//==========Estructuras==========//

//==Structs Assets==//

typedef struct 
{
    ALLEGRO_BITMAP* fondo_base;
    ALLEGRO_BITMAP* fondo_titan_colosal;
    ALLEGRO_BITMAP* cubo;
    ALLEGRO_BITMAP* grieta;
    ALLEGRO_BITMAP* escudoLegion;
    ALLEGRO_BITMAP* casa1;
    ALLEGRO_BITMAP* grietaODM;
    ALLEGRO_BITMAP* gas;

} s_AssetsPantalla;


typedef struct{
    ALLEGRO_BITMAP* levi_SS;
    //ALLEGRO_BITMAP* levi;

} s_LeviSprites;

typedef struct 
{
    ALLEGRO_BITMAP* titan_bizarro;

} s_TitanesSprites;


typedef struct{

    ALLEGRO_FONT* shingekiFont30;
    ALLEGRO_FONT* minimalistTemplateFont50;    
    ALLEGRO_FONT* minimalistTemplateFont25;

    s_AssetsPantalla assetsPantalla;
    s_LeviSprites levi;
    s_TitanesSprites titanes;

} s_Assets;

typedef enum
{
    IDLE,
    CAMINANDO,
    ATACANDO
} e_EstadoLevi;

typedef struct 
{
    int frameActual;
    int contadorAnim;
    int velocidadAnim;
    int cantidadFrames;
    bool levi_background; //Para cuando quiera dibujar a levi atras del todo
    bool repetir;
    
} s_Animacion;

typedef struct 
{
    int frameActual;
    int contadorAnim;
    int cantidadFrames;
    int velocidadAnim;
    float x;
    float y;
    bool activo;
} s_EfectoGas;

//==============================//

typedef struct 
{
    float x;
    float y;
} s_Posiciones;


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
    int escudos;
    int gasODM;
} s_Inventario;

typedef struct 
{
    float x;
    float y;
    float ancho;
    float alto;
} s_Camara;


typedef struct 
{
    float x;
    float y;
    int tipo;
    bool activo;
    s_Hitbox hitbox;
}s_Elementos;

typedef struct 
{
    int x;
    int y;
    int velocidadX;
    int velocidadY;
    int vida;
    int ataque;
    int tipo; //agregar variable para saber si esta activo, nose en que struct
    s_Hitbox hitboxAtaque;
    s_Hitbox hitboxTitan;
    s_Hitbox hitboxDeteccion;
    s_Hitbox hitboxNuca;
    bool activo;
    bool enganchadoODM;
} s_Entidades;

typedef struct
{
    s_Hitbox hitbox[MAXHITBOX];
    s_Entidades entidades[MAXENTIDADES];
    s_Elementos elementos[MAXELEMENTOS];
    bool pantallaCargada;
    char fondo[30];
    int num_elementos;
    int num_entidades;
    int num_hitbox;
    int num_pantallas;
    int ancho;
    int alto;
    
} s_Pantalla;

typedef struct 
{
    float dashX;
    float dashY;
    float cooldown;
    float distanciaRestante;
    float tiempoRecuperacionDash;
    int flagDash;
    int cantDash;
    bool frameActivacion;
    bool activo;
    s_Hitbox hitboxDash;

} s_Dash;

typedef struct 
{
    float dirX;
    float dirY;
    float cooldown;
    float tiempoRecuperacionODM;
    float velocidadODM;
    float distanciaRestanteEnganche;
    float distanciaRestanteODM;
    float puntoEngancheX;
    float puntoEngancheY;
    float auxPuntoEngancheX;
    float auxPuntoEngancheY;
    float velocidadODMPrevia;
    bool frameActivacion;
    bool engancheActivo;
    bool activo;
} s_ODM;


typedef struct
{
    int viendoDerecha;
    int puntuacion;
    float gravedad;
    float cooldownAtaque;
    float x;
    float y;
    float velocidadX;
    float velocidadY;
    bool doble_salto;
    bool levi_suelo;
    bool levi_vuelo;
    s_Inventario inventario;
    s_Dash dash;
    s_ODM ODM;
    s_Hitbox hitbox;
    s_Hitbox hitboxAtaque;
    s_Hitbox hitboxODM;
    s_Animacion animacion;
    e_EstadoLevi estadoLevi;
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
} e_EstadoPantalla;

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
    bool keyF; //Dash de levi
    bool ClickIzq;
    bool ClickDer;
    float mouseX;
    float mouseY;
} s_InputState;

typedef struct 
{
    int carga_pantalla;
    int screenX;
    int screenY;
    s_Posiciones grietas[5];
    float cooldownHitbox;
    FILE *fdata;
    bool desactivarHitbox;
    bool cambioSentido;
    bool detenerEntidades;
    s_Hitbox titan1;

} s_Variables;

typedef struct 
{
    char mapa1[MAXFIL][MAXCOL];
    int num_pantallas;

} s_Mapas;

typedef struct 
{
    s_EfectoGas gas[MAXGAS];
    int contGas;
    int contGasDS;
    bool gasDS;
} s_Animaciones;


//====s_GameState====//
typedef struct {
    e_EstadoPantalla estadoPantalla;     
    s_Temporizador tiempoJugado;    
    s_InputState input;  
    s_Levi levi;    
    s_Pantalla pantalla[MAXPANTALLAS];
    s_Variables variables;
    s_Mapas mapas;
    s_Camara camara;
    s_Animaciones animaciones;
    int pantalla_actual;
    int nivel; 
    int lado_colision;
    float escala;

    bool ejecutando; 
} s_GameState;


//==========Prototipos de funciones==========//
void game_init(s_GameState *gs, s_Assets *assets, ALLEGRO_DISPLAY *display);
void input_update(s_GameState *gs, ALLEGRO_EVENT* evento);
void update(s_GameState *gs, s_Assets *assets);
void render_gameview(s_GameState *gs, s_Assets *assets);
void render_ui(s_GameState *gs, s_Assets *assets);
void assets_load(s_Assets *assets);
void genera_entidades(s_GameState *gs, s_Assets *assets);

//==========Prototipos de funciones no principales======//
void mapa1(s_GameState *gs, s_Assets *assets);
void hitbox_init(s_GameState *gs);
void comprueba_colision(s_GameState *gs);
void colision_levi_titan(s_GameState *gs);
bool colision(s_GameState *gs, s_Hitbox h1, s_Hitbox h2);


#endif