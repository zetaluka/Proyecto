#include "commons.h"

void hitbox_init(s_GameState *gs);
void entities_init(s_GameState *gs, s_Assets *assets);
void genera_titan1(s_GameState *gs, s_Assets *assets, int j);
void genera_titan2(s_GameState *gs, s_Assets *assets, int j);
void pos_levi(s_GameState *gs, s_Assets *assets, int i, int j);
void grieta_ODM(s_GameState *gs, int i, int j);
void genera_escudo_legion(s_GameState *gs, int i, int j);

//====Funcion principal====//
void game_init(s_GameState *gs, s_Assets *assets)
{
    gs->escala = 1.0f; //Variable que multiplica fondos, elementos, hitbox, etc. Para si en un futuro quiero cambiar de resolucion, redefino la variable y se escala todo.
    gs->ejecutando =1;
    gs->estadoPantalla = PANTALLA_JUGANDO;
    gs->pantalla_actual = 0;
    gs->nivel = 1;

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

    mapa1(gs, assets);
    hitbox_init(gs);

    return;
}

void hitbox_init(s_GameState *gs)
{
    int pA = gs->pantalla_actual;
    float anchoPantalla = gs->pantalla[pA].ancho * TAM_CELDA;

    //Orden de variables: x, y, ancho, alto, color
    switch(gs->pantalla_actual)
    {
        case 0:
            gs->pantalla[pA].hitbox[0] = (s_Hitbox){0 , (SCREEN_Y - 66), gs->pantalla[0].ancho*TAM_CELDA, 66, BLANCO}; //Suelo
            gs->pantalla[pA].hitbox[1] = (s_Hitbox){(-4), (-200), 4, (SCREEN_Y + 200), BLANCO}; //Limite izquierdo de la pantalla
            gs->pantalla[pA].hitbox[2] = (s_Hitbox){0, (-200), gs->pantalla[0].ancho*TAM_CELDA, 12, BLANCO }; //Limite superior de la pantalla
            gs->pantalla[pA].hitbox[3] = (s_Hitbox){50, 600, 32, 32, BLANCO}; //Cuadrado de prueba
            gs->pantalla[pA].num_hitbox = 4;
            break;

        case (MAXPANTALLAS - 1):
            gs->pantalla[pA].hitbox[0] = (s_Hitbox){0 , (SCREEN_Y - 66), gs->pantalla[MAXPANTALLAS-1].ancho*TAM_CELDA, 66, BLANCO}; //Suelo
            gs->pantalla[pA].hitbox[1] = (s_Hitbox){(-4), (-200), 4, (SCREEN_Y + 200), BLANCO}; //Limite izquierdo de la pantalla
            gs->pantalla[pA].hitbox[2] = (s_Hitbox){0, (-200), gs->pantalla[MAXPANTALLAS-1].ancho*TAM_CELDA, 16, BLANCO }; //Limite superior de la pantalla
            gs->pantalla[pA].hitbox[3] = (s_Hitbox){gs->pantalla[MAXPANTALLAS-1].ancho*TAM_CELDA, (-100), 32, (SCREEN_Y + 100), BLANCO}; //Limite derecho de la pantalla
            gs->pantalla[pA].num_hitbox = 4;
            break;
        
        default:
            gs->pantalla[pA].hitbox[0] = (s_Hitbox){0 , (SCREEN_Y - 66), gs->pantalla[pA].ancho*TAM_CELDA, 66, BLANCO}; //Suelo
            gs->pantalla[pA].hitbox[1] = (s_Hitbox){(-4), (-200), 4, (SCREEN_Y + 200), BLANCO}; //Limite izquierdo de la pantalla
            gs->pantalla[pA].hitbox[2] = (s_Hitbox){0, (-200), gs->pantalla[pA].ancho*TAM_CELDA, 12, BLANCO }; //Limite superior de la pantalla
            gs->pantalla[pA].num_hitbox = 3;
            break;
    }

}

void mapa1(s_GameState *gs, s_Assets *assets)
{
    int i = 0, j, pA = gs->pantalla_actual, fil, col;
    char linea[BUFFER];
    
    for(i=0;i<3;i++)
    {
        if(fgets(linea, sizeof(linea), gs->variables.fdata) == NULL)
            exit(1);

        if(linea[0] == '/' || linea[0] == '\0' || linea[0] == '\n')
        {
            break;
        }

        if(i == 0)
        {
            strcpy(gs->pantalla[pA].fondo, linea);
            printf("%s",gs->pantalla[pA].fondo);
        }
        
        else if(i == 1)
        {
            sscanf(linea, "%d", &gs->pantalla[pA].alto);
            //printf("%d\n", gs->pantalla[pA].alto);
        }

        else if(i == 2)
        {
            sscanf(linea, "%d", &gs->pantalla[pA].ancho);
            //printf("%d\n",gs->pantalla[pA].ancho);
        }

    }

    if(gs->pantalla[pA].alto > 0 && gs->pantalla[pA].alto <= MAXFIL && gs->pantalla[pA].ancho > 0 && gs->pantalla[pA].ancho <= MAXCOL)
    {
        fil = gs->pantalla[pA].alto;
        col = gs->pantalla[pA].ancho;
        printf("%d %d\n", fil, col);
    }
    else
    {
        fil = 22;
        col = 40;
        gs->pantalla[pA].alto = fil;
        gs->pantalla[pA].ancho = col;
        printf("%d %d\n", fil, col);
    }

    char mapa[fil][col];

    for(i=0;i<fil;i++)
    {
        if(fgets(linea, sizeof(linea), gs->variables.fdata) == NULL)
            exit(1);

        if(linea[0] == '/' || linea[0] == '\0' || linea[0] == '\n')
        {
            i--;
            continue;
        }

        for(j=0 ; j<col && linea[j] != '\n' && linea[j] != '\0' ; j++)
        {
            mapa[i][j] = linea[j]; 
            printf("%c", mapa[i][j]);

            switch(mapa[i][j]) //Confirmar que hay espacio en el arreglo
            {
                case 'T':
                    genera_titan1(gs, assets, j);
                    break;
                case 't':
                    genera_titan2(gs, assets, j);
                    break;
                case 'L':
                    pos_levi(gs, assets, i, j);
                    break;
                case 'g':
                    grieta_ODM(gs, i, j);
                    break;
                case 'e':
                    genera_escudo_legion(gs, i, j);
                    break;

            }
        }
        printf("\n");
    }

}

