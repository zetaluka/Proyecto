#include "commons.h"

void dibujar_texto_borde(s_GameState *gs, ALLEGRO_FONT *fuente, ALLEGRO_COLOR colorTexto, ALLEGRO_COLOR colorBorde, float x, float y, int flags, const char *texto, float grosor);
void transicion(s_GameState *gs, s_Assets *assets);
void transicion2(s_GameState *gs, s_Assets *assets);
void jugando_ui(s_GameState *gs, s_Assets *assets);
void menu(s_GameState *gs, s_Assets *assets);
void opciones(s_GameState *gs, s_Assets *assets);
void muestra_puntuaciones(s_GameState *gs, s_Assets *assets);
void pausa(s_GameState *gs, s_Assets *assets);
void game_over(s_GameState *gs, s_Assets *assets);
void dibuja_controles(s_GameState *gs, s_Assets *assets);

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
        case PANTALLA_GAME_OVER:
            game_over(gs, assets);
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

    if(gs->menu.estadoMenu == MAIN)
    {
        if(gs->menu.contMenu == 0)
        {
            strcpy(texto, "JUGAR");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, 10, 175, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 205, 180, ALLEGRO_ALIGN_LEFT, texto, 3);
        }
        else
        {
            strcpy(texto, "JUGAR");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada1, 0, 0, 1450, 210, 10, 175, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 205, 180, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        if(gs->menu.contMenu == 1)
        {
            strcpy(texto, "RANKING");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, 10, 250, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 195, 255, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        else
        {
            strcpy(texto, "RANKING");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada1, 0, 0, 1450, 210, 10, 250, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 195, 255, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        if(gs->menu.contMenu == 2)
        {
            strcpy(texto, "OPCIONES");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, 10, 325, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 195, 330, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        else
        {
            strcpy(texto, "OPCIONES");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada1, 0, 0, 1450, 210, 10, 325, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 195, 330, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        if(gs->menu.contMenu == 3)
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

    else if(gs->menu.estadoMenu == JUGAR)
    {
        if(gs->menu.contMenu == 0)
        {
            strcpy(texto, "NIVEL UNO");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, 10, 250, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 195, 255, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        else
        {
            strcpy(texto, "NIVEL UNO");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada1, 0, 0, 1450, 210, 10, 250, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 195, 255, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        if(gs->menu.contMenu == 1)
        {
            strcpy(texto, "NIVEL DOS");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, 10, 325, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 195, 330, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        else
        {
            strcpy(texto, "NIVEL DOS");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada1, 0, 0, 1450, 210, 10, 325, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 195, 330, ALLEGRO_ALIGN_LEFT, texto, 3);
        }
    }

    else if(gs->menu.estadoMenu == OPCIONES)
    {
        if(gs->menu.contMenu == 0)
        {
            if(gs->pantallaCompleta == true)
                strcpy(texto, "PANTALLA COMPLETA: ON");
            else
                strcpy(texto, "PANTALLA COMPLETA: OFF");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, 10, 325, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 195, 330, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        else
        {
            if(gs->pantallaCompleta == true)
                strcpy(texto, "PANTALLA COMPLETA: ON");
            else
                strcpy(texto, "PANTALLA COMPLETA: OFF");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada1, 0, 0, 1450, 210, 10, 325, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 195, 330, ALLEGRO_ALIGN_LEFT, texto, 3);
        }
    }

    else if(gs->menu.estadoMenu == RANKING)
        muestra_puntuaciones(gs, assets);

}

void muestra_puntuaciones(s_GameState *gs, s_Assets *assets)
{
    int cantidad = carga_puntuacion(gs), i;
    char texto[50];

    for(i=0;i<cantidad;i++)
    {
        sprintf(texto, "%s: %d", gs->puntuaciones[i].nombre, gs->puntuaciones[i].puntuacion);
        dibujar_texto_borde(gs, assets->minimalistTemplateFont50, BLANCO, al_map_rgb(0, 0, 0), 30, 50 + 50*i, ALLEGRO_ALIGN_LEFT, texto, 3);
    }
}

void pausa(s_GameState *gs, s_Assets *assets)
{
    char texto[20];

    al_draw_filled_rectangle(0, 0, gs->variables.screenX, gs->variables.screenY, al_map_rgba(0, 0, 0, 180));

    if(gs->menuPausa.estadoMenu == MAIN)
    {
        if(gs->menuPausa.contMenu == 0)
        {
            strcpy(texto, "REANUDAR");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, al_get_text_width(assets->shingekiFont30, texto) + 100, 
                250, 1450*0.25, 210*0.25, ALLEGRO_FLIP_HORIZONTAL);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 75, 250, ALLEGRO_ALIGN_LEFT, texto, 3);
        }
        else
        {
            strcpy(texto, "REANUDAR");
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 75, 250, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        if(gs->menuPausa.contMenu == 1)
        {
            strcpy(texto, "REINTENTAR");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, al_get_text_width(assets->shingekiFont30, texto) + 100, 
                300, 1450*0.25, 210*0.25, ALLEGRO_FLIP_HORIZONTAL);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 75, 300, ALLEGRO_ALIGN_LEFT, texto, 3);
        }
        else
        {
            strcpy(texto, "REINTENTAR");
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 75, 300, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        if(gs->menuPausa.contMenu == 2)
        {
            strcpy(texto, "CONTROLES");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, al_get_text_width(assets->shingekiFont30, texto) + 100, 
                350, 1450*0.25, 210*0.25, ALLEGRO_FLIP_HORIZONTAL);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 75, 350, ALLEGRO_ALIGN_LEFT, texto, 3);
        }
        else
        {
            strcpy(texto, "CONTROLES");
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 75, 350, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        if(gs->menuPausa.contMenu == 3)
        {
            strcpy(texto, "OPCIONES");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, al_get_text_width(assets->shingekiFont30, texto) + 100, 
                400, 1450*0.25, 210*0.25, ALLEGRO_FLIP_HORIZONTAL);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 75, 400, ALLEGRO_ALIGN_LEFT, texto, 3);
        }
        else
        {
            strcpy(texto, "OPCIONES");
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 75, 400, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        if(gs->menuPausa.contMenu == 4)
        {
            strcpy(texto, "VOLVER AL MENU PRINCIPAL");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, al_get_text_width(assets->shingekiFont30, texto) + 100, 
                450, 1450*0.25, 210*0.25, ALLEGRO_FLIP_HORIZONTAL);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 75, 450, ALLEGRO_ALIGN_LEFT, texto, 3);
        }
        else
        {
            strcpy(texto, "VOLVER AL MENU PRINCIPAL");
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 75, 450, ALLEGRO_ALIGN_LEFT, texto, 3);
        }
    }

    else if(gs->menuPausa.estadoMenu == CONTROLES)
    {
        strcpy(texto, "VOLVER");
        al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, al_get_text_width(assets->shingekiFont30, texto) + 100, 
            300, 1450*0.25, 210*0.25, ALLEGRO_FLIP_HORIZONTAL);
        dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 75, 300, ALLEGRO_ALIGN_LEFT, texto, 3);

        dibuja_controles(gs, assets);
    }

    else if(gs->menuPausa.estadoMenu == OPCIONES)
    {
        if(gs->menuPausa.contMenu == 0)
        {
            if(gs->pantallaCompleta == true)
                strcpy(texto, "PANTALLA COMPLETA: ON");
            else
                strcpy(texto, "PANTALLA COMPLETA: OFF");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, al_get_text_width(assets->shingekiFont30, texto) + 100, 
                300, 1450*0.25, 210*0.25, ALLEGRO_FLIP_HORIZONTAL);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 75, 300, ALLEGRO_ALIGN_LEFT, texto, 3);
        }
        else
        {
            if(gs->pantallaCompleta == true)
                strcpy(texto, "PANTALLA COMPLETA: ON");
            else
                strcpy(texto, "PANTALLA COMPLETA: OFF");
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 75, 300, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        if(gs->menuPausa.contMenu == 1)
        {
            strcpy(texto, "VOLVER");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, al_get_text_width(assets->shingekiFont30, texto) + 100, 
                350, 1450*0.25, 210*0.25, ALLEGRO_FLIP_HORIZONTAL);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 75, 350, ALLEGRO_ALIGN_LEFT, texto, 3);
        }
        else
        {
            strcpy(texto, "VOLVER");
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 75, 350, ALLEGRO_ALIGN_LEFT, texto, 3);
        }
    }
}

