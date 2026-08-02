#include "commons.h"

void activa_entidades(s_GameState *gs);
void hitbox_entidades(s_GameState *gs, s_Assets *assets);
void gravedad_titanes(s_GameState *gs);
void colision_titanes(s_GameState *gs);
void movimiento_titanes(s_GameState *gs);
void ataque_titanes(s_GameState *gs, int i, float cy, float cx);
void agarre_titanes(s_GameState *gs, int i);
void titan1_ataque1(s_GameState *gs, int i, s_Entidades *entidad, float cx, float cy);
void titan1_ataque2(s_GameState *gs, int i, s_Entidades *entidad, float cx, float cy);
void titan2_ataque1(s_GameState *gs, int i, s_Entidades *entidad, float cx, float cy);
void titan2_ataque2(s_GameState *gs, int i, s_Entidades *entidad, float cx, float cy);
void titan2_ataque3(s_GameState *gs, int i, s_Entidades *entidad, float cx, float cy);
void aplicar_dano(s_GameState *gs, int dano, float empuje, bool ataqueDerecha);
void titan_hembra(s_GameState *gs);


void genera_entidades(s_GameState *gs, s_Assets *assets)
{
    if(gs->pausa == true)
        return;

    activa_entidades(gs);
    hitbox_entidades(gs, assets);
    movimiento_titanes(gs);
    gravedad_titanes(gs);
    colision_titanes(gs);

    if(gs->pantalla_actual == 3)
        titan_hembra(gs);

}

void activa_entidades(s_GameState *gs)
{
    int i, j, pA = gs->pantalla_actual;

    if(gs->input.keyG == true)    
    {
        for(i=0 ; i<gs->pantalla[pA].num_entidades ; i++) //Desctiva todas las entidades de la pantalla
            if(gs->pantalla[pA].entidades[i].vida > 0)
            {
                gs->pantalla[pA].entidades[i].activo = true;
            }
    }

    else if(gs->input.keyG == false)
    {
        for(i=0 ; i<gs->pantalla[pA].num_entidades ; i++) //Activa todas las entidades de la pantalla
            if(gs->pantalla[pA].entidades[i].vida > 0)
                gs->pantalla[pA].entidades[i].activo = false;
    }

}

void titan_hembra(s_GameState *gs)
{
    gs->titanHembra.hitbox.x = gs->titanHembra.x + 80;
    gs->titanHembra.hitbox.y = gs->titanHembra.y + 20;
    gs->titanHembra.hitbox.ancho = 135;
    gs->titanHembra.hitbox.alto = 425;

    if(gs->input.ClickIzq && colision(gs, gs->levi.hitboxAtaque, gs->titanHembra.hitbox))
        gs->titanHembra.vida -= 10;

    if(gs->titanHembra.vida <= 0)
    {
        gs->titanHembra.activa = false;
        gs->nivelCompletado = true;
    }
}

