#include "commons.h"

//====Prototipos====//
int carga_fuentes(s_Assets *assets);
int carga_sprites_levi(s_Assets *assets);
int carga_fondo(s_Assets *assets);
int carga_sprites_titanes(s_Assets *assets);
int carga_objetos(s_Assets *assets);

//====Funcion principal====//
void assets_load(s_Assets *assets)
{
    //al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
    al_set_new_bitmap_flags(0);

    carga_sprites_levi(assets);
    carga_fondo(assets);
    carga_sprites_titanes(assets);
    carga_objetos(assets);
    carga_fuentes(assets);

    return;
}

//====Funciones====//
int carga_fuentes(s_Assets *assets)
{
    assets->shingekiFont30 = al_load_font("assets/fonts/Ditty.ttf", 30, 0);
    if(!assets->shingekiFont30){
        printf("Error cargando shingekiFont\n");
        exit(1);
    }

    assets->minimalistTemplateFont50 = al_load_font("assets/fonts/MinimalistTemplate.otf", 50, 0);
    if(!assets->minimalistTemplateFont50){
        printf("Error cargando minimalistTemplateFont50\n");
        exit(1);
    }

    assets->minimalistTemplateFont25 = al_load_font("assets/fonts/MinimalistTemplate.otf", 25, 0);
    if(!assets->minimalistTemplateFont25){
        printf("Error cargando minimalistTemplateFont25\n");
        exit(1);
    }

    return 0;
}

int carga_sprites_levi(s_Assets *assets)
{
    assets->levi.levi_SS = al_load_bitmap("assets/imgs/levi_spritesheet.png");
    if(!assets->levi.levi_SS){
        printf("Error cargando levi_spritesheet");
        exit(1);
    }

    /*assets->levi.levi = al_load_bitmap("assets/imgs/levi.png");
    if(!assets->levi.levi){
        printf("Error cargando levi.png");
        exit(1);
    }*/

    return 0;
}

int carga_fondo(s_Assets *assets)
{
    //Pantalla 0
    assets->assetsPantalla.fondo_base = al_load_bitmap("assets/imgs/fondo_base.png");
    if(!assets->assetsPantalla.fondo_base){
        printf("Error cargando fondo_base");
        return 1;
    }

    assets->assetsPantalla.cubo = al_load_bitmap("assets/imgs/cubo.png");
    if(!assets->assetsPantalla.cubo){
        printf("Error cargando cubo");
        return 1;
    }

    //Pantalla 1
    assets->assetsPantalla.fondo_titan_colosal = al_load_bitmap("assets/imgs/fondo_base_titan_colosal.png");
    if(!assets->assetsPantalla.fondo_titan_colosal){
        printf("Error cargando fondo_base_titan_colosal");
        return 1;
    }


    return 0;
}
int carga_sprites_titanes(s_Assets *assets)
{
    assets->titanes.titan_bizarro = al_load_bitmap("assets/imgs/titanbizarro.png");
    if(!assets->titanes.titan_bizarro){
        printf("Error cargando titanbizarro");
        return 1;
    }

    return 0;
}

int carga_objetos(s_Assets *assets)
{
    assets->assetsPantalla.grieta = al_load_bitmap("assets/imgs/grieta.png");
    if(!assets->assetsPantalla.grieta){
        printf("Error cargando grieta");
        return 1;
    }

    assets->assetsPantalla.escudoLegion = al_load_bitmap("assets/imgs/escudo_legion.png");
    if(!assets->assetsPantalla.escudoLegion){
        printf("Error cargando escudo_legion");
        return 1;
    }

    assets->assetsPantalla.casa1 = al_load_bitmap("assets/imgs/casa1.png");
    if(!assets->assetsPantalla.casa1){
        printf("Error cargando casa1");
        return 1;
    }

    assets->assetsPantalla.grietaODM = al_load_bitmap("assets/imgs/grietita.png");
    if(!assets->assetsPantalla.grietaODM){
        printf("Error cargando grietita");
        return 1;
    }

    return 0;

}

