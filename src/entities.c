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
void actualizar_animacion_titanes(s_GameState *gs, int i);
void cambiar_animacion_titan(s_GameState *gs, e_EstadoTitan nuevaAnim, int i);
void fin_animacion_titan(s_GameState *gs, int i);
void actualizar_animacion_TH(s_GameState *gs);
void cambiar_animacion_TH(s_GameState *gs, e_EstadoTH nuevaAnim);
void fin_animacion_TH(s_GameState *gs);
void movimiento_TH(s_GameState *gs);
void ataques_TH(s_GameState *gs, float cx, float cy, float distanciaX);
void patada1_TH(s_GameState *gs, float cx, float cy, float distanciaX);
void patada2_TH(s_GameState *gs, float cx);
void ataque1_TH(s_GameState *gs, float cx);
void ataque2_TH(s_GameState *gs, float cx);


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
    int i, pA = gs->pantalla_actual;


    /*for(i=0 ; i<gs->pantalla[pA].num_entidades ; i++) // activa las entidades
        if(gs->pantalla[pA].entidades[i].vida > 0)
            gs->pantalla[pA].entidades[i].activo = true;*/

    if(gs->input.keyG == true)    
    {
        for(i=0 ; i<gs->pantalla[pA].num_entidades ; i++) //Desctiva todas las entidades de la pantalla
            if(gs->pantalla[pA].entidades[i].vida > 0)
            {
                gs->pantalla[pA].entidades[i].activo = false;
            }
    }

    if(gs->input.keyG == false) //keyG usada para debugg
    {
        for(i=0 ; i<gs->pantalla[pA].num_entidades ; i++) //Activa todas las entidades de la pantalla
            if(gs->pantalla[pA].entidades[i].vida > 0)
                gs->pantalla[pA].entidades[i].activo = true;
    }
}

void titan_hembra(s_GameState *gs)
{
    //====Actualiza la hitbox del titan hembra y reinicia la hitbox de ataque===========//
    gs->titanHembra.y = 205;
    gs->titanHembra.hitbox.x = gs->titanHembra.x + 255;
    gs->titanHembra.hitbox.y = gs->titanHembra.y + 35;
    gs->titanHembra.hitbox.ancho = 150;
    gs->titanHembra.hitbox.alto = 420;

    gs->titanHembra.hitboxAtaque1.x = 0;
    gs->titanHembra.hitboxAtaque1.y = 0;
    gs->titanHembra.hitboxAtaque1.ancho = 0;
    gs->titanHembra.hitboxAtaque1.alto = 0;

    gs->titanHembra.hitboxAtaque2.x = 0;
    gs->titanHembra.hitboxAtaque2.y = 0;
    gs->titanHembra.hitboxAtaque2.ancho = 0;
    gs->titanHembra.hitboxAtaque2.alto = 0;
    //=======================================================================//

    //========Activa fase 2 y verifica si muere=============//
    if(gs->titanHembra.vida > 25000 || gs->titanHembra.fase2Activa)
        movimiento_TH(gs);
    else if(gs->titanHembra.vida <= 25000)
        cambiar_animacion_TH(gs, CRISTALIZACION_TH);

    if(gs->titanHembra.vida <= 0)
    {
        gs->titanHembra.activa = false;
        gs->nivelCompletado = true;
    }
    //=============================================//

    actualizar_animacion_TH(gs);
}

void movimiento_TH(s_GameState *gs)
{
    float leviMitadX = gs->levi.hitbox.x + (gs->levi.hitbox.ancho/2), leviMitadY = gs->levi.hitbox.y + (gs->levi.hitbox.alto/2);
    float THMitadX = gs->titanHembra.hitbox.x + (gs->titanHembra.hitbox.ancho/2) + 20, THMitadY = gs->titanHembra.hitbox.y + (gs->titanHembra.hitbox.alto/2);
    float cx = leviMitadX - THMitadX, cy = leviMitadY - THMitadY, distanciaX = fabsf(leviMitadX - THMitadX);
    float posXAnteriorTH = gs->titanHembra.x;

    //===================Resta cooldowns=======================//
    if(gs->titanHembra.cooldownAtaque > 0)
        gs->titanHembra.cooldownAtaque -= 1.0f/FPS;
    //==========================================================//

    //============Caso de seguridad para rotar la anmacion=======//
    if(gs->titanHembra.atacando == false)
    {
        if(cx <= 0)
        {
            gs->titanHembra.viendoDer = false;
            gs->titanHembra.animacion.rotarAnim = true;
        }
        else if(cx > 0)
        {
            gs->titanHembra.viendoDer = true;
            gs->titanHembra.animacion.rotarAnim = false;
        }
    }
    //==========================================================//

    //==============Baja cooldown y asigna un caso==============//
    if(gs->titanHembra.cdCasoMovimiento > 0)
        gs->titanHembra.cdCasoMovimiento -= 1.0f/FPS;

    if(gs->titanHembra.cdCasoMovimiento <= 0)
        gs->titanHembra.casoMovimiento = rand()%10;
    //=========================================================//

    //=============Actua segun el caso====================//
    if(gs->levi.ODM.activo && distanciaX <= 250 && gs->titanHembra.atacando == false)
    {
        if(gs->titanHembra.nucaCubierta == false)
            cambiar_animacion_TH(gs, CUBRIRNUCA_TH);
    }
    else
    {
        if(gs->titanHembra.casoMovimiento >= 5)
        {
            if(gs->titanHembra.cdCasoMovimiento <= 0)
                gs->titanHembra.cdCasoMovimiento = 3;

            if(distanciaX >= 80 && gs->titanHembra.atacando == false)
            {
                if(cx <= 0)
                {
                    gs->titanHembra.x -= 3.5f;
                    gs->titanHembra.viendoDer = false;
                    gs->titanHembra.animacion.rotarAnim = true;
                    gs->titanHembra.THQuieta = false;
                }
                else if(cx > 0)
                {
                    gs->titanHembra.x += 3.5f;
                    gs->titanHembra.viendoDer = true;
                    gs->titanHembra.animacion.rotarAnim = false;
                    gs->titanHembra.THQuieta = false;
                }
            }
            else 
                gs->titanHembra.THQuieta = true;
        }   
        else if(gs->titanHembra.casoMovimiento < 5)
        {
            if(gs->titanHembra.cdCasoMovimiento <= 0)
                gs->titanHembra.cdCasoMovimiento = 2;
            gs->titanHembra.THQuieta = true;
        }
    }

    //=====================================================//

    //=======Calcula si hay un desplazamiento==============//
    if(gs->titanHembra.atacando == false && gs->levi.ODM.activo == false)
    {
        if(posXAnteriorTH != gs->titanHembra.x)
            cambiar_animacion_TH(gs, CAMINANDO_TH);
        else
            cambiar_animacion_TH(gs, IDLE_TH);
    }
        
    //======================================================//

    if(distanciaX <= 325 && gs->titanHembra.cooldownAtaque <= 0) //Llama a los ataques si esta a distancia
        ataques_TH(gs, cx, cy, distanciaX);


    //=========Sirve para seguir llamando al ataque si este se ejecuto=========//
    switch(gs->titanHembra.estadoTH)
    {
        case PATADA1_TH:
            patada1_TH(gs, cx, cy, distanciaX);
            break;
        case ATAQUE1_TH:
            ataque1_TH(gs, cx);
            break;
        case ATAQUE2_TH:
            ataque2_TH(gs, cx);
            break;
        case PATADA2_TH:
            patada2_TH(gs,cx);
            break;
    }
    //========================================================================//

    //===========Verifica si levi recibe dano del titan hembra=============//
    if((colision(gs, gs->levi.hitbox, gs->titanHembra.hitboxAtaque1) || colision(gs, gs->levi.hitbox, gs->titanHembra.hitboxAtaque2)) && gs->titanHembra.ataqueHecho == false)
    {
        if(gs->titanHembra.fase2Activa)
            aplicar_dano(gs, 4, 0, false);
        else    
            aplicar_dano(gs, 7, 0, false);

        gs->titanHembra.ataqueHecho = true;
    }
    //==============================================================//

}

