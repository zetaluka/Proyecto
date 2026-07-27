#include "commons.h"

void hitbox_init(s_GameState *gs);
void entities_init(s_GameState *gs, s_Assets *assets);
void genera_titan1(s_GameState *gs, s_Assets *assets, int j);
void genera_titan2(s_GameState *gs, s_Assets *assets, int j);
void pos_levi(s_GameState *gs, s_Assets *assets, int i, int j);
void grieta_ODM(s_GameState *gs, int i, int j);
void genera_escudo_legion(s_GameState *gs, int i, int j);
void actualiza_res(s_GameState *gs, ALLEGRO_DISPLAY *display);
void genera_casa1(s_GameState *gs, int i, int j);
void genera_casa2(s_GameState *gs, int i, int j);
void genera_casa3(s_GameState *gs, int i, int j);
void genera_puesto_comida(s_GameState *gs, int i, int j);

//====Funcion principal====//
void game_init(s_GameState *gs, s_Assets *assets, ALLEGRO_DISPLAY *display)
{
    //gs->escala = 1.0f; //Variable que multiplica fondos, elementos, hitbox, etc. Para si en un futuro quiero cambiar de resolucion, redefino la variable y se escala todo.
    actualiza_res(gs, display);
    gs->ejecutando =1;
    gs->estadoPantalla = PANTALLA_JUGANDO;
    gs->pantalla_actual = 0;
    gs->nivel = 1;
    gs->variables.gravedad = 0.8;
    gs->input.keyG = true;

    //Inicializacion de levi
    gs->levi.vida = 10;
    gs->levi.x = 700;
    gs->levi.y = 100;
    gs->levi.velocidadX = 0;
    gs->levi.velocidadY = 0;
    gs->levi.doble_salto = true;
    gs->levi.levi_suelo = false;
    gs->levi.viendoDerecha = 1;
    gs->levi.cooldownAtaque = 0;
    gs->levi.estadoLevi = IDLE;
    gs->levi.animacion.frameActual = 0;
    gs->levi.animacion.contadorAnim = 0;
    gs->levi.animacion.cantidadFrames = 6;
    gs->levi.animacion.velocidadAnim = 12;
    gs->levi.animacion.repetir = true;

    //Inicia fdata

    if ((gs->variables.fdata = fopen("mapa1.txt","r")) == NULL)
    {
        printf("Error al abrir el archivo");
        exit(1);
    }

    mapa1(gs, assets);

    return;
}

void actualiza_res(s_GameState *gs, ALLEGRO_DISPLAY *display)
{
    gs->variables.screenX = al_get_display_width(display);
    gs->variables.screenY = al_get_display_height(display);
    gs->escala = (float)gs->variables.screenX / SCREEN_X;

}

