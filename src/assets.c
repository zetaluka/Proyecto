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

    assets->assetsPantalla.fondo_bosque = al_load_bitmap("assets/imgs/bosque.jpg");
    if(!assets->assetsPantalla.fondo_bosque){
        printf("Error cargando fondo_bosque");
        return 1;
    }

    assets->assetsPantalla.fondo_menu = al_load_bitmap("assets/imgs/fondomenu2.jpg");
    if(!assets->assetsPantalla.fondo_menu){
        printf("Error cargando fondo_menu");
        return 1;
    }


    return 0;
}
int carga_sprites_titanes(s_Assets *assets)
{
    assets->titanes.titan_hembra = al_load_bitmap("assets/imgs/titanhembra.png");
    if(!assets->titanes.titan_hembra){
        printf("Error cargando titanhembra");
        return 1;
    }

    assets->titanes.titan1 = al_load_bitmap("assets/imgs/dina.png");
    if(!assets->titanes.titan1){
        printf("Error cargando titan1");
        return 1;
    }

    assets->titanes.titan2 = al_load_bitmap("assets/imgs/titangloton.png");
    if(!assets->titanes.titan2){
        printf("Error cargando titan2");
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

    assets->assetsPantalla.casa2 = al_load_bitmap("assets/imgs/casa2.2.png");
    if(!assets->assetsPantalla.casa2){
        printf("Error cargando casa2");
        return 1;
    }

    assets->assetsPantalla.casa3 = al_load_bitmap("assets/imgs/casa3.png");
    if(!assets->assetsPantalla.casa3){
        printf("Error cargando casa3");
        return 1;
    }

    assets->assetsPantalla.puestoComida = al_load_bitmap("assets/imgs/puesto_comida.png");
    if(!assets->assetsPantalla.puestoComida){
        printf("Error cargando puesto_comida");
        return 1;
    }

    assets->assetsPantalla.grietaODM = al_load_bitmap("assets/imgs/grietita.png");
    if(!assets->assetsPantalla.grietaODM){
        printf("Error cargando grietita");
        return 1;
    }

    assets->assetsPantalla.gas = al_load_bitmap("assets/imgs/gas.png");
    if(!assets->assetsPantalla.gas){
        printf("Error cargando gas");
        return 1;
    }

    assets->assetsPantalla.dash = al_load_bitmap("assets/imgs/dash.png");
    if(!assets->assetsPantalla.dash){
        printf("Error cargando dash.png");
        return 1;
    }

    assets->assetsPantalla.marcoVida = al_load_bitmap("assets/imgs/marcovida.png");
    if(!assets->assetsPantalla.marcoVida){
        printf("Error cargando marcovida.png");
        return 1;
    }

    assets->assetsPantalla.galonGas = al_load_bitmap("assets/imgs/galonGas.png");
    if(!assets->assetsPantalla.galonGas){
        printf("Error cargando galonGas");
        return 1;
    }

    assets->assetsPantalla.transicion = al_load_bitmap("assets/imgs/transicion.png");
    if(!assets->assetsPantalla.transicion){
        printf("Error cargando transicion.png");
        return 1;
    }

    assets->assetsPantalla.espada1 = al_load_bitmap("assets/imgs/espada.png");
    if(!assets->assetsPantalla.espada1){
        printf("Error cargando espada.png");
        return 1;
    }

    assets->assetsPantalla.espada2 = al_load_bitmap("assets/imgs/espada2.png");
    if(!assets->assetsPantalla.espada2){
        printf("Error cargando espada2.png");
        return 1;
    }

    for(int i=0; i<14; i++)
        assets->assetsPantalla.dashSB[i] = al_create_sub_bitmap(assets->assetsPantalla.dash, i*280, 0, 280, 160);

    return 0;

}

