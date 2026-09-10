#include "commons.h"

void dibujar_texto_borde(s_GameState *gs, ALLEGRO_FONT *fuente, ALLEGRO_COLOR colorTexto, ALLEGRO_COLOR colorBorde, float x, float y, int flags, const char *texto, float grosor);
void transicion(s_GameState *gs, s_Assets *assets);
void transicion2(s_GameState *gs, s_Assets *assets);
void efectoSangre(s_GameState *gs, s_Assets *assets);
void jugando_ui(s_GameState *gs, s_Assets *assets);
void menu(s_GameState *gs, s_Assets *assets);
void muestra_puntuaciones(s_GameState *gs, s_Assets *assets, const char *archivo);
void pausa(s_GameState *gs, s_Assets *assets);
void game_over(s_GameState *gs, s_Assets *assets);
void dibuja_controles(s_GameState *gs, s_Assets *assets);
void tutorial(s_GameState *gs, s_Assets *assets);
void puntuacion(s_GameState *gs, s_Assets *assets);
void ui_elementos(s_GameState *gs, s_Assets *assets);

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
            if(gs->menu.contDif == 0)
                strcpy(texto, "DIFICULTAD <NORMAL>");
            else if(gs->menu.contDif == 1)
                strcpy(texto, "DIFICULTAD <DIFICIL>");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, 10, 175, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 140, 180, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        else
        {
            if(gs->menu.contDif == 0)
                strcpy(texto, "DIFICULTAD <NORMAL>");
            else if(gs->menu.contDif == 1)
                strcpy(texto, "DIFICULTAD <DIFICIL>");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada1, 0, 0, 1450, 210, 10, 175, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 140, 180, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        if(gs->menu.contMenu == 1)
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

        if(gs->menu.contMenu == 2)
        {
            strcpy(texto, "MODO OLEADA");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, 10, 325, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 180, 330, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        else
        {
            strcpy(texto, "MODO OLEADA");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada1, 0, 0, 1450, 210, 10, 325, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 180, 330, ALLEGRO_ALIGN_LEFT, texto, 3);
        }


        if(gs->menu.contMenu == 3)
        {
            strcpy(texto, "VS. TITAN HEMBRA");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, 10, 400, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 135, 405, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        else
        {
            strcpy(texto, "VS. TITAN HEMBRA");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada1, 0, 0, 1450, 210, 10, 400, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 135, 405, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        if(gs->menu.contMenu == 4)
        {
            strcpy(texto, "TUTORIAL");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, 10, 475, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 195, 480, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        else
        {
            strcpy(texto, "TUTORIAL");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada1, 0, 0, 1450, 210, 10, 475, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 195, 480, ALLEGRO_ALIGN_LEFT, texto, 3);
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
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 140, 330, ALLEGRO_ALIGN_LEFT, texto, 3);
        }
        else 
        {
            if(gs->pantallaCompleta == true)
                strcpy(texto, "PANTALLA COMPLETA: ON");
            else
                strcpy(texto, "PANTALLA COMPLETA: OFF");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada1, 0, 0, 1450, 210, 10, 325, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 140, 330, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        if(gs->menu.contMenu == 1)
        {
            if(gs->levi.vestuario == true)
                strcpy(texto, "VESTUARIO: CON CAPA");
            else
                strcpy(texto, "VESTUARIO: SIN CAPA");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, 10, 400, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 140, 405, ALLEGRO_ALIGN_LEFT, texto, 3);
        }
        else
        {
            if(gs->levi.vestuario == true)
                strcpy(texto, "VESTUARIO: CON CAPA");
            else
                strcpy(texto, "VESTUARIO: SIN CAPA");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada1, 0, 0, 1450, 210, 10, 400, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 140, 405, ALLEGRO_ALIGN_LEFT, texto, 3);
        }
    }

    else if(gs->menu.estadoMenu == RANKING)
    {
        if(gs->menu.contMenu == 0)
        {
            strcpy(texto, "R. NIVEL UNO");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, 10, 250, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 145, 255, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        else
        {
            strcpy(texto, "R. NIVEL UNO");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada1, 0, 0, 1450, 210, 10, 250, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 145, 255, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        if(gs->menu.contMenu == 1)
        {
            strcpy(texto, "R. MODO OLEADA");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, 10, 325, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 145, 330, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        else
        {
            strcpy(texto, "R. MODO OLEADA");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada1, 0, 0, 1450, 210, 10, 325, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 145, 330, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        if(gs->menu.contMenu == 2)
        {
            strcpy(texto, "R. VS TITAN HEMBRA");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, 10, 400, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 145, 405, ALLEGRO_ALIGN_LEFT, texto, 3);
        }

        else
        {
            strcpy(texto, "R. VS TITAN HEMBRA");
            al_draw_scaled_bitmap(assets->assetsPantalla.espada1, 0, 0, 1450, 210, 10, 400, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 145, 405, ALLEGRO_ALIGN_LEFT, texto, 3);
        }
    }

    else if(gs->menu.estadoMenu == RANKINGNIVEL1)
        muestra_puntuaciones(gs, assets, "rankingNivel1.txt");

    else if(gs->menu.estadoMenu == RANKINGOLEADA)
        muestra_puntuaciones(gs, assets, "rankingModoOleada.txt");

    else if(gs->menu.estadoMenu == RANKINGVSTITANHEMBRA)
        muestra_puntuaciones(gs, assets, "rankingVsTitanHembra.txt");

    else if(gs->menu.estadoMenu == OLEADA)
    {
        switch(gs->menu.contMapa)
        {
            case 0:
                strcpy(texto, "MAPA <UNO>");  
                break;
            case 1:
                strcpy(texto, "MAPA <DOS>");  
                break;
            case 2:
                strcpy(texto, "MAPA <TRES>");  
                break;
        }

        al_draw_scaled_bitmap(assets->assetsPantalla.espada2, 0, 0, 1450, 210, 10, 325, 1450*0.25, 210*0.25, ALLEGRO_ALIGN_LEFT);
        dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), 195, 330, ALLEGRO_ALIGN_LEFT, texto, 3);

        al_draw_scaled_bitmap(assets->assetsPantalla.minMapas, 1500*gs->menu.contMapa, 0, 1500, 900, SCREEN_X - 740, (SCREEN_Y - 200)/2 - 100, 1500*0.4, 900*0.4, 0);
    }

    if(gs->variables.ingresandoNombre)
    {
        strcpy(texto, "INGRESA TU NOMBRE:");
        dibujar_texto_borde(gs, assets->biggestThingsFont18, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 30, 125, ALLEGRO_ALIGN_LEFT, texto, 3);
        dibujar_texto_borde(gs, assets->biggestThingsFont18, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 255, 125, ALLEGRO_ALIGN_LEFT, gs->variables.nombreTemp, 3);
    }

}

