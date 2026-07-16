#include "commons.h"
#include <allegro5/allegro_font.h>

void levi_sprites(s_GameState *gs, s_Assets *assets);
void pantalla_0(s_Assets *assets, s_GameState *gs);
void jugando(s_Assets *assets, s_GameState *gs);
void pantalla_1(s_Assets *assets, s_GameState *gs);
void muestra_hitbox(s_GameState *gs, s_Assets *assets);
void titanes_sprites(s_GameState *gs, s_Assets *assets);
void dibujar_fondo(s_GameState *gs, s_Assets *assets);
void dibuja_gas(s_GameState *gs, s_Assets *assets);


//====Funcion principal====//
void render_gameview(s_GameState *gs, s_Assets *assets)
{
    al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);

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
    //Parametros al_draw_bitmap_region: spritesheet, frameX, frameY, ancho frame, alto frame, pos x, pos y, flags

    int leviX = round(gs->levi.x);
    int leviY = round(gs->levi.y);
    float leviXODM = gs->levi.hitbox.x;
    float leviYODM = gs->levi.hitbox.y + 50;
    int numFrameX = gs->levi.animacion.frameActual * LEVI_SS_ANCHO; //Calcula el frame a usar dependiendo del frame actual

    dibuja_gas(gs,assets);

    al_draw_bitmap_region(assets->levi.levi_SS, numFrameX, 0, LEVI_SS_ANCHO, LEVI_SS_ALTO, leviX, leviY, 0);

    if(gs->levi.ODM.engancheActivo == true || gs->levi.ODM.activo == true)
    {
        al_draw_line(leviXODM, leviYODM, (gs->levi.ODM.puntoEngancheX - 10), gs->levi.ODM.puntoEngancheY, al_map_rgb(70, 70, 70), 1);
        al_draw_line((leviXODM + 20), leviYODM, (gs->levi.ODM.puntoEngancheX + 10), gs->levi.ODM.puntoEngancheY, al_map_rgb(70, 70, 70), 1);
    }
}
void dibuja_gas(s_GameState *gs, s_Assets *assets)
{
    //Parametros al_draw_scaled_bitmap: Spritesheet, frameX, frameY, ancho, alto, x , y , (ancho destino, alto destino (escalado)), flags

    int i;
    int numFrameX;
    int gas_ss_ancho = 64;

    for(i=0;i<MAXGAS;i++)
    {
        if(gs->animaciones.gas[i].activo == false) //Si el gas no esta activo lo salta
            continue;

        numFrameX = gas_ss_ancho * gs->animaciones.gas[i].frameActual; //Calcula el frame a ocupar
        al_draw_scaled_bitmap(assets->assetsPantalla.gas, numFrameX, 0, gas_ss_ancho, gas_ss_ancho, gs->animaciones.gas[i].x - 10, gs->animaciones.gas[i].y + 48, gas_ss_ancho * 0.2f, 
            gas_ss_ancho * 0.2f, 0);
        al_draw_scaled_bitmap(assets->assetsPantalla.gas, numFrameX, 0, gas_ss_ancho, gas_ss_ancho, gs->animaciones.gas[i].x + 20, gs->animaciones.gas[i].y + 50, gas_ss_ancho * 0.2f, 
            gas_ss_ancho * 0.2f, 0);
    }

}

void jugando(s_Assets *assets, s_GameState *gs)
{
    //Sirve para escalar todo por gs->escala, para cuando pase a pantalla completa//
    ALLEGRO_TRANSFORM transform; 
    al_identity_transform(&transform);
    al_translate_transform(&transform, -gs->camara.x, 0);
    al_scale_transform(&transform, gs->escala, gs->escala);
    al_use_transform(&transform);
    ////////////////////////////////////////////////////////////////////////////////

    dibujar_fondo(gs, assets);
    titanes_sprites(gs, assets);
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
                al_draw_bitmap(assets->titanes.titan_bizarro, titanX, titanY, 0);
            }
        }
    }
}