void genera_titan1(s_GameState *gs, s_Assets *assets, int j)
{
    int pA = gs->pantalla_actual;
    int nE = gs->pantalla[pA].num_entidades;

    if(nE >= MAXENTIDADES)
        return;

    gs->pantalla[pA].entidades[nE].x = j*TAM_CELDA;
    gs->pantalla[pA].entidades[nE].y = SCREEN_Y - ALTO_SUELO - al_get_bitmap_height(assets->titanes.titan_bizarro);
    gs->pantalla[pA].entidades[nE].velocidadX = 2;
    gs->pantalla[pA].entidades[nE].velocidadY = 0;
    gs->pantalla[pA].entidades[nE].vida = 700;
    gs->pantalla[pA].entidades[nE].ataque = 500;
    gs->pantalla[pA].entidades[nE].activo = false;
    gs->pantalla[pA].num_entidades++;
    gs->pantalla[pA].entidades[nE].tipo = 1;

}

void genera_titan2(s_GameState *gs, s_Assets *assets, int j)
{
    int pA = gs->pantalla_actual;
    int nE = gs->pantalla[pA].num_entidades;

    if(nE >= MAXENTIDADES)
        return;

    gs->pantalla[pA].entidades[nE].x = j*TAM_CELDA;
    gs->pantalla[pA].entidades[nE].y = SCREEN_Y - ALTO_SUELO - al_get_bitmap_height(assets->titanes.titan_bizarro);
    gs->pantalla[pA].entidades[nE].velocidadX = 3;
    gs->pantalla[pA].entidades[nE].velocidadY = 0;
    gs->pantalla[pA].entidades[nE].vida = 300;
    gs->pantalla[pA].entidades[nE].ataque = 300;
    gs->pantalla[pA].entidades[nE].activo = false;
    gs->pantalla[pA].num_entidades++;
    gs->pantalla[pA].entidades[nE].tipo = 1;

}

void pos_levi(s_GameState *gs, s_Assets *assets, int i, int j)
{
    int pA = gs->pantalla_actual;

    gs->levi.x = j*TAM_CELDA;
    gs->levi.y = i*TAM_CELDA;

}

void grieta_ODM(s_GameState *gs, int i, int j)
{
    int pA = gs->pantalla_actual, cont;
    int nE = gs->pantalla[pA].num_elementos;

    if(nE >= MAXELEMENTOS)
        return;

    gs->pantalla[pA].elementos[nE].x = j*TAM_CELDA;
    gs->pantalla[pA].elementos[nE].y = i*TAM_CELDA;
    gs->pantalla[pA].elementos[nE].hitbox.x = gs->pantalla[pA].elementos[nE].x + 7;
    gs->pantalla[pA].elementos[nE].hitbox.y = gs->pantalla[pA].elementos[nE].y + 5;
    gs->pantalla[pA].elementos[nE].hitbox.alto = TAM_CELDA;
    gs->pantalla[pA].elementos[nE].hitbox.ancho = TAM_CELDA;
    gs->pantalla[pA].elementos[nE].hitbox.color = al_map_rgb(0,0,255);
    gs->pantalla[pA].elementos[nE].tipo = 1;
    gs->pantalla[pA].elementos[nE].activo = true;
    gs->pantalla[pA].num_elementos++;
}

void genera_escudo_legion(s_GameState *gs, int i, int j)
{
    int pA = gs->pantalla_actual, cont;
    int nE = gs->pantalla[pA].num_elementos;

    if(nE >= MAXELEMENTOS)
        return;

    gs->pantalla[pA].elementos[nE].x = j*TAM_CELDA;
    gs->pantalla[pA].elementos[nE].y = i*TAM_CELDA;
    gs->pantalla[pA].elementos[nE].hitbox.x = gs->pantalla[pA].elementos[nE].x + 2;
    gs->pantalla[pA].elementos[nE].hitbox.y = gs->pantalla[pA].elementos[nE].y + 2;
    gs->pantalla[pA].elementos[nE].hitbox.alto = TAM_CELDA;
    gs->pantalla[pA].elementos[nE].hitbox.ancho = TAM_CELDA;
    gs->pantalla[pA].elementos[nE].hitbox.color = al_map_rgb(255, 255, 0);
    gs->pantalla[pA].elementos[nE].tipo = 2;
    gs->pantalla[pA].elementos[nE].activo = true;
    gs->pantalla[pA].num_elementos++;
    
}