void movimiento_titanes(s_GameState *gs)
{
    int i, j, pA = gs->pantalla_actual;

    float leviMitadX = gs->levi.hitbox.x + (gs->levi.hitbox.ancho/2), leviMitadY = gs->levi.hitbox.y + (gs->levi.hitbox.alto/2);
    float titanMitadX, titanMitadY, cx, cy, distanciaX;

    for(i=0;i<gs->pantalla[pA].num_entidades;i++)
    {
        if(gs->pantalla[pA].entidades[i].activo)
        {

            titanMitadX = gs->pantalla[pA].entidades[i].hitboxTitan.x + (gs->pantalla[pA].entidades[i].hitboxTitan.ancho/2);
            titanMitadY = gs->pantalla[pA].entidades[i].hitboxTitan.y + (gs->pantalla[pA].entidades[i].hitboxTitan.alto/2);
            cx = leviMitadX - titanMitadX;
            cy = leviMitadY - titanMitadY;
            distanciaX= fabsf(leviMitadX - titanMitadX); //Valor absoluto para calcular las distancias

            if(gs->pantalla[pA].entidades[i].tiempoGolpeRegistrado > 0)
                gs->pantalla[pA].entidades[i].tiempoGolpeRegistrado -= 1.0f/FPS;

            //printf("TiempoGolpe: %.1f\n", gs->pantalla[pA].entidades[i].tiempoGolpeRegistrado);

            if(gs->pantalla[pA].entidades[i].tipo == 1) ///////////////////Titan 1////////////////////////////
            {
                if(gs->input.keyL == true)
                {
                    printf("flagDash: %d\n", gs->levi.dash.flagDash);
                    printf("Cantidad de dashes: %d\n", gs->levi.dash.cantDash);
                    printf("cy: %f\n", cy);
                    printf("cx: %f\n", cx);
                    gs->input.keyL = false;
                }

                if(gs->pantalla[pA].entidades[i].quieto == false)
                {
                    if(cx < 500 && cx > 0 && distanciaX > 80) //Calcula el rango para empezar a moverse
                    {
                        if(!(fabs(cx) <= 140 && cy < 35 && cy > -320)) //Si puede ejecutar ataque 2 no avanza mas de lo necesario
                        {
                            gs->pantalla[pA].entidades[i].x += gs->pantalla[pA].entidades[i].velocidadX;
                            gs->pantalla[pA].entidades[i].animacion.rotarAnim = false;
                            gs->pantalla[pA].entidades[i].viendoDerecha = true;
                        }
                    }
                    else if(cx > -500 && cx < 0 && distanciaX > 80) //Calcula el rango para empezar a moverse
                    {
                        if(!(fabs(cx) <= 140 && cy < 35 && cy > -320)) //Si puede ejecutar ataque 2 no avanza mas de lo necesario
                        {
                            gs->pantalla[pA].entidades[i].x -= gs->pantalla[pA].entidades[i].velocidadX;
                            gs->pantalla[pA].entidades[i].animacion.rotarAnim = true;
                            gs->pantalla[pA].entidades[i].viendoDerecha = false;
                        }
                    }
                
                    else if(distanciaX > 500) //Si no esta a rango se mueve de un lado para otro
                    {   
                        if(gs->pantalla[pA].entidades[i].tiempoQuieto > 0)
                        {
                            gs->pantalla[pA].entidades[i].tiempoQuieto -= 1.0f/FPS;
                            if(gs->pantalla[pA].entidades[i].tiempoQuieto <= 0)
                                gs->pantalla[pA].entidades[i].cambioDireccion = true; 
                            continue;
                        }

                        if(gs->pantalla[pA].entidades[i].cambioDireccion == true)
                        {
                            if(gs->pantalla[pA].entidades[i].viendoDerecha == true)
                            {
                                gs->pantalla[pA].entidades[i].viendoDerecha = false;
                                gs->pantalla[pA].entidades[i].cambioDireccion = false;
                                gs->pantalla[pA].entidades[i].animacion.rotarAnim = true;
                            }
                            else if (gs->pantalla[pA].entidades[i].viendoDerecha == false)
                            {
                                gs->pantalla[pA].entidades[i].viendoDerecha = true;
                                gs->pantalla[pA].entidades[i].cambioDireccion = false;
                                gs->pantalla[pA].entidades[i].animacion.rotarAnim = false;
                            }
                        }

                        if(gs->pantalla[pA].entidades[i].viendoDerecha == true)
                        {
                            gs->pantalla[pA].entidades[i].x += gs->pantalla[pA].entidades[i].velocidadX - 1;
                            gs->pantalla[pA].entidades[i].distanciaRecorrida += gs->pantalla[pA].entidades[i].velocidadX - 1;

                            if(gs->pantalla[pA].entidades[i].distanciaRecorrida >= 120)
                            {
                                gs->pantalla[pA].entidades[i].tiempoQuieto = 1.5f;
                                gs->pantalla[pA].entidades[i].distanciaRecorrida = 0;
                            }
                        }

                        else if(gs->pantalla[pA].entidades[i].viendoDerecha == false)
                        {
                            gs->pantalla[pA].entidades[i].x -= gs->pantalla[pA].entidades[i].velocidadX - 1;
                            gs->pantalla[pA].entidades[i].distanciaRecorrida += gs->pantalla[pA].entidades[i].velocidadX - 1;

                            if(gs->pantalla[pA].entidades[i].distanciaRecorrida >= 120)
                            {
                                gs->pantalla[pA].entidades[i].tiempoQuieto = 1.5f;
                                gs->pantalla[pA].entidades[i].distanciaRecorrida = 0;
                            }
                        }
                    }
            }

                if(gs->pantalla[pA].entidades[i].cooldownAtaque > 0) //Reestablece las hb
                {
                    gs->pantalla[pA].entidades[i].hitboxAtaqueBasico.x = 0;
                    gs->pantalla[pA].entidades[i].hitboxAtaqueBasico.y = 0;
                    gs->pantalla[pA].entidades[i].hitboxAtaqueBasico.alto = 0;
                    gs->pantalla[pA].entidades[i].hitboxAtaqueBasico.ancho = 0;
                    gs->pantalla[pA].entidades[i].cooldownAtaque -= 1.0f/FPS;

                    gs->pantalla[pA].entidades[i].agarre.manoHB.x = 0;
                    gs->pantalla[pA].entidades[i].agarre.manoHB.y = 0;
                    gs->pantalla[pA].entidades[i].agarre.manoHB.alto = 0;
                    gs->pantalla[pA].entidades[i].agarre.manoHB.ancho = 0;
                }

                else if(distanciaX <= 140 || gs->pantalla[pA].entidades[i].ataqueActivo || gs->pantalla[pA].entidades[i].patadaActiva)
                {
                    if(gs->pantalla[pA].entidades[i].ataqueActivo == false)
                    {
                        if(cx > 0)
                        {
                            gs->pantalla[pA].entidades[i].viendoDerecha = true;
                            gs->pantalla[pA].entidades[i].animacion.rotarAnim = false;
                        }
                        else if(cx <= 0)
                        {
                            gs->pantalla[pA].entidades[i].viendoDerecha = false;
                            gs->pantalla[pA].entidades[i].animacion.rotarAnim = true;
                        }
                    }
                    ataque_titanes(gs, i, cy, cx);
                }
            }

        //=============================Titan 2================================================================================//

            else if(gs->pantalla[pA].entidades[i].tipo == 2) 
            {
                //printf("cy: %f\n", cy);
                if(gs->pantalla[pA].entidades[i].quieto == false)
                {
                    if(cx < 500 && cx > 0 && distanciaX >= 40)
                    {
                        gs->pantalla[pA].entidades[i].x += gs->pantalla[pA].entidades[i].velocidadX;
                        gs->pantalla[pA].entidades[i].animacion.rotarAnim = false;
                        gs->pantalla[pA].entidades[i].viendoDerecha = true;
                    }
                    else if(cx > -500 && cx < 0 && distanciaX >= 40 )
                    {
                        gs->pantalla[pA].entidades[i].x -= gs->pantalla[pA].entidades[i].velocidadX;
                        gs->pantalla[pA].entidades[i].animacion.rotarAnim = true;
                        gs->pantalla[pA].entidades[i].viendoDerecha = false;
                    }
                    else if(distanciaX > 500) //Cuando tenga anim hacer que se siente
                    {
                        gs->pantalla[pA].entidades[i].quieto = false;
                    }
                }

                if(gs->pantalla[pA].entidades[i].cooldownAtaque > 0 && gs->pantalla[pA].entidades[i].saltoActivo == false)
                {
                    gs->pantalla[pA].entidades[i].hitboxAtaqueBasico.x = 0;
                    gs->pantalla[pA].entidades[i].hitboxAtaqueBasico.y = 0;
                    gs->pantalla[pA].entidades[i].hitboxAtaqueBasico.alto = 0;
                    gs->pantalla[pA].entidades[i].hitboxAtaqueBasico.ancho = 0;
                    gs->pantalla[pA].entidades[i].cooldownAtaque -= 1.0f/FPS;

                    gs->pantalla[pA].entidades[i].agarre.manoHB.x = 0;
                    gs->pantalla[pA].entidades[i].agarre.manoHB.y = 0;
                    gs->pantalla[pA].entidades[i].agarre.manoHB.alto = 0;
                    gs->pantalla[pA].entidades[i].agarre.manoHB.ancho = 0;

                }
                else if(distanciaX <= 175 || gs->pantalla[pA].entidades[i].tiempoAtaqueActivo > 0 || gs->pantalla[pA].entidades[i].saltoActivo)
                {
                    if(cx > 0)
                    {
                        gs->pantalla[pA].entidades[i].viendoDerecha = true;
                        gs->pantalla[pA].entidades[i].animacion.rotarAnim = false;
                    }
                    else if(cx <= 0)
                    {
                        gs->pantalla[pA].entidades[i].viendoDerecha = false;
                        gs->pantalla[pA].entidades[i].animacion.rotarAnim = true;
                    }
                    ataque_titanes(gs, i, cy, cx);
                }
            }
        }
    }
}