void ataques_TH(s_GameState *gs, float cx, float cy, float distanciaX)
{
    int decideAtacar = 0, casoAtaque = 0;

    decideAtacar = rand()%10;

    if(decideAtacar < 3) // Probabilidad de que no ataque 
    {
        gs->titanHembra.cooldownAtaque = 0.5f; 
        return;
    }

    //===========Calcula el caso optimo para atacar==========//
    if(cy >= 50 && cx <= 250)
        patada1_TH(gs, cx, cy, distanciaX);
    else if(cy < 50 && cx <= 300 && cy > (-190))
    {
        casoAtaque = rand()%10;
        printf("Caso golpe: %d\n", casoAtaque);
        if(casoAtaque >= 2 && gs->levi.ODM.activo == false)
            ataque2_TH(gs, cx);
        else
            ataque1_TH(gs, cx);
        }
    else if(cx <= 300 && cy <= (-190))
        patada2_TH(gs, cx);
    //==================================================//
}

void ataque1_TH(s_GameState *gs, float cx)
{
    gs->titanHembra.cooldownAtaque = 2;
    gs->titanHembra.atacando = true;

    if(gs->titanHembra.nucaCubierta)
        cambiar_animacion_TH(gs, ATAQUE1_CNTH);
    else
        cambiar_animacion_TH(gs, ATAQUE1_TH);

    if(gs->titanHembra.animacion.frameActual >= 1)
    {
        if(cx <= 0)
            gs->titanHembra.hitboxAtaque1.x = gs->titanHembra.x + 50;
        else 
            gs->titanHembra.hitboxAtaque1.x = gs->titanHembra.hitbox.x + (gs->titanHembra.hitbox.ancho/2);
            
        gs->titanHembra.hitboxAtaque1.y = gs->titanHembra.y + 120;
        gs->titanHembra.hitboxAtaque1.ancho = 250;
        gs->titanHembra.hitboxAtaque1.alto = 75;
    }
}

void ataque2_TH(s_GameState *gs, float cx)
{
    gs->titanHembra.cooldownAtaque = 2;
    gs->titanHembra.atacando = true;
    cambiar_animacion_TH(gs, ATAQUE2_TH);

    if(gs->titanHembra.animacion.frameActual >= 1 && gs->titanHembra.animacion.frameActual < 5)
    {
        if(cx <= 0)
            gs->titanHembra.hitboxAtaque1.x = gs->titanHembra.x + 50;
        else 
            gs->titanHembra.hitboxAtaque1.x = gs->titanHembra.hitbox.x + (gs->titanHembra.hitbox.ancho/2);
            
        gs->titanHembra.hitboxAtaque1.y = gs->titanHembra.y + 120;
        gs->titanHembra.hitboxAtaque1.ancho = 250;
        gs->titanHembra.hitboxAtaque1.alto = 75;
    }

    else if(gs->titanHembra.animacion.frameActual >= 5)
    {
        if(gs->titanHembra.segundoGolpe == false)
        {
            gs->titanHembra.ataqueHecho = false;
            gs->titanHembra.segundoGolpe = true;
        }
        if(cx <= 0)
            gs->titanHembra.hitboxAtaque1.x = gs->titanHembra.x + 100;
        else 
            gs->titanHembra.hitboxAtaque1.x = gs->titanHembra.hitbox.x + (gs->titanHembra.hitbox.ancho/2) - 50;
            

        gs->titanHembra.hitboxAtaque1.y = gs->titanHembra.y + 130;
        gs->titanHembra.hitboxAtaque1.ancho = 250;
        gs->titanHembra.hitboxAtaque1.alto = 75;
    }
}

void patada1_TH(s_GameState *gs, float cx, float cy, float distanciaX)
{
    gs->titanHembra.cooldownAtaque = 2;
    gs->titanHembra.atacando = true;

    if(gs->titanHembra.nucaCubierta)
        cambiar_animacion_TH(gs, PATADA1_CNTH);
    else
        cambiar_animacion_TH(gs, PATADA1_TH);

    if(gs->titanHembra.animacion.frameActual >= 1)
    {
        if(cx <= 0)
            gs->titanHembra.hitboxAtaque1.x = gs->titanHembra.x + 50;
        else 
            gs->titanHembra.hitboxAtaque1.x = gs->titanHembra.hitbox.x + (gs->titanHembra.hitbox.ancho/2);
            

        gs->titanHembra.hitboxAtaque1.y = gs->titanHembra.y + 330;
        gs->titanHembra.hitboxAtaque1.ancho = 250;
        gs->titanHembra.hitboxAtaque1.alto = 75;
    }

}

