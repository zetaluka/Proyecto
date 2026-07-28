#include "commons.h"

//====Prototipos====//
void update_jugando(s_GameState *gs, s_Assets *assets);
void update_menu(s_GameState *gs, s_Assets *assets);
void update_tiempo_jugado(s_GameState* gs);
void update_levi_movimiento(s_GameState *gs);
void transicion_pantalla(s_GameState *gs, s_Assets *assets);
void hitbox_levi(s_GameState *gs, s_Assets *assets);
void levi_dash(s_GameState *gs);
void camara_scroll(s_GameState *gs);
void cuadrado_prueba (s_GameState *gs);
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
void verifica_estado_nivel(s_GameState *gs);

//====Funcion principal====//
void update(s_GameState *gs, s_Assets *assets)
{
    switch(gs->estadoPantalla) //Detecta en que estado esta, ejemplo: Menu, jugando, pausa, etc.
    {
        case PANTALLA_MENU:
            update_menu(gs, assets);
            break;

        case PANTALLA_JUGANDO:
            update_jugando(gs,assets);
            break;

        case PANTALLA_GAME_OVER:
            break;
    }

}

void update_menu(s_GameState *gs, s_Assets *assets)
{
    
}


void update_jugando(s_GameState *gs, s_Assets *assets) //Funcion si para cuando se este en la pantalla de juego
{
    update_tiempo_jugado(gs);
    update_levi_movimiento(gs);
    //actualizar_estado_levi(gs);
    hitbox_levi(gs,assets);
    hitbox_mouse(gs);
    levi_dash(gs);
    if(gs->variables.desactivarHitbox == true)
        activar_hitbox(gs);
    comprueba_colision(gs);
    aumenta_dash(gs);
    actualizar_animacion(gs);
    actualizar_gas(gs);
    camara_scroll(gs);
    transicion_pantalla(gs, assets);
    verifica_estado_nivel(gs);

    return;
}

