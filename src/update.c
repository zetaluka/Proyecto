#include "commons.h"

//====Prototipos====//
void update_jugando(s_GameState *gs, s_Assets *assets);
void update_tiempo_jugado(s_GameState* gs);
void update_levi_movimiento(s_GameState *gs, s_Assets *assets);
void valida_levi_suelo(s_Assets *assets, s_GameState *gs);
void transicion_pantalla(s_GameState *gs, s_Assets *assets);
void hitbox_levi(s_GameState *gs, s_Assets *assets);
void comprueba_colision(s_GameState *gs);
void cuadrado_prueba (s_GameState *gs);
void ataque_levi(s_GameState *gs, s_Assets *assets);
bool colision_ataque(s_GameState *gs, int i);
bool colision(s_GameState *gs, int i);


//====Funcion principal====//
void update(s_GameState *gs, s_Assets *assets)
{
    switch(gs->estadoPantalla) //Detecta en que estado esta, ejemplo: Menu, jugando, pausa, etc.
    {
        case PANTALLA_MENU:
            break;

        case PANTALLA_JUGANDO:
            update_jugando(gs,assets);
            break;

        case PANTALLA_GAME_OVER:
            break;
    }

}


void update_jugando(s_GameState *gs, s_Assets *assets) //Funcion si para cuando se este en la pantalla de juego
{
    update_tiempo_jugado(gs);
    update_levi_movimiento(gs, assets);

    return;
}

void update_tiempo_jugado(s_GameState* gs) //Funcion para hacer funcionar el cronometro de tiempo jugado
{
    if(gs->tiempoJugado.validacion == 1)
    {
        gs->tiempoJugado.segundos++;
        if(gs->tiempoJugado.segundos>=60)
        {
            gs->tiempoJugado.segundos=0;
            gs->tiempoJugado.minutos++;
        }
        gs->tiempoJugado.validacion=0;
    }
}

void update_levi_movimiento(s_GameState *gs, s_Assets *assets)
{ 

    if(gs->input.keyLShift == 1 && gs->input.keyD == 1) //Si mantiene el LShift corre
        gs->levi.x += 4.5f;
        
    else if(gs->input.keyLShift == 1 && gs->input.keyA == 1) //Si mantiene el LShift corre
        gs->levi.x -= 4.5f;

    else if(gs->input.keyA == 1) //Camina izquierda
        gs->levi.x -= 3;

    else if(gs->input.keyD == 1) //Camina derecha
        gs->levi.x += 3;

    /*if(gs->input.keyW == 1)
    gs->levi.y -= 2;
    if(gs->input.keyS == 1) 
    gs->levi.y += 2;*/

    //Gravedad
    gs->levi.velocidadY += 0.5; //Lo hace moverse todo el rato hacia abajo
    gs->levi.y = gs->levi.y + gs->levi.velocidadY;

    gs->levi.levi_suelo = false;
    cuadrado_prueba(gs);
    hitbox_levi(gs,assets);
    comprueba_colision(gs);
    ataque_levi(gs,assets);

    //====Doble salto====//
    if(gs->input.keySpace == 1 && gs->levi.levi_suelo) //Salto y habilita doble salto
        {
            gs->levi.velocidadY = -11;
            gs->levi.doble_salto = true;
            gs->input.keySpace = 0;
        }

    if(gs->input.keySpace == 1 && gs->levi.doble_salto) //Confirma se doble salto esta habilitado
    {
        gs->levi.velocidadY = -8;
        gs->levi.doble_salto = false;
        gs->input.keySpace = 0;
    }


    //========================================================//

    transicion_pantalla(gs, assets);
    
    if(gs->input.keyL == 1) //**Para debuggear: indica la posicion del personaje y la pantalla en la que se encuentra
    {
        printf("levi.x = %.0f, levi.y = %.0f\n",gs->levi.x, gs->levi.y);
        printf("pantalla actual: %d\n",gs->pantalla_actual);
        printf("Hitbox levi: x: [%.1f][%.1f][%.1f][%.1f]\nHitbox espada: (%.1f)(%.1f)(%.1f)(%.1f)\n", gs->levi.hitbox.x, gs->levi.hitbox.y, gs->levi.hitbox.ancho,gs->levi.hitbox.alto, gs->levi.hitboxAtaque.x, gs->levi.hitboxAtaque.y, gs->levi.hitboxAtaque.ancho, gs->levi.hitboxAtaque.alto);
        printf("Viendo derecha: %d",gs->levi.viendoDerecha);
        //gs->input.keyL = 0;
    }
}