void patada2_TH(s_GameState *gs, float cx)
{
    gs->titanHembra.cooldownAtaque = 2;
    gs->titanHembra.atacando = true;
    cambiar_animacion_TH(gs, PATADA2_TH);

    if(gs->titanHembra.animacion.frameActual == 1 || gs->titanHembra.animacion.frameActual == 2)
    {
        if(cx <= 0)
            gs->titanHembra.hitboxAtaque1.x = gs->titanHembra.x + 70;
        else 
            gs->titanHembra.hitboxAtaque1.x = (gs->titanHembra.hitbox.x + gs->titanHembra.hitbox.ancho) + 60;
            
        gs->titanHembra.hitboxAtaque1.y = gs->titanHembra.y + 70;
        gs->titanHembra.hitboxAtaque1.ancho = 100;
        gs->titanHembra.hitboxAtaque1.alto = 100;

        if(cx <= 0)
            gs->titanHembra.hitboxAtaque2.x = gs->titanHembra.hitboxAtaque1.x + gs->titanHembra.hitboxAtaque1.ancho;
        else 
            gs->titanHembra.hitboxAtaque2.x = gs->titanHembra.hitboxAtaque1.x - gs->titanHembra.hitboxAtaque1.ancho;

        gs->titanHembra.hitboxAtaque2.y = gs->titanHembra.hitboxAtaque1.y + gs->titanHembra.hitboxAtaque1.alto;
        gs->titanHembra.hitboxAtaque2.ancho = 100;
        gs->titanHembra.hitboxAtaque2.alto = 100;

    }

}

