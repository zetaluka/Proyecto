#include "commons.h"

//====Prototipos====//
void update_jugando(s_GameState *gs, s_Assets *assets);
void update_tiempo_jugado(s_GameState* gs);
void update_levi_movimiento(s_GameState *gs, s_Assets *assets);
void valida_levi_suelo(s_Assets *assets, s_GameState *gs);
void transicion_pantalla(s_GameState *gs, s_Assets *assets);
void hitbox_levi(s_GameState *gs, s_Assets *assets);
bool colision(s_GameState *gs);

//====Funcion principal====//
void update(s_GameState *gs, s_Assets *assets)
{
    switch(gs->estadoPantalla)
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


void update_jugando(s_GameState *gs, s_Assets *assets)
{
    update_tiempo_jugado(gs);
    update_levi_movimiento(gs, assets);

    return;
}

void update_tiempo_jugado(s_GameState* gs)
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
    
    if(gs->levi.y == SCREEN_Y - 66 - al_get_bitmap_height(assets->levi.levi_parado)*2)
        gs->levi.doble_salto = 0;


    if(gs->input.keyLShift == 1 && gs->input.keyD == 1)
        gs->levi.x += 4.5f;
    else if(gs->input.keyLShift == 1 && gs->input.keyA == 1)
        gs->levi.x -= 4.5f;
    else if(gs->input.keyA == 1) //Camina izquierda
        gs->levi.x -= 3;
    else if(gs->input.keyD == 1) //Camina derecha
        gs->levi.x += 3;
    /*if(gs->input.keyW == 1)
    gs->levi.y -= 2;
    if(gs->input.keyS == 1) 
    gs->levi.y += 2;*/

    //====Doble salto====//
    if(gs->input.keySpace == 1 && gs->levi.y == SCREEN_Y - 66 - al_get_bitmap_height(assets->levi.levi_parado)*2) //Salto
        {
            gs->levi.velocidadY = -11;
            gs->levi.doble_salto = 1;
            gs->input.keySpace = 0;
        }

    if(gs->input.keySpace == 1 && gs->levi.doble_salto == 1)
    {
        gs->levi.velocidadY = -8;
        gs->levi.doble_salto = 0;
    }

    //========================================================//

    //Gravedad
    gs->levi.velocidadY += 0.5; //Lo hace moverse todo el rato hacia abajo
    gs->levi.y = gs->levi.y + gs->levi.velocidadY;

    //Limites del mapa
    /*if(gs->levi.y >= SCREEN_Y - (al_get_bitmap_height(assets->levi.levi_parado)*2)) //Limite inferior del mapa con respecto al personaje
        gs->levi.y = SCREEN_Y - (al_get_bitmap_height(assets->levi.levi_parado)*2);*/

    if(gs->levi.y >= (SCREEN_Y - 66 - al_get_bitmap_height(assets->levi.levi_parado)*2)) //Limite suelo
        gs->levi.y = (SCREEN_Y - 66 - al_get_bitmap_height(assets->levi.levi_parado)*2);

    if(gs->levi.x <= -35) //Limite izquierdo del mapa con respecto al personaje
        gs->levi.x = -35;

    if(gs->levi.y <= -30) //Limite superior del mapa
        gs->levi.y = -30;
    
   
    hitbox_levi(gs,assets);
    colision(gs);

    transicion_pantalla(gs, assets);
    
    if(gs->input.keyL == 1)
    {
        printf("levi.x = %.0f, levi.y = %.0f\n",gs->levi.x, gs->levi.y);
        printf("pantalla actual: %d\n",gs->pantalla_actual);
        gs->input.keyL = 0;
    }
}

void transicion_pantalla(s_GameState *gs, s_Assets *assets)
{
    if(gs->levi.x+40 >= SCREEN_X && gs->pantalla_actual == 0)
    {
        gs->pantalla_actual=1;
        gs->levi.x = -20; 
    }

    if(gs->levi.x+35 <= 0 && gs->pantalla_actual == 1)
    {
        gs->pantalla_actual = 0;
        gs->levi.x = SCREEN_X - 70; 
    }
}

void hitbox_levi(s_GameState *gs, s_Assets *assets)
{
    gs->levi.hitbox.alto = al_get_bitmap_height(assets->levi.levi_parado)*2;
    gs->levi.hitbox.ancho = LEVI_HB_RECORTE - 10;
    gs->levi.hitbox.x = gs->levi.x + LEVI_HB_RECORTE;
    gs->levi.hitbox.y = gs->levi.y;
}

bool colision(s_GameState *gs)
{
    float distancia_izquierda = (gs->levi.hitbox.x + gs->levi.hitbox.ancho) - gs->pantalla[0].hitbox[3].x;
    float distancia_derecha = (gs->pantalla[0].hitbox[3].x + gs->pantalla[0].hitbox[3].ancho) - gs->levi.hitbox.x;
    float distancia_arriba = (gs->levi.hitbox.y + gs->levi.hitbox.alto) - gs->pantalla[0].hitbox[3].y;
    float distancia_abajo = (gs->pantalla[0].hitbox[3].y + gs->pantalla[0].hitbox[3].alto) - gs->levi.hitbox.y;

    switch(gs->pantalla_actual)
    {
        case 0:
            if(gs->levi.hitbox.x + gs->levi.hitbox.ancho >= gs->pantalla[0].hitbox[3].x
            && gs->levi.hitbox.y + gs->levi.hitbox.alto >= gs->pantalla[0].hitbox[3].y 
            && gs->levi.hitbox.x <= gs->pantalla[0].hitbox[3].x + gs->pantalla[0].hitbox[3].ancho
            && gs->levi.hitbox.y <= gs->pantalla[0].hitbox[3].y + gs->pantalla[0].hitbox[3].alto)
                if(distancia_izquierda < distancia_derecha && distancia_izquierda < distancia_arriba && distancia_izquierda < distancia_abajo)
                    gs->levi.x = gs->pantalla[0].hitbox[3].x - gs->levi.hitbox.ancho - LEVI_HB_RECORTE;
                else if(distancia_derecha < distancia_izquierda && distancia_derecha < distancia_arriba && distancia_derecha < distancia_abajo)
                    gs->levi.x = gs->pantalla[0].hitbox[3].x + gs->pantalla[0].hitbox[3].ancho - LEVI_HB_RECORTE;
                else if(distancia_abajo < distancia_arriba && distancia_abajo < distancia_izquierda && distancia_abajo < distancia_derecha)
                    gs->levi.y = gs->pantalla[0].hitbox[3].y + gs->pantalla[0].hitbox[3].alto;
                else if(distancia_arriba < distancia_abajo && distancia_arriba < distancia_izquierda && distancia_arriba < distancia_derecha)
                {
                    gs->levi.y = gs->pantalla[0].hitbox[3].y - gs->levi.hitbox.alto;
                    gs->levi.velocidadY = 0;
                }
    }

}
