#include "commons.h"
#include <allegro5/allegro_font.h>

void levi_sprites(s_GameState *gs, s_Assets *assets);
void pantalla_0(s_Assets *assets, s_GameState *gs);
void dibujar_menu(s_GameState *gs, s_Assets *assets);
void jugando(s_Assets *assets, s_GameState *gs);
void pantalla_1(s_Assets *assets, s_GameState *gs);
void muestra_hitbox(s_GameState *gs, s_Assets *assets);
void titanes_sprites(s_GameState *gs, s_Assets *assets);
void dibujar_fondo(s_GameState *gs, s_Assets *assets);
void dibuja_gas(s_GameState *gs, s_Assets *assets);
void titan_hembra_sprites(s_GameState *gs, s_Assets *assets);

//====Funcion principal====//
void render_gameview(s_GameState *gs, s_Assets *assets)
{
    al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);

    al_clear_to_color(al_map_rgb(0,0,0));
    
    switch (gs->estadoPantalla)
    {
        case PANTALLA_MENU:
            dibujar_menu(gs, assets);
            break;
        case PANTALLA_JUGANDO:
            jugando(assets, gs); 
            break;
        case PANTALLA_GAME_OVER:
            break;
    }

    return;
}

void dibujar_menu(s_GameState *gs, s_Assets *assets)
{
    al_draw_bitmap(assets->assetsPantalla.fondo_menu, 0, 0, 0);
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
    titan_hembra_sprites(gs, assets);
    levi_sprites(gs, assets);
}

void levi_sprites(s_GameState *gs, s_Assets *assets)
{
    //Parametros al_draw_bitmap_region: spritesheet, frameX, frameY, ancho frame, alto frame, pos x, pos y, flags

    int leviX = round(gs->levi.x);
    int leviY = round(gs->levi.y);
    float leviXODM = gs->levi.hitbox.x;
    float leviYODM = gs->levi.hitbox.y + 40;
    int numFrameX = gs->levi.animacion.frameActual * LEVI_SS_ANCHO; //Calcula el frame a usar dependiendo del frame actual
    ALLEGRO_BITMAP* leviSS;

    if(gs->tutorialEjecutando)
        leviSS = assets->levi.levi_SS_SC;
    else 
    {
        if(gs->levi.vestuario)
            leviSS = assets->levi.levi_SS;
        else
            leviSS = assets->levi.levi_SS_SC;
    }

    dibuja_gas(gs,assets);

    if(gs->levi.agarrado)
    {
        al_draw_bitmap_region(leviSS, 0, LEVI_SS_ALTO*19, LEVI_SS_ANCHO, LEVI_SS_ALTO, leviX, leviY, 0);
        return;
    }

    if(!gs->levi.dash.activo)
    {
        if(gs->levi.animacion.rotarAnim == true)
            al_draw_bitmap_region(leviSS, numFrameX, gs->levi.animacion.fila_ss, LEVI_SS_ANCHO, LEVI_SS_ALTO, leviX, leviY, ALLEGRO_FLIP_HORIZONTAL);
        else
            al_draw_bitmap_region(leviSS, numFrameX, gs->levi.animacion.fila_ss, LEVI_SS_ANCHO, LEVI_SS_ALTO, leviX, leviY, 0);

        if(gs->levi.ODM.engancheActivo == true || gs->levi.ODM.activo == true)
        {
            al_draw_line(leviXODM, leviYODM, (gs->levi.ODM.puntoEngancheX - 10), gs->levi.ODM.puntoEngancheY, al_map_rgb(70, 70, 70), 1);
            al_draw_line((leviXODM + 20), leviYODM, (gs->levi.ODM.puntoEngancheX + 10), gs->levi.ODM.puntoEngancheY , al_map_rgb(70, 70, 70), 1);
        }
    }

    if(gs->levi.dash.animDashActiva)
        al_draw_rotated_bitmap(assets->assetsPantalla.dashSB[gs->levi.dash.animDash.frameActual], 0, 80, gs->levi.dash.x, gs->levi.dash.y, gs->levi.dash.angulo, 0);

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
        al_draw_scaled_bitmap(assets->assetsPantalla.gas, numFrameX, 0, gas_ss_ancho, gas_ss_ancho, gs->animaciones.gas[i].x - 5, gs->animaciones.gas[i].y + 48, gas_ss_ancho * 0.2f, 
            gas_ss_ancho * 0.2f, 0);
        al_draw_scaled_bitmap(assets->assetsPantalla.gas, numFrameX, 0, gas_ss_ancho, gas_ss_ancho, gs->animaciones.gas[i].x + 10, gs->animaciones.gas[i].y + 50, gas_ss_ancho * 0.2f, 
            gas_ss_ancho * 0.2f, 0);
    }

}