void movimiento_titanes(s_GameState *gs)
{
    int i, pA = gs->pantalla_actual;

    float leviMitadX = gs->levi.hitbox.x + (gs->levi.hitbox.ancho/2), leviMitadY = gs->levi.hitbox.y + (gs->levi.hitbox.alto/2);
    float titanMitadX, titanMitadY, cx, cy, distanciaX;

    if(gs->tutorialEjecutando)
    {
        if(gs->tutorial.fase != 10)
            return;
    }

    for(i=0;i<gs->pantalla[pA].num_entidades;i++)
    {
        actualizar_animacion_titanes(gs, i);

        if(gs->pantalla[pA].entidades[i].activo)
        {
            titanMitadX = gs->pantalla[pA].entidades[i].hitboxTitan.x + (gs->pantalla[pA].entidades[i].hitboxTitan.ancho/2);
            titanMitadY = gs->pantalla[pA].entidades[i].hitboxTitan.y + (gs->pantalla[pA].entidades[i].hitboxTitan.alto/2);
            cx = leviMitadX - titanMitadX;
            cy = leviMitadY - titanMitadY;
            distanciaX= fabsf(leviMitadX - titanMitadX); //Valor absoluto para calcular las distancias

            if(gs->pantalla[pA].entidades[i].estadoTitan == CAMINANDOTITAN)
            {
                if(distanciaX > 500)
                    gs->pantalla[pA].entidades[i].animacion.velocidadAnim = 14;
                else 
                    gs->pantalla[pA].entidades[i].animacion.velocidadAnim = 10;
            }

            if(gs->pantalla[pA].entidades[i].tiempoGolpeRegistrado > 0)
                gs->pantalla[pA].entidades[i].tiempoGolpeRegistrado -= 1.0f/FPS;


            if(gs->pantalla[pA].entidades[i].tipo == 1) ///////////////////Titan 1////////////////////////////
            {
                if(gs->pantalla[pA].entidades[i].quieto == false)
                {
                    if(cx < 500 && cx > 0 && distanciaX > 80) //Calcula el rango para empezar a moverse
                    {
                        if(!(fabs(cx) <= 140 && cy < 35 && cy > -320)) //Si puede ejecutar ataque 2 no avanza mas de lo necesario
                        {
                            gs->pantalla[pA].entidades[i].x += gs->pantalla[pA].entidades[i].velocidadX;
                            gs->pantalla[pA].entidades[i].animacion.rotarAnim = false;
                            gs->pantalla[pA].entidades[i].viendoDerecha = true;
                            cambiar_animacion_titan(gs, CAMINANDOTITAN, i);
                        }
                    }
                    else if(cx > -500 && cx < 0 && distanciaX > 80) //Calcula el rango para empezar a moverse
                    {
                        if(!(fabs(cx) <= 140 && cy < 35 && cy > -320)) //Si puede ejecutar ataque 2 no avanza mas de lo necesario
                        {
                            gs->pantalla[pA].entidades[i].x -= gs->pantalla[pA].entidades[i].velocidadX;
                            gs->pantalla[pA].entidades[i].animacion.rotarAnim = true;
                            gs->pantalla[pA].entidades[i].viendoDerecha = false;
                            cambiar_animacion_titan(gs, CAMINANDOTITAN, i);
                        }
                    }
                
                    else if(distanciaX > 500) //Si no esta a rango se mueve de un lado para otro
                    {   
                        if(gs->pantalla[pA].entidades[i].tiempoQuieto > 0)
                        {
                            cambiar_animacion_titan(gs, QUIETO, i);
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
                            cambiar_animacion_titan(gs, CAMINANDOTITAN, i);

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
                            cambiar_animacion_titan(gs, CAMINANDOTITAN, i);

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

                else 
                {
                    gs->pantalla[pA].entidades[i].hitboxAtaqueBasico.x = 0;
                    gs->pantalla[pA].entidades[i].hitboxAtaqueBasico.y = 0;
                    gs->pantalla[pA].entidades[i].hitboxAtaqueBasico.alto = 0;
                    gs->pantalla[pA].entidades[i].hitboxAtaqueBasico.ancho = 0;
                }
            }

        //=============================Titan 2================================================================================//

            else if(gs->pantalla[pA].entidades[i].tipo == 2) 
            {
                if(gs->pantalla[pA].entidades[i].quieto == false)  //Calcula distancias para activar un comportamiento
                {
                    if(gs->pantalla[pA].entidades[i].saltoActivo == false && gs->pantalla[pA].entidades[i].tiempoAtaqueActivo <= 0)  
                    {
                        if(cx < 500 && cx > 0 && distanciaX >= 40)
                        {
                            gs->pantalla[pA].entidades[i].x += gs->pantalla[pA].entidades[i].velocidadX;
                            gs->pantalla[pA].entidades[i].animacion.rotarAnim = false;
                            gs->pantalla[pA].entidades[i].viendoDerecha = true;
                            cambiar_animacion_titan(gs, CAMINANDOTITAN, i);
                        }
                        else if(cx > -500 && cx < 0 && distanciaX >= 40 )
                        {
                            gs->pantalla[pA].entidades[i].x -= gs->pantalla[pA].entidades[i].velocidadX;
                            gs->pantalla[pA].entidades[i].animacion.rotarAnim = true;
                            gs->pantalla[pA].entidades[i].viendoDerecha = false;
                            cambiar_animacion_titan(gs, CAMINANDOTITAN, i);
                        }
                        else if(distanciaX > 500)
                        {
                            gs->pantalla[pA].entidades[i].quieto = false;
                            cambiar_animacion_titan(gs, SENTADO, i);
                        }
                        else   
                        {
                            if(gs->pantalla[pA].entidades[i].estadoTitan != ATAQUE && gs->pantalla[pA].entidades[i].estadoTitan != MORDISCO && gs->pantalla[pA].entidades[i].estadoTitan != SALTO)
                            cambiar_animacion_titan(gs, QUIETO, i);
                        }
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

                else 
                {
                    gs->pantalla[pA].entidades[i].hitboxAtaqueBasico.x = 0;
                    gs->pantalla[pA].entidades[i].hitboxAtaqueBasico.y = 0;
                    gs->pantalla[pA].entidades[i].hitboxAtaqueBasico.alto = 0;
                    gs->pantalla[pA].entidades[i].hitboxAtaqueBasico.ancho = 0;
                }
            }
        }
    }
}

void ataque_titanes(s_GameState *gs, int i, float cy, float cx)
{
    int pA = gs->pantalla_actual, tipo = gs->pantalla[pA].entidades[i].tipo;
    s_Entidades *entidad = gs->pantalla[pA].entidades;

    if(entidad[i].agarre.activo)  //Sirve para que se sigan ejecutando las funciones necesarias si levi esta agarrado, esto para evitar bugs de que otro titan lo agarre
    {
        if(entidad[i].tipo == 1)
            titan1_ataque2(gs, i, entidad, cx, cy);
        agarre_titanes(gs, i);
        return;
    }

    if(tipo == 1)  //Calcula que ataque debe usar el titan 1
    {
        if(entidad[i].tiempoAtaqueActivo <= 0)
        {
            if(fabs(cx) <= 125 && cy > 30 || entidad[i].patadaActiva == true)
                titan1_ataque1(gs, i, entidad, cx, cy);
            else if((fabs(cx) <= 140 && cy < 35 && cy > -320) || entidad[i].ataqueActivo == true)
                titan1_ataque2(gs, i, entidad, cx, cy);
        }

    }

    else if(tipo == 2) //Calcula que ataque debe usar el titan 2
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
                entidad[i].casoAtaque = (rand()%6) + 2; //Genera un numero random con el fin de que sea aleatorio si el titan 2 pega un ataque o intenta agarrarlo

            else 
                entidad[i].casoAtaque = 0;

        }

        if(entidad[i].casoAtaque == 1)
            titan2_ataque1(gs, i, entidad, cx, cy);

        else if(entidad[i].casoAtaque >= 1 && entidad[i].casoAtaque < 9)
            titan2_ataque2(gs, i, entidad, cx, cy);
        
        else if(entidad[i].casoAtaque >= 9) 
            titan2_ataque3(gs, i, entidad, cx, cy);
    }

}

void titan1_ataque1(s_GameState *gs, int i, s_Entidades *entidad, float cx, float cy)
{
    //Patada

    //Funcion que genera una hitbox de ataque que se mueve acorde a la animacion para mas realismo

    if(entidad[i].viendoDerecha == false && entidad[i].patadaActiva == false)
        entidad[i].ataqueDerecha = false;
    else if(entidad[i].viendoDerecha == true && entidad[i].patadaActiva == false)
        entidad[i].ataqueDerecha = true;

    if(entidad[i].ataqueDerecha == false)
    {
        if(entidad[i].patadaActiva )
        {
            entidad[i].hitboxAtaqueBasico.x -= 3;
            entidad[i].distanciaRecorridaAtaque += 3;
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
            cambiar_animacion_titan(gs, PATADA, i);

            if(entidad[i].estadoTitan == PATADA && entidad[i].animacion.frameActual == 1)
            {
                if(entidad[i].estadoTitan == PATADA && entidad[i].animacion.frameActual == 1)
                {
                    entidad[i].hitboxAtaqueBasico.x = entidad[i].hitboxTitan.x - 10;
                    entidad[i].hitboxAtaqueBasico.y = entidad[i].hitboxTitan.y + 300;
                    entidad[i].hitboxAtaqueBasico.alto = 80;
                    entidad[i].hitboxAtaqueBasico.ancho = 40;
                    entidad[i].patadaActiva = true;
                }
            }
        }
    }

    else if(entidad[i].ataqueDerecha == true)
    {
        if(entidad[i].patadaActiva)
        {
            entidad[i].hitboxAtaqueBasico.x += 3;
            entidad[i].distanciaRecorridaAtaque += 3;
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
            cambiar_animacion_titan(gs, PATADA, i);
            
            if(entidad[i].animacion.frameActual == 1)
            {
                if(entidad[i].estadoTitan == PATADA && entidad[i].animacion.frameActual == 1)
                {
                    entidad[i].hitboxAtaqueBasico.x = entidad[i].hitboxTitan.x + entidad[i].hitboxTitan.ancho - 10;
                    entidad[i].hitboxAtaqueBasico.y = entidad[i].hitboxTitan.y + 300;
                    entidad[i].hitboxAtaqueBasico.alto = 80;
                    entidad[i].hitboxAtaqueBasico.ancho = 40;
                    entidad[i].patadaActiva = true;
                }
            }
        }
    }

    if(colision(gs, gs->levi.hitbox, entidad[i].hitboxAtaqueBasico) && entidad[i].tiempoGolpeRegistrado <= 0)
    {
        entidad[i].tiempoGolpeRegistrado = 1.0f;
        aplicar_dano(gs, 4, 30, entidad[i].ataqueDerecha);
    }
    
}

void titan1_ataque2(s_GameState *gs, int i, s_Entidades *entidad, float cx, float cy)
{
    //Agarre titan 1: 3 fases; si levi es agarrado y no se suelta el titan lleva a levi hasta su boca para morderlo y matarlo instantaneamente

    //=======Sirve para saber en que lado activar las hitbox de ataque========//
    if(entidad[i].viendoDerecha == false && entidad[i].ataqueActivo == false)
        entidad[i].ataqueDerecha = false;
    else if(entidad[i].viendoDerecha == true && entidad[i].ataqueActivo == false)
        entidad[i].ataqueDerecha = true;
    //========================================================================//

    if(entidad[i].agarreFase3Activa)  //Fase 3: Espera el tiempo necesario para ejecutar el mordisco
    {
        if(entidad[i].cooldownMordida > 0)
            entidad[i].cooldownMordida -= 1.0f/FPS;
        else if(entidad[i].cooldownMordida <= 0)
        {
            if(colision(gs, gs->levi.hitbox, entidad[i].hitboxAtaqueBasico) && gs->levi.vida > 0)
            {
                cambiar_animacion_titan(gs, MORDISCO, i);
                aplicar_dano(gs, gs->levi.vida, 0, false);
            }
        }
    }

    else if(entidad[i].agarreFase2Activa) //Fase 2: Si levi no se ha soltado en el momento de que la distancia recorrida de la fase 1 se cumpla activa la fase 2 la cual lo lleva hasta la boca del titan
    {
        entidad[i].hitboxAtaqueBasico.x = entidad[i].hitboxTitan.x + 30;
        entidad[i].hitboxAtaqueBasico.y = entidad[i].hitboxTitan.y + 30;
        entidad[i].hitboxAtaqueBasico.ancho = 30;
        entidad[i].hitboxAtaqueBasico.alto = 30;

        if(entidad[i].ataqueDerecha == true)
            entidad[i].agarre.manoHB.x -= 3.5f;
        else if(entidad[i].ataqueDerecha == false)
            entidad[i].agarre.manoHB.x += 3.5f;
        entidad[i].agarre.manoHB.y -= 3.5f;
        entidad[i].distanciaRecorridaAtaque += 3.5f;

        if(entidad[i].distanciaRecorridaAtaque >= 360)
        {
            entidad[i].agarreFase3Activa = true;
            entidad[i].agarreFase2Activa = false;
            entidad[i].cooldownMordida = 0.2f;
        }
            
    }

    else if(entidad[i].ataqueActivo)
    {
        entidad[i].agarre.manoHB.y += 3;
        entidad[i].hitboxAtaqueBasico.y += 3;
        entidad[i].distanciaRecorridaAtaque += 3;
        if(entidad[i].ataqueDerecha == false)
            entidad[i].hitboxAtaqueBasico.ancho += 0.4;
        else if(entidad[i].ataqueDerecha == true)
        {
            entidad[i].hitboxAtaqueBasico.ancho += 0.4;
            entidad[i].hitboxAtaqueBasico.x -= 0.4;
        }
        entidad[i].quieto = true;

        if(colision(gs,gs->levi.hitbox, entidad[i].agarre.manoHB) && gs->levi.agarrado == false && gs->levi.invulnerabilidad == false
                && gs->levi.tiempoInvulnerabilidad <= 0)
        {
            entidad[i].agarre.activo = true;
            entidad[i].cooldownMordida = 0.5f; //Tiempo de espera antes que el titan pueda morder justo despues de agarrar
            agarre_titanes(gs, i);
        }
        else if(colision(gs, gs->levi.hitbox, entidad[i].hitboxAtaqueBasico) && entidad[i].tiempoGolpeRegistrado <= 0)
        {
            entidad[i].tiempoGolpeRegistrado = 1.0f;
            aplicar_dano(gs, 2, 0, false);
        }

        if(entidad[i].distanciaRecorridaAtaque >= 210) //Usado en hartas funciones, calcula cuanta distancia recorrio para activar la siguiente fase
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
        cambiar_animacion_titan(gs, ATAQUE, i);

        if(entidad[i].estadoTitan == ATAQUE && entidad[i].animacion.frameActual == 3)
        {
            entidad[i].agarre.manoHB.y = entidad[i].hitboxTitan.y - 30;
            entidad[i].agarre.manoHB.alto = 30;
            entidad[i].agarre.manoHB.ancho = 30;
            entidad[i].agarre.manoHB.color = al_map_rgb(0, 0, 255);

            entidad[i].hitboxAtaqueBasico.y = entidad[i].agarre.manoHB.y;
            entidad[i].hitboxAtaqueBasico.alto = 30;
            entidad[i].hitboxAtaqueBasico.ancho = 30;

            entidad[i].ataqueActivo = true;

            if(entidad[i].ataqueDerecha == false)
            {
                entidad[i].agarre.manoHB.x = entidad[i].hitboxTitan.x - 130;
                entidad[i].hitboxAtaqueBasico.x = entidad[i].agarre.manoHB.x + entidad[i].agarre.manoHB.ancho + 2;
            }
            else if(entidad[i].ataqueDerecha == true)
            {
                entidad[i].agarre.manoHB.x = entidad[i].hitboxTitan.x + entidad[i].hitboxTitan.ancho - entidad[i].agarre.manoHB.ancho + 130;
                entidad[i].hitboxAtaqueBasico.x = entidad[i].agarre.manoHB.x - entidad[i].agarre.manoHB.ancho - 2; 
            }
        }
    }

}

void titan2_ataque1(s_GameState *gs, int i, s_Entidades *entidad, float cx, float cy)
{
    //Salto

    //Si levi esta a cierta distancia vertical del titan 2 este salta para morderlo

    if(entidad[i].saltoActivo == false)
    {
        entidad[i].velocidadY = -18;
        entidad[i].saltoActivo = true;
        entidad[i].frameActivacion = true;

        if(cx > 0)
            entidad[i].velocidadXSalto = 4.0f;

        else 
            entidad[i].velocidadXSalto = -4.0f;

        cambiar_animacion_titan(gs, SALTO, i);
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
                aplicar_dano(gs, 1, 0, entidad[i].viendoDerecha);
            }

            if(entidad[i].tiempoAtaqueActivo <= 0)
            {
                entidad[i].cooldownAtaque = 1;
                entidad[i].quieto = false;
            }
        }
        else 
        {
            cambiar_animacion_titan(gs, ATAQUE, i);

            if(entidad[i].animacion.frameActual == 1)
                entidad[i].tiempoAtaqueActivo = 0.4;
        }
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
                aplicar_dano(gs, 1, 0, entidad[i].viendoDerecha);
            }

            if(entidad[i].tiempoAtaqueActivo <= 0)
            {
                entidad[i].cooldownAtaque = 1;
                entidad[i].quieto = false;
            }
        }
        else 
        {
            cambiar_animacion_titan(gs, ATAQUE, i);

            if(entidad[i].animacion.frameActual == 1)
                entidad[i].tiempoAtaqueActivo = 0.4;
        }
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

            if(colision(gs,gs->levi.hitbox, entidad[i].agarre.manoHB) && gs->levi.agarrado == false && gs->levi.invulnerabilidad == false
                && gs->levi.tiempoInvulnerabilidad <= 0)
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
        {
            cambiar_animacion_titan(gs, INTENTO_AGARRE, i);
            entidad[i].tiempoAtaqueActivo = 0.4;
        }
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

            if(colision(gs,gs->levi.hitbox, entidad[i].agarre.manoHB) && gs->levi.agarrado == false && gs->levi.invulnerabilidad == false
                && gs->levi.tiempoInvulnerabilidad <= 0)
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
        {
            cambiar_animacion_titan(gs, INTENTO_AGARRE, i);
            entidad[i].tiempoAtaqueActivo = 0.4;
        }
    }
}

