#include "commons.h"

void dibujar_texto_borde(s_GameState *gs, ALLEGRO_FONT *fuente, ALLEGRO_COLOR colorTexto, ALLEGRO_COLOR colorBorde, float x, float y, int flags, const char *texto, float grosor);

//====Funcion principal====//
void render_ui(s_GameState *gs, s_Assets *assets)
{
    //Parametros funcion dibujar_texto_borde: gs, fuente, color texto, color borde, posicion x, posicion y, flags, texto y grosor del borde

    //Sirve para escalar todo por gs->escala, para cuando pase a pantalla completa//
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_scale_transform(&transform, gs->escala, gs->escala);
    al_use_transform(&transform);
    ////////////////////////////////////////////////////////////////////////////////

    char texto[20];

    sprintf(texto, "%02d: %02d", gs->tiempoJugado.minutos, gs->tiempoJugado.segundos);
    al_draw_text(assets->minimalistTemplateFont50, al_map_rgb(220, 220, 220), (SCREEN_X-100), 10, 0, texto); //arreglar...
    al_draw_text(assets->shingekiFont30,al_map_rgb(220, 220, 220), 10, 10, 0, "CodeNoKyojin");

    //============Escudos=============//

    al_draw_scaled_bitmap(assets->assetsPantalla.escudoLegion, 0, 0, al_get_bitmap_width(assets->assetsPantalla.escudoLegion),
    al_get_bitmap_height(assets->assetsPantalla.escudoLegion), (SCREEN_X - 100) , 55 ,
    al_get_bitmap_width(assets->assetsPantalla.escudoLegion), al_get_bitmap_height(assets->assetsPantalla.escudoLegion), 0);

    sprintf(texto, "x%d", gs->levi.inventario.escudos);
    dibujar_texto_borde(gs, assets->minimalistTemplateFont50, al_map_rgb(255,255,255), al_map_rgb(0,0,0), (SCREEN_X -30), 55, ALLEGRO_ALIGN_RIGHT, texto, 1.5);

    //============================//

    al_flip_display(); 


    return;
}

void dibujar_texto_borde(s_GameState *gs, ALLEGRO_FONT *fuente, ALLEGRO_COLOR colorTexto, ALLEGRO_COLOR colorBorde, float x, float y, int flags, const char *texto, float grosor) 
{
    al_draw_text(fuente, colorBorde, (x - grosor), y, flags, texto);
    al_draw_text(fuente, colorBorde, (x + grosor), y, flags, texto);
    al_draw_text(fuente, colorBorde, x, (y - grosor), flags, texto);
    al_draw_text(fuente, colorBorde, x, (y + grosor), flags, texto);

    al_draw_text(fuente, colorTexto, x, y, flags, texto);

}