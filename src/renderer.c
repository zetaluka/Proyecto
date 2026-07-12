#include "commons.h"
#include <allegro5/allegro_font.h>

void levi_sprites(s_GameState *gs, s_Assets *assets);
void pantalla_0(s_Assets *assets, s_GameState *gs);
void jugando(s_Assets *assets, s_GameState *gs);
void pantalla_1(s_Assets *assets, s_GameState *gs);
void muestra_hitbox(s_GameState *gs, s_Assets *assets);
void titanes_sprites(s_GameState *gs, s_Assets *assets);


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
    int leviX = round(gs->levi.x)*gs->escala;
    int leviY = round(gs->levi.y)*gs->escala;

    /*al_draw_scaled_bitmap(assets->levi.levi_parado, 0, 0, al_get_bitmap_width(assets->levi.levi_parado),
    al_get_bitmap_height(assets->levi.levi_parado),gs->levi.x*gs->escala, gs->levi.y*gs->escala, al_get_bitmap_width(assets->levi.levi_parado)*2*gs->escala,
    al_get_bitmap_height(assets->levi.levi_parado)*2*gs->escala, 0);*/

    al_draw_scaled_bitmap(assets->levi.levi, 0, 0, al_get_bitmap_width(assets->levi.levi),
    al_get_bitmap_height(assets->levi.levi), leviX, leviY, al_get_bitmap_width(assets->levi.levi)*gs->escala,
    al_get_bitmap_height(assets->levi.levi)*gs->escala, 0);
}

void jugando(s_Assets *assets, s_GameState *gs)
{
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_translate_transform(&transform, -gs->camara.x, 0);
    al_use_transform(&transform);

    switch(gs->pantalla_actual)
    {
        case 0:
            pantalla_0(assets, gs);
            titanes_sprites(gs, assets);
            al_draw_bitmap(assets->assetsPantalla.cubo, gs->pantalla[0].hitbox[3].x, gs->pantalla[0].hitbox[3].y, 0);
            break;

        case 1:
            pantalla_1(assets,gs);
            titanes_sprites(gs, assets);
            break;
        default:
            pantalla_0(assets, gs);
            titanes_sprites(gs, assets);
            break;
    }
    muestra_hitbox(gs,assets);
}

void titanes_sprites(s_GameState *gs, s_Assets *assets)
{
    int i, pA = gs->pantalla_actual;
    int titanX, titanY;

    for(i = 0; i < gs->pantalla[pA].num_entidades; i++)
    {
        titanX = round(gs->pantalla[pA].entidades[i].x);
        titanY = round(gs->pantalla[pA].entidades[i].y);

        if(gs->pantalla[pA].entidades[i].activo == true)
        {
            if(gs->pantalla[pA].entidades[i].vida > 0)
            {
                al_draw_scaled_bitmap(assets->titanes.titan_bizarro,0, 0, al_get_bitmap_width(assets->titanes.titan_bizarro),
                al_get_bitmap_height(assets->titanes.titan_bizarro), titanX * gs->escala, titanY * gs->escala,
                al_get_bitmap_width(assets->titanes.titan_bizarro) * gs->escala,al_get_bitmap_height(assets->titanes.titan_bizarro) * gs->escala,0);
            }
        }
    }
}