void agarre_titanes(s_GameState *gs, int i)
{
    //Funcion que se ejecuta si levi esta agarrado por un titan, lo mantiene en la posicion de su mano (manoHB)

    int pA = gs->pantalla_actual, cont;
    s_Entidades *entidad = gs->pantalla[pA].entidades;

    gs->variables.bloquearControles = true;
    gs->levi.agarrado = true;
    gs->variables.gravedad = 0;
    gs->levi.habilidad1Activa = false;
    gs->levi.habilidad2Activa = false;

    if(entidad[i].tipo == 1)
    {
        gs->variables.agarradoPorTitan1 = true;
        gs->levi.y = entidad[i].agarre.manoHB.y - 35;
        gs->levi.x = entidad[i].agarre.manoHB.x - 35;
    }

    else if(entidad[i].tipo == 2)
    {
        gs->variables.agarradoPorTitan1 = false;
        if(entidad[i].estadoTitan != MORDISCO)
            cambiar_animacion_titan(gs, AGARRANDO, i);
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
                entidad[i].animacion.rotarAnim = true;
                cambiar_animacion_titan(gs, MORDISCO, i);
            }
            else if(entidad[i].viendoDerecha == true)
            {
                entidad[i].mordidaHB.x = entidad[i].hitboxTitan.x + entidad[i].hitboxTitan.ancho - 15;
                entidad[i].mordidaHB.y = entidad[i].hitboxTitan.y + 60;
                entidad[i].mordidaHB.alto = 40;
                entidad[i].mordidaHB.ancho = 55;
                entidad[i].mordidaHB.color = al_map_rgb(255, 0, 0);
                entidad[i].tiempoMordidaActivo = 0.4f; 
                entidad[i].animacion.rotarAnim = false;
                cambiar_animacion_titan(gs, MORDISCO, i);
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

    if(gs->variables.agarradoPorTitan1)
        cont = 10;
    else    
        cont = 5;

    if(gs->input.keySpace)
    {
        gs->levi.contSoltarse++;
        gs->input.keySpace = false;
    }
    if(gs->levi.contSoltarse >= cont)  //Reestablece variables para evitar bugs 
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
    //Aplica dano a levi y si se pasa como parametro el empuje lo empuja esa cantidad

    if(gs->tutorialEjecutando)
        return;

    if(ataqueDerecha == false)
        empuje = (-empuje);

    if(gs->levi.tiempoInvulnerabilidad <= 0 && gs->levi.invulnerabilidad == false)
    {
        gs->levi.vida -= dano;
        gs->levi.velocidadX = empuje;
        gs->levi.dash.tiempoRecuperacionDash = 0.15f;
        if(gs->titanHembra.activa == false)
            gs->levi.cooldownParry = 0.5f;
    }
}

void gravedad_titanes(s_GameState *gs)
{
    int i, pA = gs->pantalla_actual;
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
                        if(gs->pantalla[pA].entidades[i].estadoTitan == SALTO)
                            cambiar_animacion_titan(gs, ATERRIZAJE, i);
                        gs->pantalla[pA].entidades[i].saltoActivo = false;
                        gs->pantalla[pA].entidades[i].y = gs->pantalla[pA].hitbox[j].y - gs->pantalla[pA].entidades[i].hitboxTitan.alto - 22;
                        gs->pantalla[pA].entidades[i].velocidadY = 0;
                    }
                    
                }
}