void ataque_titanes(s_GameState *gs, int i, float cy, float cx)
{
    int pA = gs->pantalla_actual, tipo = gs->pantalla[pA].entidades[i].tipo;
    s_Entidades *entidad = gs->pantalla[pA].entidades;

    if(entidad[i].agarre.activo)
    {
        if(entidad[i].tipo == 1)
            titan1_ataque2(gs, i, entidad, cx, cy);
        agarre_titanes(gs, i);
        return;
    }

    if(tipo == 1)
    {
        if(entidad[i].tiempoAtaqueActivo <= 0)
        {
            if(fabs(cx) <= 125 && cy > 30 || entidad[i].patadaActiva == true)
                titan1_ataque1(gs, i, entidad, cx, cy);
            else if((fabs(cx) <= 140 && cy < 35 && cy > -320) || entidad[i].ataqueActivo == true)
                titan1_ataque2(gs, i, entidad, cx, cy);
        }

    }

    else if(tipo == 2)
    {
        if(entidad[i].tiempoAtaqueActivo <= 0)
        {
            if((cy <= -95 && cy >= -288 && fabsf(cx) <= 175) || entidad[i].saltoActivo) //Salto
            {
                if(gs->levi.agarrado == true)
                    entidad[i].casoAtaque = 5;
                else
                    entidad[i].casoAtaque = 1; 
            }

            else if(fabsf(cx) <= 40 && cy > -90 && gs->levi.agarrado == false)
                entidad[i].casoAtaque = (rand()%8) + 2; //Golpe melee

            else if(fabsf(cx) <= 40 && cy > -90 && gs->levi.agarrado == true) //Golpe melee
                entidad[i].casoAtaque = (rand()%6) + 2;

            else 
                entidad[i].casoAtaque = 0;

            //printf("%d\n",entidad[i].casoAtaque);
        }

        if(entidad[i].casoAtaque == 1)
            titan2_ataque1(gs, i, entidad, cx, cy);

        else if(entidad[i].casoAtaque >= 1 && entidad[i].casoAtaque < 8)
            titan2_ataque2(gs, i, entidad, cx, cy);
        
        else if(entidad[i].casoAtaque >= 8) 
            titan2_ataque3(gs, i, entidad, cx, cy);
    }

}