void pantalla_0(s_Assets *assets, s_GameState *gs)
{
    int pA = gs->pantalla_actual, nE = gs->pantalla[pA].num_elementos;

    //Agrega el fondo de la pantalla 0
    al_draw_scaled_bitmap(assets->assetsPantalla.fondo_base, 0, 0, al_get_bitmap_width(assets->assetsPantalla.fondo_base),
    al_get_bitmap_height(assets->assetsPantalla.fondo_base), 0, 0, al_get_bitmap_width(assets->assetsPantalla.fondo_base)*2*gs->escala,
    al_get_bitmap_height(assets->assetsPantalla.fondo_base)*2*gs->escala, 0);


    for(int i = 0; i<nE; i++)
    {
        if(gs->pantalla[pA].elementos[i].tipo == 1)
            al_draw_scaled_bitmap(assets->assetsPantalla.grieta, 0, 0, al_get_bitmap_width(assets->assetsPantalla.grieta),
            al_get_bitmap_height(assets->assetsPantalla.grieta), gs->pantalla[pA].elementos[i].x * gs->escala, gs->pantalla[pA].elementos[i].y * gs->escala,
            al_get_bitmap_width(assets->assetsPantalla.grieta) * 1.5 * gs->escala, al_get_bitmap_height(assets->assetsPantalla.grieta) * 1.5 * gs->escala, 0);

        else if(gs->pantalla[pA].elementos[i].tipo == 2 && gs->pantalla[pA].elementos[i].activo == true)
            al_draw_scaled_bitmap(assets->assetsPantalla.escudoLegion, 0, 0, al_get_bitmap_width(assets->assetsPantalla.escudoLegion),
            al_get_bitmap_height(assets->assetsPantalla.escudoLegion), gs->pantalla[pA].elementos[i].x * gs->escala, gs->pantalla[pA].elementos[i].y * gs->escala,
            al_get_bitmap_width(assets->assetsPantalla.escudoLegion)* gs->escala, al_get_bitmap_height(assets->assetsPantalla.escudoLegion)* gs->escala, 0);
    }

}

void pantalla_1(s_Assets *assets, s_GameState *gs)
{
    int pA = gs->pantalla_actual, nE = gs->pantalla[pA].num_elementos;

    //Agrega el fondo de la pantalla 1
    al_draw_scaled_bitmap(assets->assetsPantalla.fondo_titan_colosal, 0, 0, al_get_bitmap_width(assets->assetsPantalla.fondo_titan_colosal),
    al_get_bitmap_height(assets->assetsPantalla.fondo_titan_colosal), 0, 0, al_get_bitmap_width(assets->assetsPantalla.fondo_titan_colosal)*2*gs->escala,
    al_get_bitmap_height(assets->assetsPantalla.fondo_titan_colosal)*2*gs->escala, 0);

    for(int i = 0; i<nE; i++)
    {
        if(gs->pantalla[pA].elementos[i].tipo == 1)
            al_draw_scaled_bitmap(assets->assetsPantalla.grieta, 0, 0, al_get_bitmap_width(assets->assetsPantalla.grieta),
            al_get_bitmap_height(assets->assetsPantalla.grieta), gs->pantalla[pA].elementos[i].x * gs->escala, gs->pantalla[pA].elementos[i].y * gs->escala,
            al_get_bitmap_width(assets->assetsPantalla.grieta) * 1.5 * gs->escala, al_get_bitmap_height(assets->assetsPantalla.grieta) * 1.5 * gs->escala, 0);

        else if(gs->pantalla[pA].elementos[i].tipo == 2 && gs->pantalla[pA].elementos[i].activo == true)
            al_draw_scaled_bitmap(assets->assetsPantalla.escudoLegion, 0, 0, al_get_bitmap_width(assets->assetsPantalla.escudoLegion),
            al_get_bitmap_height(assets->assetsPantalla.escudoLegion), gs->pantalla[pA].elementos[i].x * gs->escala, gs->pantalla[pA].elementos[i].y * gs->escala,
            al_get_bitmap_width(assets->assetsPantalla.escudoLegion)* gs->escala, al_get_bitmap_height(assets->assetsPantalla.escudoLegion)* gs->escala, 0);
    }
    
}

