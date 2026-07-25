#include "commons.h"

void activa_entidades(s_GameState *gs);
void hitbox_entidades(s_GameState *gs, s_Assets *assets);
void gravedad_titanes(s_GameState *gs);
void colision_titanes(s_GameState *gs);
void movimiento_titanes(s_GameState *gs);
void ataque_titanes(s_GameState *gs);


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

    float leviMitad = gs->levi.hitbox.x + (gs->levi.hitbox.ancho/2);
    float titanMitadX, cx, cy, distanciaX;

    for(i=0;i<gs->pantalla[pA].num_entidades;i++)
    {
        if(gs->pantalla[pA].entidades[i].activo)
        {

            titanMitadX = gs->pantalla[pA].entidades[i].hitboxTitan.x + (gs->pantalla[pA].entidades[i].hitboxTitan.ancho/2);
            cx = leviMitad - titanMitadX;
            distanciaX= fabsf(leviMitad - titanMitadX); //Valor absoluto para calcular las distancias

            if(gs->pantalla[pA].entidades[i].tipo == 1)
            {
                if(cx < 500 && cx > 0 && distanciaX > 100) //Calcula el rango para empezar a moverse
                {
                    gs->pantalla[pA].entidades[i].x += gs->pantalla[pA].entidades[i].velocidadX;
                    gs->pantalla[pA].entidades[i].animacion.rotarAnim = false;
                    gs->pantalla[pA].entidades[i].viendoDerecha = true;
                }
                else if(cx > -500 && cx < 0 && distanciaX > 100) //Calcula el rango para empezar a moverse
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
            }

            else if(gs->pantalla[pA].entidades[i].tipo == 2)
            {
                if(cx < 500 && cx > 0 && distanciaX > 50)
                {
                    gs->pantalla[pA].entidades[i].quieto = false;
                    gs->pantalla[pA].entidades[i].x += gs->pantalla[pA].entidades[i].velocidadX;
                    gs->pantalla[pA].entidades[i].animacion.rotarAnim = false;
                }
                else if(cx > -500 && cx < 0 && distanciaX > 50)
                {
                    gs->pantalla[pA].entidades[i].quieto = false;
                    gs->pantalla[pA].entidades[i].x -= gs->pantalla[pA].entidades[i].velocidadX;
                    gs->pantalla[pA].entidades[i].animacion.rotarAnim = true;
                }
                else if(distanciaX > 500) //Cuando tenga anim hacer que se siente
                {
                    gs->pantalla[pA].entidades[i].quieto = true;
                }
            }

            if(cx < 30)
                ataque_titanes(gs);
        }
    }
}

void ataque_titanes(s_GameState *gs)
{
    

}

void gravedad_titanes(s_GameState *gs)
{
    int i, j, pA = gs->pantalla_actual;
    int nE = gs->pantalla[pA].num_entidades;

    for(i=0;i<nE;i++) 
        if(gs->pantalla[pA].entidades[i].activo == true)
            {
                gs->pantalla[pA].entidades[i].velocidadY += gs->variables.gravedad;
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