void muestra_puntuaciones(s_GameState *gs, s_Assets *assets, const char *archivo)
{
    int cantidad, i;
    char texto[50];

    if(gs->menu.estadoMenu == RANKINGVSTITANHEMBRA)
        cantidad = carga_puntuacionTH(gs, archivo);
    else
        cantidad = carga_puntuacion(gs, archivo);

    for(i=0;i<cantidad;i++)
    {
        if(gs->menu.estadoMenu == RANKINGVSTITANHEMBRA)
        {
            sprintf(texto, "%s: %02d:%02d %s", gs->puntuacionesTH[i].nombre, gs->puntuacionesTH[i].minutos, gs->puntuacionesTH[i].segundos, gs->puntuacionesTH[i].dificultad);
            dibujar_texto_borde(gs, assets->biggestThingsFont25, BLANCO, al_map_rgb(0, 0, 0), 30, 50 + 50*i, ALLEGRO_ALIGN_LEFT, texto, 3);
        }
        else
        {
            sprintf(texto, "%s: %d %s", gs->puntuaciones[i].nombre, gs->puntuaciones[i].puntuacion, gs->puntuaciones[i].dificultad);
            dibujar_texto_borde(gs, assets->biggestThingsFont25, BLANCO, al_map_rgb(0, 0, 0), 30, 50 + 50*i, ALLEGRO_ALIGN_LEFT, texto, 3);
        }
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
    dibujar_texto_borde(gs, assets->biggestThingsFont18, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 800, 50, 0, "A: MOVER HACIA LA IZQUIERDA", 3);
    dibujar_texto_borde(gs, assets->biggestThingsFont18, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 800, 100, 0, "D: MOVER HACIA LA DERECHA", 3);
    dibujar_texto_borde(gs, assets->biggestThingsFont18, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 800, 150, 0, "LSHIFT: CORRER", 3);
    dibujar_texto_borde(gs, assets->biggestThingsFont18, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 800, 200, 0, "SPACE: SALTAR", 3);
    dibujar_texto_borde(gs, assets->biggestThingsFont18, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 800, 250, 0, "CLICK IZQ: ATAQUE BASICO", 3);
    dibujar_texto_borde(gs, assets->biggestThingsFont18, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 800, 300, 0, "CLICK DER: EQUIPO DE MANIOBRAS / ODM", 3);
    dibujar_texto_borde(gs, assets->biggestThingsFont18, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 800, 350, 0, "F: DASH", 3);
    dibujar_texto_borde(gs, assets->biggestThingsFont18, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 800, 400, 0, "1 (CON EQUIPO DE MANIOBRAS): HABILIDAD 1", 3);
    dibujar_texto_borde(gs, assets->biggestThingsFont18, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 800, 450, 0, "2 (CON EQUIPO DE MANIOBRAS): HABILIDAD 2", 3);
    dibujar_texto_borde(gs, assets->biggestThingsFont18, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 800, 500, 0, "3 : RECARGAR GAS", 3);
    dibujar_texto_borde(gs, assets->biggestThingsFont18, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 800, 550, 0, "C : PARRY / DESVIAR ATAQUE", 3);
    dibujar_texto_borde(gs, assets->biggestThingsFont18, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 800, 600, 0, "X : MODO ACKERMAN", 3);

}

void jugando_ui(s_GameState *gs, s_Assets *assets)
{
    char texto[20];

    if(gs->animaciones.efectoSangre.activo)
        efectoSangre(gs, assets);

    sprintf(texto, "%02d: %02d", gs->tiempoJugado.minutos, gs->tiempoJugado.segundos);
    al_draw_text(assets->biggestThingsFont18, al_map_rgb(220, 220, 220), (SCREEN_X-80), 10, 0, texto); 

    dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(220, 220, 220), al_map_rgb(0, 0, 0), SCREEN_X/2, 0, ALLEGRO_ALIGN_CENTER, "Code No Kyojin", 2);

    if(gs->dificultad == NORMAL)
        dibujar_texto_borde(gs, assets->shingekiFont20, al_map_rgb(230, 230, 230), al_map_rgb(0, 0, 0), SCREEN_X/2, 40, ALLEGRO_ALIGN_CENTER, "NORMAL", 2);
    else if(gs->dificultad == DIFICIL)
        dibujar_texto_borde(gs, assets->shingekiFont20, al_map_rgb(255, 0, 0), al_map_rgb(0, 0, 0), SCREEN_X/2, 40, ALLEGRO_ALIGN_CENTER, "DIFICIL", 2);


    ui_elementos(gs, assets);

    //============Escudos=============//

    al_draw_scaled_bitmap(assets->assetsPantalla.escudoLegion, 0, 0, al_get_bitmap_width(assets->assetsPantalla.escudoLegion), al_get_bitmap_height(assets->assetsPantalla.escudoLegion), (SCREEN_X - 100) , 55 ,
        al_get_bitmap_width(assets->assetsPantalla.escudoLegion), al_get_bitmap_height(assets->assetsPantalla.escudoLegion), 0);

    sprintf(texto, "x%d", gs->levi.inventario.escudos);
    dibujar_texto_borde(gs, assets->biggestThingsFont18, al_map_rgb(255,255,255), al_map_rgb(0,0,0), (SCREEN_X -30), 55, ALLEGRO_ALIGN_RIGHT, texto, 1.5);


    //==================Hud=====================//

    al_draw_scaled_bitmap(assets->assetsPantalla.HUD, 0, 0, al_get_bitmap_width(assets->assetsPantalla.HUD), 
        al_get_bitmap_height(assets->assetsPantalla.HUD), 0 , 200 , al_get_bitmap_width(assets->assetsPantalla.HUD) * 0.24, 
        al_get_bitmap_height(assets->assetsPantalla.HUD) * 0.24, 0);

    al_draw_scaled_bitmap(assets->assetsPantalla.galonGas, 0, 0, al_get_bitmap_width(assets->assetsPantalla.galonGas), al_get_bitmap_height(assets->assetsPantalla.galonGas), 
        7 , 240 , al_get_bitmap_width(assets->assetsPantalla.galonGas) * 0.045, al_get_bitmap_height(assets->assetsPantalla.galonGas) * 0.045, 0);

    sprintf(texto, "x%d", gs->levi.inventario.gasODM);
    dibujar_texto_borde(gs, assets->biggestThingsFont12, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 57, 265, ALLEGRO_ALIGN_LEFT, texto, 1.5);

    if(gs->levi.cooldownHabilidad1 <= 0)
        al_draw_scaled_bitmap(assets->assetsPantalla.habilidades[0], 0, 0, 120, 80, 5, 297, 120*0.62, 80*0.62, 0);
    else if(gs->levi.cooldownHabilidad1 > 0)
    {
        al_draw_scaled_bitmap(assets->assetsPantalla.habilidades[1], 0, 0, 120, 80, 5, 297, 120*0.62, 80*0.62, 0);
        sprintf(texto, "%.1f", gs->levi.cooldownHabilidad1);
        dibujar_texto_borde(gs, assets->biggestThingsFont17, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 28, 305, ALLEGRO_ALIGN_LEFT, texto, 1.5);
    }

    if(gs->levi.cooldownHabilidad2 <= 0)
        al_draw_scaled_bitmap(assets->assetsPantalla.habilidades[2], 0, 0, 120, 80, 5, 355, 120*0.62, 80*0.62, 0);
    else if(gs->levi.cooldownHabilidad2 > 0)
    {
        al_draw_scaled_bitmap(assets->assetsPantalla.habilidades[3], 0, 0, 120, 80, 5, 355, 120*0.62, 80*0.62, 0);
        sprintf(texto, "%.1f", gs->levi.cooldownHabilidad2);
        dibujar_texto_borde(gs, assets->biggestThingsFont17, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 28, 363, ALLEGRO_ALIGN_LEFT, texto, 1.5);
    }

    if(gs->levi.dash.cantDash > 0)
    {
        al_draw_scaled_bitmap(assets->assetsPantalla.habilidades[4], 0, 0, 120, 80, 5, 413, 120*0.62, 80*0.62, 0);
        sprintf(texto, "x%d", gs->levi.dash.cantDash);
        dibujar_texto_borde(gs, assets->biggestThingsFont12, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 57, 450, ALLEGRO_ALIGN_LEFT, texto, 1.5);
        sprintf(texto, "%d/2", gs->levi.dash.flagDash);
        dibujar_texto_borde(gs, assets->biggestThingsFont10, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 10, 453, ALLEGRO_ALIGN_LEFT, texto, 1.5);
    }
    else if(gs->levi.dash.cantDash <= 0)
    {
        al_draw_scaled_bitmap(assets->assetsPantalla.habilidades[5], 0, 0, 120, 80, 5, 413, 120*0.62, 80*0.62, 0);
        sprintf(texto, "x%d", gs->levi.dash.cantDash);
        dibujar_texto_borde(gs, assets->biggestThingsFont12, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 57, 450, ALLEGRO_ALIGN_LEFT, texto, 1.5);
        sprintf(texto, "%d/2", gs->levi.dash.flagDash);
        dibujar_texto_borde(gs, assets->biggestThingsFont10, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 10, 453, ALLEGRO_ALIGN_LEFT, texto, 1.5);
    }

    //============================//

    al_draw_scaled_bitmap(assets->assetsPantalla.marcoVida, 0, 0, al_get_bitmap_width(assets->assetsPantalla.marcoVida), al_get_bitmap_height(assets->assetsPantalla.marcoVida),
        10, 20, al_get_bitmap_width(assets->assetsPantalla.marcoVida) * 0.1f, al_get_bitmap_height(assets->assetsPantalla.marcoVida) * 0.15f, 0);

    if(gs->levi.vida > 0)
        al_draw_line(20, 32, gs->levi.vida * 4.6 + 20, 32, al_map_rgb(200, 35, 35), 8);
    
    if(gs->levi.tiempoModoAckerman > 0)
        al_draw_line(20, 45, gs->levi.tiempoModoAckerman * 23 + 20, 45, al_map_rgb(70, 170, 240), 8);
    else   
        al_draw_line(20, 45, gs->levi.aumentaMA* 23 + 20, 45, al_map_rgb(220, 220, 220), 8);

    al_draw_scaled_bitmap(assets->assetsPantalla.galonGas, 0, 0, al_get_bitmap_width(assets->assetsPantalla.galonGas), al_get_bitmap_height(assets->assetsPantalla.galonGas),
        0, 15, al_get_bitmap_width(assets->assetsPantalla.galonGas) * 0.15f, al_get_bitmap_height(assets->assetsPantalla.galonGas) * 0.15f, ALLEGRO_FLIP_HORIZONTAL);

    if(gs->levi.gasRestante > 0)
        al_draw_line(20, 82, (gs->levi.gasRestante * 0.167) + 20, 82, al_map_rgb(120, 128, 135), 7);

    if(gs->levi.cooldownParry > 0)
        al_draw_filled_circle(30, 150, 10, al_map_rgb(0, 0, 0));
    else 
        al_draw_filled_circle(30, 150, 10, al_map_rgb(76, 217, 100));

    if(gs->levi.agarrado)
    {
        float anchoDest = 2172/7.0f;
        float altoDest = 724/7.0f;

        al_draw_scaled_bitmap(assets->assetsPantalla.cuadroTexto, 0, 0, 2172, 724, 
            SCREEN_X/2 - anchoDest/2.0f, 122, anchoDest, altoDest, 0);

        if(gs->variables.agarradoPorTitan1)
            sprintf(texto, "PRESIONA CLICK IZQ: %d/10", gs->levi.contSoltarse);
        else
            sprintf(texto, "PRESIONA CLICK IZQ: %d/5", gs->levi.contSoltarse);
        dibujar_texto_borde(gs, assets->biggestThingsFont18, al_map_rgb(245, 250, 255), al_map_rgb(0, 0, 0), SCREEN_X/2, 154, 
            ALLEGRO_ALIGN_CENTER, texto, 3); 
    }

    if(gs->titanHembra.activa)
    {
        al_draw_line(315, 75, 965, 75, al_map_rgb(0, 0, 0), 10);

        if(!gs->titanHembra.fase2Activa)
        {
            if(gs->dificultad == NORMAL)
                al_draw_line(315, 75, (gs->titanHembra.vida * 0.013) + 320, 75, al_map_rgb(219, 68, 107), 10);
            else if(gs->dificultad == DIFICIL)
                al_draw_line(315, 75, (gs->titanHembra.vida * (0.013/2)) + 320, 75, al_map_rgb(219, 68, 107), 10);
        }
        else
        { 
            if(gs->dificultad == NORMAL)
                al_draw_line(315, 75, (gs->titanHembra.vida * 0.013) + 315, 75, al_map_rgb(138, 180, 214), 10);
            else if(gs->dificultad == DIFICIL)
                al_draw_line(315, 75, (gs->titanHembra.vida * (0.013/2)) + 315, 75, al_map_rgb(138, 180, 214), 10);
        }
        if(gs->dificultad == NORMAL)
            al_draw_scaled_bitmap(assets->assetsPantalla.cabezaTH, 0, 0, 33, 33, (gs->titanHembra.vida * 0.013) + 310, 58, 33, 33, 0);
        else if(gs->dificultad == DIFICIL)
            al_draw_scaled_bitmap(assets->assetsPantalla.cabezaTH, 0, 0, 33, 33, (gs->titanHembra.vida * (0.013/2)) + 310, 58, 33, 33, 0);

    }

    puntuacion(gs, assets);

    //==============================//

    if(gs->animaciones.transicion2.activo == true)
        transicion2(gs, assets);

    else if(gs->animaciones.transicion.activo == true)
        transicion(gs, assets);

    if(gs->pausa)
        pausa(gs, assets);

    if(gs->tutorialEjecutando)
        tutorial(gs, assets);


}