void hitbox_entidades(s_GameState *gs, s_Assets *assets)
{
    //Establece las hitboxes de los titanes

    int i, pA = gs->pantalla_actual;
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
            
            else if(gs->pantalla[pA].entidades[i].tipo == 2)
            {
                gs->pantalla[pA].entidades[i].hitboxTitan.x = gs->pantalla[pA].entidades[i].x + 70;
                gs->pantalla[pA].entidades[i].hitboxTitan.y = gs->pantalla[pA].entidades[i].y + 24;
                gs->pantalla[pA].entidades[i].hitboxTitan.alto = 168;
                gs->pantalla[pA].entidades[i].hitboxTitan.ancho = 60;

                gs->pantalla[pA].entidades[i].hitboxNuca.x = gs->pantalla[pA].entidades[i].hitboxTitan.x;
                gs->pantalla[pA].entidades[i].hitboxNuca.y = gs->pantalla[pA].entidades[i].hitboxTitan.y + 60;
                gs->pantalla[pA].entidades[i].hitboxNuca.alto = 20;
                gs->pantalla[pA].entidades[i].hitboxNuca.ancho = 60;

            } 
        }
    
    //==========//


}

void cambiar_animacion_titan(s_GameState *gs, e_EstadoTitan nuevaAnim, int i)
{
    int pA = gs->pantalla_actual;
    s_Entidades *entidad = gs->pantalla[pA].entidades;

    if(entidad[i].estadoTitan == nuevaAnim) 
        return;

    entidad[i].estadoTitan = nuevaAnim;
    entidad[i].animacion.contadorAnim = 0;

    if(entidad[i].tipo == 1)
    {
        switch(entidad[i].estadoTitan)
        {
            case QUIETO:
                entidad[i].animacion.cantidadFrames = 1;
                entidad[i].animacion.contadorAnim = 0;
                entidad[i].animacion.frameActual = 0;
                entidad[i].animacion.fila_ss = 0;
                entidad[i].animacion.velocidadAnim = 10;
                entidad[i].animacion.repetir = false;
                break;
            case CAMINANDOTITAN:
                entidad[i].animacion.cantidadFrames = 8;
                entidad[i].animacion.contadorAnim = 0;
                entidad[i].animacion.frameActual = 0;
                entidad[i].animacion.fila_ss = 3;
                entidad[i].animacion.repetir = true;
                entidad[i].animacion.velocidadAnim = 14;
                break;
            case ATAQUE:
                entidad[i].animacion.cantidadFrames = 10;
                entidad[i].animacion.contadorAnim = 0;
                entidad[i].animacion.frameActual = 0;
                entidad[i].animacion.fila_ss = 0;
                entidad[i].animacion.velocidadAnim = 9;
                entidad[i].animacion.repetir = false;
                break;
            case AGARRANDO:
                entidad[i].animacion.cantidadFrames = 7;
                entidad[i].animacion.contadorAnim = 0;
                entidad[i].animacion.frameActual = 0;
                entidad[i].animacion.fila_ss = 1;
                entidad[i].animacion.velocidadAnim = 7;
                entidad[i].animacion.repetir = false;
                break;
            case MORDISCO:
                entidad[i].animacion.cantidadFrames = 3;
                entidad[i].animacion.contadorAnim = 0;
                entidad[i].animacion.frameActual = 0;
                entidad[i].animacion.fila_ss = 2;
                entidad[i].animacion.velocidadAnim = 5;
                entidad[i].animacion.repetir = false;
                break;
            case PATADA:
                entidad[i].animacion.cantidadFrames = 7;
                entidad[i].animacion.contadorAnim = 0;
                entidad[i].animacion.frameActual = 0;
                entidad[i].animacion.fila_ss = 4;
                entidad[i].animacion.velocidadAnim = 8;
                entidad[i].animacion.repetir = false;
                break;
        }
    }

    else if(entidad[i].tipo == 2)
    {
        switch(entidad[i].estadoTitan)
        {
           case CAMINANDOTITAN:
                entidad[i].animacion.cantidadFrames = 8;
                entidad[i].animacion.contadorAnim = 0;
                entidad[i].animacion.frameActual = 0;
                entidad[i].animacion.fila_ss = 0;
                entidad[i].animacion.velocidadAnim = 7;
                entidad[i].animacion.repetir = true;
                break; 
            case SALTO:
                entidad[i].animacion.cantidadFrames = 4;
                entidad[i].animacion.contadorAnim = 0;
                entidad[i].animacion.frameActual = 0;
                entidad[i].animacion.fila_ss = 1;
                entidad[i].animacion.velocidadAnim = 12;
                entidad[i].animacion.repetir = false;
                break; 
            case ATERRIZAJE:
                entidad[i].animacion.cantidadFrames = 2;
                entidad[i].animacion.contadorAnim = 0;
                entidad[i].animacion.frameActual = 0;
                entidad[i].animacion.fila_ss = 2;
                entidad[i].animacion.velocidadAnim = 14;
                entidad[i].animacion.repetir = false;
                break; 
            case INTENTO_AGARRE:
                entidad[i].animacion.cantidadFrames = 5;
                entidad[i].animacion.contadorAnim = 0;
                entidad[i].animacion.frameActual = 0;
                entidad[i].animacion.fila_ss = 3;
                entidad[i].animacion.velocidadAnim = 8;
                entidad[i].animacion.repetir = false;
                break; 
            case QUIETO:
                entidad[i].animacion.cantidadFrames = 1;
                entidad[i].animacion.contadorAnim = 0;
                entidad[i].animacion.frameActual = 0;
                entidad[i].animacion.fila_ss = 3;
                entidad[i].animacion.velocidadAnim = 15;
                entidad[i].animacion.repetir = true;
                break; 
            case AGARRANDO:
                entidad[i].animacion.cantidadFrames = 2;
                entidad[i].animacion.contadorAnim = 0;
                entidad[i].animacion.frameActual = 0;
                entidad[i].animacion.fila_ss = 4;
                entidad[i].animacion.velocidadAnim = 20;
                entidad[i].animacion.repetir = true;
                break;
            case MORDISCO:
                entidad[i].animacion.cantidadFrames = 4;
                entidad[i].animacion.contadorAnim = 0;
                entidad[i].animacion.frameActual = 0;
                entidad[i].animacion.fila_ss = 5;
                entidad[i].animacion.velocidadAnim = 7;
                entidad[i].animacion.repetir = false;
                break;
            case ATAQUE:
                entidad[i].animacion.cantidadFrames = 5;
                entidad[i].animacion.contadorAnim = 0;
                entidad[i].animacion.frameActual = 0;
                entidad[i].animacion.fila_ss = 6;
                entidad[i].animacion.velocidadAnim = 7;
                entidad[i].animacion.repetir = false;
                break;
            case SENTADO:
                entidad[i].animacion.cantidadFrames = 3;
                entidad[i].animacion.contadorAnim = 0;
                entidad[i].animacion.frameActual = 0;
                entidad[i].animacion.fila_ss = 7;
                entidad[i].animacion.velocidadAnim = 7;
                entidad[i].animacion.repetir = false;
                break;
            case LEVANTANDOSE:
                entidad[i].animacion.cantidadFrames = 2;
                entidad[i].animacion.contadorAnim = 0;
                entidad[i].animacion.frameActual = 0;
                entidad[i].animacion.fila_ss = 8;
                entidad[i].animacion.velocidadAnim = 7;
                entidad[i].animacion.repetir = false;
                break;
        }
    }

}

