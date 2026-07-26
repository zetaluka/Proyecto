#include "commons.h"

void activa_entidades(s_GameState *gs);
void hitbox_entidades(s_GameState *gs, s_Assets *assets);
void gravedad_titanes(s_GameState *gs);
void colision_titanes(s_GameState *gs);
void movimiento_titanes(s_GameState *gs);
void ataque_titanes(s_GameState *gs, int i, float cy, float cx);
void agarre_titanes(s_GameState *gs, int i);


void genera_entidades(s_GameState *gs, s_Assets *assets)
{

    activa_entidades(gs);
    movimiento_titanes(gs);
    gravedad_titanes(gs);
    hitbox_entidades(gs, assets);
    colision_titanes(gs);

}

void activa_entidades(s_GameState *gs)
{
    int i, j, pA = gs->pantalla_actual;

    if(gs->input.keyL == true)
    {
        printf("flagDash: %d\n", gs->levi.dash.flagDash);
        printf("Cantidad de dashes: %d\n", gs->levi.dash.cantDash);
        gs->input.keyL = false;
    }


    if(gs->input.keyG == true)    
    {
        for(i=0 ; i<gs->pantalla[pA].num_entidades ; i++) //Desctiva todas las entidades de la pantalla
            if(gs->pantalla[pA].entidades[i].vida > 0)
                gs->pantalla[pA].entidades[i].activo = false;
    }

    else if(gs->input.keyG == false)
    {
        for(i=0 ; i<gs->pantalla[pA].num_entidades ; i++) //Activa todas las entidades de la pantalla
            if(gs->pantalla[pA].entidades[i].vida > 0)
                gs->pantalla[pA].entidades[i].activo = true;
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

            if(gs->pantalla[pA].entidades[i].tipo == 1)
            {
                if(cx < 500 && cx > 0 && distanciaX > 80) //Calcula el rango para empezar a moverse
                {
                    gs->pantalla[pA].entidades[i].x += gs->pantalla[pA].entidades[i].velocidadX;
                    gs->pantalla[pA].entidades[i].animacion.rotarAnim = false;
                    gs->pantalla[pA].entidades[i].viendoDerecha = true;
                }
                else if(cx > -500 && cx < 0 && distanciaX > 80) //Calcula el rango para empezar a moverse
                {
                    gs->pantalla[pA].entidades[i].x -= gs->pantalla[pA].entidades[i].velocidadX;
                    gs->pantalla[pA].entidades[i].animacion.rotarAnim = true;
                    gs->pantalla[pA].entidades[i].viendoDerecha = false;
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

                if(distanciaX <= 80 || gs->pantalla[pA].entidades[i].tiempoAtaqueActivo > 0)
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

            else if(gs->pantalla[pA].entidades[i].tipo == 2)
            {
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

                    gs->pantalla[pA].entidades[i].agarre.manoHB.x = 0;
                    gs->pantalla[pA].entidades[i].agarre.manoHB.y = 0;
                    gs->pantalla[pA].entidades[i].agarre.manoHB.alto = 0;
                    gs->pantalla[pA].entidades[i].agarre.manoHB.ancho = 0;
                }
                else if(distanciaX <= 175 || gs->pantalla[pA].entidades[i].tiempoAtaqueActivo > 0)
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
        agarre_titanes(gs, i);
        return;
    }

    if(tipo == 2)
    {
        if(entidad[i].tiempoAtaqueActivo <= 0)
        {
            if((cy <= -70 && cy >= -315 && fabsf(cx) <= 175) || entidad[i].saltoActivo) //Salto
                entidad[i].casoAtaque = 0; 

            else if(fabsf(cx) <= 40 && cy > -70 && gs->levi.agarrado == false)
                entidad[i].casoAtaque = rand()%10; //Golpe melee

            else if(fabsf(cx) <= 40 && cy > -70 && gs->levi.agarrado == true) //Golpe melee
                entidad[i].casoAtaque = rand()%7;

            //printf("%d\n",entidad[i].casoAtaque);
        }

        if(entidad[i].casoAtaque == 0)
        {
            if(entidad[i].saltoActivo == false)
            {
                entidad[i].velocidadY = -18;
                entidad[i].saltoActivo = true;

                if(cx > 0)
                    entidad[i].velocidadXSalto = 4.0f;

                else 
                    entidad[i].velocidadXSalto = -4.0f;
            }

            if(entidad[i].saltoActivo)
            {
                entidad[i].cooldownAtaque = 1.0f;
                entidad[i].x += entidad[i].velocidadXSalto;

                if(entidad[i].velocidadY <= 0) 
                {
                    entidad[i].hitboxAtaqueBasico.x = entidad[i].hitboxTitan.x;
                    entidad[i].hitboxAtaqueBasico.y = entidad[i].hitboxTitan.y + 20;
                    entidad[i].hitboxAtaqueBasico.alto = 60;
                    entidad[i].hitboxAtaqueBasico.ancho = 60;

                    if(colision(gs, gs->levi.hitbox, entidad[i].hitboxAtaqueBasico))
                        printf("Mordido pa\n");
                }
                else 
                {
                    entidad[i].hitboxAtaqueBasico.x = 0;
                    entidad[i].hitboxAtaqueBasico.y = 0;
                    entidad[i].hitboxAtaqueBasico.alto = 0;
                    entidad[i].hitboxAtaqueBasico.ancho = 0;
                }
            }
        }

        else if(entidad[i].casoAtaque >=1 && entidad[i].casoAtaque < 7) ////////////////////Ataque 1///////////////////////////
        {
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
        
        else if(entidad[i].casoAtaque >= 7) ///////////////////Ataque 2//////////////////////
        {
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
    }

}

void agarre_titanes(s_GameState *gs, int i)
{
    int pA = gs->pantalla_actual;
    s_Entidades *entidad = gs->pantalla[pA].entidades;

    gs->variables.bloquearControles = true;
    gs->levi.agarrado = true;
    gs->variables.gravedad = 0;
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

        if(colision(gs, gs->levi.hitbox, entidad[i].mordidaHB))
        {
            printf("Mordida\n");
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

        cambiar_animacion(gs, SALIDA_TITAN_AGARRE);
    }
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