void dibujar_texto_borde(s_GameState *gs, ALLEGRO_FONT *fuente, ALLEGRO_COLOR colorTexto, ALLEGRO_COLOR colorBorde, float x, float y, int flags, const char *texto, float grosor) 
{
    al_draw_text(fuente, colorBorde, (x - grosor), y, flags, texto);
    al_draw_text(fuente, colorBorde, (x + grosor), y, flags, texto);
    al_draw_text(fuente, colorBorde, x, (y - grosor), flags, texto);
    al_draw_text(fuente, colorBorde, x, (y + grosor), flags, texto);

    al_draw_text(fuente, colorTexto, x, y, flags, texto);
    al_draw_text(fuente, colorTexto, x + 0.5f, y, flags, texto);

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
        sprintf(texto, "Puntuacion: %d", gs->levi.puntuacion);
        dibujar_texto_borde(gs, assets->biggestThingsFont25, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 450, ALLEGRO_ALIGN_CENTER, texto, 3);

        if(gs->contOpcionesGO == 0)
        {
            if(gs->modoOleadaEjecutando)
            {
                sprintf(texto, "REINTENTAR");
                dibujar_texto_borde(gs, assets->biggestThingsFont18, al_map_rgb(255,0,0), al_map_rgb(0,0,0), SCREEN_X/2, 525, ALLEGRO_ALIGN_CENTER, texto, 3);
            }
            else
            {
                strcpy(texto, "REINTENTAR");
                dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), SCREEN_X/2, 525, ALLEGRO_ALIGN_CENTER, texto, 3);
            }
        }

        else
        {
            if(gs->modoOleadaEjecutando)
            {
                sprintf(texto, "REINTENTAR");
                dibujar_texto_borde(gs, assets->biggestThingsFont18, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 525, ALLEGRO_ALIGN_CENTER, texto, 3);
            }
            else
            {
                strcpy(texto, "REINTENTAR");
                dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 525, ALLEGRO_ALIGN_CENTER, texto, 3);
            }
        }

        if(gs->contOpcionesGO == 1)
        {
            strcpy(texto, "VOLVER AL MENU PRINCIPAL");
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), SCREEN_X/2, 600, ALLEGRO_ALIGN_CENTER, texto, 3);
        }

        else
        {
            strcpy(texto, "VOLVER AL MENU PRINCIPAL");
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 600, ALLEGRO_ALIGN_CENTER, texto, 3);
        }
    }
    else if(gs->nivelCompletado)
    {
        strcpy(texto, "GANASTE!!");
        dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 450, ALLEGRO_ALIGN_CENTER, texto, 3);

        if(gs->vsTitanHembraEjecutando)
            sprintf(texto, "TIEMPO: %02d:%02d", gs->tiempoJugado.minutos, gs->tiempoJugado.segundos);
        else
            sprintf(texto, "PUNTUACION: %d", gs->puntuacionJugador.puntuacion);

        dibujar_texto_borde(gs, assets->biggestThingsFont18, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 525, ALLEGRO_ALIGN_CENTER, texto, 3);

        if(gs->contOpcionesGO == 0)
        {
            strcpy(texto, "VOLVER AL MENU PRINCIPAL");
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,0,0), al_map_rgb(0,0,0), SCREEN_X/2, 600, ALLEGRO_ALIGN_CENTER, texto, 3);
        }
        else
        {
            strcpy(texto, "VOLVER AL MENU PRINCIPAL");
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 600, ALLEGRO_ALIGN_CENTER, texto, 3);
        } 
    }
}

