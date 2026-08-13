#include "commons.h"

void hitbox_init(s_GameState *gs);
void entities_init(s_GameState *gs, s_Assets *assets);
void genera_titan1(s_GameState *gs, int i, int j);
void genera_titan2(s_GameState *gs, int i, int j);
void pos_levi(s_GameState *gs, s_Assets *assets, int i, int j);
void grieta_ODM(s_GameState *gs, int i, int j);
void genera_escudo_legion(s_GameState *gs, int i, int j);
void genera_casa1(s_GameState *gs, int i, int j);
void genera_casa2(s_GameState *gs, int i, int j);
void genera_casa3(s_GameState *gs, int i, int j);
void genera_puesto_comida(s_GameState *gs, int i, int j);
void genera_titan_hembra(s_GameState *gs, int i, int j);
void genera_gas_elemento(s_GameState *gs, int i, int j); 
void genera_hitbox_arbol(s_GameState *gs, int i, int j);

//====Funcion principal====//
void game_init(s_GameState *gs, s_Assets *assets, ALLEGRO_DISPLAY *display)
{
    //gs->escala = 1.0f; //Variable que multiplica fondos, elementos, hitbox, etc. Para si en un futuro quiero cambiar de resolucion, redefino la variable y se escala todo.
    if(gs->nivel1Ejecutando || gs->tutorialEjecutando)
        gs->estadoPantalla = PANTALLA_JUGANDO;
    else
        gs->estadoPantalla = PANTALLA_MENU;

    gs->ejecutando = 1;
    gs->pantalla_actual = 0;
    gs->nivel = 1;
    gs->nivelCompletado = false;
    gs->variables.gravedad = 0.8;
    gs->menu.estadoMenu = MAIN;

    //Inicializacion de levi
    gs->levi.vida = 50;
    gs->levi.gasRestante = 1000;
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

    lee_opciones(gs, display);
    actualiza_res(gs, display);

    //Inicia fdata
    if(gs->nivel1Ejecutando)
    {
        if((gs->variables.fdata = fopen("mapa1.txt","r")) == NULL)
        {
            printf("Error al abrir el archivo");
            exit(1);
        }
    }

    else if(gs->tutorialEjecutando)
    {
        if((gs->variables.fdata = fopen("tutorial.txt","r")) == NULL)
        {
            printf("Error al abrir el archivo");
            exit(1);
        }
    }
    
    if(gs->nivel1Ejecutando || gs->tutorialEjecutando)
        mapa(gs, assets);

    return;
}

void guarda_opciones(s_GameState *gs)
{
    FILE *fdata;

    if((fdata = fopen("opciones.txt","w")) == NULL)
    {
        printf("Error al abrir el archivo");
        exit(1);
    }

    if(gs->pantallaCompleta)
        fprintf(fdata, "true\n");
    else
        fprintf(fdata, "false\n");

    if(gs->levi.vestuario)
        fprintf(fdata, "true\n");
    else
        fprintf(fdata, "false\n");  

    fclose(fdata);    

}

