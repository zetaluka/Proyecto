#include "commons.h"

void hitbox_init(s_GameState *gs);
void entities_init(s_GameState *gs, s_Assets *assets);
void genera_titan1(s_GameState *gs, s_Assets *assets, int *j);
void genera_titan2(s_GameState *gs, s_Assets *assets, int *j);
void pos_levi(s_GameState *gs, s_Assets *assets, int *i, int *j);


//====Funcion principal====//
void game_init(s_GameState *gs, s_Assets *assets)
{
    gs->escala = 1.0f; //Variable que multiplica fondos, elementos, hitbox, etc. Para si en un futuro quiero cambiar de resolucion, redefino la variable y se escala todo.
    gs->ejecutando =1;
    gs->estadoPantalla = PANTALLA_JUGANDO;
    gs->pantalla_actual = 0;
    gs->nivel = 1;
    hitbox_init(gs);

    //Inicializacion de levi
    gs->levi.x = 700;
    gs->levi.y = 100;
    gs->levi.velocidadX = 0;
    gs->levi.velocidadY = 0;
    gs->levi.doble_salto = true;
    gs->levi.levi_suelo = false;
    gs->levi.viendoDerecha = 1;
    gs->levi.cooldownAtaque = 0;

    //Inicia fdata

    if ((gs->variables.fdata = fopen("mapa1.txt","r")) == NULL)
    {
        printf("Error al abrir el archivo");
        exit(1);
    }


    entities_init(gs, assets);
    mapa1(gs, assets);

    return;
}

void hitbox_init(s_GameState *gs)
{
    int pA = gs->pantalla_actual;

    //Orden de variables: x, y, ancho, alto, color

    switch(gs->pantalla_actual)
    {
        case 0:
            gs->pantalla[pA].hitbox[0] = (s_Hitbox){0 , (SCREEN_Y - 66), SCREEN_X, 66, BLANCO}; //Suelo
            gs->pantalla[pA].hitbox[1] = (s_Hitbox){(-4), 0, 4, SCREEN_Y, BLANCO}; //Limite izquierdo de la pantalla
            gs->pantalla[pA].hitbox[2] = (s_Hitbox){0, -2, SCREEN_X, 4, BLANCO }; //Limite superior de la pantalla
            gs->pantalla[pA].hitbox[3] = (s_Hitbox){50, 600, 32, 32, BLANCO}; //Cuadrado de prueba
            gs->pantalla[pA].num_hitbox = 4;
            break;

        case (MAXPANTALLAS - 1):
            gs->pantalla[pA].hitbox[0] = (s_Hitbox){0 , (SCREEN_Y - 66), SCREEN_X, 66, BLANCO}; //Suelo
            gs->pantalla[pA].hitbox[1] = (s_Hitbox){(-4), 0, 4, SCREEN_Y, BLANCO}; //Limite izquierdo de la pantalla
            gs->pantalla[pA].hitbox[2] = (s_Hitbox){0, -2, SCREEN_X, 4, BLANCO }; //Limite superior de la pantalla
            gs->pantalla[pA].hitbox[3] = (s_Hitbox){SCREEN_X, 0, 4, SCREEN_Y, BLANCO}; //Limite derecho de la pantalla
            gs->pantalla[pA].num_hitbox = 4;
            break;
        
        default:
            gs->pantalla[pA].hitbox[0] = (s_Hitbox){0 , (SCREEN_Y - 66), SCREEN_X, 66, BLANCO}; //Suelo
            gs->pantalla[pA].hitbox[1] = (s_Hitbox){(-4), 0, 4, SCREEN_Y, BLANCO}; //Limite izquierdo de la pantalla
            gs->pantalla[pA].hitbox[2] = (s_Hitbox){0, -2, SCREEN_X, 4, BLANCO }; //Limite superior de la pantalla
            gs->pantalla[pA].num_hitbox = 3;
            break;
    }

}

void entities_init(s_GameState *gs, s_Assets *assets)
{
    //Orden de las variables: x, y, velocidadX, velocidadY, vida, ataque, numEntidades, hitboxTitan, hitboxDeteccion y activo

    //Pantalla 0

    //gs->pantalla[0].entidades[0] = (s_Entidades){1100, SCREEN_Y - gs->pantalla[0].hitbox[0].alto - al_get_bitmap_height(assets->titanes.titan_bizarro), 2, 0, 500, 500, {0}, {0}, false};

    gs->pantalla[0].num_entidades = 0;

}

void mapa1(s_GameState *gs, s_Assets *assets)
{
    int i, j;
    char linea[MAXCOL + 5];

    for(i=0;i<MAXFIL;i++)
    {
        if(fgets(linea, sizeof(linea), gs->variables.fdata) == NULL)
                exit(1);

        if(linea[0] == '/' || linea[0] == '\0')
        {
            i--;
            continue;
        }

        for(j=0 ; j<MAXCOL && linea[j] != '\n' && linea[j] != '\0' && linea[j] != '\r' ; j++)
        {
            gs->mapas.mapa1[i][j] = linea[j]; 
            printf("%c", gs->mapas.mapa1[i][j]);

            switch(gs->mapas.mapa1[i][j])
            {
                case 'T':
                    genera_titan1(gs, assets, &j);
                    break;
                case 't':
                    genera_titan2(gs, assets, &j);
                    break;
                case 'L':
                    pos_levi(gs, assets, &i, &j);
                    break;

            }
        }

        printf("\n");
    }

}

void genera_titan1(s_GameState *gs, s_Assets *assets, int *j)
{
    int pA = gs->pantalla_actual;
    int nE = gs->pantalla[pA].num_entidades;

    gs->pantalla[pA].entidades[nE].x = *j*TAM_CELDA;
    gs->pantalla[pA].entidades[nE].y = SCREEN_Y - gs->pantalla[0].hitbox[0].alto - al_get_bitmap_height(assets->titanes.titan_bizarro);
    gs->pantalla[pA].entidades[nE].velocidadX = 2;
    gs->pantalla[pA].entidades[nE].velocidadY = 0;
    gs->pantalla[pA].entidades[nE].vida = 500;
    gs->pantalla[pA].entidades[nE].ataque = 500;
    gs->pantalla[pA].entidades[nE].activo = false;
    gs->pantalla[pA].num_entidades++;

}

void genera_titan2(s_GameState *gs, s_Assets *assets, int *j)
{
    int pA = gs->pantalla_actual;
    int nE = gs->pantalla[pA].num_entidades;

    gs->pantalla[pA].entidades[nE].x = *j*TAM_CELDA;
    gs->pantalla[pA].entidades[nE].y = SCREEN_Y - gs->pantalla[0].hitbox[0].alto - al_get_bitmap_height(assets->titanes.titan_bizarro);
    gs->pantalla[pA].entidades[nE].velocidadX = 3;
    gs->pantalla[pA].entidades[nE].velocidadY = 0;
    gs->pantalla[pA].entidades[nE].vida = 200;
    gs->pantalla[pA].entidades[nE].ataque = 300;
    gs->pantalla[pA].entidades[nE].activo = false;
    gs->pantalla[pA].num_entidades++;

}

void pos_levi(s_GameState *gs, s_Assets *assets, int *i, int *j)
{
    int pA = gs->pantalla_actual;

    gs->levi.x = *j*TAM_CELDA;
    gs->levi.y = *i*TAM_CELDA;

}