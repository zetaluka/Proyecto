#include "commons.h"

void dibujar_texto_borde(s_GameState *gs, ALLEGRO_FONT *fuente, ALLEGRO_COLOR colorTexto, ALLEGRO_COLOR colorBorde, float x, float y, int flags, const char *texto, float grosor);
void transicion(s_GameState *gs, s_Assets *assets);
void jugando_ui(s_GameState *gs, s_Assets *assets);
void menu(s_GameState *gs, s_Assets *assets);
void opciones(s_GameState *gs, s_Assets *assets);

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

    switch(gs->estadoPantalla)
    {
        case PANTALLA_MENU:
            menu(gs, assets);
            break;
        case PANTALLA_JUGANDO:
            jugando_ui(gs, assets);
            break;

    }

    al_flip_display(); 
}

void menu(s_GameState *gs, s_Assets *assets)
{
    opciones(gs, assets);

}

void opciones(s_GameState *gs, s_Assets *assets)
{
   //Parametros al_draw_scaled_bitmap: Spritesheet, frameX, frameY, ancho, alto, x , y , (ancho destino, alto destino (escalado)), flags

    char texto[20];

    if(gs->estadoMenu == MAIN)
    {
        if(gs->contMenu == 0)
        {
            strcpy(texto, "JUGAR");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, 10, 175, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 205, 180, ALLEGRO_ALIGN_LEFT, texto, 3);
            strcpy(gs->opcion, "jugar");
        }
        else
        {
            strcpy(texto, "JUGAR");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada1, 0, 0, 1450, 210, 10, 175, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 205, 180, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        if(gs->contMenu == 1)
        {
            strcpy(texto, "RANKING");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, 10, 250, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 195, 255, ALLEGRO_ALIGN_LEFT, texto, 3);
            strcpy(gs->opcion, "ranking");
        }

        else
        {
            strcpy(texto, "RANKING");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada1, 0, 0, 1450, 210, 10, 250, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 195, 255, ALLEGRO_ALIGN_LEFT, texto, 3);
            strcpy(gs->opcion, "ranking");
        }

        if(gs->contMenu == 2)
        {
            strcpy(texto, "OPCIONES");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, 10, 325, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 195, 330, ALLEGRO_ALIGN_LEFT, texto, 3);
            strcpy(gs->opcion, "opciones");
        }

        else
        {
            strcpy(texto, "OPCIONES");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada1, 0, 0, 1450, 210, 10, 325, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 195, 330, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        if(gs->contMenu == 3)
        {
            strcpy(texto, "SALIR");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, 10, 400, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 215, 405, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        else
        {
            strcpy(texto, "SALIR");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada1, 0, 0, 1450, 210, 10, 400, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 215, 405, ALLEGRO_ALIGN_LEFT, texto, 3);
        }
    }

    else if(gs->estadoMenu == JUGAR)
    {
        if(gs->contMenu == 0)
        {
            strcpy(texto, "NIVEL UNO");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, 10, 250, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 195, 255, ALLEGRO_ALIGN_LEFT, texto, 3);
            strcpy(gs->opcion, "ranking");
        }

        else
        {
            strcpy(texto, "NIVEL UNO");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada1, 0, 0, 1450, 210, 10, 250, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 195, 255, ALLEGRO_ALIGN_LEFT, texto, 3);
            strcpy(gs->opcion, "ranking");
        }

        if(gs->contMenu == 1)
        {
            strcpy(texto, "NIVEL DOS");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, 10, 325, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 195, 330, ALLEGRO_ALIGN_LEFT, texto, 3);
            strcpy(gs->opcion, "opciones");
        }

        else
        {
            strcpy(texto, "NIVEL DOS");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada1, 0, 0, 1450, 210, 10, 325, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 195, 330, ALLEGRO_ALIGN_LEFT, texto, 3);
        }
    }

}

void jugando_ui(s_GameState *gs, s_Assets *assets)
{
    char texto[20];

    sprintf(texto, "%02d: %02d", gs->tiempoJugado.minutos, gs->tiempoJugado.segundos);
    al_draw_text(assets->minimalistTemplateFont50, al_map_rgb(220, 220, 220), (SCREEN_X-100), 10, 0, texto); //arreglar...
    al_draw_text(assets->shingekiFont30,al_map_rgb(220, 220, 220), SCREEN_X/2, 0, ALLEGRO_ALIGN_CENTER, "CodeNoKyojin");

    //============Escudos=============//

    al_draw_scaled_bitmap(assets->assetsPantalla.escudoLegion, 0, 0, al_get_bitmap_width(assets->assetsPantalla.escudoLegion), al_get_bitmap_height(assets->assetsPantalla.escudoLegion), (SCREEN_X - 100) , 55 ,
        al_get_bitmap_width(assets->assetsPantalla.escudoLegion), al_get_bitmap_height(assets->assetsPantalla.escudoLegion), 0);

    sprintf(texto, "x%d", gs->levi.inventario.escudos);
    dibujar_texto_borde(gs, assets->minimalistTemplateFont50, al_map_rgb(255,255,255), al_map_rgb(0,0,0), (SCREEN_X -30), 55, ALLEGRO_ALIGN_RIGHT, texto, 1.5);

    //============================//

    al_draw_scaled_bitmap(assets->assetsPantalla.marcoVida, 0, 0, al_get_bitmap_width(assets->assetsPantalla.marcoVida), al_get_bitmap_height(assets->assetsPantalla.marcoVida),
        10, 20, al_get_bitmap_width(assets->assetsPantalla.marcoVida) * 0.1f, al_get_bitmap_height(assets->assetsPantalla.marcoVida) * 0.1f, 0);

    if(gs->levi.vida > 0)
        al_draw_line(20, 33, gs->levi.vida * 15, 33, al_map_rgb(200, 35, 35), 10);

    al_draw_scaled_bitmap(assets->assetsPantalla.galonGas, 0, 0, al_get_bitmap_width(assets->assetsPantalla.galonGas), al_get_bitmap_height(assets->assetsPantalla.galonGas),
        0, 0, al_get_bitmap_width(assets->assetsPantalla.galonGas) * 0.15f, al_get_bitmap_height(assets->assetsPantalla.galonGas) * 0.15f, ALLEGRO_FLIP_HORIZONTAL);

    if(gs->levi.gasRestante > 0)
        al_draw_line(20, 67, (gs->levi.gasRestante * 0.167) + 20, 67, al_map_rgb(120, 128, 135), 7);

    if(gs->animaciones.transicion.activo == true)
        transicion(gs, assets);

}

void dibujar_texto_borde(s_GameState *gs, ALLEGRO_FONT *fuente, ALLEGRO_COLOR colorTexto, ALLEGRO_COLOR colorBorde, float x, float y, int flags, const char *texto, float grosor) 
{
    al_draw_text(fuente, colorBorde, (x - grosor), y, flags, texto);
    al_draw_text(fuente, colorBorde, (x + grosor), y, flags, texto);
    al_draw_text(fuente, colorBorde, x, (y - grosor), flags, texto);
    al_draw_text(fuente, colorBorde, x, (y + grosor), flags, texto);

    al_draw_text(fuente, colorTexto, x, y, flags, texto);

}

void transicion(s_GameState *gs, s_Assets *assets)
{
    int frameX = (gs->animaciones.transicion.frameActual % 5) * 1280;
    int frameY = (gs->animaciones.transicion.frameActual / 5) * 720;

    al_draw_bitmap_region(assets->assetsPantalla.transicion, frameX, frameY, 1280, 720, 0, 0, 0);
}