void dibujar_fondo(s_GameState *gs, s_Assets *assets)
{
    int pA = gs->pantalla_actual, nE = gs->pantalla[pA].num_elementos;

    if(strcmp("fondo_titan_colosal", gs->pantalla[pA].fondo) == 0)
        al_draw_scaled_bitmap(assets->assetsPantalla.fondo_titan_colosal, 0, 0,
            al_get_bitmap_width(assets->assetsPantalla.fondo_titan_colosal), al_get_bitmap_height(assets->assetsPantalla.fondo_titan_colosal), 0, 0,
            al_get_bitmap_width(assets->assetsPantalla.fondo_titan_colosal)*2, al_get_bitmap_height(assets->assetsPantalla.fondo_titan_colosal)*2, 0);
    else 
        al_draw_scaled_bitmap(assets->assetsPantalla.fondo_base, 0, 0,
            al_get_bitmap_width(assets->assetsPantalla.fondo_base), al_get_bitmap_height(assets->assetsPantalla.fondo_base), 0, 0,
            al_get_bitmap_width(assets->assetsPantalla.fondo_base)*2, al_get_bitmap_height(assets->assetsPantalla.fondo_base)*2, 0);

    for(int i = 0; i<nE; i++)
    {
        if(gs->pantalla[pA].elementos[i].tipo == 1)
            al_draw_scaled_bitmap(assets->assetsPantalla.grieta, 0, 0,
                al_get_bitmap_width(assets->assetsPantalla.grieta), al_get_bitmap_height(assets->assetsPantalla.grieta),
                gs->pantalla[pA].elementos[i].x, gs->pantalla[pA].elementos[i].y,
                al_get_bitmap_width(assets->assetsPantalla.grieta) * 1.5, al_get_bitmap_height(assets->assetsPantalla.grieta) * 1.5, 0);

        else if(gs->pantalla[pA].elementos[i].tipo == 2 && gs->pantalla[pA].elementos[i].activo == true)
            al_draw_bitmap(assets->assetsPantalla.escudoLegion, gs->pantalla[pA].elementos[i].x, gs->pantalla[pA].elementos[i].y, 0);
    }

    for(int i = 0; i<nE; i++)
        if(gs->pantalla[pA].elementos[i].tipo == 3)
            al_draw_bitmap(assets->assetsPantalla.casa1, gs->pantalla[pA].elementos[i].x, gs->pantalla[pA].elementos[i].y, 0);

    for(int i=0; i<5; i++)
        if(gs->variables.grietas[i].x != 0)
            al_draw_bitmap(assets->assetsPantalla.grietaODM, gs->variables.grietas[i].x - 11, gs->variables.grietas[i].y -4, 0);

}

void pantalla_0(s_Assets *assets, s_GameState *gs)
{
    int pA = gs->pantalla_actual, nE = gs->pantalla[pA].num_elementos;

    al_draw_scaled_bitmap(assets->assetsPantalla.fondo_base, 0, 0,
        al_get_bitmap_width(assets->assetsPantalla.fondo_base), al_get_bitmap_height(assets->assetsPantalla.fondo_base), 0, 0,
        al_get_bitmap_width(assets->assetsPantalla.fondo_base)*2, al_get_bitmap_height(assets->assetsPantalla.fondo_base)*2, 0);

    for(int i = 0; i<nE; i++)
    {
        if(gs->pantalla[pA].elementos[i].tipo == 1)
            al_draw_scaled_bitmap(assets->assetsPantalla.grieta, 0, 0,
                al_get_bitmap_width(assets->assetsPantalla.grieta), al_get_bitmap_height(assets->assetsPantalla.grieta),
                gs->pantalla[pA].elementos[i].x, gs->pantalla[pA].elementos[i].y,
                al_get_bitmap_width(assets->assetsPantalla.grieta) * 1.5, al_get_bitmap_height(assets->assetsPantalla.grieta) * 1.5, 0);

        else if(gs->pantalla[pA].elementos[i].tipo == 2 && gs->pantalla[pA].elementos[i].activo == true)
            al_draw_bitmap(assets->assetsPantalla.escudoLegion, gs->pantalla[pA].elementos[i].x, gs->pantalla[pA].elementos[i].y, 0);
    }

    for(int i = 0; i<nE; i++)
        if(gs->pantalla[pA].elementos[i].tipo == 3)
            al_draw_bitmap(assets->assetsPantalla.casa1, gs->pantalla[pA].elementos[i].x, gs->pantalla[pA].elementos[i].y, 0);

    for(int i=0; i<5; i++)
        if(gs->variables.grietas[i].x != 0)
            al_draw_bitmap(assets->assetsPantalla.grietaODM, gs->variables.grietas[i].x - 11, gs->variables.grietas[i].y -4, 0);

}

