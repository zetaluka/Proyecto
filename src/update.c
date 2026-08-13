#include "commons.h"

//====Prototipos====//
void update_jugando(s_GameState *gs, s_Assets *assets, ALLEGRO_DISPLAY *display, s_GameState *auxgs, ALLEGRO_EVENT *evento);
void update_menu(s_GameState *gs, s_Assets *assets, ALLEGRO_DISPLAY *display, s_GameState *auxgs, ALLEGRO_EVENT *evento);
void update_tiempo_jugado(s_GameState* gs);
void update_levi_movimiento(s_GameState *gs);
void transicion_pantalla(s_GameState *gs, s_Assets *assets, s_GameState *auxgs);
void hitbox_levi(s_GameState *gs, s_Assets *assets);
void levi_dash(s_GameState *gs);
void camara_scroll(s_GameState *gs);
void colision_levi_dash(s_GameState *gs);
void colision_levi_ataque(s_GameState *gs);
void colision_levi_mapa(s_GameState *gs);
void colision_ODM(s_GameState *gs);
void colision_levi_elementos(s_GameState *gs);
void inicia_ODM(s_GameState *gs, float *cx, float *cy);
void hitbox_mouse(s_GameState *gs);
void actualizar_animacion(s_GameState *gs);
void activar_hitbox(s_GameState *gs);
void desactivar_hitbox(s_GameState *gs, float tiempo);
void aumenta_dash(s_GameState *gs);
void aumenta_puntuacion(s_GameState *gs, int i, char *tipo);
void vuelve_ODM(s_GameState *gs);
void genera_gas(s_GameState *gs);
void actualizar_gas(s_GameState *gs);
void fin_animacion(s_GameState *gs);
void levi_ataques(s_GameState *gs);
void actualizar_transicion(s_GameState *gs);
void actualizar_transicion2(s_GameState *gs);
void verifica_estado_nivel(s_GameState *gs, s_Assets *assets, ALLEGRO_DISPLAY *display);
void guarda_puntuacion(s_GameState *gs, int cantidad);
void logica_menu(s_GameState *gs, s_Assets *assets, ALLEGRO_DISPLAY *display, s_GameState *auxgs);
void menu_pausa(s_GameState *gs, s_Assets *assets, ALLEGRO_DISPLAY *display, s_GameState *auxgs);
void interactua_inventario(s_GameState *gs);
void update_game_over(s_GameState *gs, s_Assets *assets, s_GameState *auxgs, ALLEGRO_DISPLAY *display);
void parry(s_GameState *gs);
void tutorial_upd(s_GameState *gs);
void modo_ackerman(s_GameState *gs);

//====Funcion principal====//
void update(s_GameState *gs, s_Assets *assets, ALLEGRO_DISPLAY *display, s_GameState *auxgs, ALLEGRO_EVENT *evento)
{
    if(gs->estadoPantalla == PANTALLA_MENU) //Reproduce la cancion del menu
    {
        if(!al_get_audio_stream_playing(gs->audio.musica_menu))
        {
            al_rewind_audio_stream(gs->audio.musica_menu);
            al_set_audio_stream_playing(gs->audio.musica_menu, true);
        }
    }
    else
    {
        if(al_get_audio_stream_playing(gs->audio.musica_menu))
            al_set_audio_stream_playing(gs->audio.musica_menu, false);
    }

    switch(gs->estadoPantalla) //Detecta en que estado esta, ejemplo: Menu, jugando, pausa, etc.
    {
        case PANTALLA_MENU:
            update_menu(gs, assets, display, auxgs, evento);
            break;

        case PANTALLA_JUGANDO:
            update_jugando(gs,assets, display, auxgs, evento);
            break;

        case PANTALLA_GAME_OVER:
            update_game_over(gs,assets, auxgs, display);
            break;
    }

}

void update_jugando(s_GameState *gs, s_Assets *assets, ALLEGRO_DISPLAY *display, s_GameState *auxgs, ALLEGRO_EVENT *evento) 
{
    if(gs->input.keyEsc)
    {
        if(gs->pausa == true)
            gs->pausa = false;
        else if(gs->pausa == false)
        {
            gs->pausa = true;
            gs->menuPausa.estadoMenu = MAIN;
            gs->menuPausa.contMenu = 0;
        }

        gs->input.keyEsc = false;
    }
    
    if(gs->pausa == true)
    {
        menu_pausa(gs, assets, display, auxgs);
        return;
    }

    if(gs->tutorialEjecutando)
        tutorial_upd(gs);

    update_tiempo_jugado(gs);
    update_levi_movimiento(gs);
    hitbox_levi(gs,assets);
    hitbox_mouse(gs);
    levi_dash(gs);
    if(gs->variables.desactivarHitbox == true)
        activar_hitbox(gs);
    comprueba_colision(gs);
    aumenta_dash(gs);
    interactua_inventario(gs);
    modo_ackerman(gs);
    actualizar_animacion(gs);
    actualizar_gas(gs);
    camara_scroll(gs);
    transicion_pantalla(gs, assets, auxgs);

    verifica_estado_nivel(gs, assets, display);

    return;
}

void tutorial_upd(s_GameState *gs)
{
    //Funcion que va aumentando las fases del tutorial y pidiendo distintos requisitos para poder completarlo

    gs->levi.gasRestante = 1000;
    gs->levi.vida = 50;
    gs->levi.dash.cantDash = 1;

    for(int i=0; i <gs->pantalla[gs->pantalla_actual].num_entidades; i++)
        if(gs->pantalla[gs->pantalla_actual].entidades[i].activo)
            gs->pantalla[gs->pantalla_actual].entidades[i].vida = 5;

    switch(gs->tutorial.fase)
    {
        case 0:
            if(gs->input.keyA || gs->input.keyD)
                gs->tutorial.teclaPulsada = true;
            if(gs->levi.x >= 300 && gs->tutorial.teclaPulsada)
            {
                gs->tutorial.fase++;
                gs->tutorial.teclaPulsada = false;
            }
            break;
        case 1:
            if(gs->input.keyLShift)
                gs->tutorial.teclaPulsada = true; 
            if(gs->levi.x >= 700 && gs->tutorial.teclaPulsada)
            {
                gs->tutorial.fase++;
                gs->tutorial.teclaPulsada = false;
            }
            break;
        case 2:
            if(gs->input.keySpace && gs->levi.levi_suelo == false)
                gs->tutorial.teclaPulsada = true;
            if(gs->tutorial.teclaPulsada && gs->tutorial.requisitoCumplido) //El requisito cumplido se verifica en colision_levi_mapa
            {
                gs->tutorial.fase++;
                gs->tutorial.teclaPulsada = false;
                gs->tutorial.requisitoCumplido = false;
            }
            break;
        case 3:
            gs->variables.bloquearControles = true;
            gs->input.keyD = false;
            gs->input.keyA = false;
            if(gs->input.keyS)
            {
                gs->tutorial.fase++;
                gs->tutorial.teclaPulsada = false;
                gs->variables.bloquearControles = true;
            }
            break;
        case 4:
            for(int i=0; i <gs->pantalla[gs->pantalla_actual].num_entidades; i++)
                if(gs->pantalla[gs->pantalla_actual].entidades[i].vida <= 0)
                {
                    gs->tutorial.fase++;
                    break;
                }
            break;
        case 5: 
            if(gs->levi.ODM.activo)
                gs->tutorial.fase++;
            break;
        case 6:
            if(gs->tutorial.requisitoCumplido)
            {
                gs->tutorial.fase++;
                gs->tutorial.requisitoCumplido = false;
            }
            break;
        case 7:
            if(gs->levi.x >= 2450)
                gs->tutorial.fase++;
            break;
        case 8:
            if(gs->levi.habilidad1Activa || gs->levi.habilidad2Activa)
                gs->tutorial.fase++;
            break;
        case 9:
            if(gs->levi.dash.activo)
                gs->tutorial.fase++;
            break;
        case 10:
            if(gs->tutorial.requisitoCumplido)
            {
                gs->tutorial.fase++;
                gs->tutorial.requisitoCumplido = false;
            }
            break;
        case 11:
            if(gs->levi.x >= 4850)
                gs->tutorial.fase++;
            break;
        case 12:
            if(gs->tutorial.requisitoCumplido)
                gs->nivelCompletado = true;
            break;
            
    }
}

void update_game_over(s_GameState *gs, s_Assets *assets, s_GameState *auxgs, ALLEGRO_DISPLAY *display)
{
    //Menu al morir o completar un nivel

    int lim;

    if(gs->levi.vida <= 0)
        lim = 1;
    else if(gs->nivelCompletado)
        lim = 0;

    if(gs->input.keyS && gs->contOpcionesGO < lim)
    {
        gs->contOpcionesGO++;
        gs->input.keyS = false;
    }
    else if(gs->input.keyW && gs->contOpcionesGO > 0)
    {
        gs->contOpcionesGO--;
        gs->input.keyW = false;
    }

    if(gs->levi.vida <= 0)
    {
        if(gs->input.keyEnter || gs->input.keyE)
        {
            if(gs->contOpcionesGO == 0)
            {
                *gs = *auxgs; 
                gs->levi.vida = 50;
                gs->levi.gasRestante = 1000;
                gs->animaciones.transicion.activo = false;
                gs->input.keyEnter = false;
                gs->input.keyE = false;
                actualiza_res(gs, display);
            }

            else if(gs->contOpcionesGO == 1)
            {
                gs->menuPausa.estadoMenu = MAIN;
                gs->estadoPantalla = PANTALLA_MENU;
                gs->nivel1Ejecutando = false;
                gs->tutorialEjecutando = false;
                gs->input.keyEnter = false;
                gs->input.keyE = false;
            }
        }
    }

    else if(gs->nivelCompletado)
    {
        if(gs->puntuacionGuardada == false)
        {
            guarda_puntuacion(gs, carga_puntuacion(gs));
            gs->puntuacionGuardada = true;
        }
        if(gs->input.keyEnter || gs->input.keyE)
        {
            gs->menuPausa.estadoMenu = MAIN;
            gs->estadoPantalla = PANTALLA_MENU;
            gs->nivel1Ejecutando = false;
            gs->tutorialEjecutando = false;
            gs->input.keyEnter = false;
            gs->input.keyE = false;
        }
    }

}

void update_menu(s_GameState *gs, s_Assets *assets, ALLEGRO_DISPLAY *display, s_GameState *auxgs, ALLEGRO_EVENT *evento)
{   
    if(gs->variables.ingresandoNombre)
    {
        if(gs->input.keyEsc)
        {
            gs->input.keyEsc = false;
            gs->variables.ingresandoNombre = false;
            gs->variables.nombreIngresado = false;
            strcpy(gs->variables.nombreTemp, "\0");
        }
        return;
    }

    logica_menu(gs, assets, display, auxgs);
}

