#include "commons.h"
#include <allegro5/allegro_font.h>

void levi_sprites(s_GameState *gs, s_Assets *assets);
void pantalla_0(s_Assets *assets, s_GameState *gs);
void jugando(s_Assets *assets, s_GameState *gs);
void pantalla_1(s_Assets *assets, s_GameState *gs);
void muestra_hitbox(s_GameState *gs, s_Assets *assets);


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
    muestra_hitbox(gs,assets);
}

void pantalla_0(s_Assets *assets, s_GameState *gs)
{

    al_draw_scaled_bitmap(assets->assetsPantalla[0].fondo_base, 0, 0, al_get_bitmap_width(assets->assetsPantalla[0].fondo_base),
    al_get_bitmap_height(assets->assetsPantalla[0].fondo_base), 0, 0, al_get_bitmap_width(assets->assetsPantalla[0].fondo_base)*2*gs->escala,
    al_get_bitmap_height(assets->assetsPantalla[0].fondo_base)*2*gs->escala, 0);

}

void pantalla_1(s_Assets *assets, s_GameState *gs)
{

    al_draw_scaled_bitmap(assets->assetsPantalla[1].fondo_base, 0, 0, al_get_bitmap_width(assets->assetsPantalla[1].fondo_base),
    al_get_bitmap_height(assets->assetsPantalla[1].fondo_base), 0, 0, al_get_bitmap_width(assets->assetsPantalla[1].fondo_base)*2*gs->escala,
    al_get_bitmap_height(assets->assetsPantalla[1].fondo_base)*2*gs->escala, 0);
    
}

void muestra_hitbox(s_GameState *gs, s_Assets *assets)
{
    if(gs->input.keyH == 1)
        {
            al_draw_rectangle(gs->levi.hitbox.x*gs->escala, gs->levi.hitbox.y*gs->escala, (gs->levi.hitbox.x+gs->levi.hitbox.ancho)*gs->escala,
            (gs->levi.hitbox.y+gs->levi.hitbox.alto)*gs->escala, BLANCO,2); //Levi

            al_draw_rectangle(gs->pantalla[0].hitbox[0].x*gs->escala, gs->pantalla[0].hitbox[0].y*gs->escala,
            (gs->pantalla[0].hitbox[0].x+gs->pantalla[0].hitbox[0].ancho)*gs->escala, (gs->pantalla[0].hitbox[0].y+gs->pantalla[0].hitbox[0].alto)*gs->escala, 
            BLANCO, 2); //Suelo

            al_draw_rectangle(gs->pantalla[0].hitbox[1].x*gs->escala, gs->pantalla[0].hitbox[1].y*gs->escala,
            (gs->pantalla[0].hitbox[1].x+gs->pantalla[0].hitbox[1].ancho)*gs->escala, (gs->pantalla[0].hitbox[1].y+gs->pantalla[0].hitbox[1].alto)*gs->escala, 
            BLANCO, 2); //Limite izquerdo de la pantalla

            al_draw_rectangle(gs->pantalla[0].hitbox[2].x*gs->escala, gs->pantalla[0].hitbox[2].y*gs->escala,
            (gs->pantalla[0].hitbox[2].x+gs->pantalla[0].hitbox[2].ancho)*gs->escala, (gs->pantalla[0].hitbox[2].y+gs->pantalla[0].hitbox[2].alto)*gs->escala, 
            BLANCO, 2);

            al_draw_filled_rectangle(gs->pantalla[0].hitbox[3].x*gs->escala, gs->pantalla[0].hitbox[3].y*gs->escala,
            (gs->pantalla[0].hitbox[3].x+gs->pantalla[0].hitbox[3].ancho)*gs->escala, (gs->pantalla[0].hitbox[3].y+gs->pantalla[0].hitbox[3].alto)*gs->escala, 
            al_map_rgb(25, 80, 120));
        }

}