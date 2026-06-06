#include "commons.h"

//====Prototipos====//
void update_tiempo_jugado(s_GameState* gs);

//====Funcion principal====//
void update(s_GameState *gs, s_InputState *input)
{
    update_tiempo_jugado(gs);

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