void titan1_ataque1(s_GameState *gs, int i, s_Entidades *entidad, float cx, float cy)
{
    //Patada

    if(entidad[i].viendoDerecha == false && entidad[i].patadaActiva == false)
        entidad[i].ataqueDerecha = false;
    else if(entidad[i].viendoDerecha == true && entidad[i].patadaActiva == false)
        entidad[i].ataqueDerecha = true;

    if(entidad[i].ataqueDerecha == false)
    {
        if(entidad[i].patadaActiva )
        {
            entidad[i].hitboxAtaqueBasico.x -= 2;
            entidad[i].distanciaRecorridaAtaque += 2;
            entidad[i].quieto = true;
            if(entidad[i].distanciaRecorridaAtaque >= 90)
            {
                entidad[i].cooldownAtaque = 2;
                entidad[i].quieto = false;
                entidad[i].distanciaRecorridaAtaque = 0;
                entidad[i].patadaActiva = false;
            }
        }
        else 
        {
            entidad[i].hitboxAtaqueBasico.x = entidad[i].hitboxTitan.x - 10;
            entidad[i].hitboxAtaqueBasico.y = entidad[i].hitboxTitan.y + 300;
            entidad[i].hitboxAtaqueBasico.alto = 80;
            entidad[i].hitboxAtaqueBasico.ancho = 40;
            entidad[i].patadaActiva = true;
        }
    }

    else if(entidad[i].ataqueDerecha == true)
    {
        if(entidad[i].patadaActiva)
        {
            entidad[i].hitboxAtaqueBasico.x += 2;
            entidad[i].distanciaRecorridaAtaque += 2;
            entidad[i].quieto = true;
            if(entidad[i].distanciaRecorridaAtaque >= 90)
            {
                entidad[i].cooldownAtaque = 2;
                entidad[i].quieto = false;
                entidad[i].distanciaRecorridaAtaque = 0;
                entidad[i].patadaActiva = false;
            }
        }
        else 
        {
            entidad[i].hitboxAtaqueBasico.x = entidad[i].hitboxTitan.x + entidad[i].hitboxTitan.ancho - 10;
            entidad[i].hitboxAtaqueBasico.y = entidad[i].hitboxTitan.y + 300;
            entidad[i].hitboxAtaqueBasico.alto = 80;
            entidad[i].hitboxAtaqueBasico.ancho = 40;
            entidad[i].patadaActiva = true;
        }
    }

    if(colision(gs, gs->levi.hitbox, entidad[i].hitboxAtaqueBasico) && entidad[i].tiempoGolpeRegistrado <= 0)
    {
        entidad[i].tiempoGolpeRegistrado = 1.0f;
        aplicar_dano(gs, 4, 3.0f, entidad[i].ataqueDerecha);
    }
    
}