void transicion_pantalla(s_GameState *gs, s_Assets *assets) //Efecto de transicion por pantallas
{

    if(gs->levi.x+40 >= SCREEN_X && gs->pantalla_actual < MAXPANTALLAS - 1)
    {
        gs->pantalla_actual++;
        gs->levi.x = -20; 
    }

    if(gs->levi.x+35 <= 0 && gs->pantalla_actual > 0)
    {
        gs->pantalla_actual--;
        gs->levi.x = SCREEN_X - 70;
        hitbox_init(gs);
    }

     if(gs->pantalla_actual > gs->variables.carga_pantalla)
    {
        mapa1(gs, assets);
        gs->variables.carga_pantalla++;
        hitbox_init(gs);
    }

}

void hitbox_levi(s_GameState *gs, s_Assets *assets) //Actualiza la hitbox del personaje principal
{
    gs->levi.hitbox.alto = al_get_bitmap_height(assets->levi.levi_parado)*2;
    gs->levi.hitbox.ancho = LEVI_HB_RECORTE - 10;
    gs->levi.hitbox.x = gs->levi.x + LEVI_HB_RECORTE;
    gs->levi.hitbox.y = gs->levi.y;
}

bool colision(s_GameState *gs, int i) // Detecta si la hitbox del personaje choca con alguna otra
{
    int pA = gs->pantalla_actual;

    if(gs->levi.hitbox.x + gs->levi.hitbox.ancho >= gs->pantalla[pA].hitbox[i].x
    && gs->levi.hitbox.y + gs->levi.hitbox.alto >= gs->pantalla[pA].hitbox[i].y 
    && gs->levi.hitbox.x <= gs->pantalla[pA].hitbox[i].x + gs->pantalla[pA].hitbox[i].ancho
    && gs->levi.hitbox.y <= gs->pantalla[pA].hitbox[i].y + gs->pantalla[pA].hitbox[i].alto)
        return true;
    
    return false;
}

void comprueba_colision(s_GameState *gs) //Comprueba si la hitbox del personaje choca con alguna otra y actua conforme por cual lado choco
{   
    int i, pA = gs->pantalla_actual;
    float distancia_izquierda = 0, distancia_derecha = 0, distancia_arriba = 0, distancia_abajo = 0;

    for(i=0;i<gs->pantalla[pA].num_hitbox;i++) //Bucle para comparar hitbox y encontrar la coincidente
    {
        if(colision(gs, i))
        {
            //Define las distancias con las paredes
            distancia_izquierda = (gs->levi.hitbox.x + gs->levi.hitbox.ancho) - gs->pantalla[pA].hitbox[i].x;
            distancia_derecha = (gs->pantalla[pA].hitbox[i].x + gs->pantalla[pA].hitbox[i].ancho) - gs->levi.hitbox.x;
            distancia_arriba = (gs->levi.hitbox.y + gs->levi.hitbox.alto) - gs->pantalla[pA].hitbox[i].y;
            distancia_abajo = (gs->pantalla[pA].hitbox[i].y + gs->pantalla[pA].hitbox[i].alto) - gs->levi.hitbox.y;

            //Comprueba la menor distancia entre las paredes para detectar por donde choco
            if(distancia_izquierda < distancia_derecha && distancia_izquierda < distancia_arriba && distancia_izquierda < distancia_abajo)
            {
                gs->levi.x = gs->pantalla[pA].hitbox[i].x - gs->levi.hitbox.ancho - LEVI_HB_RECORTE;
            }
            else if(distancia_derecha < distancia_izquierda && distancia_derecha < distancia_arriba && distancia_derecha < distancia_abajo)
            {
                gs->levi.x = gs->pantalla[pA].hitbox[i].x + gs->pantalla[pA].hitbox[i].ancho - LEVI_HB_RECORTE;
            }
            else if(distancia_abajo < distancia_arriba && distancia_abajo < distancia_izquierda && distancia_abajo < distancia_derecha)
            {
                gs->levi.y = gs->pantalla[pA].hitbox[i].y + gs->pantalla[pA].hitbox[i].alto;
            }
            else if(distancia_arriba < distancia_abajo && distancia_arriba < distancia_izquierda && distancia_arriba < distancia_derecha)
            {
                gs->levi.y = gs->pantalla[pA].hitbox[i].y - gs->levi.hitbox.alto;
                gs->levi.velocidadY = 0;
                gs->levi.levi_suelo = true; //Habilita levi_suelo, lo que hace que lo habilite a dar un salto
            }

        }
    }

}

