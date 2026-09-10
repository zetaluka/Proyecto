#ifndef COMMONS_H
#define COMMONS_H


//==========Librerias=========//
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/display.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/events.h>
#include <allegro5/timer.h>
#include <allegro5/color.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

//==========Defines==========//
#define SCREEN_X 1280
#define SCREEN_Y 720
#define FPS 60
#define GRAVEDAD 1
#define MAXPANTALLAS 4
#define MAXHITBOX 50
#define TAM_CELDA 32
#define MAXFIL 30
#define MAXCOL 400
#define MAXENTIDADES 50
#define MAXELEMENTOS 50
#define LEVI_HB_RECORTE 42
#define LEVI_HB_OFFSET_Y 14
#define BORDE_CAM 250
#define BUFFER 400
#define ALTO_SUELO 66
#define LEVI_SS_ANCHO 120
#define LEVI_SS_ALTO 80
#define MAXGAS 100
#define BLANCO al_map_rgb(220, 220, 220)

//==========Estructuras==========//

//==Structs Assets==//

typedef struct 
{
    ALLEGRO_BITMAP* fondo_base;
    ALLEGRO_BITMAP* fondo_titan_colosal;
    ALLEGRO_BITMAP* fondo_bosque;
    ALLEGRO_BITMAP* fondo_menu;
    ALLEGRO_BITMAP* fondo_ol1;
    ALLEGRO_BITMAP* fondo_ol2;
    ALLEGRO_BITMAP* fondo_ol3;
    ALLEGRO_BITMAP* minMapas;
    ALLEGRO_BITMAP* marcoVida;
    ALLEGRO_BITMAP* galonGas;
    ALLEGRO_BITMAP* grieta;
    ALLEGRO_BITMAP* escudoLegion;
    ALLEGRO_BITMAP* grietaODM;
    ALLEGRO_BITMAP* gas;
    ALLEGRO_BITMAP* casa1;
    ALLEGRO_BITMAP* casa2;
    ALLEGRO_BITMAP* casa3; 
    ALLEGRO_BITMAP* puestoComida;
    ALLEGRO_BITMAP* dash;
    ALLEGRO_BITMAP* dashSB[14];
    ALLEGRO_BITMAP* transicion;
    ALLEGRO_BITMAP* transicion2;
    ALLEGRO_BITMAP* efectoSangre;
    ALLEGRO_BITMAP* imgGameOver;
    ALLEGRO_BITMAP* espada1;
    ALLEGRO_BITMAP* espada2;
    ALLEGRO_BITMAP* HUD;
    ALLEGRO_BITMAP* habilidadesHud;
    ALLEGRO_BITMAP* habilidades[6];
    ALLEGRO_BITMAP* cabezaTH;
    ALLEGRO_BITMAP* e_Icon;
    ALLEGRO_BITMAP* cuadroTexto;

} s_AssetsPantalla;


typedef struct{
    ALLEGRO_BITMAP* levi_SS;
    ALLEGRO_BITMAP* levi_SS_SC;

} s_LeviSprites;

typedef struct 
{
    ALLEGRO_BITMAP* titan1;
    ALLEGRO_BITMAP* titan2;
    ALLEGRO_BITMAP* titan_hembra;
    ALLEGRO_BITMAP* titanHembraFase1;
    ALLEGRO_BITMAP* titanHembraFase2;

} s_TitanesSprites;


typedef struct{

    ALLEGRO_FONT* shingekiFont30;
    ALLEGRO_FONT* shingekiFont20;
    ALLEGRO_FONT* biggestThingsFont25;
    ALLEGRO_FONT* biggestThingsFont18;
    ALLEGRO_FONT* biggestThingsFont17;
    ALLEGRO_FONT* biggestThingsFont10;
    ALLEGRO_FONT* biggestThingsFont12;

    s_AssetsPantalla assetsPantalla;
    s_LeviSprites levi;
    s_TitanesSprites titanes;

} s_Assets;

typedef enum
{
    IDLE,
    CAMINANDO,
    CORRIENDO,
    ATAQUE_BASICO,
    SALTANDO,
    CAYENDO,
    ATERRIZANDO,
    ODM,
    DASH,
    AGARRADO,
    SALIDA_DASH,
    SALIDA_ODM_ATAQUE1,
    SALIDA_ODM_ATAQUE2,
    ATAQUE_BASICO_CAYENDO,
    ODMATAQUE,
    ODM_ATAQUE1,
    ODM_ATAQUE2,
    PARRY,
    PARRY_EXITOSO,
    SALIDA_TITAN_AGARRE,
    ODM_ATAQUE_BASICO,
    ODM_NORMAL_ATAQUE_BASICO
} e_EstadoLevi;