void lee_opciones(s_GameState *gs , ALLEGRO_DISPLAY *display)
{
    FILE* fdata;
    char linea[10];

    if((fdata = fopen("opciones.txt","r")) == NULL)
    {
        gs->pantallaCompleta = false;
        gs->levi.vestuario = false;
        al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, gs->pantallaCompleta);
        guarda_opciones(gs);
        return;
    }

    if(fgets(linea, sizeof(linea), fdata) == NULL)
        exit(1);

    if(strcmp(linea, "true\n") == 0)
    {
        gs->pantallaCompleta = true;
        al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, gs->pantallaCompleta);
    }
    else if(strcmp(linea, "false\n") == 0)
    {
        gs->pantallaCompleta = false;
        al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, gs->pantallaCompleta);
    }

    if(fgets(linea, sizeof(linea), fdata) == NULL)
        exit(1);
    
        if(strcmp(linea, "true\n") == 0)
    {
        gs->levi.vestuario = true;
    }
    else if(strcmp(linea, "false\n") == 0)
    {
        gs->levi.vestuario = false;
    }

    fclose(fdata);
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

    //Orden de variables: x, y, ancho, alto, color
    switch(gs->pantalla_actual)
    {
        case 0:
            gs->pantalla[pA].hitbox[0] = (s_Hitbox){0 , (SCREEN_Y - 66), gs->pantalla[0].ancho*TAM_CELDA + 10, 66, BLANCO}; //Suelo
            gs->pantalla[pA].hitbox[1] = (s_Hitbox){(-4), (-200), 4, (SCREEN_Y + 200), BLANCO}; //Limite izquierdo de la pantalla
            gs->pantalla[pA].hitbox[2] = (s_Hitbox){0, (-200), gs->pantalla[0].ancho*TAM_CELDA, 12, BLANCO }; //Limite superior de la pantalla
            gs->pantalla[pA].hitbox[3] = (s_Hitbox){gs->pantalla[0].ancho*TAM_CELDA, (-100), 32, (SCREEN_Y + 100), BLANCO}; //Limite derecho de la pantalla

            gs->pantalla[pA].elementos[0].activo = true;
            gs->pantalla[pA].elementos[0].tipo = 4;
            gs->pantalla[pA].elementos[0].hitbox = (s_Hitbox){10030, 570, 100, 80, BLANCO};

            gs->pantalla[pA].num_elementos = 1;
            gs->pantalla[pA].num_hitbox = 4;
            break;

        case 1:

            gs->pantalla[pA].hitbox[0] = (s_Hitbox){0 , (SCREEN_Y - 66), gs->pantalla[0].ancho*TAM_CELDA + 10, 66, BLANCO}; //Suelo
            gs->pantalla[pA].hitbox[1] = (s_Hitbox){(-4), (-200), 4, (SCREEN_Y + 200), BLANCO}; //Limite izquierdo de la pantalla
            gs->pantalla[pA].hitbox[2] = (s_Hitbox){0, (-200), gs->pantalla[0].ancho*TAM_CELDA, 12, BLANCO }; //Limite superior de la pantalla
            gs->pantalla[pA].hitbox[3] = (s_Hitbox){gs->pantalla[1].ancho*TAM_CELDA, (-100), 32, (SCREEN_Y + 100), BLANCO}; //Limite derecho de la pantalla

            gs->pantalla[pA].elementos[0].activo = true;
            gs->pantalla[pA].elementos[0].tipo = 4;
            gs->pantalla[pA].elementos[0].hitbox = (s_Hitbox){9870, 500, 200, 150, BLANCO};

            gs->pantalla[pA].num_elementos = 1;
            gs->pantalla[pA].num_hitbox = 4;
            break;

        case (MAXPANTALLAS - 1):
            gs->pantalla[pA].hitbox[0] = (s_Hitbox){0 , (SCREEN_Y - 66), gs->pantalla[MAXPANTALLAS-1].ancho*TAM_CELDA + 30, 66, BLANCO}; //Suelo
            gs->pantalla[pA].hitbox[1] = (s_Hitbox){(-4), (-200), 4, (SCREEN_Y + 200), BLANCO}; //Limite izquierdo de la pantalla
            gs->pantalla[pA].hitbox[2] = (s_Hitbox){0, (-200), gs->pantalla[MAXPANTALLAS-1].ancho*TAM_CELDA, 16, BLANCO }; //Limite superior de la pantalla
            gs->pantalla[pA].hitbox[3] = (s_Hitbox){gs->pantalla[MAXPANTALLAS-1].ancho*TAM_CELDA, (-100), 32, (SCREEN_Y + 100), BLANCO}; //Limite derecho de la pantalla
            gs->pantalla[pA].num_hitbox = 4;
            break;
        
        default:
            gs->pantalla[pA].hitbox[0] = (s_Hitbox){0 , (SCREEN_Y - 66), gs->pantalla[pA].ancho*TAM_CELDA + 30, 66, BLANCO}; //Suelo
            gs->pantalla[pA].hitbox[1] = (s_Hitbox){(-4), (-200), 4, (SCREEN_Y + 200), BLANCO}; //Limite izquierdo de la pantalla
            gs->pantalla[pA].hitbox[2] = (s_Hitbox){0, (-200), gs->pantalla[pA].ancho*TAM_CELDA, 12, BLANCO }; //Limite superior de la pantalla
            gs->pantalla[pA].num_hitbox = 3;
            break;
    }

}