void verifica_estado_nivel(s_GameState *gs)
{
    if(gs->nivelCompletado == true)
        gs->estadoPantalla = PANTALLA_MENU;

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
    if(gs->levi.estadoLevi != DASH && gs->levi.estadoLevi != SALIDA_DASH && gs->levi.estadoLevi != ODM_ATAQUE1 && gs->levi.estadoLevi != ODM_ATAQUE2
        && gs->levi.estadoLevi != SALIDA_ODM_ATAQUE1 && gs->levi.estadoLevi != SALIDA_ODM_ATAQUE2 && gs->levi.agarrado == false)
        gs->variables.gravedad = 0.8;

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

    bool bloqueaDerecha = false, bloqueaIzquierda = false;
    //printf("%f",gs->levi.velocidadY);

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

    if(gs->levi.dash.tiempoRecuperacionDash >= 0)
    {
        //printf("recuperacion=%f\n", gs->levi.dash.tiempoRecuperacionDash);

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

    else
    {
        gs->levi.levi_vuelo = false;

        if(gs->input.keyLShift && gs->input.keyD && !gs->variables.bloquearControles) //Si mantiene el LShift corre
        {
            gs->levi.x += 4.5f;
            if(gs->levi.levi_suelo && !gs->levi.animacion.bloquearAnimacion)
                cambiar_animacion(gs, CORRIENDO); //Hacer que bloquear controles sea falso y quitar condicion de bloquearAnimacion, y bloquear controles solo cuando caiga cierta distancia
            gs->levi.animacion.rotarAnim = false;
        }
            
        else if(gs->input.keyLShift && gs->input.keyA && !gs->variables.bloquearControles) //Si mantiene el LShift corre
        {
            gs->levi.x -= 4.5f;
            if(gs->levi.levi_suelo && !gs->levi.animacion.bloquearAnimacion)
                cambiar_animacion(gs, CORRIENDO);
            gs->levi.animacion.rotarAnim = true;
        }

        else if(gs->input.keyA && !gs->variables.bloquearControles) //Camina izquierda
        {
            gs->levi.x -= 2;
            if(gs->levi.levi_suelo && !gs->levi.animacion.bloquearAnimacion)
                cambiar_animacion(gs, CAMINANDO);
            gs->levi.animacion.rotarAnim = true;
        }

        else if(gs->input.keyD && !gs->variables.bloquearControles) //Camina derecha
        {
            gs->levi.x += 2;
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

void transicion_pantalla(s_GameState *gs, s_Assets *assets) //Efecto de transicion por pantallas
{
    float anchoPantalla = gs->pantalla[gs->pantalla_actual].ancho * TAM_CELDA;
    
    if(gs->levi.x+40 >= anchoPantalla && gs->pantalla_actual < MAXPANTALLAS - 1 && gs->pantalla_actual == 2 && gs->animaciones.transicion.activo == false)
    {   
        gs->animaciones.transicion.activo = true;
        gs->animaciones.transicion.cantidadFrames = 38;
        gs->animaciones.transicion.contadorAnim = 0;
        gs->animaciones.transicion.frameActual = 0;
        gs->animaciones.transicion.velocidadAnim = 2;
        gs->animaciones.cambioPantallaHecho = false;
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

    /*if(gs->levi.x+35 <= 0 && gs->pantalla_actual > 0)
    {
        gs->pantalla_actual--;
        anchoPantalla = gs->pantalla[gs->pantalla_actual].ancho * TAM_CELDA;
        gs->levi.x = anchoPantalla - 70;

        if(gs->pantalla[gs->pantalla_actual].pantallaCargada == false)
        {
            hitbox_init(gs);
            gs->pantalla[gs->pantalla_actual].pantallaCargada = true;
        }

        for(int i=0;i<5;i++)
        {
            gs->variables.grietas[i].x = 0;
            gs->variables.grietas[i].y = 0;
        }
    }*/

     if(gs->pantalla_actual > gs->variables.carga_pantalla) //Servia para detectar si la pantalla habia sido cargada 
    {
        mapa1(gs, assets);
        gs->variables.carga_pantalla++;
        
        if(gs->pantalla[gs->pantalla_actual].pantallaCargada == false)
        {
            hitbox_init(gs);
            gs->pantalla[gs->pantalla_actual].pantallaCargada = true;
        }
    }

}

void hitbox_levi(s_GameState *gs, s_Assets *assets) //Actualiza la hitbox del personaje principal
{
    gs->levi.hitbox.alto = LEVI_SS_ALTO - LEVI_HB_OFFSET_Y;  
    gs->levi.hitbox.ancho = LEVI_HB_RECORTE - 18;
    gs->levi.hitbox.x = gs->levi.x + LEVI_HB_RECORTE + 3;
    gs->levi.hitbox.y = gs->levi.y + LEVI_HB_OFFSET_Y;
}

void cuadrado_prueba (s_GameState *gs)
{
    if(gs->variables.cambioSentido == false)
        gs->pantalla[0].hitbox[3].y -= 2;
    else if(gs->variables.cambioSentido == true)
        gs->pantalla[0].hitbox[3].y += 2;

    if(gs->pantalla[0].hitbox[3].y >= (SCREEN_Y - gs->pantalla[0].hitbox[0].alto - 50))
        gs->variables.cambioSentido = false;
    if(gs->pantalla[0].hitbox[3].y <= 500)
        gs->variables.cambioSentido = true;
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
    int pA = gs->pantalla_actual;

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
    colision_levi_titan(gs); //Funcion en entities.c
    colision_levi_ataque(gs);
    colision_ODM(gs);
    if(gs->levi.dash.activo == true || gs->levi.estadoLevi == SALIDA_DASH)
        colision_levi_dash(gs);
    colision_levi_elementos(gs);
    //Hacer funcion de detectar colision entre titan y estructuras(del mapa) para ponerles gravedad

}

void levi_ataques(s_GameState *gs)
{
    gs->levi.leviAtacando = false;

    if(gs->levi.cooldownAtaque > 0)
        gs->levi.cooldownAtaque -= 1.0f/FPS;

    if(gs->levi.cooldownHabilidad1 > 0)
        gs->levi.cooldownHabilidad1 -= 1.0f/FPS;

    if(gs->levi.cooldownHabilidad2 > 0)
        gs->levi.cooldownHabilidad2 -= 1.0f/FPS;

    if(gs->levi.ODM.activo && gs->input.key1 && gs->levi.cooldownHabilidad1 <= 0)
        gs->levi.habilidad1Activa = true;
    if(gs->levi.ODM.activo && gs->input.key2 && gs->levi.cooldownHabilidad2 <= 0)
        gs->levi.habilidad2Activa = true;

    if(gs->levi.habilidad1Activa)
    {
        gs->levi.hitboxAtaque.x = gs->levi.x + 20;
        gs->levi.hitboxAtaque.y = gs->levi.y + 5;
        gs->levi.hitboxAtaque.alto = 60;
        gs->levi.hitboxAtaque.ancho = 90;
        gs->input.key1 = false;
        gs->levi.leviAtacando = true;
        //gs->levi.cooldownHabilidad1 = 20;
        cambiar_animacion(gs, ODM_ATAQUE1);
    }

    else if(gs->levi.habilidad2Activa)
    {
        gs->levi.hitboxAtaque.x = gs->levi.x + 20;
        gs->levi.hitboxAtaque.y = gs->levi.y;
        gs->levi.hitboxAtaque.alto = 75;
        gs->levi.hitboxAtaque.ancho = 85;
        gs->input.key2 = false;
        gs->levi.leviAtacando = true;
        //gs->levi.cooldownHabilidad2 = 30;
        cambiar_animacion(gs, ODM_ATAQUE2);
    }

    else if(gs->levi.ODM.engancheTitan)
    {
        gs->levi.hitboxAtaque.x = gs->levi.hitbox.x - 40;
        gs->levi.hitboxAtaque.y = gs->levi.hitbox.y + gs->levi.hitbox.alto/2;
        gs->levi.hitboxAtaque.alto = 7;
        gs->levi.hitboxAtaque.ancho = 110;

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
        if(gs->levi.hitboxODM.x >= gs->levi.hitbox.x + gs->levi.hitbox.ancho/2)
            gs->levi.viendoDerecha = 1;
        else
            gs->levi.viendoDerecha = 0;

        if(gs->levi.viendoDerecha == 1)
        {
            gs->levi.hitboxAtaque.x = gs->levi.hitbox.x + gs->levi.hitbox.ancho;
            gs->levi.hitboxAtaque.y = gs->levi.hitbox.y + gs->levi.hitbox.alto/2;
            gs->levi.hitboxAtaque.alto = 5;
            gs->levi.hitboxAtaque.ancho = 40;
        }
        else if(gs->levi.viendoDerecha == 0)
        {
            gs->levi.hitboxAtaque.x = gs->levi.hitbox.x - gs->levi.hitboxAtaque.ancho;
            gs->levi.hitboxAtaque.y = gs->levi.hitbox.y + gs->levi.hitbox.alto/2;
            gs->levi.hitboxAtaque.alto = 5;
            gs->levi.hitboxAtaque.ancho = 40;
        }

        if(gs->input.ClickIzq && gs->levi.cooldownAtaque <= 0)
        {
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
    int i, pA = gs->pantalla_actual;

    if(gs->levi.agarrado == true)
        return;

    levi_ataques(gs);

    if(gs->levi.leviAtacando)
    {
        printf("Ataque\n");
        for(i=0;i<gs->pantalla[pA].num_entidades;i++)
        {
            if(gs->pantalla[pA].entidades[i].activo == false)
                continue;
            if(colision(gs, gs->levi.hitboxAtaque, gs->pantalla[pA].entidades[i].hitboxNuca)) //Comprueba si pega en la nuca, si es asi rompe el bucle
            {
                printf("Colisiono en la nuca\n");
                printf("Titan %d vida antes: %d\n", i, gs->pantalla[pA].entidades[i].vida);

                if(gs->levi.habilidad1Activa)
                {
                    gs->pantalla[pA].entidades[i].vida -= 100;

                    if(gs->pantalla[pA].entidades[i].vida <= 0) //Desactiva al titan en caso de que no tenga vida
                    {
                        gs->pantalla[pA].entidades[i].activo = false;
                        gs->levi.puntuacion += 75;
                        printf("Puntuacion: %d\n",gs->levi.puntuacion);
                    }
                }
                else if(gs->levi.habilidad2Activa)
                {
                    gs->pantalla[pA].entidades[i].vida -= 50;

                    if(gs->pantalla[pA].entidades[i].vida <= 0) //Desactiva al titan en caso de que no tenga vida
                    {
                        gs->pantalla[pA].entidades[i].activo = false;
                        gs->levi.puntuacion += 75;
                        printf("Puntuacion: %d\n",gs->levi.puntuacion);
                    }
                }

                else
                {
                    gs->pantalla[pA].entidades[i].vida = 0;
                    printf("Titan %d vida despues:%d\n", i, gs->pantalla[pA].entidades[i].vida);
                    if(gs->pantalla[pA].entidades[i].vida <= 0) //Desactiva al titan en caso de que no tenga vida
                    {
                        gs->pantalla[pA].entidades[i].activo = false;
                        gs->levi.puntuacion += 500;
                        printf("Puntuacion: %d\n",gs->levi.puntuacion);
                    }
                }

                continue;
            }

            if(colision(gs, gs->levi.hitboxAtaque, gs->pantalla[pA].entidades[i].hitboxTitan)) //Comprueba si pega en cualquier parte de la hitbox del titan
            {
                printf("Colisiono\n");
                printf("Titan %d vida antes: %d\n", i, gs->pantalla[pA].entidades[i].vida);

                if(gs->levi.habilidad1Activa)
                {
                    gs->pantalla[pA].entidades[i].vida -= 20;

                    if(gs->pantalla[pA].entidades[i].vida <= 0) //Desactiva al titan en caso de que no tenga vida
                    {
                        gs->pantalla[pA].entidades[i].activo = false;
                        gs->levi.puntuacion += 50;
                        printf("Puntuacion: %d\n",gs->levi.puntuacion);
                    }
                }
                else if(gs->levi.habilidad2Activa)
                {
                    gs->pantalla[pA].entidades[i].vida -= 30;

                    if(gs->pantalla[pA].entidades[i].vida <= 0) //Desactiva al titan en caso de que no tenga vida
                    {
                        gs->pantalla[pA].entidades[i].activo = false;
                        gs->levi.puntuacion += 50;
                        printf("Puntuacion: %d\n",gs->levi.puntuacion);
                    }
                }

                else
                {
                    gs->pantalla[pA].entidades[i].vida -= 100;
                    printf("Titan %d vida despues:%d\n", i, gs->pantalla[pA].entidades[i].vida);
                    if(gs->pantalla[pA].entidades[i].vida <= 0) //Desactiva al titan en caso de que no tenga vida
                    {
                        gs->pantalla[pA].entidades[i].activo = false;
                        gs->levi.puntuacion += 100;
                        printf("Puntuacion: %d\n",gs->levi.puntuacion);
                    }
                }
            }
        }
    }
}

void aumenta_dash(s_GameState *gs)
{
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
            if(colision(gs, gs->levi.hitbox, gs->pantalla[pA].hitbox[i]))
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

        for(i=0;i<gs->pantalla[pA].num_elementos;i++)
        {
            if(gs->pantalla[pA].elementos[i].tipo == 3 && colision(gs, gs->levi.hitbox, gs->pantalla[pA].elementos[i].hitbox) && gs->pantalla[pA].elementos[i].activo == true)
            {
                distancia_arriba = (gs->levi.hitbox.y + gs->levi.hitbox.alto) - gs->pantalla[pA].elementos[i].hitbox.y;

                if(gs->levi.velocidadY >= 0 && distancia_arriba < 20 && gs->levi.levi_vuelo == false)
                {
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
            }
        }

}

void genera_gas(s_GameState *gs)
{
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
}

void colision_ODM(s_GameState *gs)
{
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
            if((gs->pantalla[pA].elementos[i].tipo == 1 || gs->pantalla[pA].elementos[i].tipo == 3) && colision(gs, gs->levi.hitboxODM, gs->pantalla[pA].elementos[i].hitbox))
            {
                inicia_ODM(gs, &cx, &cy);
                gs->levi.habilidad1Activa = false;
                gs->levi.habilidad2Activa = false;
                gs->levi.ODM.engancheNormal = true;
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

    if(gs->input.keyF == true && gs->levi.dash.activo == false && gs->levi.dash.cooldown <= 0 && gs->levi.agarrado == false)
    {
        if(colision(gs, gs->levi.hitboxODM, gs->pantalla[gs->pantalla_actual].hitbox[0]) && gs->levi.levi_suelo) 
            return; //Evita bug de activar dash con el mouse en el suelo

        cx = mouseX - (gs->levi.x + gs->levi.hitbox.ancho); //Calcula cateto x 
        cy = mouseY - (gs->levi.y + gs->levi.hitbox.alto); //Calcula cateto y
        distancia = sqrt(cx*cx + cy*cy); //Calcula la hipotenusa (distancia de levi al punto)

        gs->levi.dash.activo = true;
        gs->levi.dash.distanciaRestante = 250; //Distancia fija a recorrer
        gs->levi.dash.cooldown = 1; 
        gs->levi.dash.dashX = cx/distancia; //Direccion x del dash (coseno)
        gs->levi.dash.dashY = cy/distancia; //Direccion y del dash (seno)
        gs->levi.ODM.activo = false;
        if(gs->levi.dash.dashY < 0 && gs->levi.levi_suelo == true) //Si el dash es hacia arriba activa esta variable
            gs->levi.dash.frameActivacion = true; //Sirve para ignorar por 1 frame las colisiones del mapa, asi permite despegar el dash si esta en suelo

        gs->levi.dash.animDashActiva = true;
        gs->levi.dash.angulo = atan2(cy, cx);
        gs->levi.dash.x = gs->levi.x + 40;
        gs->levi.dash.y = gs->levi.y + 55;
        gs->variables.gravedad = 0;

        if(cx < 0 )
            gs->levi.animacion.rotarAnim = true;
        else if(cx > 0 )
            gs->levi.animacion.rotarAnim = false;

        cambiar_animacion(gs, DASH);
        desactivar_hitbox(gs, 0.5f);

    }

    if(gs->levi.dash.activo)
    {
        if(gs->levi.agarrado)
            gs->levi.dash.distanciaRestante = 0;
        gs->input.keyF = false;
        gs->levi.x += gs->levi.dash.dashX*10; //Mueve a levi segun la direccion x y lo multiplica por la velocidad 8
        gs->levi.y += gs->levi.dash.dashY*10; //Mueve a levi segun la direccion x y lo multiplica por la velocidad 8
        gs->levi.dash.distanciaRestante -= 10; //Resta la distancia restante 

        if(gs->levi.dash.distanciaRestante <= 0) //Calcula el fin del dash
        {
            gs->levi.dash.activo = false;
            gs->levi.dash.tiempoRecuperacionDash = 0.3f;
            gs->levi.velocidadX = gs->levi.dash.dashX * 25; //Inercia horizontal
            gs->levi.velocidadY = gs->levi.dash.dashY * 18; //Inercia vertical

            /*gs->levi.x += gs->levi.dash.dashX * 50; 
            gs->levi.y += gs->levi.dash.dashY * 50;*/

            cambiar_animacion(gs, SALIDA_DASH);
        }
    }

    if(gs->levi.dash.cooldown > 0) 
        gs->levi.dash.cooldown -= 1.0/FPS;
    
}

void colision_levi_dash(s_GameState *gs)
{
    int pA = gs->pantalla_actual, i;

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
                //printf("Colision en la nuca\n");
                //printf("%d\n", gs->pantalla[pA].entidades[i].vida);
                gs->pantalla[pA].entidades[i].vida = 0;
                gs->pantalla[pA].entidades[i].activo = false;
                
                if(gs->pantalla[pA].entidades[i].vida <= 0)
                {
                    gs->pantalla[pA].entidades[i].activo = false;
                    gs->levi.puntuacion += 500;
                    printf("Puntuacion: %d\n", gs->levi.puntuacion); 
                }
                //printf("%d\n", gs->pantalla[pA].entidades[i].vida);
                continue;
            }

            if(colision(gs, gs->levi.dash.hitboxDash, gs->pantalla[pA].entidades[i].hitboxTitan)) //Comprueba si pega en cualquier parte de la hitbox del titan
            {
                //printf("Colisiono\n");
                //printf("Titan %d vida antes: %d\n", i, gs->pantalla[pA].entidades[i].vida);       
                gs->pantalla[pA].entidades[i].vida -= 75;

                if(gs->pantalla[pA].entidades[i].vida <= 0)
                {
                    gs->pantalla[pA].entidades[i].activo = false;
                    gs->levi.puntuacion += 100;
                    printf("Puntuacion: %d\n", gs->levi.puntuacion); 
                }
                //printf("Titan %d vida despues:%d\n", i, gs->pantalla[pA].entidades[i].vida);
            }

        }

}

void colision_levi_elementos(s_GameState *gs)
{
    int i, pA = gs->pantalla_actual, nE = gs->pantalla[pA].num_elementos;

    for(i=0;i<nE;i++)
    {
        if(colision(gs, gs->levi.hitbox, gs->pantalla[pA].elementos[i].hitbox) && gs->pantalla[pA].elementos[i].activo && gs->pantalla[pA].elementos[i].tipo == 2)
        {
            gs->levi.inventario.escudos++;
            gs->pantalla[pA].elementos[i].activo = false;
        }

        if(gs->input.keyE)
        {
            if(colision(gs, gs->levi.hitbox, gs->pantalla[pA].elementos[i].hitbox2))
            {
                printf("Colision con puerta\n");
                gs->input.keyE = false;
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
    if(gs->levi.estadoLevi == nuevaAnim) //Verifica si el estado de levi es nuevo, si no es asi, retorna
        return;

    if(gs->levi.agarrado == true)
        return;

    bool Interrumpible = (nuevaAnim == IDLE || nuevaAnim == CAMINANDO || nuevaAnim == CORRIENDO || nuevaAnim == SALTANDO || nuevaAnim == CAYENDO);

    if(Interrumpible && gs->levi.animacion.bloquearAnimacion)
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

        case PARRY:

            gs->levi.animacion.bloquearAnimacion = true;
            gs->levi.animacion.cantidadFrames = 3;
            gs->levi.animacion.velocidadAnim = 9;
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
        || gs->levi.estadoLevi == SALIDA_ODM_ATAQUE2 || gs->levi.estadoLevi == SALIDA_TITAN_AGARRE || gs->levi.estadoLevi == PARRY)
    {
        gs->levi.animacion.bloquearAnimacion = false;
        gs->variables.bloquearControles = false;
        gs->variables.gravedad = 0.8;

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
            if(gs->pantalla[pA].elementos[i].activo == false && gs->pantalla[pA].elementos[i].tipo != 2)
                gs->pantalla[pA].elementos[i].activo = true;

}

void desactivar_hitbox(s_GameState *gs, float tiempo)
{
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
        printf("Puntuacion: %d\n", gs->levi.puntuacion); 
    }

    if(strcmp(tipo,"dash/nuca"))
    {
        gs->levi.puntuacion += 500;
        gs->levi.dash.flagDash++;
        printf("Puntuacion: %d\n", gs->levi.puntuacion); 
    }

    if(strcmp(tipo,"ataque/cuerpo"))
    {
        gs->levi.puntuacion += 100;
        printf("Puntuacion: %d\n", gs->levi.puntuacion); 
    }

    if(strcmp(tipo,"dash/cuerpo"))
    {
        gs->levi.puntuacion += 500;
        printf("Puntuacion: %d\n", gs->levi.puntuacion); 
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
//================================================//