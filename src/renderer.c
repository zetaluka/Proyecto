#include "commons.h"
#include <allegro5/allegro_font.h>

void levi_sprites(s_GameState *gs, s_Assets *assets);
void pantalla_0(s_Assets *assets, s_GameState *gs);
void jugando(s_Assets *assets, s_GameState *gs);
void pantalla_1(s_Assets *assets, s_GameState *gs);


//====Funcion principal====//
void render_gameview(s_GameState *gs, s_Assets *assets)
{
    al_clear_to_color(al_map_rgb(0,0,0));
    switch (gs->estadoPantalla)
    {
        case PANTALLA_MENU:
            break;
        case PANTALLA_JUGANDO:
            jugando(assets, gs);
            break;
        case PANTALLA_GAME_OVER:
            break;
    }
    levi_sprites(gs, assets);

    return;
}

void levi_sprites(s_GameState *gs, s_Assets *assets)
{
    al_draw_scaled_bitmap(assets->levi.levi_parado, 0, 0, al_get_bitmap_width(assets->levi.levi_parado),
    al_get_bitmap_height(assets->levi.levi_parado),gs->levi.x*gs->escala, gs->levi.y*gs->escala, al_get_bitmap_width(assets->levi.levi_parado)*2*gs->escala,
    al_get_bitmap_height(assets->levi.levi_parado)*2*gs->escala, 0);
}

void jugando(s_Assets *assets, s_GameState *gs)
{
    switch(gs->pantalla_actual)
    {
        case 0:
            pantalla_0(assets, gs);
            break;
        case 1:
            pantalla_1(assets,gs);
            break;
    }
}

void pantalla_0(s_Assets *assets, s_GameState *gs)
{

    al_draw_scaled_bitmap(assets->assetsPantalla[0].fondo_base, 0, 0, al_get_bitmap_width(assets->assetsPantalla[0].fondo_base),
    al_get_bitmap_height(assets->assetsPantalla[0].fondo_base), 0, 0, al_get_bitmap_width(assets->assetsPantalla[0].fondo_base)*2*gs->escala,
    al_get_bitmap_height(assets->assetsPantalla[0].fondo_base)*2*gs->escala, 0);

}

void pantalla_1(s_Assets *assets, s_GameState *gs)
{

    al_draw_scaled_bitmap(assets->assetsPantalla[0].fondo_base, 0, 0, al_get_bitmap_width(assets->assetsPantalla[0].fondo_base),
    al_get_bitmap_height(assets->assetsPantalla[0].fondo_base), 0, 0, al_get_bitmap_width(assets->assetsPantalla[0].fondo_base)*2*gs->escala,
    al_get_bitmap_height(assets->assetsPantalla[0].fondo_base)*2*gs->escala, 0);
    al_draw_filled_rectangle( 200, 200, 400, 400, al_map_rgb(0, 0, 0));
    
}