void pantalla_1(s_Assets *assets, s_GameState *gs)
{
    int pA = gs->pantalla_actual, nE = gs->pantalla[pA].num_elementos;

    //Agrega el fondo de la pantalla 1
    al_draw_scaled_bitmap(assets->assetsPantalla.fondo_titan_colosal, 0, 0,
        al_get_bitmap_width(assets->assetsPantalla.fondo_titan_colosal), al_get_bitmap_height(assets->assetsPantalla.fondo_titan_colosal), 0, 0,
        al_get_bitmap_width(assets->assetsPantalla.fondo_titan_colosal)*2, al_get_bitmap_height(assets->assetsPantalla.fondo_titan_colosal)*2, 0);

    for(int i = 0; i<nE; i++)
    {
        if(gs->pantalla[pA].elementos[i].tipo == 1)
            al_draw_scaled_bitmap(assets->assetsPantalla.grieta, 0, 0,
                al_get_bitmap_width(assets->assetsPantalla.grieta), al_get_bitmap_height(assets->assetsPantalla.grieta),
                gs->pantalla[pA].elementos[i].x, gs->pantalla[pA].elementos[i].y,
                al_get_bitmap_width(assets->assetsPantalla.grieta) * 1.5, al_get_bitmap_height(assets->assetsPantalla.grieta) * 1.5, 0);

        else if(gs->pantalla[pA].elementos[i].tipo == 2 && gs->pantalla[pA].elementos[i].activo == true)
            al_draw_bitmap(assets->assetsPantalla.escudoLegion,
                gs->pantalla[pA].elementos[i].x, gs->pantalla[pA].elementos[i].y, 0);
    }
    
}

void muestra_hitbox(s_GameState *gs, s_Assets *assets)
{
    int i, pA = gs->pantalla_actual;

    if(gs->input.keyH == 1)
    {
        al_draw_rectangle(gs->levi.hitbox.x, gs->levi.hitbox.y,
            gs->levi.hitbox.x+gs->levi.hitbox.ancho, gs->levi.hitbox.y+gs->levi.hitbox.alto, BLANCO, 2);

        al_draw_rectangle(gs->levi.hitboxAtaque.x, gs->levi.hitboxAtaque.y,
            gs->levi.hitboxAtaque.x+gs->levi.hitboxAtaque.ancho, gs->levi.hitboxAtaque.y+gs->levi.hitboxAtaque.alto,
            al_map_rgb(255, 0, 0), 2);

        if(gs->levi.dash.activo)
            al_draw_rectangle(gs->levi.dash.hitboxDash.x, gs->levi.dash.hitboxDash.y,
                gs->levi.dash.hitboxDash.x+gs->levi.dash.hitboxDash.ancho, gs->levi.dash.hitboxDash.y+gs->levi.dash.hitboxDash.alto,
                al_map_rgb(0, 0, 255), 2);

        al_draw_rectangle(gs->levi.hitboxODM.x, gs->levi.hitboxODM.y,
            gs->levi.hitboxODM.x+gs->levi.hitboxODM.ancho, gs->levi.hitboxODM.y+gs->levi.hitboxODM.alto,
            al_map_rgb(255, 0, 0), 2);

        for(i=0; i<gs->pantalla[pA].num_hitbox; i++)
            al_draw_rectangle(gs->pantalla[pA].hitbox[i].x, gs->pantalla[pA].hitbox[i].y,
                gs->pantalla[pA].hitbox[i].x+gs->pantalla[pA].hitbox[i].ancho, gs->pantalla[pA].hitbox[i].y+gs->pantalla[pA].hitbox[i].alto,
                gs->pantalla[pA].hitbox[i].color, 2);

        for(i=0; i<gs->pantalla[pA].num_elementos; i++)
            if(gs->pantalla[pA].elementos[i].activo == true)
                al_draw_rectangle(gs->pantalla[pA].elementos[i].hitbox.x, gs->pantalla[pA].elementos[i].hitbox.y,
                    gs->pantalla[pA].elementos[i].hitbox.x+gs->pantalla[pA].elementos[i].hitbox.ancho,
                    gs->pantalla[pA].elementos[i].hitbox.y+gs->pantalla[pA].elementos[i].hitbox.alto,
                    gs->pantalla[pA].elementos[i].hitbox.color, 2);

        for(i=0; i<gs->pantalla[pA].num_entidades; i++)
            if(gs->pantalla[pA].entidades[i].activo == true)
            {
                al_draw_rectangle(gs->pantalla[pA].entidades[i].hitboxTitan.x, gs->pantalla[pA].entidades[i].hitboxTitan.y,
                    gs->pantalla[pA].entidades[i].hitboxTitan.x + gs->pantalla[pA].entidades[i].hitboxTitan.ancho,
                    gs->pantalla[pA].entidades[i].hitboxTitan.y + gs->pantalla[pA].entidades[i].hitboxTitan.alto, BLANCO, 2);

                al_draw_rectangle(gs->pantalla[pA].entidades[i].hitboxNuca.x, gs->pantalla[pA].entidades[i].hitboxNuca.y,
                    gs->pantalla[pA].entidades[i].hitboxNuca.x + gs->pantalla[pA].entidades[i].hitboxNuca.ancho,
                    gs->pantalla[pA].entidades[i].hitboxNuca.y + gs->pantalla[pA].entidades[i].hitboxNuca.alto,
                    al_map_rgb(255, 165, 0), 2);
            }
    }
}