void logica_menu(s_GameState *gs, s_Assets *assets, ALLEGRO_DISPLAY *display, s_GameState *auxgs)
{
    //Funcion para el menu principal y sus opciones

    int lim;
    char auxNombre[20];
    s_Audio audioBU;

    if(gs->variables.nombreIngresado)
    {
        if(gs->nivel1Ejecutando == false)
        {
            audioBU = gs->audio;
            strcpy(auxNombre, gs->puntuacionJugador.nombre);
            *gs = (s_GameState){0};
            strcpy(gs->puntuacionJugador.nombre, auxNombre);
            gs->audio = audioBU;
            gs->ejecutando = true;
            gs->nivel1Ejecutando = true;
            game_init(gs, assets, display);
            *auxgs = *gs;
        }
    }

    if(gs->menu.estadoMenu == MAIN)
        lim = 3;
    else if(gs->menu.estadoMenu == JUGAR)
        lim = 1;
    else if(gs->menu.estadoMenu == RANKING)
        lim = 0;
    else if(gs->menu.estadoMenu == OPCIONES)
        lim = 1;

    if(gs->input.keyS && gs->menu.contMenu < lim)
    {
        gs->menu.contMenu++;
        gs->input.keyS = false;
    }
    else if(gs->input.keyW && gs->menu.contMenu > 0)
    {
        gs->menu.contMenu--;
        gs->input.keyW = false;
    }

    if(gs->input.keyEnter || gs->input.keyE)
    {
        if(gs->menu.estadoMenu == MAIN)
        {
            if(gs->menu.contMenu == 0)
            {
                gs->menu.contMenu = 0;
                gs->input.keyEnter = false;
                gs->input.keyE = false;
                gs->menu.estadoMenuAnterior = gs->menu.estadoMenu;
                gs->menu.estadoMenu = JUGAR;
            }

            else if(gs->menu.contMenu == 1)
            {
                gs->menu.contMenu = 0;
                gs->input.keyEnter = false;
                gs->input.keyE = false;
                gs->menu.estadoMenuAnterior = gs->menu.estadoMenu;
                gs->menu.estadoMenu = RANKING;
            }

            else if(gs->menu.contMenu == 2)
            {
                gs->menu.contMenu = 0;
                gs->input.keyEnter = false;
                gs->input.keyE = false;
                gs->menu.estadoMenuAnterior = gs->menu.estadoMenu;
                gs->menu.estadoMenu = OPCIONES;
            }

            else if(gs->menu.contMenu == 3)
            {
                exit(0);
            }
        }

        else if(gs->menu.estadoMenu == JUGAR)
        {
            if(gs->menu.contMenu == 0)
                gs->variables.ingresandoNombre = true;

            else if(gs->menu.contMenu == 1)
            {
                gs->menu.contMenu = 0;
                gs->input.keyEnter = false;
                gs->input.keyE = false;
                if(gs->tutorialEjecutando == false)
                {
                    audioBU = gs->audio;
                    *gs = (s_GameState){0};
                    gs->audio = audioBU;
                    gs->ejecutando = true;
                    gs->tutorialEjecutando = true;
                    game_init(gs, assets, display);
                    *auxgs = *gs;
                }
            }
        }

        else if(gs->menu.estadoMenu == OPCIONES)
        {
            if(gs->menu.contMenu == 0)
            {
                if(gs->pantallaCompleta == false)
                    gs->pantallaCompleta = true;
                else if(gs->pantallaCompleta == true)
                    gs->pantallaCompleta = false;
                gs->input.keyEnter = false;
                gs->input.keyE = false;

                guarda_opciones(gs);
                al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, gs->pantallaCompleta);
                actualiza_res(gs, display);
            }
            else if(gs->menu.contMenu == 1)
            {
                if(gs->levi.vestuario == false)
                    gs->levi.vestuario = true;
                else if(gs->levi.vestuario == true)
                    gs->levi.vestuario = false;
                gs->input.keyEnter = false;
                gs->input.keyE = false;

                guarda_opciones(gs);
            }
        }
    }

    if(gs->input.keyEsc)
    {
        gs->input.keyEnter = false;
        gs->menu.contMenu = 0;
        gs->menu.estadoMenu = gs->menu.estadoMenuAnterior;
    }

}

void menu_pausa(s_GameState *gs, s_Assets *assets, ALLEGRO_DISPLAY *display, s_GameState *auxgs)
{
    int lim;
    
    if(gs->menuPausa.estadoMenu == MAIN)
        lim = 4;
    else if(gs->menuPausa.estadoMenu == CONTROLES)
        lim = 0;
    else if(gs->menuPausa.estadoMenu == OPCIONES)
        lim = 1;

    if(gs->input.keyS && gs->menuPausa.contMenu < lim)
    {
        gs->menuPausa.contMenu++;
        gs->input.keyS = false;
    }
    else if(gs->input.keyW && gs->menuPausa.contMenu > 0)
    {
        gs->menuPausa.contMenu--;
        gs->input.keyW = false;
    }

    if(gs->input.keyEnter || gs->input.keyE)
    {
        if(gs->menuPausa.estadoMenu == MAIN)
        {
            if(gs->menuPausa.contMenu == 0)
            {
                gs->menuPausa.contMenu = 0;
                gs->input.keyEnter = false;
                gs->input.keyE = false;
                gs->pausa = false;
            }

            else if(gs->menuPausa.contMenu == 1)
            {
                *gs = *auxgs; 
                gs->levi.vida = 50;
                gs->levi.gasRestante = 1000;
                gs->animaciones.transicion.activo = false;
                gs->input.keyEnter = false;
                gs->input.keyE = false;
                gs->input.keyD = false;
                gs->input.keyA = false;
                actualiza_res(gs, display);
            }

            else if(gs->menuPausa.contMenu == 2)
            {
                gs->menuPausa.contMenu = 0;
                gs->input.keyEnter = false;
                gs->input.keyE = false;
                gs->menuPausa.estadoMenuAnterior = gs->menuPausa.estadoMenu;
                gs->menuPausa.estadoMenu = CONTROLES;
            }

            else if(gs->menuPausa.contMenu == 3)
            {
                gs->menuPausa.contMenu = 0;
                gs->input.keyEnter = false;
                gs->input.keyE = false;
                gs->menuPausa.estadoMenuAnterior = gs->menuPausa.estadoMenu;
                gs->menuPausa.estadoMenu = OPCIONES;
            }
            else if(gs->menuPausa.contMenu == 4)
            {
                gs->menuPausa.contMenu = 0;
                gs->menuPausa.estadoMenu = MAIN;
                gs->estadoPantalla = PANTALLA_MENU;
                gs->nivel1Ejecutando = false;
                gs->tutorialEjecutando = false;
                gs->input.keyEnter = false;
                gs->input.keyE = false;
            }    
        }

        else if(gs->menuPausa.estadoMenu == CONTROLES)
        {
            gs->menu.contMenu = 0;
            gs->input.keyEnter = false;
            gs->input.keyE = false;
            gs->menuPausa.estadoMenu = gs->menuPausa.estadoMenuAnterior;
        }

        else if(gs->menuPausa.estadoMenu == OPCIONES)
        {
            if(gs->menuPausa.contMenu == 0)
            {
                if(gs->pantallaCompleta == false)
                    gs->pantallaCompleta = true;
                else if(gs->pantallaCompleta == true)
                    gs->pantallaCompleta = false;
                gs->input.keyEnter = false;
                gs->input.keyE = false;
                
                guarda_opciones(gs);
                al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, gs->pantallaCompleta);
                actualiza_res(gs, display);
            }

            else if(gs->menuPausa.contMenu == 1)
            {
                gs->menu.contMenu = 0;
                gs->input.keyEnter = false;
                gs->input.keyE = false;
                gs->menuPausa.estadoMenu = gs->menuPausa.estadoMenuAnterior;
            }
        }
    }
    
}

void verifica_estado_nivel(s_GameState *gs, s_Assets *assets, ALLEGRO_DISPLAY *display)
{
    if(gs->nivelCompletado == true)
    {
        if(gs->animaciones.transicion2.activo == false)
        {
            gs->animaciones.transicion2.activo = true;
            gs->animaciones.transicion2.cantidadFrames = 7;
            gs->animaciones.transicion2.contadorAnim = 0;
            gs->animaciones.transicion2.frameActual = 0;
            gs->animaciones.transicion2.velocidadAnim = 2;
        }

        actualizar_transicion2(gs);
    }

    if(gs->levi.vida <= 0)
    {
        if(gs->animaciones.transicion2.activo == false)
        {
            gs->animaciones.transicion2.activo = true;
            gs->animaciones.transicion2.cantidadFrames = 7;
            gs->animaciones.transicion2.contadorAnim = 0;
            gs->animaciones.transicion2.frameActual = 0;
            gs->animaciones.transicion2.velocidadAnim = 2;
        }

        actualizar_transicion2(gs);
    }

}

void update_tiempo_jugado(s_GameState* gs) //Funcion para hacer funcionar el cronometro de tiempo jugado
{
    if(gs->tiempoJugado.validacion == 1)
    {
        gs->tiempoJugado.segundos++;
        if(gs->tiempoJugado.segundos>=60)
        {
            gs->tiempoJugado.segundos=0;
            gs->tiempoJugado.minutos++;
        }
        gs->tiempoJugado.validacion=0;
    }
}