void tutorial(s_GameState *gs, s_Assets *assets)
{
    if(gs->levi.agarrado)
        return;

    switch(gs->tutorial.fase)
    {
        case 0:
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 100, ALLEGRO_ALIGN_CENTER, "PULSA A/D PARA MOVERTE ", 1.5);
            break;
        case 1:
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 100, ALLEGRO_ALIGN_CENTER, "PULSA LSHIFT PARA CORRER", 1.5);
            break;
        case 2:
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 100, ALLEGRO_ALIGN_CENTER, "PULSA ESPACIO PARA SALTAR Y ESPACIO EN EL AIRE", 1.5);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 150, ALLEGRO_ALIGN_CENTER, "PARA UN DOBLE SALTO Y SUBIRTE AL PUESTO", 1.5);
            break;
        case 3:
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 100, ALLEGRO_ALIGN_CENTER, "PULSA S PARA BAJAR", 1.5);
            break;
        case 4:
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 100, ALLEGRO_ALIGN_CENTER, "PULSA CLICK IZQUIERDO PARA ATACAR Y MATAR AL TITAN", 1.5);
            break;
        case 5:
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 100, ALLEGRO_ALIGN_CENTER, "PULSA CLICK DERECHO EN LA CASA / GRIETA", 1.5);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 150, ALLEGRO_ALIGN_CENTER, "PARA OCUPAR EL EQUIPO DE MANIOBRAS", 1.5);
            break;
        case 6:
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 100, ALLEGRO_ALIGN_CENTER, "OCUPA EL EQUIPO DE MANIOBRAS EN EL TITAN Y ATACALO EN LA NUCA", 1.5);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 150, ALLEGRO_ALIGN_CENTER, "PARA MATARLO INSTANTANEAMENTE", 1.5);
            break;
        case 7:
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 100, ALLEGRO_ALIGN_CENTER, "SIGUE AVANZANDO", 1.5);
            break;
        case 8:
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 100, ALLEGRO_ALIGN_CENTER, "CON EL EQUIPO DE MANIOBRAS ACTIVADO OCUPA", 1.5);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 150, ALLEGRO_ALIGN_CENTER, "LA TECLA UNO O DOS PARA UTILIZAR UNA HABILIDAD", 1.5);
            dibujar_texto_borde(gs, assets->shingekiFont20, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 10, 640, ALLEGRO_ALIGN_LEFT, "NOTA: AL OCUPAR UNA HABILIDAD TENDRAS QUE ESPERAR PARA VOLVER A OCUPARLA", 1.5);
            break;
        case 9:
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 100, ALLEGRO_ALIGN_CENTER, "PULA F PARA OCUPAR EL DASH Y MATAR AL TITAN", 1.5);
            break;
        case 10:
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 100, ALLEGRO_ALIGN_CENTER, "ACERCATE AL TITAN Y PULSA LA C JUSTO ANTES DE QUE IMAPCTE SU ATAQUE", 1.5);
            dibujar_texto_borde(gs, assets->shingekiFont20, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 10, 640, ALLEGRO_ALIGN_LEFT, "NOTA: SI LOGRAS DESVIAR UN ATAQUE CON UN PARRY SE REESTABLECEN TUS HABILIDADES", 1.5);
            dibujar_texto_borde(gs, assets->shingekiFont20, al_map_rgb(255,255,255), al_map_rgb(0,0,0), 10, 670, ALLEGRO_ALIGN_LEFT, "AUMENTA TU VIDA, OBTIENES INVULNERABILIDAD MOMENTANEA Y OBTIENES UNA CARGA DEL DASH", 1.5);
            break;
        case 11:
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 100, ALLEGRO_ALIGN_CENTER, "CADA DIEZ TITANES QUE MATES CON UN ATAQUE BASICO EN LA NUCA", 1.5);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 150, ALLEGRO_ALIGN_CENTER, "PODRAS OCUPAR EL MODO ACKERMAN EL MODO ACKERMAN CON LA LETRA X,", 1.5);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 200, ALLEGRO_ALIGN_CENTER, "DONDE SE TE REDUCIRA EL TIEMPO DE ESPERA POR HABILIDAD, AUMENTARA TU VELOCIDAD, ", 1.5);
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 250, ALLEGRO_ALIGN_CENTER, "TU ATAQUE Y OBTENDRAS TRES CARGAS DE DASH", 1.5);
            break;
        case 12:
            dibujar_texto_borde(gs, assets->shingekiFont30, al_map_rgb(255,255,255), al_map_rgb(0,0,0), SCREEN_X/2, 100, ALLEGRO_ALIGN_CENTER, "PULSA E EN LA PUERTA DE LA CASA PARA TERMINAR EL TUTORIAL", 1.5);
            break;
    }
    
}

