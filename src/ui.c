#include "commons.h"


//====Funcion principal====//
void render_ui(s_GameState *gs, s_Assets *assets)
{
    char textoTemporizador[20];

    sprintf(textoTemporizador, "%02d: %02d", gs->tiempoJugado.minutos, gs->tiempoJugado.segundos);
    al_draw_text(assets->minimalistTemplateFont, al_map_rgb(220, 220, 220), SCREEN_X-100, 10, 0, textoTemporizador); //arreglar...
    al_draw_text(assets->shingekiFont,al_map_rgb(220, 220, 220), 10, 10, 0, "CodeNoKyojin");

    al_flip_display(); 

    return;
}