void update_levi_movimiento(s_GameState *gs)
{ 
    if(gs->levi.estadoLevi != DASH && gs->levi.estadoLevi != SALIDA_DASH && gs->levi.estadoLevi != ODM_ATAQUE1 && gs->levi.estadoLevi != ODM_ATAQUE2 //Sirve para reestablecer la gravedad que se cambia en las habilidad, con tal de evitar bugs
        && gs->levi.estadoLevi != SALIDA_ODM_ATAQUE1 && gs->levi.estadoLevi != SALIDA_ODM_ATAQUE2 && gs->levi.agarrado == false)
        gs->variables.gravedad = 0.8;

    //Calcula la distancia que recorrio levi desde que empezo a caer, si es mucha al caer bloquea los controles para mas realismo (impacto)
    if(gs->levi.velocidadY > 0)
    {
        gs->levi.distanciaYRecorrida += gs->levi.velocidadY;
        gs->levi.distanciaYRegistrada = true;
    }
    else if(gs->levi.velocidadY <= 0)
    {
        gs->levi.distanciaYRecorrida = 0;
        gs->levi.distanciaYRegistrada = false;
    }
    //=============================================================================================//

    //====Doble salto====//
    if(gs->input.keySpace == 1 && gs->levi.levi_suelo && !gs->levi.dash.activo && !gs->variables.bloquearControles) //Salto y habilita doble salto
    {
        gs->levi.velocidadY = -10;
        gs->levi.levi_suelo = false;
        gs->levi.doble_salto = true;
        gs->input.keySpace = 0;
        gs->levi.dash.tiempoRecuperacionDash = 0;
        gs->levi.ODM.activo = false;
        gs->levi.animacion.saltoActivo = true;
        gs->levi.animacion.bloquearAnimacion = false;
        gs->variables.bloquearControles = false;
        cambiar_animacion(gs, SALTANDO);
    }

    if(gs->input.keySpace == 1 && gs->levi.doble_salto && !gs->levi.dash.activo && !gs->variables.bloquearControles) //Confirma si doble salto esta habilitado
    {
        gs->levi.velocidadY = -15;
        gs->levi.doble_salto = false;
        gs->input.keySpace = 0;
        gs->levi.dash.tiempoRecuperacionDash = 0;
        gs->levi.ODM.activo = false;
        gs->animaciones.gasDS = true;
        gs->levi.animacion.saltoActivo = true;
        gs->levi.animacion.bloquearAnimacion = false;
        gs->variables.bloquearControles = false;
        gs->levi.gasRestante -= 10;
        cambiar_animacion(gs, SALTANDO);

    }

    if(gs->animaciones.gasDS)
    {
        gs->animaciones.contGasDS++;
        genera_gas(gs);

        if(gs->animaciones.contGasDS >= 15)
        {
            gs->animaciones.gasDS = false;
            gs->animaciones.contGasDS = 0;
        }
    }

    //Gravedad
    if(!gs->levi.dash.activo && !gs->levi.ODM.activo && gs->levi.estadoLevi != ODM_ATAQUE1 && gs->levi.estadoLevi != ODM_ATAQUE2)
    {
        gs->levi.velocidadY += gs->variables.gravedad;
        gs->levi.y = gs->levi.y + gs->levi.velocidadY;

        if(gs->levi.velocidadY >= 0 && !gs->levi.ODM.activo && !gs->levi.dash.activo && !gs->levi.levi_suelo && gs->levi.estadoLevi != SALIDA_DASH && gs->levi.estadoLevi != DASH
        && gs->levi.estadoLevi != ODM_ATAQUE1 && gs->levi.estadoLevi != ODM_ATAQUE2 && gs->levi.estadoLevi != SALIDA_ODM_ATAQUE1 && gs->levi.estadoLevi != SALIDA_ODM_ATAQUE2)
            cambiar_animacion(gs, CAYENDO);
    }

    if(gs->levi.ODM.activo == true)
        gs->levi.levi_suelo = false;

    if(gs->levi.dash.activo || gs->levi.ODM.activo)
        return;

    if(gs->levi.dash.tiempoRecuperacionDash >= 0) //Cuando sale del dash/ODM los controles mantienen resistencia, esto con el fin de que no se pueda mover libremente en el aire despues de un impulso (inercia)
    {
       if(gs->levi.levi_suelo)
            gs->levi.dash.tiempoRecuperacionDash = 0;

        gs->levi.dash.tiempoRecuperacionDash -= 1.0f/FPS;    
        gs->levi.velocidadX *= 0.9f; //Va bajando gradualmente la inercia horizontal
        gs->levi.velocidadY *= 0.9f; //Va bajando gradualmente la inercia vertical

        if(gs->input.keyLShift && gs->input.keyD && !gs->variables.bloquearControles) //Resta gradualmente la inercia
            gs->levi.velocidadX += 0.4f;
        
        else if(gs->input.keyLShift && gs->input.keyA && !gs->variables.bloquearControles) //Resta gradualmente la inercia
        {
            gs->levi.velocidadX -= 0.4f;
            gs->levi.animacion.rotarAnim = true;
        }

        else if(gs->input.keyA && !gs->variables.bloquearControles) //Resta gradualmente la inercia en menor cantidad
        {
            gs->levi.velocidadX -= 0.2f;
            gs->levi.animacion.rotarAnim = true;
        }

        else if(gs->input.keyD && !gs->variables.bloquearControles) //Resta gradualmente la inercia en menor cantidad
            gs->levi.velocidadX += 0.2f;

        gs->levi.x += gs->levi.velocidadX; //Hace que levi se mueva con la inercia
        gs->levi.y += gs->levi.velocidadY; //Hace que levi se mueva con la inercia
    }

    else  //Movimiento normal de levi
    {
        gs->levi.levi_vuelo = false;

        if(gs->input.keyLShift && gs->input.keyD && !gs->variables.bloquearControles) //Si mantiene el LShift corre
        {
            gs->levi.x += 4.5f * gs->levi.velocidadMA;
            if(gs->levi.levi_suelo && !gs->levi.animacion.bloquearAnimacion)
                cambiar_animacion(gs, CORRIENDO); //Hacer que bloquear controles sea falso y quitar condicion de bloquearAnimacion, y bloquear controles solo cuando caiga cierta distancia
            gs->levi.animacion.rotarAnim = false;
        }
            
        else if(gs->input.keyLShift && gs->input.keyA && !gs->variables.bloquearControles) //Si mantiene el LShift corre
        {
            gs->levi.x -= 4.5f * gs->levi.velocidadMA;
            if(gs->levi.levi_suelo && !gs->levi.animacion.bloquearAnimacion)
                cambiar_animacion(gs, CORRIENDO);
            gs->levi.animacion.rotarAnim = true;
        }

        else if(gs->input.keyA && !gs->variables.bloquearControles) //Camina izquierda
        {
            gs->levi.x -= 2 * gs->levi.velocidadMA;
            if(gs->levi.levi_suelo && !gs->levi.animacion.bloquearAnimacion)
                cambiar_animacion(gs, CAMINANDO);
            gs->levi.animacion.rotarAnim = true;
        }

        else if(gs->input.keyD && !gs->variables.bloquearControles) //Camina derecha
        {
            gs->levi.x += 2 * gs->levi.velocidadMA;
            if(gs->levi.levi_suelo && !gs->levi.animacion.bloquearAnimacion)
                cambiar_animacion(gs, CAMINANDO);
            gs->levi.animacion.rotarAnim = false;
        }
    }

    if(gs->levi.levi_suelo && !gs->input.keyA && !gs->input.keyD && !gs->levi.ODM.activo && !gs->levi.dash.activo && !gs->levi.animacion.bloquearAnimacion && gs->levi.estadoLevi != ATERRIZANDO)
        cambiar_animacion(gs, IDLE);

    if(gs->input.keyS && gs->levi.levi_suelo)
    {
        desactivar_hitbox(gs, 0.1f);
        gs->input.keyS = false;
    }
    else if(gs->input.keyS && !gs->levi.levi_suelo)
        desactivar_hitbox(gs, 0.5f);
    
}

void transicion_pantalla(s_GameState *gs, s_Assets *assets, s_GameState *auxgs) //Efecto de transicion por pantallas
{
    //Funcion que hace pasar de una pantalla a otra, ademas, llama nuevamente a la funcion mapa para cargar la siguiente pantalla

    float anchoPantalla = gs->pantalla[gs->pantalla_actual].ancho * TAM_CELDA;
    
    if(gs->levi.x+40 >= anchoPantalla && gs->pantalla_actual < MAXPANTALLAS - 1 && gs->pantalla_actual == 2 && gs->animaciones.transicion.activo == false)
    {   
        gs->animaciones.transicion.activo = true;
        gs->animaciones.transicion.cantidadFrames = 38;
        gs->animaciones.transicion.contadorAnim = 0;
        gs->animaciones.transicion.frameActual = 0;
        gs->animaciones.transicion.velocidadAnim = 2;
        gs->animaciones.cambioPantallaHecho = false;
        gs->levi.agarrado = false;
    }

    if(gs->input.keyE)
        for(int i=0;i<gs->pantalla[gs->pantalla_actual].num_elementos;i++)
            if(colision(gs, gs->levi.hitbox, gs->pantalla[gs->pantalla_actual].elementos[i].hitbox) && gs->pantalla[gs->pantalla_actual].elementos[i].tipo == 4 && gs->animaciones.transicion.activo == false)
            {
                gs->animaciones.transicion.activo = true;
                gs->animaciones.transicion.cantidadFrames = 38;
                gs->animaciones.transicion.contadorAnim = 0;
                gs->animaciones.transicion.frameActual = 0;
                gs->animaciones.transicion.velocidadAnim = 2;
                gs->animaciones.cambioPantallaHecho = false;
                gs->input.keyE = false;
                gs->levi.agarrado = false;
            }

    if(gs->animaciones.transicion.activo == true)
        actualizar_transicion(gs);

    if(gs->animaciones.transicion.frameActual == 20 && !gs->animaciones.cambioPantallaHecho)
    {
        gs->animaciones.cambioPantallaHecho = true;

        if(gs->pantalla_actual < MAXPANTALLAS - 1)
        {
            gs->pantalla_actual++;
            gs->camara.x = 0;
            gs->levi.agarrado = false;
            for(int i=0;i<5;i++)
            {
                gs->variables.grietas[i].x = 0;
                gs->variables.grietas[i].y = 0;
            }

            if(gs->pantalla[gs->pantalla_actual].pantallaCargada == false)
            {
                hitbox_init(gs);
                gs->pantalla[gs->pantalla_actual].pantallaCargada = true;
            }
        }
    }

    if(gs->pantalla_actual > gs->variables.carga_pantalla) //Servia para detectar si la pantalla habia sido cargada 
    {
        mapa(gs, assets);
        gs->variables.carga_pantalla++;
        
        if(gs->pantalla[gs->pantalla_actual].pantallaCargada == false)
        {
            hitbox_init(gs);
            gs->pantalla[gs->pantalla_actual].pantallaCargada = true;
        }

        *auxgs = *gs;
    }

}

void hitbox_levi(s_GameState *gs, s_Assets *assets) //Actualiza la hitbox del personaje principal
{
    gs->levi.hitbox.alto = LEVI_SS_ALTO - LEVI_HB_OFFSET_Y;  
    gs->levi.hitbox.ancho = LEVI_HB_RECORTE - 18;
    gs->levi.hitbox.x = gs->levi.x + LEVI_HB_RECORTE + 3;
    gs->levi.hitbox.y = gs->levi.y + LEVI_HB_OFFSET_Y;
}

void hitbox_mouse(s_GameState *gs)
{
    gs->levi.hitboxODM.x = (gs->input.mouseX/gs->escala) + gs->camara.x;
    gs->levi.hitboxODM.y = (gs->input.mouseY/gs->escala) + gs->camara.y;
    gs->levi.hitboxODM.ancho = 5;
    gs->levi.hitboxODM.alto = 5;

}

//===========Funciones de colision==============//

bool colision(s_GameState *gs, s_Hitbox h1, s_Hitbox h2)
{
    if( h1.x + h1.ancho >= h2.x && 
        h1.y + h1.alto >= h2.y &&
        h1.x <= h2.x + h2.ancho && 
        h1.y <= h2.y + h2.alto )
        return true;
    
    return false;

}

void comprueba_colision(s_GameState *gs) //Comprueba si la hitbox del personaje choca con alguna otra y actua conforme al caso
{   

    colision_levi_mapa(gs);
    colision_levi_ataque(gs);
    colision_ODM(gs);
    if(gs->levi.dash.activo == true || gs->levi.estadoLevi == SALIDA_DASH)
        colision_levi_dash(gs);
    colision_levi_elementos(gs);
    //Hacer funcion de detectar colision entre titan y estructuras(del mapa) para ponerles gravedad

}

