#include "commons.h"

//====Prototipos====//
int carga_fuentes(s_Assets *assets);
int carga_sprites_levi(s_Assets *assets);
int carga_fondo(s_Assets *assets);
int carga_sprites_titanes(s_Assets *assets);

//====Funcion principal====//
void assets_load(s_Assets *assets)
{
    carga_fuentes(assets);
    carga_sprites_levi(assets);
    carga_fondo(assets);
    carga_sprites_titanes(assets);

    return;
}

//====Funciones====//
int carga_fuentes(s_Assets *assets)
{
    assets->shingekiFont = al_load_font("assets/fonts/Ditty.ttf", 30, 0);
    if(!assets->shingekiFont) {
        printf("Error cargando shingekiFont\n");
        exit(1);
    }

    assets->minimalistTemplateFont = al_load_font("assets/fonts/MinimalistTemplate.otf", 50, 0);
    if(!assets->minimalistTemplateFont) {
        printf("Error cargando minimalistTemplateFont\n");
        exit(1);
    }

    return 0;
}

int carga_sprites_levi(s_Assets *assets)
{
    assets->levi.levi_parado = al_load_bitmap("assets/imgs/levi_parado.png");
    if(!assets->levi.levi_parado){
        printf("Error cargando levi_parado");
        exit(1);
    }

    return 0;
}

int carga_fondo(s_Assets *assets)
{
    //Pantalla 0
    assets->assetsPantalla[0].fondo_base = al_load_bitmap("assets/imgs/fondo_base.png");
    if(!assets->assetsPantalla[0].fondo_base){
        printf("Error cargando fondo_base");
        return 1;
    }

    assets->assetsPantalla[0].cubo = al_load_bitmap("assets/imgs/cubo.png");
    if(!assets->assetsPantalla[0].cubo){
        printf("Error cargando cubo");
        return 1;
    }

    //Pantalla 1
    assets->assetsPantalla[1].fondo_base = al_load_bitmap("assets/imgs/fondo_base_titan_colosal.png");
    if(!assets->assetsPantalla[1].fondo_base){
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

