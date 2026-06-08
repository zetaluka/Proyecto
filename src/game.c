#include "commons.h"

void hitbox_init(s_GameState *gs);

//====Funcion principal====//
void game_init(s_GameState *gs)
{
    gs->escala = 1.0f; //Variable que multiplica fondos, elementos, hitbox, etc. Para si en un futuro quiero cambiar de resolucion, redefino la variable y se escala todo.
    gs->ejecutando = 1;
    gs->estadoPantalla = PANTALLA_JUGANDO;
    gs->pantalla_actual = 0;
    gs->nivel = 1;
    gs->pantalla_actual = 0;
    hitbox_init(gs);

    //Inicializacion de levi
    gs->levi.x = 1100;
    gs->levi.y = 200;
    gs->levi.velocidadX = 0;
    gs->levi.velocidadY = 0;
    gs->levi.doble_salto = 0;

    return;
}

void hitbox_init(s_GameState *gs)
{
    //====Pantalla 0====//

    //gs->pantalla[0].hitbox[0] = {0 , 654*gs->escala, SCREEN_X*gs->escala, 0};

    //==================//
}