void puntuacion(s_GameState *gs, s_Assets *assets)
{
    char texto[20];
    ALLEGRO_COLOR color;

    switch(gs->variables.multiplicador)
    {
        case 1:
            color = al_map_rgb(230, 225, 210);
            break;
        case 2:
            color = al_map_rgb(255, 230, 120);
            break;
        case 3:
            color = al_map_rgb(255, 180, 60);
            break;
        case 4: 
            color = al_map_rgb(255, 100, 40);
            break;
        case 5:
            color = al_map_rgb(220, 20, 20);
            break;
    }

    sprintf(texto, "Puntuacion x%d: %d", gs->variables.multiplicador, gs->levi.puntuacion);
    dibujar_texto_borde(gs, assets->biggestThingsFont17, color, al_map_rgb(0,0,0), 20, 100, ALLEGRO_ALIGN_LEFT, texto, 2);

    if(gs->variables.multiplicador < 5)
    {
        sprintf(texto, "%d/%d - %.1f", gs->variables.flagPunt, gs->variables.multiplicador + 1, gs->variables.contPunt);
        dibujar_texto_borde(gs, assets->biggestThingsFont17, color, al_map_rgb(0,0,0), SCREEN_X - 20, 100, ALLEGRO_ALIGN_RIGHT, texto, 2);
    }
    else 
    {
        sprintf(texto, "%.1f", gs->variables.contPunt);
        dibujar_texto_borde(gs, assets->biggestThingsFont17, color, al_map_rgb(0,0,0), SCREEN_X - 30, 100, ALLEGRO_ALIGN_RIGHT, texto, 2);
    }
}

void efectoSangre(s_GameState *gs, s_Assets *assets)
{
    int frameX = gs->animaciones.efectoSangre.frameActual*1280;

    al_draw_bitmap_region(assets->assetsPantalla.efectoSangre, frameX, 0, 1280, 720, 0, 0, 0);
}

void ui_elementos(s_GameState *gs, s_Assets *assets)
{
    int pA = gs->pantalla_actual, i;

    for(i = 0; i < gs->pantalla[pA].num_elementos; i++)
        if(colision(gs, gs->levi.hitbox, gs->pantalla[pA].elementos[i].hitbox) && gs->pantalla[pA].elementos[i].tipo == 4)
            al_draw_scaled_bitmap(assets->assetsPantalla.e_Icon, 0, 0, 1312, 1199, gs->pantalla[pA].elementos[i].hitbox.x + gs->pantalla[pA].elementos[i].hitbox.ancho/2.0f - (1312/12.0f)/2.0f - gs->camara.x, 
                gs->pantalla[pA].elementos[i].hitbox.y - 120, 1312/12.0f, 1199/12.0f, 0);
}