void titan1_ataque2(s_GameState *gs, int i, s_Entidades *entidad, float cx, float cy)
{
    if(entidad[i].viendoDerecha == false && entidad[i].ataqueActivo == false)
        entidad[i].ataqueDerecha = false;
    else if(entidad[i].viendoDerecha == true && entidad[i].ataqueActivo == false)
        entidad[i].ataqueDerecha = true;

    if(entidad[i].agarreFase3Activa)
    {
        if(entidad[i].cooldownMordida > 0)
            entidad[i].cooldownMordida -= 1.0f/FPS;
        else if(entidad[i].cooldownMordida <= 0)
        {
            if(colision(gs, gs->levi.hitbox, entidad[i].hitboxAtaqueBasico) && gs->levi.vida > 0)
                aplicar_dano(gs, gs->levi.vida, 0, false);
        }
    }

    else if(entidad[i].agarreFase2Activa)
    {
        entidad[i].hitboxAtaqueBasico.x = entidad[i].hitboxTitan.x + 30;
        entidad[i].hitboxAtaqueBasico.y = entidad[i].hitboxTitan.y + 30;
        entidad[i].hitboxAtaqueBasico.ancho = 30;
        entidad[i].hitboxAtaqueBasico.alto = 30;

        if(entidad[i].ataqueDerecha == true)
            entidad[i].agarre.manoHB.x -= 1.5f;
        else if(entidad[i].ataqueDerecha == false)
            entidad[i].agarre.manoHB.x += 1.5f;
        entidad[i].agarre.manoHB.y -= 2;
        entidad[i].distanciaRecorridaAtaque += 2;

        if(entidad[i].distanciaRecorridaAtaque >= 410)
        {
            entidad[i].agarreFase3Activa = true;
            entidad[i].agarreFase2Activa = false;
            entidad[i].cooldownMordida = 1.0f;
        }
            
    }

    else if(entidad[i].ataqueActivo)
    {
        entidad[i].agarre.manoHB.y += 2;
        entidad[i].hitboxAtaqueBasico.y += 2;
        entidad[i].distanciaRecorridaAtaque += 2;
        if(entidad[i].ataqueDerecha == false)
            entidad[i].hitboxAtaqueBasico.ancho += 0.4;
        else if(entidad[i].ataqueDerecha == true)
        {
            entidad[i].hitboxAtaqueBasico.ancho += 0.4;
            entidad[i].hitboxAtaqueBasico.x -= 0.4;
        }
        entidad[i].quieto = true;

        if(colision(gs,gs->levi.hitbox, entidad[i].agarre.manoHB) && gs->levi.agarrado == false)
        {
            entidad[i].agarre.activo = true;
            entidad[i].cooldownMordida = 1.0f; //Tiempo de espera antes que el titan pueda morder justo despues de agarrar
            cambiar_animacion(gs, IDLE);
            agarre_titanes(gs, i);
        }
        else if(colision(gs, gs->levi.hitbox, entidad[i].hitboxAtaqueBasico) && entidad[i].tiempoGolpeRegistrado <= 0)
        {
            entidad[i].tiempoGolpeRegistrado = 1.0f;
            aplicar_dano(gs, 2, 0, false);
        }

        if(entidad[i].distanciaRecorridaAtaque >= 250)
        {
            if(gs->levi.agarrado)
                entidad[i].agarreFase2Activa = true;
            else
            {
                entidad[i].cooldownAtaque = 2;
                entidad[i].quieto = false;
                entidad[i].distanciaRecorridaAtaque = 0;
                entidad[i].ataqueActivo = false;
            }
        }
    }
    else 
    {
        entidad[i].agarre.manoHB.y = entidad[i].hitboxTitan.y - 50;
        entidad[i].agarre.manoHB.alto = 30;
        entidad[i].agarre.manoHB.ancho = 30;
        entidad[i].agarre.manoHB.color = al_map_rgb(0, 0, 255);

        entidad[i].hitboxAtaqueBasico.y = entidad[i].agarre.manoHB.y;
        entidad[i].hitboxAtaqueBasico.alto = 30;
        entidad[i].hitboxAtaqueBasico.ancho = 30;

        entidad[i].ataqueActivo = true;

        if(entidad[i].ataqueDerecha == false)
        {
            entidad[i].agarre.manoHB.x = entidad[i].hitboxTitan.x - 100;
            entidad[i].hitboxAtaqueBasico.x = entidad[i].agarre.manoHB.x + entidad[i].agarre.manoHB.ancho + 2;
        }
        else if(entidad[i].ataqueDerecha == true)
        {
            entidad[i].agarre.manoHB.x = entidad[i].hitboxTitan.x + entidad[i].hitboxTitan.ancho - entidad[i].agarre.manoHB.ancho + 100;
            entidad[i].hitboxAtaqueBasico.x = entidad[i].agarre.manoHB.x - entidad[i].agarre.manoHB.ancho - 2; 
        }
    }

}

void titan2_ataque1(s_GameState *gs, int i, s_Entidades *entidad, float cx, float cy)
{
    //Salto

    if(entidad[i].saltoActivo == false)
    {
        entidad[i].velocidadY = -18;
        entidad[i].saltoActivo = true;
        entidad[i].frameActivacion = true;

        if(cx > 0)
            entidad[i].velocidadXSalto = 4.0f;

        else 
            entidad[i].velocidadXSalto = -4.0f;
    }

    if(entidad[i].saltoActivo)
    {
        entidad[i].cooldownAtaque = 1.0f;
        entidad[i].x += entidad[i].velocidadXSalto;

        if(entidad[i].velocidadY < 8) //Funciona para mantener cierta inercia al saltar
        {
            entidad[i].hitboxAtaqueBasico.x = entidad[i].hitboxTitan.x;
            entidad[i].hitboxAtaqueBasico.y = entidad[i].hitboxTitan.y + 20;
            entidad[i].hitboxAtaqueBasico.alto = 60;
            entidad[i].hitboxAtaqueBasico.ancho = 60;

            if(colision(gs, gs->levi.hitbox, entidad[i].hitboxAtaqueBasico) && entidad[i].tiempoGolpeRegistrado <= 0)
            {
                entidad[i].tiempoGolpeRegistrado = 1.0f;
                aplicar_dano(gs, 2, 0, false);
            }
        }
        else 
        {
            entidad[i].hitboxAtaqueBasico.x = 0;
            entidad[i].hitboxAtaqueBasico.y = 0;
            entidad[i].hitboxAtaqueBasico.alto = 0;
            entidad[i].hitboxAtaqueBasico.ancho = 0;
            entidad[i].saltoActivo = false;
            entidad[i].casoAtaque = 0; //Para que no caiga en un bucle infinito de salto aunque no se cumpla la condicion
        }
    }
}