void mapa(s_GameState *gs, s_Assets *assets)
{
    //Funcion que abre un archivo y lee el mapa, ademas lee el fondo a ocupar en la pantalla y la cantidad de filas y columnas necesarias para crear un arreglo que pueda contener
    //el tamano necesario para crear mapas de distintos tamanos

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
            printf("%s\n",gs->pantalla[pA].fondo);
        }
        
        else if(i == 1)
        {
            sscanf(linea, "%d", &gs->pantalla[pA].alto);
        }

        else if(i == 2)
        {
            sscanf(linea, "%d", &gs->pantalla[pA].ancho);
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
                    genera_titan1(gs, i, j);
                    break;
                case 't':
                    genera_titan2(gs, i, j);
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
                case 'H':
                    genera_titan_hembra(gs, i, j);
                    break;
                case 'o':
                    genera_gas_elemento(gs, i, j);
                    break;
                case 'A':
                    genera_hitbox_arbol(gs, i, j);
                    break;

            }
        }
        printf("\n");
    }

}

void genera_titan1(s_GameState *gs, int i, int j)
{
    int pA = gs->pantalla_actual;
    int nE = gs->pantalla[pA].num_entidades;

    if(nE >= MAXENTIDADES)
        return;

    gs->pantalla[pA].entidades[nE].x = j*TAM_CELDA;
    gs->pantalla[pA].entidades[nE].y = i*TAM_CELDA;
    gs->pantalla[pA].entidades[nE].velocidadX = 2;
    gs->pantalla[pA].entidades[nE].velocidadY = 0;
    gs->pantalla[pA].entidades[nE].vida = 700;
    gs->pantalla[pA].entidades[nE].ataque = 500;
    gs->pantalla[pA].entidades[nE].activo = true;
    gs->pantalla[pA].entidades[nE].viendoDerecha = rand()%2;
    if(gs->pantalla[pA].entidades[nE].viendoDerecha == false)
        gs->pantalla[pA].entidades[nE].animacion.rotarAnim = true;
    else
        gs->pantalla[pA].entidades[nE].animacion.rotarAnim = false;
    gs->pantalla[pA].num_entidades++;
    gs->pantalla[pA].entidades[nE].tipo = 1;
    gs->pantalla[pA].entidades[nE].estadoTitan = SPAWN;

    if(gs->tutorialEjecutando)
        gs->pantalla[pA].entidades[nE].animacion.rotarAnim = true;


}

void genera_titan2(s_GameState *gs, int i, int j)
{
    int pA = gs->pantalla_actual;
    int nE = gs->pantalla[pA].num_entidades;

    if(nE >= MAXENTIDADES)
        return;

    gs->pantalla[pA].entidades[nE].x = j*TAM_CELDA;
    gs->pantalla[pA].entidades[nE].y = i*TAM_CELDA;
    gs->pantalla[pA].entidades[nE].velocidadX = 3;
    gs->pantalla[pA].entidades[nE].velocidadY = 0;
    gs->pantalla[pA].entidades[nE].vida = 300;
    gs->pantalla[pA].entidades[nE].ataque = 300;
    gs->pantalla[pA].entidades[nE].activo = true;
    gs->pantalla[pA].num_entidades++;
    gs->pantalla[pA].entidades[nE].tipo = 2;
    gs->pantalla[pA].entidades[nE].estadoTitan = SPAWN;

    if(gs->tutorialEjecutando)
    {
        gs->pantalla[pA].entidades[nE].vida = 5;
        gs->pantalla[pA].entidades[nE].animacion.fila_ss = 7;
        gs->pantalla[pA].entidades[nE].animacion.frameActual = 2; 
        gs->pantalla[pA].entidades[nE].animacion.rotarAnim = true;
    }

}