typedef struct 
{
    int frameActual;
    int contadorAnim;
    int velocidadAnim;
    int cantidadFrames;
    int fila_ss;
    int frameXRepetir;
    bool rotarAnim;
    bool levi_background; //Para cuando quiera dibujar a levi atras del todo
    bool repetir;
    bool bloquearAnimacion;
    bool saltoActivo;
    
} s_AnimacionLevi;

typedef struct 
{
    int frameActual;
    int contadorAnim;
    int velocidadAnim;
    int cantidadFrames;
    int fila_ss;
    int frameXRepetir;
    bool rotarAnim;
    bool repetir;
} s_AnimacionTitanes;


typedef struct 
{
    int frameActual;
    int contadorAnim;
    int cantidadFrames;
    int velocidadAnim;
    float x;
    float y;
    bool activo;
} s_Animacion;

typedef struct 
{
    ALLEGRO_SAMPLE* sfx_odm;
    ALLEGRO_SAMPLE* sfx_attack;
    ALLEGRO_SAMPLE* sfx_attack2;
    ALLEGRO_SAMPLE* sfx_dash;
    ALLEGRO_SAMPLE* sfx_habilidad1;
    ALLEGRO_SAMPLE* sfx_habilidad2;
    ALLEGRO_SAMPLE* sfx_muerteTitan;
    ALLEGRO_SAMPLE* sfx_menu;
    ALLEGRO_SAMPLE* sfx_salto;
    ALLEGRO_AUDIO_STREAM* musica_menu;
    ALLEGRO_AUDIO_STREAM* musica_AOT;
    ALLEGRO_AUDIO_STREAM* musica_XLTT;
    ALLEGRO_AUDIO_STREAM* musica_SWS;
    float cdSfxAttack;

    ALLEGRO_AUDIO_STREAM* pistaActual;    
    ALLEGRO_AUDIO_STREAM* pistaSaliendo;  
    float gainObjetivo;                   
    float velocidadFade;                  
} s_Audio;


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
    int tipoCasa;
    bool activo;
    s_Hitbox hitbox;
    s_Hitbox hitbox2;
}s_Elementos;

typedef struct {
    bool activo;
    float dirX;
    float dirY;
    float distanciaRestante;
    s_Hitbox manoHB;
} s_AgarreTitan;

typedef enum{
    QUIETO,
    CAMINANDOTITAN,
    SALTO,
    SENTADO,
    ATERRIZAJE,
    MORDISCO,
    ATAQUE,
    AGARRANDO,
    INTENTO_AGARRE,
    LEVANTANDOSE,
    PATADA,
    SPAWN,
    MUERTE,

} e_EstadoTitan;

typedef struct 
{
    int x;
    int y;
    int vida;
    int ataque;
    int tipo; 
    int casoAtaque;
    float velocidadXSalto;
    float velocidadX;
    float velocidadY;
    float tiempoQuieto;
    float distanciaRecorrida;
    float distanciaRecorridaRegistrada;
    float cooldownAtaque;
    float cooldownMordida;
    float tiempoAtaqueActivo;
    float tiempoMordidaActivo;
    float gravedadTitan;
    float distanciaRecorridaAtaque;
    float tiempoGolpeRegistrado;
    s_Hitbox mordidaHB;
    s_Hitbox hitboxAtaqueBasico;
    s_Hitbox hitboxTitan;
    s_Hitbox hitboxNuca;
    s_AnimacionTitanes animacion;
    s_AgarreTitan agarre;
    e_EstadoTitan estadoTitan;
    bool agarreFase2Activa;
    bool agarreFase3Activa;
    bool ataqueDerecha;
    bool ataqueRegistrado;
    bool patadaActiva;
    bool ataqueActivo;
    bool saltoActivo;
    bool cambioDireccion;
    bool viendoDerecha;
    bool quieto;
    bool activo;
    bool enganchadoODM;
    bool frameActivacion;
    bool muriendo;
} s_Entidades;