void muestra_hitbox(s_GameState *gs, s_Assets *assets) //Muestra las hitbox de la pantalla
{
    int i, pA = gs->pantalla_actual;

    if(gs->input.keyH == 1)
    {
        al_draw_rectangle(gs->levi.hitbox.x*gs->escala, gs->levi.hitbox.y*gs->escala, (gs->levi.hitbox.x+gs->levi.hitbox.ancho)*gs->escala,
        (gs->levi.hitbox.y+gs->levi.hitbox.alto)*gs->escala, BLANCO,2); //Levi

        al_draw_rectangle(gs->levi.hitboxAtaque.x*gs->escala, gs->levi.hitboxAtaque.y*gs->escala, (gs->levi.hitboxAtaque.x+gs->levi.hitboxAtaque.ancho)*gs->escala,
        (gs->levi.hitboxAtaque.y+gs->levi.hitboxAtaque.alto)*gs->escala, al_map_rgb(255, 0, 0) ,2); //Espadas

        if(gs->levi.dash.activo)
            al_draw_rectangle(gs->levi.dash.hitboxDash.x*gs->escala, gs->levi.dash.hitboxDash.y*gs->escala, (gs->levi.dash.hitboxDash.x+gs->levi.dash.hitboxDash.ancho)*gs->escala,
            (gs->levi.dash.hitboxDash.y+gs->levi.dash.hitboxDash.alto)*gs->escala, al_map_rgb(0, 0, 255) ,2); //Dash
        
        al_draw_rectangle(gs->levi.hitboxODM.x*gs->escala, gs->levi.hitboxODM.y*gs->escala, (gs->levi.hitboxODM.x+gs->levi.hitboxODM.ancho)*gs->escala,
        (gs->levi.hitboxODM.y+gs->levi.hitboxODM.alto)*gs->escala, al_map_rgb(255, 0, 0) ,2); //ODM (Posicion del mouse)

        for(i=0; i<gs->pantalla[pA].num_hitbox; i++)
        {
            al_draw_rectangle(gs->pantalla[pA].hitbox[i].x*gs->escala, gs->pantalla[pA].hitbox[i].y*gs->escala,
            (gs->pantalla[pA].hitbox[i].x+gs->pantalla[pA].hitbox[i].ancho)*gs->escala, (gs->pantalla[pA].hitbox[i].y+gs->pantalla[pA].hitbox[i].alto)*gs->escala, 
            gs->pantalla[pA].hitbox[i].color, 2); //Dibuja las hitbox de cada mapa
        }

        for(i=0; i<gs->pantalla[pA].num_elementos; i++)
            if(gs->pantalla[pA].elementos[i].activo == true)
            {
                al_draw_rectangle(gs->pantalla[pA].elementos[i].hitbox.x*gs->escala, gs->pantalla[pA].elementos[i].hitbox.y*gs->escala,
                (gs->pantalla[pA].elementos[i].hitbox.x+gs->pantalla[pA].elementos[i].hitbox.ancho)*gs->escala, (gs->pantalla[pA].elementos[i].hitbox.y+gs->pantalla[pA].
                elementos[i].hitbox.alto)*gs->escala, 
                gs->pantalla[pA].elementos[i].hitbox.color, 2); //Dibuja las hitbox de cada mapa
            }
        
        

        for(i=0; i<gs->pantalla[pA].num_entidades; i++)
            if(gs->pantalla[pA].entidades[i].activo == true)
            {
                al_draw_rectangle(gs->pantalla[pA].entidades[i].hitboxTitan.x*gs->escala, gs->pantalla[pA].entidades[i].hitboxTitan.y*gs->escala,
                (gs->pantalla[pA].entidades[i].hitboxTitan.x + gs->pantalla[pA].entidades[i].hitboxTitan.ancho)*gs->escala,
                (gs->pantalla[pA].entidades[i].hitboxTitan.y + gs->pantalla[pA].entidades[i].hitboxTitan.alto)*gs->escala, BLANCO, 2);

                al_draw_rectangle(gs->pantalla[pA].entidades[i].hitboxNuca.x*gs->escala, gs->pantalla[pA].entidades[i].hitboxNuca.y*gs->escala,
                (gs->pantalla[pA].entidades[i].hitboxNuca.x + gs->pantalla[pA].entidades[i].hitboxNuca.ancho)*gs->escala,
                (gs->pantalla[pA].entidades[i].hitboxNuca.y + gs->pantalla[pA].entidades[i].hitboxNuca.alto)*gs->escala, al_map_rgb(255, 165, 0), 2);
            }
            
    }
}

