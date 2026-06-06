#include "commons.h"

//====Prototipos====//
int carga_fuentes(s_Assets* fuentes);


//====Funcion principal====//
void assets_load(s_Assets *assets)
{
    carga_fuentes(assets);

    return;
}

//====Funciones====//
int carga_fuentes(s_Assets* assets)
{
    assets->shingekiFont = al_load_font("assets/fonts/Ditty.ttf", 30, 0);
    if(!assets->shingekiFont) {
        printf("Error cargando shingekiFont\n");
        return 1;
    }

    assets->minimalistTemplateFont = al_load_font("assets/fonts/MinimalistTemplate.otf", 50, 0);
    if(!assets->minimalistTemplateFont) {
        printf("Error cargando minimalistTemplateFont\n");
        return 1;
    }

    return 0;
}