void titan_hembra_sprites(s_GameState *gs, s_Assets *assets)
{
    int titanX = round(gs->titanHembra.x), titanY = round(gs->titanHembra.y);
    ALLEGRO_BITMAP* sprite; 
    ALLEGRO_BITMAP* SS;

    if(gs->titanHembra.fase2Activa)
        SS = assets->titanes.titanHembraFase2;
    else
        SS = assets->titanes.titanHembraFase1;

    if(gs->titanHembra.activa == true)
    {
        sprite = al_create_sub_bitmap(SS, gs->titanHembra.animacion.frameActual*155, 
            gs->titanHembra.animacion.fila_ss*110, 155, 110);

        if(gs->titanHembra.animacion.rotarAnim)
            al_draw_scaled_bitmap(sprite,0, 0, 155, 110, titanX, titanY, 155*4.2, 110*4.2, ALLEGRO_FLIP_HORIZONTAL);
        else
            al_draw_scaled_bitmap(sprite,0, 0, 155, 110, titanX, titanY, 155*4.2, 110*4.2, 0);
    }

}

void titanes_sprites(s_GameState *gs, s_Assets *assets)
{
    int i, pA = gs->pantalla_actual;
    int titanX, titanY;
    ALLEGRO_BITMAP* sprite; 

    for(i = 0; i < gs->pantalla[pA].num_entidades; i++)
    {
        titanX = round(gs->pantalla[pA].entidades[i].x);
        titanY = round(gs->pantalla[pA].entidades[i].y);

        if(gs->pantalla[pA].entidades[i].vida > 0)
        {

            if(gs->pantalla[pA].entidades[i].tipo == 1)
            {
                sprite = al_create_sub_bitmap(assets->titanes.titan1, gs->pantalla[pA].entidades[i].animacion.frameActual*220, 
                    gs->pantalla[pA].entidades[i].animacion.fila_ss*165, 220, 165);

                if(gs->pantalla[pA].entidades[i].animacion.rotarAnim)
                    al_draw_scaled_bitmap(sprite,0, 0, 220, 165, titanX, titanY, 219*2.5, 164*2.5, ALLEGRO_FLIP_HORIZONTAL);
                else
                    al_draw_scaled_bitmap(sprite,0, 0, 220, 165, titanX, titanY, 219*2.5, 164*2.5, 0);

                al_destroy_bitmap(sprite);
            }
            else if(gs->pantalla[pA].entidades[i].tipo == 2)
            {
                sprite = al_create_sub_bitmap(assets->titanes.titan2, gs->pantalla[pA].entidades[i].animacion.frameActual*130, 
                        gs->pantalla[pA].entidades[i].animacion.fila_ss*130, 130, 130);

                if(gs->pantalla[pA].entidades[i].animacion.rotarAnim)
                    al_draw_scaled_bitmap(sprite,0, 0, 130, 130, titanX, titanY, 130*1.5, 130*1.5, ALLEGRO_FLIP_HORIZONTAL);
                else
                    al_draw_scaled_bitmap(sprite,0, 0, 130, 130, titanX, titanY, 130*1.5, 130*1.5, 0);

                al_destroy_bitmap(sprite);
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

    else if(strcmp("fondo_bosque", gs->pantalla[pA].fondo) == 0)
        al_draw_scaled_bitmap(assets->assetsPantalla.fondo_bosque, 0, 0,
            al_get_bitmap_width(assets->assetsPantalla.fondo_bosque), al_get_bitmap_height(assets->assetsPantalla.fondo_bosque), 0, 0,
            al_get_bitmap_width(assets->assetsPantalla.fondo_bosque)*1, al_get_bitmap_height(assets->assetsPantalla.fondo_bosque)*1, 0);

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

        else if(gs->pantalla[pA].elementos[i].tipo == 5 && gs->pantalla[pA].elementos[i].activo == true)
            al_draw_scaled_bitmap(assets->assetsPantalla.galonGas, 0, 0,
                al_get_bitmap_width(assets->assetsPantalla.galonGas), al_get_bitmap_height(assets->assetsPantalla.galonGas),
                gs->pantalla[pA].elementos[i].x, gs->pantalla[pA].elementos[i].y,
                al_get_bitmap_width(assets->assetsPantalla.galonGas) * 0.05, al_get_bitmap_height(assets->assetsPantalla.galonGas) * 0.05, 0);
    }

    for(int i = 0; i<nE; i++)
        if(gs->pantalla[pA].elementos[i].tipoCasa == 1)
            al_draw_scaled_bitmap(assets->assetsPantalla.casa1, 0, 0, al_get_bitmap_width(assets->assetsPantalla.casa1), al_get_bitmap_height(assets->assetsPantalla.casa1),
                gs->pantalla[pA].elementos[i].x, gs->pantalla[pA].elementos[i].y, al_get_bitmap_width(assets->assetsPantalla.casa1) * 0.3f,
                al_get_bitmap_height(assets->assetsPantalla.casa1) * 0.3f, 0);

    for(int i =0;i<nE;i++)
        if(gs->pantalla[pA].elementos[i].tipoCasa == 2)
            al_draw_scaled_bitmap(assets->assetsPantalla.casa2, 0, 0, al_get_bitmap_width(assets->assetsPantalla.casa2), al_get_bitmap_height(assets->assetsPantalla.casa2),
                gs->pantalla[pA].elementos[i].x, gs->pantalla[pA].elementos[i].y, al_get_bitmap_width(assets->assetsPantalla.casa2) * 0.5f,
                al_get_bitmap_height(assets->assetsPantalla.casa2) * 0.5f, 0);

    for(int i =0;i<nE;i++)
        if(gs->pantalla[pA].elementos[i].tipoCasa == 3)
            al_draw_scaled_bitmap(assets->assetsPantalla.casa3, 0, 0, al_get_bitmap_width(assets->assetsPantalla.casa3), al_get_bitmap_height(assets->assetsPantalla.casa3),
                gs->pantalla[pA].elementos[i].x, gs->pantalla[pA].elementos[i].y, al_get_bitmap_width(assets->assetsPantalla.casa3) * 0.4f,
                al_get_bitmap_height(assets->assetsPantalla.casa3) * 0.4f, 0);

    for(int i =0;i<nE;i++)
        if(gs->pantalla[pA].elementos[i].tipoCasa == 4)
            al_draw_scaled_bitmap(assets->assetsPantalla.puestoComida, 0, 0, al_get_bitmap_width(assets->assetsPantalla.puestoComida), al_get_bitmap_height(assets->assetsPantalla.puestoComida),
                gs->pantalla[pA].elementos[i].x, gs->pantalla[pA].elementos[i].y, al_get_bitmap_width(assets->assetsPantalla.puestoComida) * 0.2f,
                al_get_bitmap_height(assets->assetsPantalla.puestoComida) * 0.2f, 0);

    for(int i=0; i<5; i++)
        if(gs->variables.grietas[i].x != 0)
            al_draw_bitmap(assets->assetsPantalla.grietaODM, gs->variables.grietas[i].x - 11, gs->variables.grietas[i].y -4, 0);

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

        if(gs->levi.dash.activo || gs->levi.estadoLevi == SALIDA_DASH)
            al_draw_rectangle(gs->levi.dash.hitboxDash.x, gs->levi.dash.hitboxDash.y,
                gs->levi.dash.hitboxDash.x+gs->levi.dash.hitboxDash.ancho, gs->levi.dash.hitboxDash.y+gs->levi.dash.hitboxDash.alto,
                al_map_rgb(0, 0, 255), 2);

        al_draw_rectangle(gs->levi.hitboxODM.x, gs->levi.hitboxODM.y,
            gs->levi.hitboxODM.x+gs->levi.hitboxODM.ancho, gs->levi.hitboxODM.y+gs->levi.hitboxODM.alto,
            al_map_rgb(255, 0, 0), 2);
            
        al_draw_rectangle(gs->levi.parryHB.x, gs->levi.parryHB.y,
            gs->levi.parryHB.x+gs->levi.parryHB.ancho, gs->levi.parryHB.y+gs->levi.parryHB.alto, al_map_rgb(126, 34, 206), 2);

        al_draw_rectangle(gs->titanHembra.hitboxAtaque1.x, gs->titanHembra.hitboxAtaque1.y,
            gs->titanHembra.hitboxAtaque1.x+gs->titanHembra.hitboxAtaque1.ancho, gs->titanHembra.hitboxAtaque1.y+gs->titanHembra.hitboxAtaque1.alto, 
            al_map_rgb(255, 20, 147), 2);

        al_draw_rectangle(gs->titanHembra.hitboxAtaque2.x, gs->titanHembra.hitboxAtaque2.y,
            gs->titanHembra.hitboxAtaque2.x+gs->titanHembra.hitboxAtaque2.ancho, gs->titanHembra.hitboxAtaque2.y+gs->titanHembra.hitboxAtaque2.alto, 
            al_map_rgb(255, 20, 147), 2);

        for(i=0; i<gs->pantalla[pA].num_hitbox; i++)
            al_draw_rectangle(gs->pantalla[pA].hitbox[i].x, gs->pantalla[pA].hitbox[i].y,
                gs->pantalla[pA].hitbox[i].x+gs->pantalla[pA].hitbox[i].ancho, gs->pantalla[pA].hitbox[i].y+gs->pantalla[pA].hitbox[i].alto,
                gs->pantalla[pA].hitbox[i].color, 2);

        for(i=0; i<gs->pantalla[pA].num_elementos; i++)
            if(gs->pantalla[pA].elementos[i].activo == true)
            {
                al_draw_rectangle(gs->pantalla[pA].elementos[i].hitbox.x, gs->pantalla[pA].elementos[i].hitbox.y,
                    gs->pantalla[pA].elementos[i].hitbox.x+gs->pantalla[pA].elementos[i].hitbox.ancho,
                    gs->pantalla[pA].elementos[i].hitbox.y+gs->pantalla[pA].elementos[i].hitbox.alto,
                    gs->pantalla[pA].elementos[i].hitbox.color, 2);

                al_draw_rectangle(gs->pantalla[pA].elementos[i].hitbox2.x, gs->pantalla[pA].elementos[i].hitbox2.y,
                    gs->pantalla[pA].elementos[i].hitbox2.x+gs->pantalla[pA].elementos[i].hitbox2.ancho,
                    gs->pantalla[pA].elementos[i].hitbox2.y+gs->pantalla[pA].elementos[i].hitbox2.alto,
                    gs->pantalla[pA].elementos[i].hitbox2.color, 2);
            }

        for(i=0; i<gs->pantalla[pA].num_entidades; i++)
            if(gs->pantalla[pA].entidades[i].vida > 0)
            {
            al_draw_rectangle(gs->pantalla[pA].entidades[i].hitboxTitan.x, gs->pantalla[pA].entidades[i].hitboxTitan.y,
                gs->pantalla[pA].entidades[i].hitboxTitan.x + gs->pantalla[pA].entidades[i].hitboxTitan.ancho,
                gs->pantalla[pA].entidades[i].hitboxTitan.y + gs->pantalla[pA].entidades[i].hitboxTitan.alto, BLANCO, 2);

            al_draw_rectangle(gs->pantalla[pA].entidades[i].hitboxNuca.x, gs->pantalla[pA].entidades[i].hitboxNuca.y,
                gs->pantalla[pA].entidades[i].hitboxNuca.x + gs->pantalla[pA].entidades[i].hitboxNuca.ancho,
                gs->pantalla[pA].entidades[i].hitboxNuca.y + gs->pantalla[pA].entidades[i].hitboxNuca.alto,
                al_map_rgb(255, 165, 0), 2);

            al_draw_rectangle(gs->pantalla[pA].entidades[i].hitboxAtaqueBasico.x, gs->pantalla[pA].entidades[i].hitboxAtaqueBasico.y,
                gs->pantalla[pA].entidades[i].hitboxAtaqueBasico.x + gs->pantalla[pA].entidades[i].hitboxAtaqueBasico.ancho,
                gs->pantalla[pA].entidades[i].hitboxAtaqueBasico.y + gs->pantalla[pA].entidades[i].hitboxAtaqueBasico.alto, al_map_rgb(255, 0, 0), 2);

            al_draw_rectangle(gs->pantalla[pA].entidades[i].agarre.manoHB.x, gs->pantalla[pA].entidades[i].agarre.manoHB.y,
                gs->pantalla[pA].entidades[i].agarre.manoHB.x + gs->pantalla[pA].entidades[i].agarre.manoHB.ancho,
                gs->pantalla[pA].entidades[i].agarre.manoHB.y + gs->pantalla[pA].entidades[i].agarre.manoHB.alto, gs->pantalla[pA].entidades[i].agarre.manoHB.color, 2);

            al_draw_rectangle(gs->pantalla[pA].entidades[i].mordidaHB.x, gs->pantalla[pA].entidades[i].mordidaHB.y,
                gs->pantalla[pA].entidades[i].mordidaHB.x + gs->pantalla[pA].entidades[i].mordidaHB.ancho,
                gs->pantalla[pA].entidades[i].mordidaHB.y + gs->pantalla[pA].entidades[i].mordidaHB.alto, gs->pantalla[pA].entidades[i].mordidaHB.color, 2);
            }

            al_draw_rectangle(gs->titanHembra.hitbox.x, gs->titanHembra.hitbox.y, gs->titanHembra.hitbox.x + gs->titanHembra.hitbox.ancho, gs->titanHembra.hitbox.y + gs->titanHembra.hitbox.alto, BLANCO, 2);
    }
}


