#include "commons.h"

void pantalla_0_entidades(s_GameState *gs);
void hitbox_entidades(s_GameState *gs, s_Assets *assets);
void comprueba_colision_titan(s_GameState *gs);
bool colision_titan(s_GameState *gs, int i);


void genera_entidades(s_GameState *gs, s_Assets *assets)
{

    pantalla_0_entidades(gs);
    hitbox_entidades(gs, assets);
    comprueba_colision_titan(gs);

}

void pantalla_0_entidades(s_GameState *gs)
{
    int i, j, pA = gs->pantalla_actual;

    if(gs->input.keyL == true)
    {
        printf("\nNumero de entidades de la pantalla: %d, Activo: \n",gs->pantalla[pA].num_entidades, gs->pantalla[pA].entidades[1].activo);
        gs->input.keyL = false;
    }

    for(i=0 ; i<gs->pantalla[pA].num_entidades ; i++) //Activa todas las entidades de la pantalla
        gs->pantalla[pA].entidades[i].activo = true;


    if(gs->input.keyG == false)    
        for(i=0;i<gs->pantalla[pA].num_entidades;i++)
        {
            if(gs->pantalla[pA].entidades[i].x + 50 < gs->levi.hitbox.x)
                gs->pantalla[pA].entidades[i].x += gs->pantalla[pA].entidades[i].velocidadX;
            else if(gs->pantalla[pA].entidades[i].x  + 50> gs->levi.hitbox.x)
                gs->pantalla[pA].entidades[i].x -= gs->pantalla[pA].entidades[i].velocidadX;
        }

}

void hitbox_entidades(s_GameState *gs, s_Assets *assets)
{
    int i, j, pA = gs->pantalla_actual;
    int nE = gs->pantalla[pA].num_entidades;

    for(i=0;i<nE;i++) //Arreglar esto
        if(gs->pantalla[pA].entidades[i].activo == true)
            {
                gs->pantalla[pA].entidades[i].hitboxTitan = (s_Hitbox){gs->pantalla[pA].entidades[i].x + 30, gs->pantalla[pA].entidades[i].y, 
                al_get_bitmap_width(assets->titanes.titan_bizarro)-55, al_get_bitmap_height(assets->titanes.titan_bizarro)};
            }
    

    //==========//


}

bool colision_titan(s_GameState *gs, int i) // Detecta si la hitbox del personaje choca con alguna otra
{
    int pA = gs->pantalla_actual;

    if(gs->levi.hitbox.x + gs->levi.hitbox.ancho >= gs->pantalla[pA].entidades[i].hitboxTitan.x
    && gs->levi.hitbox.y + gs->levi.hitbox.alto >= gs->pantalla[pA].entidades[i].hitboxTitan.y 
    && gs->levi.hitbox.x <= gs->pantalla[pA].entidades[i].hitboxTitan.x + gs->pantalla[pA].entidades[i].hitboxTitan.ancho
    && gs->levi.hitbox.y <= gs->pantalla[pA].entidades[i].hitboxTitan.y + gs->pantalla[pA].entidades[i].hitboxTitan.alto)
        return true;
    
    return false;
}

void comprueba_colision_titan(s_GameState *gs)
{
    int i, pA = gs->pantalla_actual;

    for(i=0;i<gs->pantalla[pA].num_entidades;i++) //Bucle para comparar hitbox y encontrar la coincidente
    {
        if(colision_titan(gs, i))
        {
           continue;    

        }
    }

}
