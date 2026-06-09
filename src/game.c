#include "commons.h"

void hitbox_init(s_GameState *gs);

//====Funcion principal====//
void game_init(s_GameState *gs)
{
    gs->escala = 1.0f; //Variable que multiplica fondos, elementos, hitbox, etc. Para si en un futuro quiero cambiar de resolucion, redefino la variable y se escala todo.
    gs->ejecutando =1;
    gs->estadoPantalla = PANTALLA_JUGANDO;
    gs->pantalla_actual = 0;
    gs->nivel = 1;
    gs->pantalla_actual = 0;
    hitbox_init(gs);

    //Inicializacion de levi
    gs->levi.x = 700;
    gs->levi.y = 100;
    gs->levi.velocidadX = 0;
    gs->levi.velocidadY = 0;
    gs->levi.doble_salto = 0;

    return;
}

void hitbox_init(s_GameState *gs)
{
    //====Pantalla 0====//

    gs->pantalla[0].hitbox[0] = (s_Hitbox){0 , (SCREEN_Y - 66), SCREEN_X, 66}; //Suelo
    gs->pantalla[0].hitbox[1] = (s_Hitbox){(-4), 0, 4, SCREEN_Y}; //Limite izquierdo de la pantalla
    gs->pantalla[0].hitbox[2] = (s_Hitbox){0, -2, SCREEN_X, 4 }; //Limite superior de la pantalla
    gs->pantalla[0].hitbox[3] = (s_Hitbox){866, 600, 40, 40};

    //==================//
}