void dibuja_controles(s_GameState *gs, s_Assets *assets)
{
    dibujar_texto_borde(gs, assets->minimalistTemplateFont50, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 800, 50, 0, "A: MOVER HACIA LA IZQUIERDA", 3);
    dibujar_texto_borde(gs, assets->minimalistTemplateFont50, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 800, 100, 0, "D: MOVER HACIA LA DERECHA", 3);
    dibujar_texto_borde(gs, assets->minimalistTemplateFont50, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 800, 150, 0, "LSHIFT: CORRER", 3);
    dibujar_texto_borde(gs, assets->minimalistTemplateFont50, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 800, 200, 0, "SPACE: SALTAR", 3);
    dibujar_texto_borde(gs, assets->minimalistTemplateFont50, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 800, 250, 0, "CLICK IZQ: ATAQUE BASICO", 3);
    dibujar_texto_borde(gs, assets->minimalistTemplateFont50, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 800, 300, 0, "CLICK DER: EQUIPO DE MANIOBRAS / ODM", 3);
    dibujar_texto_borde(gs, assets->minimalistTemplateFont50, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 800, 350, 0, "F: DASH", 3);
    dibujar_texto_borde(gs, assets->minimalistTemplateFont50, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 800, 400, 0, "1 (w/ODM): HABILIDAD 1", 3);
    dibujar_texto_borde(gs, assets->minimalistTemplateFont50, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 800, 450, 0, "2 (w/ODM): HABILIDAD 2", 3);
    dibujar_texto_borde(gs, assets->minimalistTemplateFont50, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 800, 500, 0, "3 : RECARGAR GAS", 3);


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

    al_draw_scaled_bitmap(assets->assetsPantalla.galonGas, 0, 0, al_get_bitmap_width(assets->assetsPantalla.galonGas), al_get_bitmap_height(assets->assetsPantalla.galonGas), 
        10 , 200 , al_get_bitmap_width(assets->assetsPantalla.galonGas) * 0.04, al_get_bitmap_height(assets->assetsPantalla.galonGas) * 0.04, 0);

    sprintf(texto, "x%d", gs->levi.inventario.gasODM);
    dibujar_texto_borde(gs, assets->minimalistTemplateFont50, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 30, 215, ALLEGRO_ALIGN_LEFT, texto, 1.5);

    //============================//

    al_draw_scaled_bitmap(assets->assetsPantalla.marcoVida, 0, 0, al_get_bitmap_width(assets->assetsPantalla.marcoVida), al_get_bitmap_height(assets->assetsPantalla.marcoVida),
        10, 20, al_get_bitmap_width(assets->assetsPantalla.marcoVida) * 0.1f, al_get_bitmap_height(assets->assetsPantalla.marcoVida) * 0.1f, 0);

    if(gs->levi.vida > 0)
        al_draw_line(20, 33, gs->levi.vida * 5, 33, al_map_rgb(200, 35, 35), 10);

    al_draw_scaled_bitmap(assets->assetsPantalla.galonGas, 0, 0, al_get_bitmap_width(assets->assetsPantalla.galonGas), al_get_bitmap_height(assets->assetsPantalla.galonGas),
        0, 0, al_get_bitmap_width(assets->assetsPantalla.galonGas) * 0.15f, al_get_bitmap_height(assets->assetsPantalla.galonGas) * 0.15f, ALLEGRO_FLIP_HORIZONTAL);

    if(gs->levi.gasRestante > 0)
        al_draw_line(20, 67, (gs->levi.gasRestante * 0.167) + 20, 67, al_map_rgb(120, 128, 135), 7);

    if(gs->levi.agarrado)
    {
        sprintf(texto, "PRESIONA R: %d/5", gs->levi.contSoltarse);
        dibujar_texto_borde(gs, assets->minimalistTemplateFont50, al_map_rgb(230, 225, 210), al_map_rgb(0, 0, 0), SCREEN_X/2, 50, 
            ALLEGRO_ALIGN_CENTER, texto, 2); 
    }



    //==============================//

    if(gs->animaciones.transicion2.activo == true)
        transicion2(gs, assets);

    else if(gs->animaciones.transicion.activo == true)
        transicion(gs, assets);

    if(gs->pausa)
        pausa(gs, assets);

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

void transicion2(s_GameState *gs, s_Assets *assets)
{
    int frameX = gs->animaciones.transicion2.frameActual*1280;

    al_draw_bitmap_region(assets->assetsPantalla.transicion2, frameX, 0, 1280, 720, 0, 0, 0);
}

void game_over(s_GameState *gs, s_Assets *assets)
{
    char texto[20];

    al_draw_bitmap(assets->assetsPantalla.imgGameOver, 0, 0, 0);

    if(gs->levi.vida <= 0)
    {
        if(gs->contOpcionesGO == 0)
        {
            strcpy(texto, "REINTENTAR");
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), SCREEN_X/2, 500, ALLEGRO_ALIGN_CENTER, texto, 3);
        }

        else
        {
            strcpy(texto, "REINTENTAR");
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 500, ALLEGRO_ALIGN_CENTER, texto, 3);
        }

        if(gs->contOpcionesGO == 1)
        {
            strcpy(texto, "VOLVER AL MENU PRINCIPAL");
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), SCREEN_X/2, 575, ALLEGRO_ALIGN_CENTER, texto, 3);
        }

        else
        {
            strcpy(texto, "VOLVER AL MENU PRINCIPAL");
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 575, ALLEGRO_ALIGN_CENTER, texto, 3);
        }
    }
}