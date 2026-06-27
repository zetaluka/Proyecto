#include "commons.h"

void pantalla_0_entidades(s_GameState *gs);
void hitbox_entidades(s_GameState *gs, s_Assets *assets);


void genera_entidades(s_GameState *gs, s_Assets *assets)
{

    pantalla_0_entidades(gs);
    hitbox_entidades(gs, assets);

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
        if(gs->pantalla[pA].entidades[i].vida > 0)
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
                //Hitbox normal
                gs->pantalla[pA].entidades[i].hitboxTitan = (s_Hitbox){gs->pantalla[pA].entidades[i].x + 30, gs->pantalla[pA].entidades[i].y, 
                al_get_bitmap_width(assets->titanes.titan_bizarro)-55, al_get_bitmap_height(assets->titanes.titan_bizarro)};

                //Hitbox nuca
                gs->pantalla[pA].entidades[i].hitboxNuca = (s_Hitbox){gs->pantalla[pA].entidades[i].hitboxTitan.x, gs->pantalla[pA].entidades[i].hitboxTitan.y + 130,
                gs->pantalla[pA].entidades[i].hitboxTitan.ancho, 30};
                
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