void actualizar_animacion_titanes(s_GameState *gs, int i)
{
    int pA = gs->pantalla_actual;
    s_Entidades *entidad = gs->pantalla[pA].entidades;

    entidad[i].animacion.contadorAnim++;

    if(entidad[i].animacion.contadorAnim >= entidad[i].animacion.velocidadAnim) 
    {
        entidad[i].animacion.contadorAnim = 0;
        entidad[i].animacion.frameActual++;

        if(entidad[i].animacion.frameActual >= entidad[i].animacion.cantidadFrames) 
            fin_animacion_titan(gs, i);
    }
}

void fin_animacion_titan(s_GameState *gs, int i)
{
    s_Entidades *entidad = gs->pantalla[gs->pantalla_actual].entidades;

    if(entidad[i].animacion.repetir == true)
        entidad[i].animacion.frameActual = 0;
    else if(entidad[i].animacion.repetir == false)
    {
        if(entidad[i].tipo == 1)
        {
            switch(entidad[i].estadoTitan)
            {
                case ATAQUE:
                    if(gs->levi.agarrado)
                        cambiar_animacion_titan(gs, AGARRANDO, i);
                    else
                        cambiar_animacion_titan(gs, QUIETO, i);
                    break;
                case PATADA:
                    cambiar_animacion_titan(gs, QUIETO, i);
                    break;
                default:
                    entidad[i].animacion.frameActual = entidad[i].animacion.cantidadFrames - 1;
            }
        }
        else if(entidad[i].tipo == 2)
        {
            switch(entidad[i].estadoTitan)
            {
                case ATERRIZAJE:
                    cambiar_animacion_titan(gs, QUIETO, i);
                    break;
                case MORDISCO:
                    cambiar_animacion_titan(gs, AGARRANDO, i);
                    break;
                case ATAQUE:
                    cambiar_animacion_titan(gs, QUIETO, i);
                    break;
                default:
                    entidad[i].animacion.frameActual = entidad[i].animacion.cantidadFrames - 1;
            }
        }
    }
}