void genera_titan_hembra(s_GameState *gs, int i, int j)
{
    gs->titanHembra.x = j*TAM_CELDA;
    gs->titanHembra.y = i*TAM_CELDA;
    gs->titanHembra.vida = 50000;
    gs->titanHembra.velocidadX = 5;
    gs->titanHembra.activa = true;
    gs->titanHembra.estadoTH = IDLE;
    gs->titanHembra.animacion.cantidadFrames = 4;
    gs->titanHembra.animacion.contadorAnim = 0;
    gs->titanHembra.animacion.frameActual = 0;
    gs->titanHembra.animacion.fila_ss = 0;
    gs->titanHembra.animacion.velocidadAnim = 10;
    gs->titanHembra.animacion.repetir = true;
    gs->titanHembra.animacion.rotarAnim = true;

}

void pos_levi(s_GameState *gs, s_Assets *assets, int i, int j)
{
    gs->levi.x = j*TAM_CELDA;
    gs->levi.y = i*TAM_CELDA;
}

void grieta_ODM(s_GameState *gs, int i, int j)
{
    int pA = gs->pantalla_actual;
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
    int pA = gs->pantalla_actual;
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
    int pA = gs->pantalla_actual;
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
    int pA = gs->pantalla_actual;
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
    int pA = gs->pantalla_actual;
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
    int pA = gs->pantalla_actual;
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

void genera_gas_elemento(s_GameState *gs, int i, int j)
{
    int pA = gs->pantalla_actual;
    int nE = gs->pantalla[pA].num_elementos;

    if(nE >= MAXELEMENTOS)
        return;

    gs->pantalla[pA].elementos[nE].x = j*TAM_CELDA;
    gs->pantalla[pA].elementos[nE].y = i*TAM_CELDA;
    gs->pantalla[pA].elementos[nE].hitbox.x = gs->pantalla[pA].elementos[nE].x + 10;
    gs->pantalla[pA].elementos[nE].hitbox.y = gs->pantalla[pA].elementos[nE].y + 20;
    gs->pantalla[pA].elementos[nE].hitbox.alto = 10;
    gs->pantalla[pA].elementos[nE].hitbox.ancho = 60;
    gs->pantalla[pA].elementos[nE].hitbox.color = al_map_rgb(255, 255, 0);
    gs->pantalla[pA].elementos[nE].tipo = 5;
    gs->pantalla[pA].elementos[nE].activo = true;
    gs->pantalla[pA].num_elementos++;
}

void genera_hitbox_arbol(s_GameState *gs, int i, int j)
{
    int pA = gs->pantalla_actual;
    int nE = gs->pantalla[pA].num_elementos;

    if(nE >= MAXELEMENTOS)
        return;

    gs->pantalla[pA].elementos[nE].x = j*TAM_CELDA;
    gs->pantalla[pA].elementos[nE].y = i*TAM_CELDA;
    gs->pantalla[pA].elementos[nE].hitbox.x = gs->pantalla[pA].elementos[nE].x;
    gs->pantalla[pA].elementos[nE].hitbox.y = gs->pantalla[pA].elementos[nE].y;
    gs->pantalla[pA].elementos[nE].hitbox.alto = 585;
    gs->pantalla[pA].elementos[nE].hitbox.ancho = 67;
    gs->pantalla[pA].elementos[nE].hitbox.color = al_map_rgb(255, 255, 0);
    gs->pantalla[pA].elementos[nE].tipo = 6;
    gs->pantalla[pA].elementos[nE].activo = true;
    gs->pantalla[pA].num_elementos++;
}