void titan2_ataque2(s_GameState *gs, int i, s_Entidades *entidad, float cx, float cy)
{
    //Golpe melee

    if(entidad[i].viendoDerecha == false)
    {
        if(entidad[i].tiempoAtaqueActivo > 0)
        {
            entidad[i].hitboxAtaqueBasico.x = entidad[i].hitboxTitan.x - 30;
            entidad[i].hitboxAtaqueBasico.y = entidad[i].hitboxTitan.y + 60;
            entidad[i].hitboxAtaqueBasico.alto = 80;
            entidad[i].hitboxAtaqueBasico.ancho = 50;
            entidad[i].quieto = true;
            entidad[i].tiempoAtaqueActivo -= 1.0f/FPS;

            if(colision(gs, gs->levi.hitbox, entidad[i].hitboxAtaqueBasico) && entidad[i].tiempoGolpeRegistrado <= 0)
            {
                entidad[i].tiempoGolpeRegistrado = entidad[i].tiempoAtaqueActivo;
                aplicar_dano(gs, 1, 0.5f, entidad[i].viendoDerecha);
            }

            if(entidad[i].tiempoAtaqueActivo <= 0)
            {
                entidad[i].cooldownAtaque = 1;
                entidad[i].quieto = false;
            }
        }
        else 
            entidad[i].tiempoAtaqueActivo = 0.4;
    }

    else if(entidad[i].viendoDerecha == true)
    {
        if(entidad[i].tiempoAtaqueActivo > 0)
        {
            entidad[i].hitboxAtaqueBasico.x = entidad[i].hitboxTitan.x + entidad[i].hitboxTitan.ancho - 20;
            entidad[i].hitboxAtaqueBasico.y = entidad[i].hitboxTitan.y + 60;
            entidad[i].hitboxAtaqueBasico.alto = 80;
            entidad[i].hitboxAtaqueBasico.ancho = 50;
            entidad[i].quieto = true;
            entidad[i].tiempoAtaqueActivo -= 1.0f/FPS;

            if(colision(gs, gs->levi.hitbox, entidad[i].hitboxAtaqueBasico) && entidad[i].tiempoGolpeRegistrado <= 0)
            {
                entidad[i].tiempoGolpeRegistrado = entidad[i].tiempoAtaqueActivo;
                aplicar_dano(gs, 1, 0.5f, entidad[i].viendoDerecha);
            }

            if(entidad[i].tiempoAtaqueActivo <= 0)
            {
                entidad[i].cooldownAtaque = 1;
                entidad[i].quieto = false;
            }
        }
        else 
            entidad[i].tiempoAtaqueActivo = 0.4;
    }
}

void titan2_ataque3(s_GameState *gs, int i, s_Entidades *entidad, float cx, float cy)
{
    //Agarre

    if(entidad[i].viendoDerecha == false)
    {
        if(entidad[i].tiempoAtaqueActivo > 0)
        {
            entidad[i].agarre.manoHB.x = entidad[i].hitboxTitan.x - 35;
            entidad[i].agarre.manoHB.y = entidad[i].hitboxTitan.y + 80;
            entidad[i].agarre.manoHB.alto = 40;
            entidad[i].agarre.manoHB.ancho = 55;
            entidad[i].agarre.manoHB.color = al_map_rgb(0, 0, 255);
            entidad[i].quieto = true;
            entidad[i].tiempoAtaqueActivo -= 1.0f/FPS;

            if(colision(gs,gs->levi.hitbox, entidad[i].agarre.manoHB) && gs->levi.agarrado == false)
            {
                entidad[i].agarre.activo = true;
                entidad[i].cooldownMordida = 1.0f; //Tiempo de espera antes que el titan pueda morder justo despues de agarrar
                cambiar_animacion(gs, IDLE);
                agarre_titanes(gs, i);
            }

            if(entidad[i].tiempoAtaqueActivo <= 0)
            {
                entidad[i].cooldownAtaque = 1;
                entidad[i].quieto = false;
            }
        }
        else 
            entidad[i].tiempoAtaqueActivo = 0.4;
    }

    else if(entidad[i].viendoDerecha == true)
    {
        if(entidad[i].tiempoAtaqueActivo > 0)
        {
            entidad[i].agarre.manoHB.x = entidad[i].hitboxTitan.x + entidad[i].hitboxTitan.ancho - 20;
            entidad[i].agarre.manoHB.y = entidad[i].hitboxTitan.y + 80;
            entidad[i].agarre.manoHB.alto = 40;
            entidad[i].agarre.manoHB.ancho = 55;
            entidad[i].agarre.manoHB.color = al_map_rgb(0, 0, 255);
            entidad[i].quieto = true;
            entidad[i].tiempoAtaqueActivo -= 1.0f/FPS;

            if(colision(gs,gs->levi.hitbox, entidad[i].agarre.manoHB) && gs->levi.agarrado == false)
            {
                entidad[i].agarre.activo = true;
                entidad[i].cooldownMordida = 0.5f; //Tiempo de espera antes que el titan pueda morder justo despues de agarrar
                cambiar_animacion(gs, IDLE);
                agarre_titanes(gs, i);
            }

            if(entidad[i].tiempoAtaqueActivo <= 0)
            {
                entidad[i].cooldownAtaque = 1;
                entidad[i].quieto = false;
            }
        }
        else 
            entidad[i].tiempoAtaqueActivo = 0.4;
    }
}