void levi_ataques(s_GameState *gs)
{
    //Calcula que hitbox de ataque y que propiedades adquirir para cada ataque de levi, ya sea habilidades especiales, ataque normal o si esta enganchado en un titan(hitbox ataque normal duplicada para mas facilidad de acertar ataque)

    gs->levi.leviAtacando = false;

    if(gs->levi.ODM.activo && gs->input.key1 && gs->levi.cooldownHabilidad1 <= 0)
    {
        gs->levi.habilidad1Activa = true;
        gs->levi.cooldownHabilidad1 = 5;
    }
    if(gs->levi.ODM.activo && gs->input.key2 && gs->levi.cooldownHabilidad2 <= 0)
    {
        gs->levi.habilidad2Activa = true;
        gs->levi.cooldownHabilidad2 = 10;
    }

    if(gs->levi.estadoLevi == SALIDA_TITAN_AGARRE)
    {
        gs->levi.habilitaAumentaDash = false;
        gs->levi.hitboxAtaque.x = gs->levi.hitbox.x - 40;
        gs->levi.hitboxAtaque.y = gs->levi.hitbox.y + gs->levi.hitbox.alto/2 - 20;
        gs->levi.hitboxAtaque.alto = 7;
        gs->levi.hitboxAtaque.ancho = 110;
        gs->levi.ataque = 5;
        gs->levi.ataqueNuca = 5;
        gs->levi.puntuacionTitan = 15;
        gs->levi.puntuacionNuca = 50;
        gs->levi.leviAtacando = true;
        gs->levi.invulnerabilidad = true;
    }

    else if(gs->levi.estadoLevi == PARRY_EXITOSO && gs->levi.ataqueHecho == false)
    {
        gs->levi.habilitaAumentaDash = true;
        gs->levi.hitboxAtaque.x = gs->levi.hitbox.x - 40;
        gs->levi.hitboxAtaque.y = gs->levi.hitbox.y + gs->levi.hitbox.alto/2 - 20;
        gs->levi.hitboxAtaque.alto = 7;
        gs->levi.hitboxAtaque.ancho = 110;
        gs->levi.ataque = 250;
        gs->levi.ataqueNuca = 1000;
        gs->levi.puntuacionTitan = 750;
        gs->levi.puntuacionNuca = 1500;
        gs->levi.leviAtacando = true;
        gs->levi.ataqueHecho = true;
    }

    else if(gs->levi.habilidad1Activa)
    {
        gs->levi.habilitaAumentaDash = false;
        gs->levi.hitboxAtaque.x = gs->levi.x + 20;
        gs->levi.hitboxAtaque.y = gs->levi.y + 5;
        gs->levi.hitboxAtaque.alto = 60;
        gs->levi.hitboxAtaque.ancho = 90;
        gs->input.key1 = false;
        gs->levi.leviAtacando = true;
        gs->levi.ataque = 50;
        gs->levi.ataqueNuca = 75;
        gs->levi.puntuacionTitan = 50;
        gs->levi.puntuacionNuca = 75;
        gs->levi.invulnerabilidad = true;
        cambiar_animacion(gs, ODM_ATAQUE1);
    }

    else if(gs->levi.habilidad2Activa)
    {
        gs->levi.habilitaAumentaDash = false;
        gs->levi.hitboxAtaque.x = gs->levi.x + 20;
        gs->levi.hitboxAtaque.y = gs->levi.y;
        gs->levi.hitboxAtaque.alto = 75;
        gs->levi.hitboxAtaque.ancho = 85;
        gs->input.key2 = false;
        gs->levi.leviAtacando = true;
        gs->levi.ataque = 70;
        gs->levi.ataqueNuca = 90;  
        gs->levi.puntuacionTitan = 50;
        gs->levi.puntuacionNuca = 75;
        gs->levi.invulnerabilidad = true;
        cambiar_animacion(gs, ODM_ATAQUE2);
    }

    else if(gs->levi.ODM.engancheTitan)
    {
        gs->levi.habilitaAumentaDash = true;
        gs->levi.hitboxAtaque.x = gs->levi.hitbox.x - 40;
        gs->levi.hitboxAtaque.y = gs->levi.hitbox.y + gs->levi.hitbox.alto/2;
        gs->levi.hitboxAtaque.alto = 7;
        gs->levi.hitboxAtaque.ancho = 110;
        gs->levi.ataque = 100;
        gs->levi.ataqueNuca = 10000;
        gs->levi.puntuacionTitan = 100;
        gs->levi.puntuacionNuca = 500;
        gs->levi.invulnerabilidad = false;

        if(gs->input.ClickIzq && gs->levi.cooldownAtaque <= 0)
        {
            gs->levi.cooldownAtaque = 0.75f;
            gs->levi.leviAtacando = true;
            gs->input.ClickIzq = false;
            cambiar_animacion(gs,ODM_ATAQUE_BASICO);
        }
    }

    else
    {
        if(!gs->levi.dash.activo)
            gs->levi.invulnerabilidad = false;
        gs->levi.habilitaAumentaDash = true;
        gs->levi.leviAtacando = false;

        if(gs->levi.hitboxODM.x >= gs->levi.hitbox.x + gs->levi.hitbox.ancho/2)
            gs->levi.viendoDerecha = 1;
        else
            gs->levi.viendoDerecha = 0;

        if(gs->levi.viendoDerecha == 1)
        {
            gs->levi.hitboxAtaque.x = gs->levi.hitbox.x + gs->levi.hitbox.ancho;
            gs->levi.hitboxAtaque.y = gs->levi.hitbox.y + 15;
            gs->levi.hitboxAtaque.alto = 5;
            gs->levi.hitboxAtaque.ancho = 40;
        }
        else if(gs->levi.viendoDerecha == 0)
        {
            gs->levi.hitboxAtaque.x = gs->levi.hitbox.x - gs->levi.hitboxAtaque.ancho;
            gs->levi.hitboxAtaque.y = gs->levi.hitbox.y + 15;
            gs->levi.hitboxAtaque.alto = 5;
            gs->levi.hitboxAtaque.ancho = 40;
        }

        if(gs->input.ClickIzq && gs->levi.cooldownAtaque <= 0)
        {
            gs->levi.ataque = 100;
            gs->levi.ataqueNuca = 10000;
            gs->levi.puntuacionTitan = 100;
            gs->levi.puntuacionNuca = 500;

            if(!gs->levi.viendoDerecha && !gs->levi.ODM.activo)
                gs->levi.animacion.rotarAnim = true;
            else if(gs->levi.viendoDerecha && !gs->levi.ODM.activo)
                gs->levi.animacion.rotarAnim = false;


            if(gs->levi.estadoLevi == ODM)
                cambiar_animacion(gs, ODM_NORMAL_ATAQUE_BASICO);
            else
                cambiar_animacion(gs, ATAQUE_BASICO);

            gs->levi.cooldownAtaque = 0.75f;
            gs->levi.leviAtacando = true;
            gs->input.ClickIzq = false;
        }
    }   
    
}

