#include "commons.h"

void pantalla_0_entidades(s_GameState *gs);
void hitbox_entidades(s_GameState *gs, s_Assets *assets);
bool colision_titan(s_GameState *gs, int i);


void genera_entidades(s_GameState *gs, s_Assets *assets)
{
    switch(gs->pantalla_actual)
    {
        case 0:
            pantalla_0_entidades(gs);
            break;
    }

    hitbox_entidades(gs, assets);
}

void pantalla_0_entidades(s_GameState *gs)
{
    int i, j;

    for(i=0 ; i<gs->pantalla[0].num_entidades ; i++) //Activa todas las entidades de la pantalla
        gs->pantalla[0].entidades[i].activo = true;


    if(gs->input.keyG == false)    
    {
        if(gs->pantalla[0].entidades[0].x + 50 < gs->levi.hitbox.x)
            gs->pantalla[0].entidades[0].x += gs->pantalla->entidades->velocidadX;
        else if(gs->pantalla[0].entidades[0].x  + 50> gs->levi.hitbox.x)
            gs->pantalla[0].entidades[0].x -= gs->pantalla->entidades->velocidadX;
    }

}

void hitbox_entidades(s_GameState *gs, s_Assets *assets)
{
    int i, j, pA = gs->pantalla_actual;
    int nE = gs->pantalla[pA].num_entidades;

    for(i=0;i<MAXFIL;i++)
        for(j=0;j<MAXCOL;j++)
        {
            switch(gs->mapas.mapa1[i][j])
            {
                case 'T':
                    if(gs->pantalla[pA].entidades[nE].activo == true)
                        {
                            gs->pantalla[pA].entidades[nE].hitboxTitan = (s_Hitbox){gs->pantalla[pA].entidades[nE].x + 30, gs->pantalla[pA].entidades[nE].y, 
                            al_get_bitmap_width(assets->titanes.titan_bizarro)-55, al_get_bitmap_height(assets->titanes.titan_bizarro)};
                        }
                    break;
            }
        }
    

    //==========//


}

bool colision_titan(s_GameState *gs, int i) // Detecta si la hitbox del personaje choca con alguna otra
{
    int pA = gs->pantalla_actual;

    i=0;

    if(gs->levi.hitbox.x + gs->levi.hitbox.ancho >= gs->pantalla[pA].entidades[i].hitboxTitan.x
    && gs->levi.hitbox.y + gs->levi.hitbox.alto >= gs->pantalla[pA].entidades[i].hitboxTitan.y 
    && gs->levi.hitbox.x <= gs->pantalla[pA].entidades[i].hitboxTitan.x + gs->pantalla[pA].entidades[i].hitboxTitan.ancho
    && gs->levi.hitbox.y <= gs->pantalla[pA].entidades[i].hitboxTitan.y + gs->pantalla[pA].entidades[i].hitboxTitan.alto)
        return true;
    
    return false;
}