void agarre_titanes(s_GameState *gs, int i)
{
    int pA = gs->pantalla_actual;
    s_Entidades *entidad = gs->pantalla[pA].entidades;

    //printf("Agarrado por Titan[%d]\n", i);

    gs->variables.bloquearControles = true;
    gs->levi.agarrado = true;
    gs->variables.gravedad = 0;

    if(entidad[i].tipo == 1)
    {
        //if(gs->levi.vida > 0)
            //printf("Agarrado por dina\n");
        gs->levi.y = entidad[i].agarre.manoHB.y - 35;
        gs->levi.x = entidad[i].agarre.manoHB.x - 35;
    }

    else if(entidad[i].tipo == 2)
    {
        gs->levi.y = entidad[i].agarre.manoHB.y - 35;
        gs->levi.x = entidad[i].agarre.manoHB.x - 35;

        if(entidad[i].cooldownMordida > 0)
            entidad[i].cooldownMordida -= 1.0f/FPS;
        else
        {
            if(entidad[i].viendoDerecha == false)
            {
                entidad[i].mordidaHB.x = entidad[i].hitboxTitan.x - 30;
                entidad[i].mordidaHB.y = entidad[i].hitboxTitan.y + 60;
                entidad[i].mordidaHB.alto = 40;
                entidad[i].mordidaHB.ancho = 55;
                entidad[i].mordidaHB.color = al_map_rgb(255, 0, 0);
                entidad[i].tiempoMordidaActivo = 0.4f; 
            }
            else if(entidad[i].viendoDerecha == true)
            {
                entidad[i].mordidaHB.x = entidad[i].hitboxTitan.x + entidad[i].hitboxTitan.ancho - 15;
                entidad[i].mordidaHB.y = entidad[i].hitboxTitan.y + 60;
                entidad[i].mordidaHB.alto = 40;
                entidad[i].mordidaHB.ancho = 55;
                entidad[i].mordidaHB.color = al_map_rgb(255, 0, 0);
                entidad[i].tiempoMordidaActivo = 0.4f; 
            }

            if(colision(gs, gs->levi.hitbox, entidad[i].mordidaHB) && entidad[i].tiempoGolpeRegistrado <= 0)
            {
                entidad[i].tiempoGolpeRegistrado = 0.5f;
                aplicar_dano(gs, 2, 0, false);
            }

            entidad[i].cooldownMordida = 2.0f; 
        }

        if(entidad[i].tiempoMordidaActivo > 0)
            entidad[i].tiempoMordidaActivo -= 1.0f/FPS;
        else
        {
            entidad[i].mordidaHB.x = 0;
            entidad[i].mordidaHB.y = 0;
            entidad[i].mordidaHB.alto = 0;
            entidad[i].mordidaHB.ancho = 0;
        }
    }

    //////////////////////////////////////////////////////

    if(gs->input.keyR)
    {
        gs->levi.contSoltarse++;
        gs->input.keyR = false;
    }
    if(gs->levi.contSoltarse >= 5)
    {
        gs->levi.agarrado = false;
        gs->levi.contSoltarse = 0;
        gs->variables.gravedad = 0.8;
        gs->variables.bloquearControles = false;
        entidad[i].tiempoAtaqueActivo = 0; 
        entidad[i].cooldownAtaque = 1;
        entidad[i].cooldownMordida = 0;
        entidad[i].tiempoMordidaActivo = 0;

        entidad[i].agarre.activo = false;
        entidad[i].quieto = false;
        entidad[i].agarre.manoHB.x = 0;
        entidad[i].agarre.manoHB.y = 0;
        entidad[i].agarre.manoHB.alto = 0;
        entidad[i].agarre.manoHB.ancho = 0;
        entidad[i].mordidaHB.x = 0;
        entidad[i].mordidaHB.y = 0;
        entidad[i].mordidaHB.alto = 0;
        entidad[i].mordidaHB.ancho = 0;
        entidad[i].agarreFase3Activa = false;
        entidad[i].agarreFase2Activa = false;
        entidad[i].distanciaRecorridaAtaque = false;
        entidad[i].ataqueActivo = false;

        cambiar_animacion(gs, SALIDA_TITAN_AGARRE);
    }
}

void aplicar_dano(s_GameState *gs, int dano, float empuje, bool ataqueDerecha)
{
    if(ataqueDerecha == false)
        empuje = (-empuje);

    gs->levi.vida -= dano;
    gs->levi.velocidadX = empuje;

    printf("Golpe registrado; Vida: %d\n", gs->levi.vida);

}