void hitbox_init(s_GameState *gs)
{
    int pA = gs->pantalla_actual;
    float anchoPantalla = gs->pantalla[pA].ancho * TAM_CELDA;

    //Orden de variables: x, y, ancho, alto, color
    switch(gs->pantalla_actual)
    {
        case 0:
            gs->pantalla[pA].hitbox[0] = (s_Hitbox){0 , (SCREEN_Y - 66), gs->pantalla[0].ancho*TAM_CELDA + 10, 66, BLANCO}; //Suelo
            gs->pantalla[pA].hitbox[1] = (s_Hitbox){(-4), (-200), 4, (SCREEN_Y + 200), BLANCO}; //Limite izquierdo de la pantalla
            gs->pantalla[pA].hitbox[2] = (s_Hitbox){0, (-200), gs->pantalla[0].ancho*TAM_CELDA, 12, BLANCO }; //Limite superior de la pantalla
            gs->pantalla[pA].num_hitbox = 3;
            break;

        case (MAXPANTALLAS - 1):
            gs->pantalla[pA].hitbox[0] = (s_Hitbox){0 , (SCREEN_Y - 66), gs->pantalla[MAXPANTALLAS-1].ancho*TAM_CELDA, 66, BLANCO}; //Suelo
            gs->pantalla[pA].hitbox[1] = (s_Hitbox){(-4), (-200), 4, (SCREEN_Y + 200), BLANCO}; //Limite izquierdo de la pantalla
            gs->pantalla[pA].hitbox[2] = (s_Hitbox){0, (-200), gs->pantalla[MAXPANTALLAS-1].ancho*TAM_CELDA, 16, BLANCO }; //Limite superior de la pantalla
            gs->pantalla[pA].hitbox[3] = (s_Hitbox){gs->pantalla[MAXPANTALLAS-1].ancho*TAM_CELDA, (-100), 32, (SCREEN_Y + 100), BLANCO}; //Limite derecho de la pantalla
            gs->pantalla[pA].num_hitbox = 4;
            break;
        
        default:
            gs->pantalla[pA].hitbox[0] = (s_Hitbox){0 , (SCREEN_Y - 66), gs->pantalla[pA].ancho*TAM_CELDA + 10, 66, BLANCO}; //Suelo
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

        if(strcmp(linea,"default\n") == 0)
        {
            strcpy(gs->pantalla[pA].fondo, "fondo_base");
            gs->pantalla[pA].alto = 22;
            gs->pantalla[pA].ancho = 40;
            break;
        }

        if(linea[0] == '/' || linea[0] == '\0' || linea[0] == '\n' || linea[0] == '.')
        {
            i--;
            continue;
        }

        if(i == 0)
        {
            sscanf(linea, "%s", gs->pantalla[pA].fondo);
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

    hitbox_init(gs);

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
                case 'c':
                    genera_casa1(gs, i, j);
                    break;
                case 'C':
                    genera_casa2(gs, i, j);
                    break; 
                case '3':
                    genera_casa3(gs, i, j);
                    break;

                case 'p':
                    genera_puesto_comida(gs, i, j);
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
    gs->pantalla[pA].entidades[nE].tipo = 2;

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

void genera_casa1(s_GameState *gs, int i, int j)
{
    int pA = gs->pantalla_actual, cont;
    int nE = gs->pantalla[pA].num_elementos;

    if(nE >= MAXELEMENTOS)
        return;

    gs->pantalla[pA].elementos[nE].x = j*TAM_CELDA;
    gs->pantalla[pA].elementos[nE].y = i*TAM_CELDA;
    gs->pantalla[pA].elementos[nE].hitbox.x = gs->pantalla[pA].elementos[nE].x + 10;
    gs->pantalla[pA].elementos[nE].hitbox.y = gs->pantalla[pA].elementos[nE].y + 61;
    gs->pantalla[pA].elementos[nE].hitbox.alto = 246;
    gs->pantalla[pA].elementos[nE].hitbox.ancho = 285;
    gs->pantalla[pA].elementos[nE].hitbox.color = BLANCO;
    gs->pantalla[pA].elementos[nE].tipo = 3;
    gs->pantalla[pA].elementos[nE].tipoCasa = 1;
    gs->pantalla[pA].elementos[nE].activo = true;
    gs->pantalla[pA].num_elementos++;

    while(colision(gs, gs->pantalla[pA].elementos[nE].hitbox, gs->pantalla[pA].hitbox[0]) == false)
        {
            gs->pantalla[pA].elementos[nE].y++;
            gs->pantalla[pA].elementos[nE].hitbox.y++;
        }

}

void genera_casa2(s_GameState *gs, int i, int j)
{
    int pA = gs->pantalla_actual, cont;
    int nE = gs->pantalla[pA].num_elementos;

    if(nE >= MAXELEMENTOS)
        return;

    gs->pantalla[pA].elementos[nE].x = j*TAM_CELDA;
    gs->pantalla[pA].elementos[nE].y = i*TAM_CELDA;
    gs->pantalla[pA].elementos[nE].hitbox.x = gs->pantalla[pA].elementos[nE].x + 50;
    gs->pantalla[pA].elementos[nE].hitbox.y = gs->pantalla[pA].elementos[nE].y + 64;
    gs->pantalla[pA].elementos[nE].hitbox.alto = 365;
    gs->pantalla[pA].elementos[nE].hitbox.ancho = 635;
    gs->pantalla[pA].elementos[nE].hitbox.color = BLANCO;
    gs->pantalla[pA].elementos[nE].tipo = 3;
    gs->pantalla[pA].elementos[nE].tipoCasa = 2;
    gs->pantalla[pA].elementos[nE].activo = true;
    gs->pantalla[pA].num_elementos++;
    
    while(colision(gs, gs->pantalla[pA].elementos[nE].hitbox, gs->pantalla[pA].hitbox[0]) == false)
    {
        gs->pantalla[pA].elementos[nE].y++;
        gs->pantalla[pA].elementos[nE].hitbox.y++;
    }

    gs->pantalla[pA].elementos[nE].hitbox2.x = gs->pantalla[pA].elementos[nE].hitbox.x + 112;
    gs->pantalla[pA].elementos[nE].hitbox2.y = gs->pantalla[pA].elementos[nE].hitbox.y + 297;
    gs->pantalla[pA].elementos[nE].hitbox2.alto = 67;
    gs->pantalla[pA].elementos[nE].hitbox2.ancho = 40;
    gs->pantalla[pA].elementos[nE].hitbox2.color = al_map_rgb(147, 112, 219);


}

void genera_casa3(s_GameState *gs, int i, int j)
{
    int pA = gs->pantalla_actual, cont;
    int nE = gs->pantalla[pA].num_elementos;

    if(nE >= MAXELEMENTOS)
        return;

    gs->pantalla[pA].elementos[nE].x = j*TAM_CELDA;
    gs->pantalla[pA].elementos[nE].y = i*TAM_CELDA;
    gs->pantalla[pA].elementos[nE].hitbox.x = gs->pantalla[pA].elementos[nE].x + 100;
    gs->pantalla[pA].elementos[nE].hitbox.y = gs->pantalla[pA].elementos[nE].y + 58;
    gs->pantalla[pA].elementos[nE].hitbox.alto = 318;
    gs->pantalla[pA].elementos[nE].hitbox.ancho = 420;
    gs->pantalla[pA].elementos[nE].hitbox.color = BLANCO;
    gs->pantalla[pA].elementos[nE].tipo = 3;
    gs->pantalla[pA].elementos[nE].tipoCasa = 3;
    gs->pantalla[pA].elementos[nE].activo = true;
    gs->pantalla[pA].num_elementos++;

    while(colision(gs, gs->pantalla[pA].elementos[nE].hitbox, gs->pantalla[pA].hitbox[0]) == false)
        {
            gs->pantalla[pA].elementos[nE].y++;
            gs->pantalla[pA].elementos[nE].hitbox.y++;
        }

}

void genera_puesto_comida(s_GameState *gs, int i, int j)
{
    int pA = gs->pantalla_actual, cont;
    int nE = gs->pantalla[pA].num_elementos;

    if(nE >= MAXELEMENTOS)
        return;

    gs->pantalla[pA].elementos[nE].x = j*TAM_CELDA;
    gs->pantalla[pA].elementos[nE].y = i*TAM_CELDA;
    gs->pantalla[pA].elementos[nE].hitbox.x = gs->pantalla[pA].elementos[nE].x + 30;
    gs->pantalla[pA].elementos[nE].hitbox.y = gs->pantalla[pA].elementos[nE].y + 35;
    gs->pantalla[pA].elementos[nE].hitbox.alto = 150;
    gs->pantalla[pA].elementos[nE].hitbox.ancho = 67;
    gs->pantalla[pA].elementos[nE].hitbox.color = BLANCO;
    gs->pantalla[pA].elementos[nE].tipo = 3;
    gs->pantalla[pA].elementos[nE].tipoCasa = 4;
    gs->pantalla[pA].elementos[nE].activo = true;
    gs->pantalla[pA].num_elementos++;

    while(colision(gs, gs->pantalla[pA].elementos[nE].hitbox, gs->pantalla[pA].hitbox[0]) == false)
        {
            gs->pantalla[pA].elementos[nE].y++;
            gs->pantalla[pA].elementos[nE].hitbox.y++;
        }

}