typedef struct
{
    s_Hitbox hitbox[MAXHITBOX];
    s_Entidades entidades[MAXENTIDADES];
    s_Elementos elementos[MAXELEMENTOS];
    bool pantallaCargada;
    char fondo[30];
    int cantTitanes;
    int num_elementos;
    int num_entidades;
    int num_hitbox;
    int num_pantallas;
    int ancho;
    int alto;
    
} s_Pantalla;

typedef enum{
    IDLE_TH,
    CAMINANDO_TH,
    ATAQUE1_TH,
    ATAQUE2_TH,
    PATADA1_TH,
    PATADA2_TH,
    CUBRIRNUCA_TH,
    IDLE_CNTH,
    ATAQUE1_CNTH,
    PATADA1_CNTH,
    CRISTALIZACION_TH,

} e_EstadoTH;

typedef struct 
{
    int vida;
    int vidaMax;
    int casoMovimiento;
    int casoAtaque;
    float cdCasoMovimiento;
    float x;
    float y;
    float velocidadX;
    float velocidadY;
    float tiempoAtaqueActivo;
    float cooldownAtaque;
    bool retrocediendo;
    bool segundoGolpe;
    bool fase2Activa;
    bool atacando;
    bool activa;
    bool viendoDer;
    bool THQuieta;
    bool nucaCubierta;
    bool ataqueHecho;
    bool puntuacionDada;
    s_AnimacionTitanes animacion;
    s_Hitbox hitbox;
    s_Hitbox hitboxAtaque1;
    s_Hitbox hitboxAtaque2;
    e_EstadoTH estadoTH;
} s_TitanHembra;


typedef struct 
{
    float angulo;
    float x;
    float y;
    float dashX;
    float dashY;
    float cooldown;
    float distanciaRestante;
    float tiempoRecuperacionDash;
    int flagDash;
    int cantDash;
    bool frameActivacion;
    bool activo;
    bool animDashActiva;
    s_Hitbox hitboxDash;
    s_AnimacionLevi animDash;
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
    bool engancheTitan;
    bool engancheNormal;
    bool frameActivacion;
    bool engancheActivo;
    bool activo;
} s_ODM;

typedef struct
{
    int ataque;
    int ataqueMA;
    int ataqueNuca;
    int puntuacionTitan;
    int puntuacionNuca;
    int viendoDerecha;
    int contSoltarse;
    int puntuacion;
    int contModoAckerman;
    int aumentaMA;
    int vida;
    float tiempoInvulnerabilidad;
    float gravedad;
    float cooldownAtaque;
    float x;
    float y;
    float velocidadX;
    float velocidadY;
    float cooldownHabilidad1;
    float cooldownHabilidad2;
    float tiempoParryActivo;
    float cooldownParry;
    float distanciaYRecorrida;
    float gasRestante;
    float tiempoModoAckerman;
    float velocidadMA;
    bool vestuario;
    bool invulnerabilidad;
    bool habilitaAumentaDash;
    bool agarrado;
    bool distanciaYRegistrada;
    bool leviAtacando;
    bool habilidad1Activa;
    bool habilidad2Activa;
    bool doble_salto;
    bool levi_suelo;
    bool levi_vuelo;
    bool ataqueHecho;
    bool parryRecompensa;
    bool habilidad1Reiniciada;
    bool habilidad2Reiniciada;  
    s_Inventario inventario;
    s_Dash dash;
    s_ODM ODM;
    s_Hitbox hitbox;
    s_Hitbox hitboxAtaque;
    s_Hitbox hitboxODM;
    s_Hitbox parryHB;
    s_AnimacionLevi animacion;
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
    PANTALLA_GAME_OVER,
    PANTALLA_NIVEL_COMPLETADO
} e_EstadoPantalla;

typedef enum{
    MAIN,
    JUGAR,
    RANKING,
    OPCIONES,
    CONTROLES,
    OLEADA,
    RANKINGOLEADA,
    RANKINGNIVEL1,
    RANKINGVSTITANHEMBRA,
    SALIR
} e_EstadoMenu;

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
    bool keyE;
    bool keyR;
    bool keyC;
    bool keyX;
    bool ClickIzq;
    bool ClickDer;
    bool key1;
    bool key2;
    bool key3;
    bool keyEnter;
    bool keyEsc;
    float mouseX;
    float mouseY;
} s_InputState;