void gravedad_titanes(s_GameState *gs)
{
    int i, j, pA = gs->pantalla_actual;
    int nE = gs->pantalla[pA].num_entidades;


    for(i=0;i<nE;i++) 
        if(gs->pantalla[pA].entidades[i].activo == true)
            {
                gs->pantalla[pA].entidades[i].gravedadTitan = 0.8;
                gs->pantalla[pA].entidades[i].velocidadY += gs->pantalla[pA].entidades[i].gravedadTitan;
                gs->pantalla[pA].entidades[i].y += gs->pantalla[pA].entidades[i].velocidadY;
            }

}

void colision_titanes(s_GameState *gs)
{
    int i, j, pA = gs->pantalla_actual;
    int nE = gs->pantalla[pA].num_entidades;
    float distancia_arriba;

    for(i=0;i<nE;i++)
        if(gs->pantalla[pA].entidades[i].activo == true)
            for(j=0;j<gs->pantalla[pA].num_hitbox;j++)
                if(colision(gs, gs->pantalla[pA].entidades[i].hitboxTitan, gs->pantalla[pA].hitbox[j]))
                {
                    if(gs->pantalla[pA].entidades[i].tipo == 1)
                    {
                        gs->pantalla[pA].entidades[i].y = gs->pantalla[pA].hitbox[j].y - gs->pantalla[pA].entidades[i].hitboxTitan.alto - 23;
                        gs->pantalla[pA].entidades[i].velocidadY = 0;
                    }
                    else if(gs->pantalla[pA].entidades[i].tipo == 2)
                    {
                        if(gs->pantalla[pA].entidades[i].frameActivacion)
                        {
                            gs->pantalla[pA].entidades[i].frameActivacion = false;
                            continue;
                        }
                        gs->pantalla[pA].entidades[i].saltoActivo = false;
                        gs->pantalla[pA].entidades[i].y = gs->pantalla[pA].hitbox[j].y - gs->pantalla[pA].entidades[i].hitboxTitan.alto - 24;
                        gs->pantalla[pA].entidades[i].velocidadY = 0;
                    }
                    
                }
}

void hitbox_entidades(s_GameState *gs, s_Assets *assets)
{
    int i, j, pA = gs->pantalla_actual;
    int nE = gs->pantalla[pA].num_entidades;

    for(i=0;i<nE;i++) 
        if(gs->pantalla[pA].entidades[i].activo == true)
        {
            if(gs->pantalla[pA].entidades[i].tipo == 1)
            {
                gs->pantalla[pA].entidades[i].hitboxTitan.x = gs->pantalla[pA].entidades[i].x + 235;
                gs->pantalla[pA].entidades[i].hitboxTitan.y = gs->pantalla[pA].entidades[i].y + 23;
                gs->pantalla[pA].entidades[i].hitboxTitan.alto = 386;
                gs->pantalla[pA].entidades[i].hitboxTitan.ancho = 70;

                gs->pantalla[pA].entidades[i].hitboxNuca.x = gs->pantalla[pA].entidades[i].hitboxTitan.x + 20;
                gs->pantalla[pA].entidades[i].hitboxNuca.y = gs->pantalla[pA].entidades[i].hitboxTitan.y + 70;
                gs->pantalla[pA].entidades[i].hitboxNuca.alto = 35;
                gs->pantalla[pA].entidades[i].hitboxNuca.ancho = 35;

            }

            //Hitbox normal
            else if(gs->pantalla[pA].entidades[i].tipo == 2)
            {
                gs->pantalla[pA].entidades[i].hitboxTitan.x = gs->pantalla[pA].entidades[i].x + 70;
                gs->pantalla[pA].entidades[i].hitboxTitan.y = gs->pantalla[pA].entidades[i].y + 24;
                gs->pantalla[pA].entidades[i].hitboxTitan.alto = 165;
                gs->pantalla[pA].entidades[i].hitboxTitan.ancho = 60;

                gs->pantalla[pA].entidades[i].hitboxNuca.x = gs->pantalla[pA].entidades[i].hitboxTitan.x;
                gs->pantalla[pA].entidades[i].hitboxNuca.y = gs->pantalla[pA].entidades[i].hitboxTitan.y + 60;
                gs->pantalla[pA].entidades[i].hitboxNuca.alto = 20;
                gs->pantalla[pA].entidades[i].hitboxNuca.ancho = 60;

            } 
        }
    
    //==========//


}

void colision_levi_titan(s_GameState *gs)
{
    int i, pA = gs->pantalla_actual;

    for(i=0;i<gs->pantalla[pA].num_entidades;i++) //Bucle para comparar hitbox y encontrar la coincidente
    {
        if(colision(gs, gs->levi.hitbox, gs->pantalla[pA].entidades[i].hitboxTitan))
        {
            continue;    
        }
    }

}