void cambiar_animacion_TH(s_GameState *gs, e_EstadoTH nuevaAnim)
{
    if(gs->titanHembra.estadoTH == nuevaAnim) 
        return;

    gs->titanHembra.estadoTH = nuevaAnim;
    gs->titanHembra.animacion.contadorAnim = 0;

    switch(gs->titanHembra.estadoTH)
    {
        case IDLE_TH:
            gs->titanHembra.animacion.cantidadFrames = 4;
            gs->titanHembra.animacion.fila_ss = 0;
            gs->titanHembra.animacion.velocidadAnim = 10;
            gs->titanHembra.animacion.contadorAnim = 0;
            gs->titanHembra.animacion.frameActual = 0;
            gs->titanHembra.animacion.repetir = true;
            break;

        case ATAQUE1_TH:
            gs->titanHembra.animacion.cantidadFrames = 5;
            gs->titanHembra.animacion.fila_ss = 1;
            gs->titanHembra.animacion.velocidadAnim = 6;
            gs->titanHembra.animacion.contadorAnim = 0;
            gs->titanHembra.animacion.frameActual = 0;
            gs->titanHembra.animacion.repetir = false;
            break;
        case ATAQUE2_TH:
            gs->titanHembra.animacion.cantidadFrames = 8;
            gs->titanHembra.animacion.fila_ss = 2;
            gs->titanHembra.animacion.velocidadAnim = 6;
            gs->titanHembra.animacion.contadorAnim = 0;
            gs->titanHembra.animacion.frameActual = 0;
            gs->titanHembra.animacion.repetir = false;
            break;

        case PATADA1_TH:
            gs->titanHembra.animacion.cantidadFrames = 6;
            gs->titanHembra.animacion.fila_ss = 3;
            gs->titanHembra.animacion.velocidadAnim = 8;
            gs->titanHembra.animacion.contadorAnim = 0;
            gs->titanHembra.animacion.frameActual = 0;
            gs->titanHembra.animacion.repetir = false;
            break;

        case PATADA2_TH:
            gs->titanHembra.animacion.cantidadFrames = 8;
            gs->titanHembra.animacion.fila_ss = 4;
            gs->titanHembra.animacion.velocidadAnim = 8;
            gs->titanHembra.animacion.contadorAnim = 0;
            gs->titanHembra.animacion.frameActual = 0;
            gs->titanHembra.animacion.repetir = false;
            break;

        case CAMINANDO_TH:
            gs->titanHembra.animacion.cantidadFrames = 8;
            gs->titanHembra.animacion.fila_ss = 5;
            gs->titanHembra.animacion.velocidadAnim = 10;
            gs->titanHembra.animacion.contadorAnim = 0;
            gs->titanHembra.animacion.frameActual = 0;
            gs->titanHembra.animacion.repetir = true;
            break;

        case CUBRIRNUCA_TH:
            gs->titanHembra.animacion.cantidadFrames = 3;
            gs->titanHembra.animacion.fila_ss = 6;
            gs->titanHembra.animacion.velocidadAnim = 4;
            gs->titanHembra.animacion.contadorAnim = 0;
            gs->titanHembra.animacion.frameActual = 0;
            gs->titanHembra.animacion.repetir = false;
            gs->titanHembra.nucaCubierta = true;
            break;

        case IDLE_CNTH:
            gs->titanHembra.animacion.cantidadFrames = 4;
            gs->titanHembra.animacion.fila_ss = 7;
            gs->titanHembra.animacion.velocidadAnim = 10;
            gs->titanHembra.animacion.contadorAnim = 0;
            gs->titanHembra.animacion.frameActual = 0;
            gs->titanHembra.animacion.repetir = true;
            break;

        case ATAQUE1_CNTH:
            gs->titanHembra.animacion.cantidadFrames = 5;
            gs->titanHembra.animacion.fila_ss = 8;
            gs->titanHembra.animacion.velocidadAnim = 6;
            gs->titanHembra.animacion.contadorAnim = 0;
            gs->titanHembra.animacion.frameActual = 0;
            gs->titanHembra.animacion.repetir = false;
            break;

        case PATADA1_CNTH:
            gs->titanHembra.animacion.cantidadFrames = 6;
            gs->titanHembra.animacion.fila_ss = 9;
            gs->titanHembra.animacion.velocidadAnim = 8;
            gs->titanHembra.animacion.contadorAnim = 0;
            gs->titanHembra.animacion.frameActual = 0;
            gs->titanHembra.animacion.repetir = false;
            break;

        case CRISTALIZACION_TH:
            gs->titanHembra.animacion.cantidadFrames = 4;
            gs->titanHembra.animacion.fila_ss = 10;
            gs->titanHembra.animacion.velocidadAnim = 20;
            gs->titanHembra.animacion.contadorAnim = 0;
            gs->titanHembra.animacion.frameActual = 0;
            gs->titanHembra.animacion.repetir = false;
            break;

    }
}

void actualizar_animacion_TH(s_GameState *gs)
{
    if(gs->titanHembra.estadoTH != CUBRIRNUCA_TH && gs->titanHembra.estadoTH != IDLE_CNTH && gs->titanHembra.estadoTH != ATAQUE1_CNTH && gs->titanHembra.estadoTH != PATADA1_CNTH)
        gs->titanHembra.nucaCubierta = false;

    gs->titanHembra.animacion.contadorAnim++;

        if(gs->titanHembra.animacion.contadorAnim >= gs->titanHembra.animacion.velocidadAnim) 
        {
            gs->titanHembra.animacion.contadorAnim = 0;
            gs->titanHembra.animacion.frameActual++;

            if(gs->titanHembra.animacion.frameActual >= gs->titanHembra.animacion.cantidadFrames) 
                fin_animacion_TH(gs);
        }
}

void fin_animacion_TH(s_GameState *gs)
{
    int resetCD;

    if(gs->titanHembra.animacion.repetir)
        gs->titanHembra.animacion.frameActual = 0;
    else if(gs->titanHembra.estadoTH == CRISTALIZACION_TH)
    {
        gs->titanHembra.fase2Activa = true;
        cambiar_animacion_TH(gs, IDLE_TH);
    }
    else
    {
        resetCD = rand()%10;
        if(resetCD <= 6 && gs->titanHembra.estadoTH != PATADA1_TH)
            gs->titanHembra.cooldownAtaque = 0;
        else if(resetCD <= 3 && gs->titanHembra.estadoTH == PATADA1_TH)
            gs->titanHembra.cooldownAtaque = 0;

        gs->titanHembra.atacando = false;
        gs->titanHembra.ataqueHecho = false;
        gs->titanHembra.segundoGolpe = false;

        if(gs->titanHembra.estadoTH == CUBRIRNUCA_TH || gs->titanHembra.estadoTH == ATAQUE1_CNTH || gs->titanHembra.estadoTH == PATADA1_CNTH)
            cambiar_animacion_TH(gs, IDLE_CNTH);
        else
            cambiar_animacion_TH(gs, IDLE_TH);
    }
}