typedef struct 
{
    int carga_pantalla;
    int screenX;
    int screenY;
    int multiplicador;
    int flagPunt;
    float cooldownHitbox;
    float gravedad;
    float contPunt;
    char nombreTemp[40];
    s_Posiciones grietas[5];
    FILE *fdata;
    bool resetPunt;
    bool nombreIngresado;
    bool ingresandoNombre;
    bool agarradoPorTitan1;
    bool desactivarHitbox;
    bool cambioSentido;
    bool detenerEntidades;
    bool bloquearControles;
    s_Hitbox titan1;

} s_Variables;

typedef struct 
{
    char mapa1[MAXFIL][MAXCOL];
    int num_pantallas;

} s_Mapas;

typedef struct 
{
    char nombre[50];
    char dificultad[20];
    int puntuacion;
} s_Puntuacion;

typedef struct 
{
    s_Animacion gas[MAXGAS];
    s_Animacion transicion;
    s_Animacion transicion2;
    s_Animacion efectoSangre;
    s_Animacion ol1;
    int contGas;
    int contGasDS;
    bool cambioPantallaHecho;
    bool gasDS;
} s_Animaciones;

typedef struct 
{
    int contMenu;
    int contMapa;
    int contDif;
    int topePila;
    e_EstadoMenu pilaEstados[5];
    e_EstadoMenu estadoMenu;
    e_EstadoMenu estadoMenuAnterior;
    
} s_Menu;

typedef struct{
    int fase;
    bool teclaPulsada;
    bool requisitoCumplido;

} s_Tutorial;

typedef struct{
    int cdSpawn;
    float contCdSpawn;
} s_ModoOleada;

typedef enum{
    NORMAL,
    DIFICIL,
} e_Dificultad;

typedef struct 
{
    char nombre[20];
    char dificultad[10]; 
    int minutos;
    int segundos;
} s_PuntuacionTH;

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
    s_TitanHembra titanHembra;
    s_Puntuacion puntuaciones[10];
    s_Puntuacion puntuacionJugador;
    s_Menu menu;
    s_Menu menuPausa;
    s_Tutorial tutorial;
    s_Audio audio;
    s_ModoOleada oleada;
    s_PuntuacionTH puntuacionesTH[10];
    s_PuntuacionTH puntuacionJugadorTH;
    e_Dificultad dificultad;
    int contOpcionesGO;  
    int pantalla_actual;
    int nivel; 
    float escala;
    bool pausa;
    bool vsTitanHembraEjecutando;
    bool modoOleadaEjecutando;
    bool tutorialEjecutando;
    bool nivel1Ejecutando;
    bool pantallaCompleta;
    bool puntuacionGuardada;
    bool nivelCompletado;

    bool ejecutando; 
} s_GameState;


//==========Prototipos de funciones==========//
void game_init(s_GameState *gs, s_Assets *assets, ALLEGRO_DISPLAY *display);
void input_update(s_GameState *gs, ALLEGRO_EVENT* evento);
void update(s_GameState *gs, s_Assets *assets, ALLEGRO_DISPLAY *display, s_GameState *auxgs, ALLEGRO_EVENT *evento);
void render_gameview(s_GameState *gs, s_Assets *assets);
void render_ui(s_GameState *gs, s_Assets *assets);
void assets_load(s_Assets *assets, s_GameState *gs);
void genera_entidades(s_GameState *gs, s_Assets *assets);

//==========Prototipos de funciones no principales======//
void mapa(s_GameState *gs, s_Assets *assets);
void hitbox_init(s_GameState *gs);
void comprueba_colision(s_GameState *gs);
void cambiar_animacion(s_GameState *gs, e_EstadoLevi nuevaAnim);
void actualiza_res(s_GameState *gs, ALLEGRO_DISPLAY *display);
void guarda_opciones(s_GameState *gs);
void lee_opciones(s_GameState *gs, ALLEGRO_DISPLAY *display);
void ingresa_nombre(s_GameState *gs, ALLEGRO_EVENT* evento);
void parry(s_GameState *gs);
int carga_puntuacion(s_GameState *gs, const char *archivo);
int carga_puntuacionTH(s_GameState *gs, const char *archivo);
int carga_sfx(s_GameState *gs);
bool colision(s_GameState *gs, s_Hitbox h1, s_Hitbox h2);

#endif