void cuadrado_prueba (s_GameState *gs)
{
    if(gs->variables.cambioSentido == false)
        gs->pantalla[0].hitbox[3].y -= 2;
    else if(gs->variables.cambioSentido == true)
        gs->pantalla[0].hitbox[3].y += 2;

    if(gs->pantalla[0].hitbox[3].y >= (SCREEN_Y - gs->pantalla[0].hitbox[0].alto - 50))
        gs->variables.cambioSentido = false;
    if(gs->pantalla[0].hitbox[3].y <= 500)
        gs->variables.cambioSentido = true;

}

void ataque_levi(s_GameState *gs, s_Assets *assets)
{
    int i, pA = gs->pantalla_actual;

    if(gs->levi.cooldownAtaque > 0)
        gs->levi.cooldownAtaque -= 1.0f/FPS;

    if(gs->input.mouseX >= (gs->levi.hitbox.x + gs->levi.hitbox.ancho/2) * gs->escala)
        gs->levi.viendoDerecha = 1;
    else
        gs->levi.viendoDerecha = 0;

    if(gs->levi.viendoDerecha == 1)
    {
        gs->levi.hitboxAtaque.x = gs->levi.hitbox.x + gs->levi.hitbox.ancho;
        gs->levi.hitboxAtaque.y = gs->levi.hitbox.y + gs->levi.hitbox.alto/2;
        gs->levi.hitboxAtaque.alto = 5;
        gs->levi.hitboxAtaque.ancho = 40;
    }
    else if(gs->levi.viendoDerecha == 0)
    {
        gs->levi.hitboxAtaque.x = gs->levi.x; 
        gs->levi.hitboxAtaque.y = gs->levi.y + gs->levi.hitbox.alto/2;
        gs->levi.hitboxAtaque.alto = 5;
        gs->levi.hitboxAtaque.ancho = 40;
    }

    if(gs->input.ClickIzq && gs->levi.cooldownAtaque <= 0)
    {
        gs->levi.cooldownAtaque = 0.75f;
        for(i=0;i<gs->pantalla[pA].num_entidades;i++)
            if(colision_ataque(gs,i))
            {
                printf("colisiono");
            }
    }

}

bool colision_ataque(s_GameState *gs, int i) // Detecta si la hitbox de la espada choca
{
    int pA = gs->pantalla_actual;
    
    if(gs->levi.hitboxAtaque.x + gs->levi.hitboxAtaque.ancho >= gs->pantalla[pA].entidades[i].hitboxTitan.x
    && gs->levi.hitboxAtaque.y + gs->levi.hitboxAtaque.alto >= gs->pantalla[pA].entidades[i].hitboxTitan.y 
    && gs->levi.hitboxAtaque.x <= gs->pantalla[pA].entidades[i].hitboxTitan.x + gs->pantalla[pA].entidades[i].hitboxTitan.ancho
    && gs->levi.hitboxAtaque.y <= gs->pantalla[pA].entidades[i].hitboxTitan.y + gs->pantalla[pA].entidades[i].hitboxTitan.alto)
        return true;

    return false;
}