void colision_levi_ataque(s_GameState *gs)
{
    //Detecta los ataques de levi a los titanes y los desactiva si los mata / da recompensas

    int i, pA = gs->pantalla_actual;

    if(gs->levi.cooldownAtaque > 0)
        gs->levi.cooldownAtaque -= 1.0f/FPS;

    if(gs->levi.cooldownHabilidad1 > 0)
        gs->levi.cooldownHabilidad1 -= 1.0f/FPS;

    if(gs->levi.cooldownHabilidad2 > 0)
        gs->levi.cooldownHabilidad2 -= 1.0f/FPS;

    if(gs->levi.tiempoInvulnerabilidad > 0)
        gs->levi.tiempoInvulnerabilidad -= 1.0f/FPS;

    if (gs->audio.cdSfxAttack > 0)
        gs->audio.cdSfxAttack -= 1.0/FPS;

    if(gs->levi.agarrado == true)
        return;

    levi_ataques(gs);
    parry(gs);

    if((gs->tutorialEjecutando && gs->tutorial.fase == 8 && gs->levi.estadoLevi != ODM_ATAQUE1 && gs->levi.estadoLevi != ODM_ATAQUE2) || (gs->tutorialEjecutando && gs->tutorial.fase < 4))
        return;

    if(gs->levi.leviAtacando)
    {
        for(i=0;i<gs->pantalla[pA].num_entidades;i++)
        {
            if(gs->pantalla[pA].entidades[i].activo == false)
                continue;
            if(colision(gs, gs->levi.hitboxAtaque, gs->pantalla[pA].entidades[i].hitboxNuca)) //Comprueba si colisiona en la nuca, si es asi rompe el bucle
            {
                if(gs->tutorialEjecutando && gs->tutorial.fase == 6)
                    gs->tutorial.requisitoCumplido = true;

                if(gs->audio.cdSfxAttack <= 0)
                {
                    al_play_sample(gs->audio.sfx_attack, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    gs->audio.cdSfxAttack = 0.5f;
                }

                gs->pantalla[pA].entidades[i].vida -= gs->levi.ataqueNuca * gs->levi.ataqueMA;

                if(gs->pantalla[pA].entidades[i].vida <= 0) //Desactiva al titan en caso de que no tenga vida
                {
                    gs->pantalla[pA].entidades[i].activo = false;
                    gs->levi.puntuacion += gs->levi.puntuacionNuca;
                    if(gs->levi.habilitaAumentaDash)
                    {
                        gs->levi.dash.flagDash++;
                        if(gs->levi.aumentaMA < 10)
                            gs->levi.aumentaMA++;
                    }
                }

                continue;
            }

            if(colision(gs, gs->levi.hitboxAtaque, gs->pantalla[pA].entidades[i].hitboxTitan)) //Comprueba si pega en cualquier parte de la hitbox del titan
            {
                if(gs->tutorial.fase == 6)
                    continue;

                if(gs->audio.cdSfxAttack <= 0)
                {
                    al_play_sample(gs->audio.sfx_attack, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    gs->audio.cdSfxAttack = 0.5f;
                }

                gs->pantalla[pA].entidades[i].vida -= gs->levi.ataque * gs->levi.ataqueMA;

                if(gs->pantalla[pA].entidades[i].vida <= 0) //Desactiva al titan en caso de que no tenga vida
                {
                    gs->pantalla[pA].entidades[i].activo = false;
                    gs->levi.puntuacion += gs->levi.puntuacionTitan;
                }
            }
        }

        if(colision(gs, gs->levi.hitboxAtaque, gs->titanHembra.hitbox)) //Comprueba si pega en cualquier parte de la hitbox del titan
        {
            if(gs->audio.cdSfxAttack <= 0)
            {
                al_play_sample(gs->audio.sfx_attack, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                gs->audio.cdSfxAttack = 0.5f;
            }

            gs->titanHembra.vida -= gs->levi.ataque * gs->levi.ataqueMA;

            if(gs->titanHembra.vida <= 0) //Desactiva al titan en caso de que no tenga vida
            {
                gs->levi.puntuacion += 1000;
                gs->nivelCompletado = true;
            }
        }
    }
}

void parry(s_GameState *gs)
{
    //El parry es una habilidad que se activa por un muy corto periodo de tiempo, si en este periodo logra bloquear un ataque de un titan antes que este acierte un ataque sobre levi le da una recompensa

    if(gs->levi.cooldownParry > 0)
        gs->levi.cooldownParry -= 1.0/FPS;

    if(gs->input.keyC) 
    {
        gs->input.keyC = false;
        if(gs->levi.cooldownParry <= 0) //Activa el parry y le asigna un tiempo donde no podra ocuparlo
        {
            gs->input.keyC = false;
            gs->levi.tiempoParryActivo = 0.5f;
            gs->levi.cooldownParry = 1.5f;
            gs->variables.bloquearControles = true;
            cambiar_animacion(gs, PARRY);
        }
    }

    if(gs->levi.tiempoParryActivo > 0 ) //Hitbox del parry
    {
        gs->levi.tiempoParryActivo -= 1.0f/FPS;
        gs->levi.parryHB.x = gs->levi.x + 20;
        gs->levi.parryHB.y = gs->levi.y + 10;
        gs->levi.parryHB.ancho = 80;
        gs->levi.parryHB.alto = 55;
    }
    else 
    {
        gs->levi.parryHB.x = 0;
        gs->levi.parryHB.y = 0;
        gs->levi.parryHB.ancho = 0;
        gs->levi.parryHB.alto = 0;
    }

    for(int i=0; i<gs->pantalla[gs->pantalla_actual].num_entidades;i++)
    {

        if(gs->levi.tiempoParryActivo <= 0) 
            break;

        //Verifica si el parry fue exitoso y le da sus recompensas
        if(colision(gs, gs->levi.parryHB, gs->pantalla[gs->pantalla_actual].entidades[i].hitboxAtaqueBasico) && gs->levi.tiempoParryActivo > 0
            && gs->levi.ataqueHecho == false && gs->pantalla[gs->pantalla_actual].entidades[i].activo) 
        {
            if(gs->tutorialEjecutando)
                gs->tutorial.requisitoCumplido = true;

            gs->levi.dash.cantDash++;
            gs->levi.cooldownHabilidad1 = 0;
            gs->levi.cooldownHabilidad2 = 0;
            gs->levi.tiempoInvulnerabilidad = 0.5f;
            gs->levi.tiempoParryActivo = 0;
            gs->levi.vida += 10;
            if(gs->levi.vida >= 50)
                gs->levi.vida = 50;
            gs->levi.cooldownParry = 0.1f;
            cambiar_animacion(gs, PARRY_EXITOSO);
        }
    }

    //Comportamiento especial del parry para el titan hembra (mas recompensas y ligera diferencia en la mecanica)
    if((colision(gs, gs->levi.parryHB, gs->titanHembra.hitboxAtaque1) || colision(gs, gs->levi.parryHB, gs->titanHembra.hitboxAtaque2)) && gs->levi.tiempoParryActivo > 0 && gs->levi.ataqueHecho == false)
    {
        gs->levi.dash.cantDash++;
        gs->levi.cooldownHabilidad1 = 0;
        gs->levi.cooldownHabilidad2 = 0;
        gs->levi.tiempoInvulnerabilidad = 0.5f;
        gs->levi.vida += 5;
        gs->levi.gasRestante += 50;
        if(gs->levi.gasRestante >= 1000)
            gs->levi.gasRestante = 1000;
        if(gs->levi.aumentaMA >= 10)
            gs->levi.aumentaMA = 10;
        if(gs->levi.vida >= 50)
            gs->levi.vida = 50;
        gs->levi.tiempoParryActivo = 0;
        gs->levi.cooldownParry = 0.3f;
        cambiar_animacion(gs, PARRY_EXITOSO);
    }
}

void aumenta_dash(s_GameState *gs)
{
    //Cada 2 cargas obtenidas al matar a un titan con un ataque basico en la nuca obtiene una carga del dash

    int numDash = 0;

    if(gs->levi.dash.flagDash >= 2)
    {
        if(gs->levi.dash.flagDash%2 == 0)
        {
            numDash = gs->levi.dash.flagDash / 2;
            gs->levi.dash.cantDash += numDash;
            gs->levi.dash.flagDash = 0;
        }
        else if(gs->levi.dash.flagDash % 2 != 0)
        {
            gs->levi.dash.flagDash--;
            numDash = gs->levi.dash.flagDash / 2;
            gs->levi.dash.cantDash += numDash;
            gs->levi.dash.flagDash = 0;
            gs->levi.dash.flagDash++;
        }
    }

}

void colision_levi_mapa(s_GameState *gs)
{
    int i, pA = gs->pantalla_actual;
    float distancia_izquierda = 0, distancia_derecha = 0, distancia_arriba = 0, distancia_abajo = 0;

    if(gs->levi.dash.frameActivacion) //Sirve para ignorar las colisiones del mapa por 1 frame
    {
        gs->levi.dash.frameActivacion = false;
        return;
    }

    gs->levi.levi_suelo = false;

    for(i=0;i<gs->pantalla[pA].num_hitbox;i++) //Bucle para comparar hitbox y encontrar la coincidente
    {
        if(colision(gs, gs->levi.hitbox, gs->pantalla[pA].hitbox[i])) //Comprueba si la hb de levi colisiona con la del mapa y actua para que no pueda traspasarlo y asignarle los valores correspondientes, ej: detectar que levi esta en el suelo
        {
            if(gs->levi.ODM.activo && gs->levi.ODM.dirY < 0)
                continue;
            //Define las distancias con las paredes
            distancia_izquierda = (gs->levi.hitbox.x + gs->levi.hitbox.ancho) - gs->pantalla[pA].hitbox[i].x;
            distancia_derecha = (gs->pantalla[pA].hitbox[i].x + gs->pantalla[pA].hitbox[i].ancho) - gs->levi.hitbox.x;
            distancia_arriba = (gs->levi.hitbox.y + gs->levi.hitbox.alto) - gs->pantalla[pA].hitbox[i].y;
            distancia_abajo = (gs->pantalla[pA].hitbox[i].y + gs->pantalla[pA].hitbox[i].alto) - gs->levi.hitbox.y;

            //Comprueba la menor distancia entre las paredes para detectar por donde choco
            if(distancia_izquierda < distancia_derecha && distancia_izquierda < distancia_arriba && distancia_izquierda < distancia_abajo)
                gs->levi.x = gs->pantalla[pA].hitbox[i].x - gs->levi.hitbox.ancho - LEVI_HB_RECORTE;

            else if(distancia_derecha < distancia_izquierda && distancia_derecha < distancia_arriba && distancia_derecha < distancia_abajo)
                gs->levi.x = gs->pantalla[pA].hitbox[i].x + gs->pantalla[pA].hitbox[i].ancho - LEVI_HB_RECORTE;

            else if(distancia_abajo < distancia_arriba && distancia_abajo < distancia_izquierda && distancia_abajo < distancia_derecha)
                gs->levi.y = gs->pantalla[pA].hitbox[i].y + gs->pantalla[pA].hitbox[i].alto - LEVI_HB_OFFSET_Y;

            else if(distancia_arriba < distancia_abajo && distancia_arriba < distancia_izquierda && distancia_arriba < distancia_derecha)
            {
                gs->levi.y = gs->pantalla[pA].hitbox[i].y - gs->levi.hitbox.alto - LEVI_HB_OFFSET_Y;
                gs->levi.velocidadY = 0;
                gs->levi.levi_suelo = true; //Habilita levi_suelo, lo que hace que lo habilite a dar un salto
                gs->levi.doble_salto = true;
                gs->levi.ODM.activo = false; //Calcular distancia entre levi y suelo y cancelar
                if(gs->levi.estadoLevi == CAYENDO || gs->levi.estadoLevi == SALIDA_DASH || gs->levi.estadoLevi == DASH || gs->levi.estadoLevi == SALIDA_ODM_ATAQUE1 ||
                    gs->levi.estadoLevi == SALIDA_ODM_ATAQUE2 || gs->levi.estadoLevi == ODM_ATAQUE_BASICO)
                {
                    if(gs->levi.distanciaYRecorrida > 250)
                    {
                        gs->levi.animacion.bloquearAnimacion = true;
                        gs->variables.bloquearControles = true;
                    }
                    cambiar_animacion(gs, ATERRIZANDO);
                    gs->variables.gravedad = 0.8f;
                    gs->levi.dash.activo = false;
                }
            }

        }
    }

    if(gs->levi.habilidad1Activa || gs->levi.habilidad2Activa || gs->levi.estadoLevi == SALIDA_ODM_ATAQUE1 || gs->levi.estadoLevi == SALIDA_ODM_ATAQUE2)
        return;

    for(i=0;i<gs->pantalla[pA].num_elementos;i++) //Verifica la colision con las casas y actua diferente (solo puede mantenerse en el techo, ademas de poder ocupar el equipo de maniobras/ODM)
    {
        if(gs->pantalla[pA].elementos[i].tipo == 3 && colision(gs, gs->levi.hitbox, gs->pantalla[pA].elementos[i].hitbox) && gs->pantalla[pA].elementos[i].activo == true)
        {
            distancia_arriba = (gs->levi.hitbox.y + gs->levi.hitbox.alto) - gs->pantalla[pA].elementos[i].hitbox.y;

            if(gs->levi.velocidadY >= 0 && distancia_arriba < 20 && gs->levi.levi_vuelo == false)
            {
                if(gs->tutorialEjecutando && gs->tutorial.fase == 2)
                    gs->tutorial.requisitoCumplido = true;

                gs->levi.y = gs->pantalla[pA].elementos[i].hitbox.y - gs->levi.hitbox.alto - LEVI_HB_OFFSET_Y;
                gs->levi.velocidadY = 0;
                gs->levi.levi_suelo = true;
                gs->levi.doble_salto = true;
                if(gs->levi.estadoLevi == CAYENDO || gs->levi.estadoLevi == SALIDA_DASH || gs->levi.estadoLevi == DASH || gs->levi.estadoLevi == SALIDA_ODM_ATAQUE1 ||
                    gs->levi.estadoLevi == SALIDA_ODM_ATAQUE2 || gs->levi.estadoLevi == ODM_ATAQUE_BASICO)
                {
                    if(gs->levi.distanciaYRecorrida > 250)
                    {
                        gs->levi.animacion.bloquearAnimacion = true;
                        gs->variables.bloquearControles = true;
                    }
                    cambiar_animacion(gs, ATERRIZANDO);
                }
            }

            if(gs->input.keyE) //Verifica si accede a la puerta de una casa(tutorial)
            {
                if(colision(gs, gs->levi.hitbox, gs->pantalla[pA].elementos[i].hitbox2))
                {
                    if(gs->tutorialEjecutando == true)
                        gs->nivelCompletado = true;
                    gs->input.keyE = false;
                }
            }
        }
    }
}

void genera_gas(s_GameState *gs)
{
    //Constantemente genera particulas de gas (efecto) y las guarda en un arreglo

    int i;

    for(i=0;i<MAXGAS;i++)
        if(gs->animaciones.gas[i].activo == false) //Inicializa un efecto de gas
        {
            gs->animaciones.gas[i].x = gs->levi.hitbox.x;
            gs->animaciones.gas[i].y = gs->levi.hitbox.y;
            gs->animaciones.gas[i].cantidadFrames = 8;
            gs->animaciones.gas[i].velocidadAnim = 9;
            gs->animaciones.gas[i].frameActual = 0;
            gs->animaciones.gas[i].contadorAnim = 0;
            gs->animaciones.gas[i].activo = true;
            return;
        }
}

void actualizar_gas(s_GameState *gs)
{
    //Mismo principio que todas las funciones de actualizar la animacion, esta vez para las particulas de gas

    int i;

    for(i=0;i<MAXGAS; i++)
    {
        if(gs->animaciones.gas[i].activo == false)
            continue;

        gs->animaciones.gas[i].contadorAnim++;

        if(gs->animaciones.gas[i].contadorAnim >= gs->animaciones.gas[i].velocidadAnim)
        {
            gs->animaciones.gas[i].contadorAnim = 0;
            gs->animaciones.gas[i].frameActual++;

            if(gs->animaciones.gas[i].frameActual >= gs->animaciones.gas[i].cantidadFrames) //Cuando termina la animacion del gas lo desactiva
                gs->animaciones.gas[i].activo = false;

        }
    }
}

void inicia_ODM(s_GameState *gs, float *cx, float *cy)
{
    //Funcion que inicia el equipo de maniobras (Calcula distancias y diagonales)

    if(gs->levi.ODM.activo)
    {
        gs->levi.ODM.velocidadODMPrevia = gs->levi.ODM.velocidadODM;
        gs->levi.velocidadX = gs->levi.ODM.dirX * gs->levi.ODM.velocidadODMPrevia;
        gs->levi.velocidadY = gs->levi.ODM.dirY * gs->levi.ODM.velocidadODMPrevia;
        gs->levi.dash.tiempoRecuperacionDash = 1;
        gs->levi.ODM.activo = false;
    }
    else
        gs->levi.ODM.velocidadODMPrevia = 1;

    *cx = gs->levi.hitboxODM.x - (gs->levi.x + gs->levi.hitbox.ancho); //Calcula cateto x 
    *cy = gs->levi.hitboxODM.y - (gs->levi.y + gs->levi.hitbox.alto); //Calcula cateto y
    gs->levi.ODM.distanciaRestanteEnganche = sqrt(*cx * *cx + *cy * *cy); //Calcula la hipotenusa (distancia de levi al punto)

    gs->levi.ODM.engancheActivo = true;
    gs->levi.ODM.dirX = *cx/gs->levi.ODM.distanciaRestanteEnganche;
    gs->levi.ODM.dirY = *cy/gs->levi.ODM.distanciaRestanteEnganche;
    gs->levi.ODM.velocidadODM = 1;
    gs->levi.levi_vuelo = true;
    gs->levi.ODM.puntoEngancheX = gs->levi.x + gs->levi.hitbox.ancho; 
    gs->levi.ODM.puntoEngancheY = gs->levi.y + gs->levi.hitbox.alto; 
    gs->levi.dash.frameActivacion = true;
    al_play_sample(gs->audio.sfx_odm, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
}

void colision_ODM(s_GameState *gs)
{
    //Verifica si el equipo de maniobras colisiono con algo en el que se pueda enganchar y calcula si este llego al destino

    int pA = gs->pantalla_actual, i;
    int flag = 0;
    float cx, cy;
    float mouseX = gs->levi.hitboxODM.x, mouseY = gs->levi.hitboxODM.y;

    if(gs->input.ClickDer && gs->levi.agarrado == false)
    {
        for(i=0;i<gs->pantalla[pA].num_entidades;i++)
            if(colision(gs, gs->levi.hitboxODM, gs->pantalla[pA].entidades[i].hitboxTitan) && gs->pantalla[pA].entidades[i].activo == true)
            {
                inicia_ODM(gs, &cx, &cy);
                gs->levi.habilidad1Activa = false;
                gs->levi.habilidad2Activa = false;
                gs->levi.ODM.engancheTitan = true;
                break;
            }

        for(i = 0; i < gs->pantalla[pA].num_elementos; i++)
            if((gs->pantalla[pA].elementos[i].tipo == 1 || gs->pantalla[pA].elementos[i].tipo == 3 || gs->pantalla[pA].elementos[i].tipo == 6) && colision(gs, gs->levi.hitboxODM, gs->pantalla[pA].elementos[i].hitbox))
            {
                inicia_ODM(gs, &cx, &cy);
                gs->levi.habilidad1Activa = false;
                gs->levi.habilidad2Activa = false;
                gs->levi.ODM.engancheNormal = true;
            }

        if(colision(gs, gs->levi.hitboxODM, gs->titanHembra.hitbox))
            {
                inicia_ODM(gs, &cx, &cy);
                gs->levi.habilidad1Activa = false;
                gs->levi.habilidad2Activa = false;
                gs->levi.ODM.engancheTitan = true;
            }
        
        gs->input.ClickDer = false;
    }

    if(gs->levi.ODM.engancheActivo)
    {
        if(gs->levi.agarrado)
            gs->levi.ODM.engancheActivo = false;
        
        gs->levi.ODM.puntoEngancheX += gs->levi.ODM.dirX * 15;
        gs->levi.ODM.puntoEngancheY += gs->levi.ODM.dirY * 15;

        gs->levi.ODM.distanciaRestanteEnganche -= 15;

        if(gs->levi.ODM.distanciaRestanteEnganche <= 0)
        {
            mouseX = gs->levi.ODM.puntoEngancheX;
            mouseY = gs->levi.ODM.puntoEngancheY;
            gs->levi.ODM.engancheActivo = false;

            if(gs->levi.gasRestante > 0)
            {
                gs->levi.ODM.activo = true;
                cx = mouseX - (gs->levi.x + gs->levi.hitbox.ancho);
                cy = mouseY - (gs->levi.y + gs->levi.hitbox.alto);
                gs->levi.ODM.distanciaRestanteODM = sqrt(cx*cx + cy*cy);

                if(cx < 0)
                    gs->levi.animacion.rotarAnim = true;
                else 
                    gs->levi.animacion.rotarAnim = false;

                if(gs->levi.ODM.engancheTitan)
                    cambiar_animacion(gs, ODMATAQUE);
                else if(gs->levi.ODM.engancheNormal)
                    cambiar_animacion(gs, ODM);
            }
            else
            {
                gs->levi.ODM.engancheNormal = false;
                gs->levi.ODM.engancheTitan = false;
            }
            
            for(i=0;i<5;i++) //Elemento estetico, si el ODM engancha, deja unas grietas guardada en un arreglo, si el arreglo esta lleno, corre a la derecha las posiciones y reemplaza la ultima como la primera
            {                    //de esta forma, siempre reemplaza la que mas tiempo lleve en pantalla
                if(gs->variables.grietas[i].x == 0)
                {
                    gs->variables.grietas[i].x = gs->levi.ODM.puntoEngancheX;
                    gs->variables.grietas[i].y = gs->levi.ODM.puntoEngancheY;
                    break;
                }
                else if(gs->variables.grietas[i].x != 0)
                    flag++;

                if(flag == 5)
                {
                    s_Posiciones aux = gs->variables.grietas[4];

                    for(i=4;i>0;i--)
                        gs->variables.grietas[i] = gs->variables.grietas[i-1];

                    gs->variables.grietas[0] = aux;
                    gs->variables.grietas[0].x = gs->levi.ODM.puntoEngancheX;
                    gs->variables.grietas[0].y = gs->levi.ODM.puntoEngancheY;
                }
            }
        }
    }

    if(gs->levi.ODM.activo)
    {
        //printf("ODM Activo\n");
        desactivar_hitbox(gs, 0.1f);
        gs->levi.gasRestante--;

        if(gs->levi.gasRestante > 0)
        {
            cx = gs->levi.ODM.puntoEngancheX - (gs->levi.x + gs->levi.hitbox.ancho); //Calcula cateto x 
            cy = gs->levi.ODM.puntoEngancheY - (gs->levi.y + gs->levi.hitbox.alto); //Calcula cateto y
            gs->levi.ODM.distanciaRestanteODM = sqrt(cx*cx + cy*cy);
            
            gs->levi.ODM.dirX = cx/gs->levi.ODM.distanciaRestanteODM;
            gs->levi.ODM.dirY = cy/gs->levi.ODM.distanciaRestanteODM;

            gs->levi.x += gs->levi.ODM.dirX * gs->levi.ODM.velocidadODM; //Desplaza a levi
            gs->levi.y += gs->levi.ODM.dirY * gs->levi.ODM.velocidadODM;

            gs->levi.ODM.distanciaRestanteODM -= gs->levi.ODM.velocidadODM; //Resta la distancia restante
            gs->levi.ODM.velocidadODM += 0.5f; //Aumenta la velocidad de levi
            genera_gas(gs);
        }


        if(gs->levi.ODM.distanciaRestanteODM <= 0 || gs->levi.agarrado || gs->levi.gasRestante <= 0)
        {
            gs->levi.ODM.engancheNormal = false;
            gs->levi.ODM.engancheTitan = false;
            gs->levi.ODM.activo = false;
            gs->levi.velocidadX = gs->levi.ODM.dirX * gs->levi.ODM.velocidadODM;
            gs->levi.velocidadY = gs->levi.ODM.dirY * gs->levi.ODM.velocidadODM;
            gs->levi.dash.tiempoRecuperacionDash = 1;
            gs->levi.levi_vuelo = false;

            if(gs->levi.habilidad1Activa || gs->levi.habilidad2Activa)
            {
                gs->levi.velocidadX = gs->levi.ODM.dirX * 30;
                gs->levi.velocidadY = gs->levi.ODM.dirY * 18;
            }
            else
                cambiar_animacion(gs, CAYENDO);
        }
    }
}

void levi_dash(s_GameState *gs)
{
    float cx, cy, distancia; //Catetos e hipotenusa
    float mouseX = gs->levi.hitboxODM.x, mouseY = gs->levi.hitboxODM.y;

    if(gs->input.keyF == true && gs->levi.dash.activo == false && gs->levi.dash.cooldown <= 0 && gs->levi.agarrado == false && gs->levi.dash.cantDash > 0) 
    {
        if(colision(gs, gs->levi.hitboxODM, gs->pantalla[gs->pantalla_actual].hitbox[0]) && gs->levi.levi_suelo) 
            return; //Evita bug de activar dash con el mouse en el suelo

        cx = mouseX - (gs->levi.x + gs->levi.hitbox.ancho); //Calcula cateto x 
        cy = mouseY - (gs->levi.y + gs->levi.hitbox.alto); //Calcula cateto y
        distancia = sqrt(cx*cx + cy*cy); //Calcula la hipotenusa (distancia de levi al punto)

        gs->levi.dash.activo = true;
        gs->levi.dash.distanciaRestante = 250; //Distancia fija a recorrer
        gs->levi.dash.cooldown = 1; 
        gs->levi.dash.dashX = cx/distancia; //Direccion x del dash 
        gs->levi.dash.dashY = cy/distancia; //Direccion y del dash 
        gs->levi.ODM.activo = false;
        if(gs->levi.dash.dashY < 0 && gs->levi.levi_suelo == true) //Si el dash es hacia arriba activa esta variable
            gs->levi.dash.frameActivacion = true; //Sirve para ignorar por 1 frame las colisiones del mapa, asi permite despegar el dash si esta en suelo

        gs->levi.dash.animDashActiva = true;
        gs->levi.dash.angulo = atan2(cy, cx);
        gs->levi.dash.x = gs->levi.x + 40;
        gs->levi.dash.y = gs->levi.y + 55;
        gs->variables.gravedad = 0;
        gs->levi.dash.cantDash--;

        if(cx < 0 )
            gs->levi.animacion.rotarAnim = true;
        else if(cx > 0 )
            gs->levi.animacion.rotarAnim = false;

        cambiar_animacion(gs, DASH);
        desactivar_hitbox(gs, 0.5f);

    }

    if(gs->levi.dash.activo)
    {
        gs->levi.invulnerabilidad = true; 
        if(gs->levi.agarrado)
            gs->levi.dash.distanciaRestante = 0;
        gs->input.keyF = false;
        gs->levi.x += gs->levi.dash.dashX*10; //Mueve a levi segun la direccion x y multiplica la velocidad 8
        gs->levi.y += gs->levi.dash.dashY*10; //Mueve a levi segun la direccion x y multiplica la velocidad 8
        gs->levi.dash.distanciaRestante -= 10; //Resta la distancia restante 

        if(gs->levi.dash.distanciaRestante <= 0) //Calcula el fin del dash
        {
            gs->levi.dash.activo = false;
            gs->levi.dash.tiempoRecuperacionDash = 0.3f;
            gs->levi.velocidadX = gs->levi.dash.dashX * 25; //Inercia horizontal
            gs->levi.velocidadY = gs->levi.dash.dashY * 18; //Inercia vertical

            cambiar_animacion(gs, SALIDA_DASH);
        }
    }

    if(gs->levi.dash.cooldown > 0) 
        gs->levi.dash.cooldown -= 1.0/FPS;
    
}

void colision_levi_dash(s_GameState *gs)
{
    int pA = gs->pantalla_actual, i;

    if(gs->tutorialEjecutando && gs->tutorial.fase < 9)
        return;

    gs->levi.dash.hitboxDash.x = gs->levi.x;
    gs->levi.dash.hitboxDash.y = gs->levi.y;
    gs->levi.dash.hitboxDash.ancho = 96;
    gs->levi.dash.hitboxDash.alto = 60;

    for(i=0;i<gs->pantalla[pA].num_entidades;i++)
    {
        if(gs->pantalla[pA].entidades[i].activo == false)
            continue;
        if(colision(gs, gs->levi.dash.hitboxDash, gs->pantalla[pA].entidades[i].hitboxNuca)) //Comprueba si pega en la nuca, si es asi rompe el bucle
        {
            gs->pantalla[pA].entidades[i].vida = 0;
            gs->pantalla[pA].entidades[i].activo = false;
            
            if(gs->pantalla[pA].entidades[i].vida <= 0)
            {
                gs->pantalla[pA].entidades[i].activo = false;
                gs->levi.puntuacion += 500; 
            }
            continue;
        }

        if(colision(gs, gs->levi.dash.hitboxDash, gs->pantalla[pA].entidades[i].hitboxTitan)) //Comprueba si pega en cualquier parte de la hitbox del titan
        {    
            gs->pantalla[pA].entidades[i].vida -= 75;

            if(gs->pantalla[pA].entidades[i].vida <= 0)
            {
                gs->pantalla[pA].entidades[i].activo = false;
                gs->levi.puntuacion += 100;
            }
        }

    }

    if(colision(gs, gs->levi.hitboxAtaque, gs->titanHembra.hitbox)) //Comprueba si pega en cualquier parte de la hitbox del titan
        {
            gs->titanHembra.vida -= 75;

            if(gs->titanHembra.vida <= 0) //Desactiva al titan en caso de que no tenga vida
            {
                gs->levi.puntuacion += 1000;
                gs->nivelCompletado = true;
            }
        }

}

void interactua_inventario(s_GameState *gs)
{
    if(gs->input.key3 && gs->levi.inventario.gasODM > 0)
    {
        gs->levi.inventario.gasODM--;
        gs->levi.gasRestante = 1000;
        gs->input.key3 = false;
    }
}

void colision_levi_elementos(s_GameState *gs)
{
    int i, pA = gs->pantalla_actual, nE = gs->pantalla[pA].num_elementos;

    for(i=0;i<nE;i++)
    {
        if(colision(gs, gs->levi.hitbox, gs->pantalla[pA].elementos[i].hitbox) && gs->pantalla[pA].elementos[i].activo)
        {
            if(gs->pantalla[pA].elementos[i].tipo == 2)
            {
                gs->levi.inventario.escudos++;
                gs->pantalla[pA].elementos[i].activo = false;
            }
            else if(gs->pantalla[pA].elementos[i].tipo == 5)
            {
                gs->levi.inventario.gasODM++;
                gs->pantalla[pA].elementos[i].activo = false;
            }
        }
    }
}

void camara_scroll(s_GameState *gs)
{
    float bordeDer = gs->camara.x + SCREEN_X - BORDE_CAM - 150; //Limite derecho donde empieza a scrollear la camara
    float bordeIzq = gs->camara.x + BORDE_CAM; //Limite izquierdo donde empieza a scrollear la camara
    float leviX = gs->levi.x + 48;
    float anchoPantalla = gs->pantalla[gs->pantalla_actual].ancho * TAM_CELDA;

    if(leviX < bordeIzq) //Calcula cuando toca el limite izquierdo para que empiece a scrollear la camara, para luego r
        gs->camara.x = leviX - BORDE_CAM; //Obtiene un valor de la camara para luego restarlo al dibujo y que la posicion de levi quede en donde deberia estar en una pantalla normal

    else if(leviX > bordeDer) //Calcula cuando toca el limite derecho para que empiece a scrollear la camara
        gs->camara.x = leviX - (SCREEN_X - BORDE_CAM - 150); //Obtiene un valor de la camara para luego restarlo al dibujo y que la posicion de levi quede en donde deberia estar en una pantalla normal

    if(gs->camara.x < 0) //Evita bugs
        gs->camara.x = 0;

    if(anchoPantalla > SCREEN_X) 
    {
        if(gs->camara.x > anchoPantalla - SCREEN_X) //Si levi se encuentra en el borde del mundo total no se mueve la camara
            gs->camara.x = anchoPantalla - SCREEN_X;
    }
    else
        gs->camara.x = 0;

}

void cambiar_animacion(s_GameState *gs, e_EstadoLevi nuevaAnim)
{
    bool interrumpible = false;

    if(gs->levi.estadoLevi == nuevaAnim) //Verifica si el estado de levi es nuevo, si no es asi, retorna
        return;

    if(gs->levi.agarrado == true)
        return;

    if(nuevaAnim == IDLE || nuevaAnim == CAMINANDO || nuevaAnim == CORRIENDO || nuevaAnim == SALTANDO || nuevaAnim == CAYENDO)
        interrumpible = true;

    if(interrumpible && gs->levi.animacion.bloquearAnimacion)
        return;

    if(gs->levi.estadoLevi == ATAQUE_BASICO_CAYENDO)
        gs->levi.animacion.frameActual = 1;
    else
        gs->levi.animacion.frameActual = 0;

    gs->levi.estadoLevi = nuevaAnim;
    gs->levi.animacion.contadorAnim = 0;

    switch(nuevaAnim) //Escoge una animacion segun el estado
    {
        case IDLE:

            if(!gs->levi.animacion.bloquearAnimacion)
            {
                gs->levi.animacion.cantidadFrames = 6;
                gs->levi.animacion.velocidadAnim = 10;
                gs->levi.animacion.repetir = true;
                gs->levi.animacion.fila_ss = 0;
                gs->levi.animacion.contadorAnim = 0;
                gs->levi.animacion.frameXRepetir = 0;
            }
            break;

        case CAMINANDO:

            if(!gs->levi.animacion.bloquearAnimacion)
            {
                gs->levi.animacion.cantidadFrames = 6;
                gs->levi.animacion.velocidadAnim = 10;
                gs->levi.animacion.repetir = true;
                gs->levi.animacion.fila_ss = LEVI_SS_ALTO*2;
                gs->levi.animacion.contadorAnim = 0;
                gs->levi.animacion.frameXRepetir = 0;
            }
            break;

        case CORRIENDO:

            if(!gs->levi.animacion.bloquearAnimacion)
            {
                gs->levi.animacion.cantidadFrames = 8;
                if(gs->levi.tiempoModoAckerman > 0)
                    gs->levi.animacion.velocidadAnim = 5;
                else
                    gs->levi.animacion.velocidadAnim = 7;
                gs->levi.animacion.repetir = true;
                gs->levi.animacion.fila_ss = LEVI_SS_ALTO;
                gs->levi.animacion.contadorAnim = 0;
                gs->levi.animacion.frameXRepetir = 0;
            }
            break;

        case ODM:


            gs->levi.animacion.cantidadFrames = 5;
            gs->levi.animacion.velocidadAnim = 3;
            gs->levi.animacion.repetir = true;
            gs->levi.animacion.fila_ss = LEVI_SS_ALTO * 6;
            gs->levi.animacion.frameXRepetir = 2;
            gs->levi.animacion.contadorAnim = 0;
            break;

        case ODMATAQUE:

            gs->levi.animacion.cantidadFrames = 8;
            gs->levi.animacion.velocidadAnim = 3;
            gs->levi.animacion.repetir = true;
            gs->levi.animacion.fila_ss = LEVI_SS_ALTO * 8;
            gs->levi.animacion.frameXRepetir = 5;
            gs->levi.animacion.contadorAnim = 0;
            break;

        case SALTANDO:

            if(!gs->levi.animacion.bloquearAnimacion)
            {
                gs->levi.animacion.cantidadFrames = 2;
                gs->levi.animacion.velocidadAnim = 4;
                gs->levi.animacion.repetir = false;
                gs->levi.animacion.fila_ss = LEVI_SS_ALTO * 3;
                gs->levi.animacion.contadorAnim = 0;
                gs->levi.animacion.frameXRepetir = 0;
            }
            break;  

        case CAYENDO:

            gs->levi.animacion.cantidadFrames = 6;
            gs->levi.animacion.velocidadAnim = 5;
            gs->levi.animacion.repetir = true;
            gs->levi.animacion.fila_ss = LEVI_SS_ALTO * 4;
            gs->levi.animacion.frameXRepetir = 2;
            gs->levi.animacion.contadorAnim = 0;
            break;  
        
        case ATERRIZANDO:

            gs->levi.animacion.cantidadFrames = 5;
            gs->levi.animacion.velocidadAnim = 5;
            gs->levi.animacion.repetir = false;
            gs->levi.animacion.fila_ss = LEVI_SS_ALTO * 5;
            gs->levi.animacion.contadorAnim = 0;
            break;  

        case DASH: 

            gs->levi.dash.animDash.cantidadFrames = 14;
            gs->levi.dash.animDash.velocidadAnim = 3;
            gs->levi.dash.animDash.repetir = false;
            gs->levi.dash.animDash.contadorAnim = 0;
            gs->levi.dash.animDash.frameActual = 0;
            break;   

        case SALIDA_DASH:

            gs->levi.animacion.bloquearAnimacion = true;
            gs->levi.animacion.cantidadFrames = 3;
            gs->levi.animacion.velocidadAnim = 7;
            gs->levi.animacion.repetir = false;
            gs->levi.animacion.fila_ss = LEVI_SS_ALTO * 7;
            gs->levi.animacion.contadorAnim = 0;
            break;  

        case SALIDA_TITAN_AGARRE:

            gs->levi.animacion.bloquearAnimacion = true;
            gs->levi.animacion.cantidadFrames = 6;
            gs->levi.animacion.velocidadAnim = 2;
            gs->levi.animacion.repetir = false;
            gs->levi.animacion.fila_ss = LEVI_SS_ALTO * 9;
            gs->levi.animacion.contadorAnim = 0;
            gs->levi.animacion.bloquearAnimacion = true;
            gs->variables.gravedad = 0;
            break; 

        case ODM_ATAQUE_BASICO:

            gs->levi.animacion.bloquearAnimacion = true;
            gs->levi.animacion.cantidadFrames = 8;
            gs->levi.animacion.velocidadAnim = 7;
            gs->levi.animacion.repetir = true;
            gs->levi.animacion.frameXRepetir = 5;
            gs->levi.animacion.fila_ss = LEVI_SS_ALTO * 10;
            gs->levi.animacion.contadorAnim = 0;
            break;

        case ODM_ATAQUE1:

            gs->levi.animacion.bloquearAnimacion = true;
            gs->levi.animacion.cantidadFrames = 10;
            gs->levi.animacion.velocidadAnim = 4;
            gs->levi.animacion.repetir = false;
            gs->levi.animacion.fila_ss = LEVI_SS_ALTO * 11;
            gs->levi.animacion.contadorAnim = 0;
            gs->variables.bloquearControles = true;
            break;
        
        case SALIDA_ODM_ATAQUE1:

            gs->levi.animacion.bloquearAnimacion = true;
            gs->levi.animacion.cantidadFrames = 6;
            gs->levi.animacion.velocidadAnim = 5;
            gs->levi.animacion.repetir = false;
            gs->levi.animacion.fila_ss = LEVI_SS_ALTO * 16;
            gs->levi.animacion.contadorAnim = 0;
            gs->variables.gravedad = 0;
            gs->variables.bloquearControles = true;
            break;

        case ODM_ATAQUE2:

            gs->levi.animacion.bloquearAnimacion = true;
            gs->levi.animacion.cantidadFrames = 17;
            gs->levi.animacion.velocidadAnim = 2;
            gs->levi.animacion.repetir = false;
            gs->levi.animacion.fila_ss = LEVI_SS_ALTO * 14;
            gs->levi.animacion.contadorAnim = 0;
            gs->variables.bloquearControles = true;
            break;
        
        case SALIDA_ODM_ATAQUE2:

            gs->levi.animacion.bloquearAnimacion = true;
            gs->levi.animacion.cantidadFrames = 4;
            gs->levi.animacion.velocidadAnim = 8;
            gs->levi.animacion.repetir = false;
            gs->levi.animacion.fila_ss = LEVI_SS_ALTO * 15;
            gs->levi.animacion.contadorAnim = 0;
            gs->levi.habilidad2Activa = false;
            gs->variables.gravedad = 0;
            gs->variables.bloquearControles = true;
            break;

        case ATAQUE_BASICO:

            gs->levi.animacion.bloquearAnimacion = true;
            gs->levi.animacion.cantidadFrames = 5;
            gs->levi.animacion.velocidadAnim = 5;
            gs->levi.animacion.repetir = false;
            gs->levi.animacion.fila_ss = LEVI_SS_ALTO * 12;
            gs->levi.animacion.contadorAnim = 0;
            gs->variables.bloquearControles = true;
            break;

        case ATAQUE_BASICO_CAYENDO:

            gs->levi.animacion.bloquearAnimacion = true;
            gs->levi.animacion.cantidadFrames = 5;
            gs->levi.animacion.velocidadAnim = 5;
            gs->levi.animacion.repetir = false;
            gs->levi.animacion.fila_ss = LEVI_SS_ALTO * 17;
            gs->levi.animacion.contadorAnim = 0;
            gs->variables.bloquearControles = true;
            break; 
            
        case ODM_NORMAL_ATAQUE_BASICO:

            gs->levi.animacion.bloquearAnimacion = false;
            gs->levi.animacion.cantidadFrames = 3;
            gs->levi.animacion.velocidadAnim = 8;
            gs->levi.animacion.repetir = false;
            gs->levi.animacion.fila_ss = LEVI_SS_ALTO * 13;
            gs->levi.animacion.contadorAnim = 0;
            gs->variables.bloquearControles = false;
            break;

        case PARRY:

            gs->levi.animacion.bloquearAnimacion = false;
            gs->levi.animacion.cantidadFrames = 5;
            gs->levi.animacion.velocidadAnim = 5;
            gs->levi.animacion.repetir = false;
            gs->levi.animacion.fila_ss = LEVI_SS_ALTO * 18;
            gs->levi.animacion.contadorAnim = 0;
            gs->levi.animacion.bloquearAnimacion = true;
            break;
    
        case PARRY_EXITOSO:

            gs->levi.animacion.bloquearAnimacion = true;
            gs->levi.animacion.cantidadFrames = 6;
            gs->levi.animacion.velocidadAnim = 2;
            gs->levi.animacion.repetir = false;
            gs->levi.animacion.fila_ss = LEVI_SS_ALTO * 9;
            gs->levi.animacion.contadorAnim = 0;
            gs->variables.gravedad = 0;
            break; 
            
    }   

}

void actualizar_animacion(s_GameState *gs)
{
    if(gs->levi.agarrado)
        return;

    gs->levi.animacion.contadorAnim++;

    if(gs->levi.animacion.contadorAnim >= gs->levi.animacion.velocidadAnim) //Verifica si el contador llega a la velocidad para cambiar de frame
    {
        gs->levi.animacion.contadorAnim = 0;
        gs->levi.animacion.frameActual++;

        
        if(gs->levi.estadoLevi == SALIDA_ODM_ATAQUE1 && gs->levi.animacion.frameActual == 2)
            gs->variables.gravedad = 0.8;

        if(gs->levi.animacion.frameActual >= gs->levi.animacion.cantidadFrames) //Verifica si llego al fin de la animacion
            fin_animacion(gs);
    }

    if(gs->levi.dash.animDashActiva)
    {
        if(gs->levi.estadoLevi == ATERRIZANDO)
            gs->levi.dash.animDashActiva = false;

        gs->levi.dash.animDash.contadorAnim++;

        if(gs->levi.dash.animDash.contadorAnim >= gs->levi.dash.animDash.velocidadAnim)
        {
            gs->levi.dash.animDash.contadorAnim = 0;
            gs->levi.dash.animDash.frameActual++;

            if(gs->levi.dash.animDash.frameActual >= gs->levi.dash.animDash.cantidadFrames)
                gs->levi.dash.animDashActiva = false;
        }
    }
}

void fin_animacion(s_GameState *gs)
{
    if(gs->levi.animacion.repetir == true) //Repite la animacion
    {
        gs->levi.animacion.frameActual = gs->levi.animacion.frameXRepetir;
        gs->levi.animacion.bloquearAnimacion = false;
        gs->variables.bloquearControles = false;
    }

    else if(gs->levi.estadoLevi == SALIDA_DASH || gs->levi.estadoLevi == ATAQUE_BASICO || gs->levi.estadoLevi == SALIDA_ODM_ATAQUE1 
        || gs->levi.estadoLevi == SALIDA_ODM_ATAQUE2 || gs->levi.estadoLevi == SALIDA_TITAN_AGARRE || gs->levi.estadoLevi == PARRY
        || gs->levi.estadoLevi == PARRY_EXITOSO)
    {
        gs->levi.animacion.bloquearAnimacion = false;
        gs->variables.bloquearControles = false;
        gs->variables.gravedad = 0.8;
        gs->levi.ataqueHecho = false;

        if(gs->levi.estadoLevi == SALIDA_ODM_ATAQUE1 || gs->levi.estadoLevi == SALIDA_ODM_ATAQUE2)
            gs->levi.animacion.rotarAnim = true;

        if(gs->levi.levi_suelo)
            cambiar_animacion(gs, IDLE);
        else    
        {
            cambiar_animacion(gs, CAYENDO);
        }
    }

    else if(gs->levi.estadoLevi == ODM_ATAQUE2)
    {
        gs->levi.habilidad2Activa = false;
        gs->variables.gravedad = 0;
        cambiar_animacion(gs,SALIDA_ODM_ATAQUE2);
    }

    else if(gs->levi.estadoLevi == ODM_ATAQUE1)
    {
        gs->levi.habilidad1Activa = false;
        gs->variables.gravedad = 0;
        cambiar_animacion(gs,SALIDA_ODM_ATAQUE1);
    }

    else if(gs->levi.estadoLevi == ATERRIZANDO)
    {
        gs->levi.animacion.bloquearAnimacion = false;
        gs->variables.bloquearControles = false;
        cambiar_animacion(gs,IDLE);
    }
    
    else if(gs->levi.estadoLevi == ATAQUE_BASICO_CAYENDO)
    {
        gs->variables.bloquearControles = false;
        cambiar_animacion(gs, CAYENDO);
    }

    else
    {
        gs->levi.animacion.frameActual = gs->levi.animacion.cantidadFrames - 1; //Lo deja en el ultimo frame
        gs->variables.bloquearControles = false;
    }

}

void activar_hitbox(s_GameState *gs)
{
    int i, pA = gs->pantalla_actual, nE = gs->pantalla[pA].num_elementos;

    if(gs->variables.cooldownHitbox > 0)
        gs->variables.cooldownHitbox -= 1.0/FPS;

    if(gs->variables.cooldownHitbox <= 0)
        for(i=0;i<nE;i++)
            if(gs->pantalla[pA].elementos[i].activo == false && gs->pantalla[pA].elementos[i].tipo != 2 && gs->pantalla[pA].elementos[i].tipo != 5 )
                gs->pantalla[pA].elementos[i].activo = true;

}

void desactivar_hitbox(s_GameState *gs, float tiempo)
{
    //Sirve para desactivar las hitboxes momentanemente segun el tiempo que le pases como parametro, esto para evitar colisiones si ocupa el equipo de maniobras o dash, ademas de poder bajar de la casa con la S

    for(int i=0;i<gs->pantalla[gs->pantalla_actual].num_elementos;i++)
        if(gs->pantalla[gs->pantalla_actual].elementos[i].tipo == 3)
        {
            gs->pantalla[gs->pantalla_actual].elementos[i].activo = false;
            gs->variables.cooldownHitbox = tiempo;
            gs->variables.desactivarHitbox = true;
        }
}

void aumenta_puntuacion(s_GameState *gs, int i, char *tipo)
{
    if(strcmp(tipo,"ataque/nuca") == 0)
    {
        gs->levi.puntuacion += 500;
        gs->levi.dash.flagDash++;
    }

    if(strcmp(tipo,"dash/nuca"))
    {
        gs->levi.puntuacion += 500;
        gs->levi.dash.flagDash++;
    }

    if(strcmp(tipo,"ataque/cuerpo"))
    {
        gs->levi.puntuacion += 100;
    }

    if(strcmp(tipo,"dash/cuerpo"))
    {
        gs->levi.puntuacion += 500;
    }

}

void actualizar_transicion(s_GameState *gs)
{

    if(gs->animaciones.transicion.activo == true)
    {
        gs->animaciones.transicion.contadorAnim++;

        if(gs->animaciones.transicion.contadorAnim >= gs->animaciones.transicion.velocidadAnim)
        {
            gs->animaciones.transicion.contadorAnim = 0;
            gs->animaciones.transicion.frameActual++;

            if(gs->animaciones.transicion.frameActual >= gs->animaciones.transicion.cantidadFrames)
                gs->animaciones.transicion.activo = false;
        }
    }

}

void actualizar_transicion2(s_GameState *gs)
{

    if(gs->animaciones.transicion2.activo == true)
    {
        gs->animaciones.transicion2.contadorAnim++;

        if(gs->animaciones.transicion2.contadorAnim >= gs->animaciones.transicion2.velocidadAnim)
        {
            gs->animaciones.transicion2.contadorAnim = 0;
            gs->animaciones.transicion2.frameActual++;

            if(gs->animaciones.transicion2.frameActual >= gs->animaciones.transicion2.cantidadFrames)
                gs->estadoPantalla = PANTALLA_GAME_OVER;
        }
    }

}

int carga_puntuacion(s_GameState *gs)
{
    FILE* fpunt = fopen("rankingNivel1.txt","r");
    int cantidad = 0;

    if(fpunt == NULL)
        return 0;

    //Carga las puntuaciones y las guarda en el arreglo, a la vez, obtiene la cantidad de puntuaciones que hay en el archivo
    while (cantidad < 10 && fscanf(fpunt, "%s %d", gs->puntuaciones[cantidad].nombre, &gs->puntuaciones[cantidad].puntuacion) == 2) 
    {
        cantidad++;
    }

    fclose(fpunt);
    return cantidad;
}

void guarda_puntuacion(s_GameState *gs, int cantidad)
{
    FILE* fpunt = fopen("rankingNivel1.txt","w");
    int i;

    if(gs->levi.inventario.escudos >= 10)
        gs->levi.puntuacion += 2000;

    gs->puntuacionJugador.puntuacion = gs->levi.puntuacion;

    if (fpunt == NULL)
        return;

    for(i=9; i>0; i--)
    {
        if(gs->puntuacionJugador.puntuacion > gs->puntuaciones[i-1].puntuacion) //Si la puntuacion del jugador es mayor mueve hacia abajo la que estaba
            gs->puntuaciones[i] = gs->puntuaciones[i-1];
        else 
            break; //Cuando ya no es mayor que la que sigue se rompe
    }

    gs->puntuaciones[i] = gs->puntuacionJugador; //Reemplaza la posicion correspondiente en el arreglo con la puntuacion obtenida por el jugador

    if(cantidad < 10)
        cantidad++;

    for(i=0;i<cantidad;i++) //Guarda exactamente la cantidad necesaria en el archivo
    {
        fprintf(fpunt, "%s %d\n", gs->puntuaciones[i].nombre, gs->puntuaciones[i].puntuacion); 
    }
    
    fclose(fpunt);
}

void modo_ackerman(s_GameState *gs)
{
    if(gs->levi.tiempoModoAckerman > 0)
    {
        gs->levi.tiempoModoAckerman -= 1.0f/FPS;
        gs->levi.contModoAckerman++;
        if(gs->levi.contModoAckerman >= 60 && gs->levi.vida <= 48)
        {
            gs->levi.vida += 2;
            gs->levi.contModoAckerman = 0;
        }
        gs->levi.ataqueMA = 3;
        gs->levi.velocidadMA = 1.5f;
        if(gs->levi.cooldownHabilidad1 > 2.5)
            gs->levi.cooldownHabilidad1 = 2.5;
        if(gs->levi.cooldownHabilidad2 > 2.5)
            gs->levi.cooldownHabilidad2 = 2.5;
    }
    else
    {
        if(gs->input.keyX && gs->levi.aumentaMA > 0)
        {
            gs->levi.tiempoModoAckerman = gs->levi.aumentaMA;
            gs->input.keyX = false;
            gs->levi.aumentaMA = 0;
            gs->levi.dash.cantDash += 3;
            if(gs->levi.estadoLevi == CORRIENDO)
                gs->levi.animacion.velocidadAnim = 5;
        }
        else 
        {
            gs->levi.ataqueMA = 1;
            gs->levi.velocidadMA = 1;
            if(gs->levi.estadoLevi == CORRIENDO)
                gs->levi.animacion.velocidadAnim = 7;
        }
    }

}

void ingresa_nombre(s_GameState *gs, ALLEGRO_EVENT* evento)
{
    int largoNombre;

    if(evento->type == ALLEGRO_EVENT_KEY_CHAR && gs->variables.ingresandoNombre)
    {
        largoNombre = strlen(gs->variables.nombreTemp);

        switch(evento->keyboard.keycode)
        {
            case ALLEGRO_KEY_ENTER: //Si pulsa enter se ingresa el nombre
                strcpy(gs->puntuacionJugador.nombre, gs->variables.nombreTemp);
                gs->variables.ingresandoNombre = false;
                gs->variables.nombreIngresado = true;
                break;
            case ALLEGRO_KEY_BACKSPACE: //Si pulsa BackSpace se borra 1 letra
                if(largoNombre > 0)
                    gs->variables.nombreTemp[largoNombre - 1] = '\0'; 
                break;
            case ALLEGRO_KEY_SPACE: //No deja poner espacio en los nombres, para evitar bugs
                break;
            default: //Agrega la letra presionada al nombre
                if(largoNombre < 19)
                {
                    gs->variables.nombreTemp[largoNombre] = evento->keyboard.unichar;
                    gs->variables.nombreTemp[largoNombre + 1] = '\0';
                }
                break;
        }
    }
}
//================================================//