#include "commons.h"

//====Prototipos====//
void update_jugando(s_GameState *gs, s_Assets *assets);
void update_tiempo_jugado(s_GameState* gs);
void update_levi_movimiento(s_GameState *gs);
void transicion_pantalla(s_GameState *gs, s_Assets *assets);
void hitbox_levi(s_GameState *gs, s_Assets *assets);
void levi_dash(s_GameState *gs);
void cuadrado_prueba (s_GameState *gs);
void colision_levi_dash(s_GameState *gs);
void colision_levi_ataque(s_GameState *gs);
void colision_levi_mapa(s_GameState *gs);
void colision_ODM(s_GameState *gs);

//====Funcion principal====//
void update(s_GameState *gs, s_Assets *assets)
{
    switch(gs->estadoPantalla) //Detecta en que estado esta, ejemplo: Menu, jugando, pausa, etc.
    {
        case PANTALLA_MENU:
            break;

        case PANTALLA_JUGANDO:
            update_jugando(gs,assets);
            break;

        case PANTALLA_GAME_OVER:
            break;
    }

}


void update_jugando(s_GameState *gs, s_Assets *assets) //Funcion si para cuando se este en la pantalla de juego
{
    update_tiempo_jugado(gs);
    update_levi_movimiento(gs);
    cuadrado_prueba(gs);
    hitbox_levi(gs,assets);
    levi_dash(gs);
    comprueba_colision(gs);
    transicion_pantalla(gs, assets);

    return;
}

void update_tiempo_jugado(s_GameState* gs) //Funcion para hacer funcionar el cronometro de tiempo jugado
{
    if(gs->tiempoJugado.validacion == 1)
    {
        gs->tiempoJugado.segundos++;
        if(gs->tiempoJugado.segundos>=60)
        {
            gs->tiempoJugado.segundos=0;
            gs->tiempoJugado.minutos++;
        }
        gs->tiempoJugado.validacion=0;
    }
}

void update_levi_movimiento(s_GameState *gs)
{ 
    bool bloqueaDerecha = false, bloqueaIzquierda = false;

    if(gs->levi.dash.activo || gs->levi.ODM.activo)
        return;

    if(gs->levi.dash.tiempoRecuperacionDash > 0)
    {
        gs->levi.dash.tiempoRecuperacionDash -= 1.0f/FPS;    
        gs->levi.velocidadX *= 0.9f; //Va bajando gradualmente la inercia horizontal
        gs->levi.velocidadY *= 0.9f; //Va bajando gradualmente la inercia vertical

        if(gs->input.keyLShift == 1 && gs->input.keyD == 1) //Resta gradualmente la inercia
            gs->levi.velocidadX += 0.4f;
        
        else if(gs->input.keyLShift == 1 && gs->input.keyA == 1) //Resta gradualmente la inercia
            gs->levi.velocidadX -= 0.4f;

        else if(gs->input.keyA == 1) //Resta gradualmente la inercia en menor cantidad
            gs->levi.velocidadX -= 0.2f;

        else if(gs->input.keyD == 1) //Resta gradualmente la inercia en menor cantidad
            gs->levi.velocidadX += 0.2f;

        gs->levi.x += gs->levi.velocidadX; //Hace que levi se mueva con la inercia
        gs->levi.y += gs->levi.velocidadY; //Hace que levi se mueva con la inercia
    }

    else
    {
        if(gs->input.keyLShift == 1 && gs->input.keyD == 1) //Si mantiene el LShift corre
            gs->levi.x += 4.5f;
            
        else if(gs->input.keyLShift == 1 && gs->input.keyA == 1) //Si mantiene el LShift corre
            gs->levi.x -= 4.5f;

        else if(gs->input.keyA == 1) //Camina izquierda
            gs->levi.x -= 3;

        else if(gs->input.keyD == 1) //Camina derecha
            gs->levi.x += 3;
    }

    //====Doble salto====//
    if(gs->input.keySpace == 1 && gs->levi.levi_suelo) //Salto y habilita doble salto
        {
            gs->levi.velocidadY = -8;
            gs->levi.levi_suelo = false;
            gs->levi.doble_salto = true;
            gs->input.keySpace = 0;
            gs->levi.ODM.activo = false;
        }

    if(gs->input.keySpace == 1 && gs->levi.doble_salto) //Confirma se doble salto esta habilitado
    {
        gs->levi.velocidadY = -10;
        gs->levi.doble_salto = false;
        gs->input.keySpace = 0;
        gs->levi.ODM.activo = false;
    }

    //Gravedad
    if(!gs->levi.dash.activo && !gs->levi.ODM.activo)
    {
        gs->levi.velocidadY += 0.5; //Lo hace moverse todo el rato hacia abajo
        gs->levi.y = gs->levi.y + gs->levi.velocidadY;
    }
    
}

void transicion_pantalla(s_GameState *gs, s_Assets *assets) //Efecto de transicion por pantallas
{

    if(gs->levi.x+40 >= SCREEN_X && gs->pantalla_actual < MAXPANTALLAS - 1)
    {
        gs->pantalla_actual++;
        gs->levi.x = -20; 
    }

    if(gs->levi.x+35 <= 0 && gs->pantalla_actual > 0)
    {
        gs->pantalla_actual--;
        gs->levi.x = SCREEN_X - 70;
        hitbox_init(gs);
    }

     if(gs->pantalla_actual > gs->variables.carga_pantalla)
    {
        mapa1(gs, assets);
        gs->variables.carga_pantalla++;
        hitbox_init(gs);
    }

}

void hitbox_levi(s_GameState *gs, s_Assets *assets) //Actualiza la hitbox del personaje principal
{
    gs->levi.hitbox.alto = al_get_bitmap_height(assets->levi.levi_parado)*2;
    gs->levi.hitbox.ancho = LEVI_HB_RECORTE - 10;
    gs->levi.hitbox.x = gs->levi.x + LEVI_HB_RECORTE;
    gs->levi.hitbox.y = gs->levi.y;
}

void cuadrado_prueba (s_GameState *gs)
{
    if(gs->variables.cambioSentido == false)
        gs->pantalla[0].hitbox[3].y -= 2;
    else if(gs->variables.cambioSentido == true)
        gs->pantalla[0].hitbox[3].y += 2;

    if(gs->pantalla[0].hitbox[3].y >= (SCREEN_Y - gs->pantalla[0].hitbox[0].alto - 50))
        gs->variables.cambioSentido = false;
    if(gs->pantalla[0].hitbox[3].y <= 500)
        gs->variables.cambioSentido = true;

}

//===========Funciones de colision==============//

bool colision(s_GameState *gs, s_Hitbox h1, s_Hitbox h2)
{
    int pA = gs->pantalla_actual;

    if( h1.x + h1.ancho >= h2.x && 
        h1.y + h1.alto >= h2.y &&
        h1.x <= h2.x + h2.ancho && 
        h1.y <= h2.y + h2.alto )
        return true;
    
    return false;

}

void comprueba_colision(s_GameState *gs) //Comprueba si la hitbox del personaje choca con alguna otra y actua conforme al caso
{   

    colision_levi_mapa(gs);
    colision_levi_titan(gs); //Funcion en entities.c
    colision_levi_ataque(gs);
    colision_ODM(gs);
    if(gs->levi.dash.activo == true)
        colision_levi_dash(gs);
    //Hacer funcion de detectar colision entre titan y estructuras para ponerles gravedad

}

void colision_levi_ataque(s_GameState *gs)
{
    int i, pA = gs->pantalla_actual;

    if(gs->levi.cooldownAtaque > 0)
        gs->levi.cooldownAtaque -= 1.0f/FPS;

    if(gs->input.mouseX >= (gs->levi.hitbox.x + gs->levi.hitbox.ancho/2) * gs->escala)
        gs->levi.viendoDerecha = 1;
    else
        gs->levi.viendoDerecha = 0;

    if(gs->levi.viendoDerecha == 1)
    {
        gs->levi.hitboxAtaque.x = gs->levi.hitbox.x + gs->levi.hitbox.ancho;
        gs->levi.hitboxAtaque.y = gs->levi.hitbox.y + gs->levi.hitbox.alto/2;
        gs->levi.hitboxAtaque.alto = 5;
        gs->levi.hitboxAtaque.ancho = 40;
    }
    else if(gs->levi.viendoDerecha == 0)
    {
        gs->levi.hitboxAtaque.x = gs->levi.x; 
        gs->levi.hitboxAtaque.y = gs->levi.y + gs->levi.hitbox.alto/2;
        gs->levi.hitboxAtaque.alto = 5;
        gs->levi.hitboxAtaque.ancho = 40;
    }

    if(gs->input.ClickIzq && gs->levi.cooldownAtaque <= 0)
    {
        gs->levi.cooldownAtaque = 0.75f;
        printf("Ataque\n");
        for(i=0;i<gs->pantalla[pA].num_entidades;i++)
        {
            if(gs->pantalla[pA].entidades[i].activo == false)
                continue;
            if(colision(gs, gs->levi.hitboxAtaque, gs->pantalla[pA].entidades[i].hitboxNuca)) //Comprueba si pega en la nuca, si es asi rompe el bucle
            {
                printf("Colision en la nuca\n");
                printf("%d\n", gs->pantalla[pA].entidades[i].vida);
                gs->pantalla[pA].entidades[i].vida = 0;
                gs->pantalla[pA].entidades[i].activo = false;
                printf("%d\n", gs->pantalla[pA].entidades[i].vida);
                continue;
            }

            if(colision(gs, gs->levi.hitboxAtaque, gs->pantalla[pA].entidades[i].hitboxTitan)) //Comprueba si pega en cualquier parte de la hitbox del titan
            {
                printf("Colisiono\n");
                printf("Titan %d vida antes: %d\n", i, gs->pantalla[pA].entidades[i].vida);
                gs->pantalla[pA].entidades[i].vida -= 100;
                printf("Titan %d vida despues:%d\n", i, gs->pantalla[pA].entidades[i].vida);
            }

            if(gs->pantalla[pA].entidades[i].vida <= 0) //Desactiva al titan en caso de que no tenga vida
                gs->pantalla[pA].entidades[i].activo = false;

        }

    }

}

void colision_levi_mapa(s_GameState *gs)
{
    int i, pA = gs->pantalla_actual;
    float distancia_izquierda = 0, distancia_derecha = 0, distancia_arriba = 0, distancia_abajo = 0;

    if(gs->levi.dash.frameActivacion) //Sirve para ignorar las colisiones del mapa por 1 frame
    {
        gs->levi.dash.frameActivacion = false;
        return;
    }

    for(i=0;i<gs->pantalla[pA].num_hitbox;i++) //Bucle para comparar hitbox y encontrar la coincidente
        {
            if(colision(gs, gs->levi.hitbox, gs->pantalla[pA].hitbox[i]))
            {
                //Define las distancias con las paredes
                distancia_izquierda = (gs->levi.hitbox.x + gs->levi.hitbox.ancho) - gs->pantalla[pA].hitbox[i].x;
                distancia_derecha = (gs->pantalla[pA].hitbox[i].x + gs->pantalla[pA].hitbox[i].ancho) - gs->levi.hitbox.x;
                distancia_arriba = (gs->levi.hitbox.y + gs->levi.hitbox.alto) - gs->pantalla[pA].hitbox[i].y;
                distancia_abajo = (gs->pantalla[pA].hitbox[i].y + gs->pantalla[pA].hitbox[i].alto) - gs->levi.hitbox.y;

                //Comprueba la menor distancia entre las paredes para detectar por donde choco
                if(distancia_izquierda < distancia_derecha && distancia_izquierda < distancia_arriba && distancia_izquierda < distancia_abajo)
                    gs->levi.x = gs->pantalla[pA].hitbox[i].x - gs->levi.hitbox.ancho - LEVI_HB_RECORTE;

                else if(distancia_derecha < distancia_izquierda && distancia_derecha < distancia_arriba && distancia_derecha < distancia_abajo)
                    gs->levi.x = gs->pantalla[pA].hitbox[i].x + gs->pantalla[pA].hitbox[i].ancho - LEVI_HB_RECORTE;

                else if(distancia_abajo < distancia_arriba && distancia_abajo < distancia_izquierda && distancia_abajo < distancia_derecha)
                    gs->levi.y = gs->pantalla[pA].hitbox[i].y + gs->pantalla[pA].hitbox[i].alto;

                else if(distancia_arriba < distancia_abajo && distancia_arriba < distancia_izquierda && distancia_arriba < distancia_derecha)
                {
                    gs->levi.y = gs->pantalla[pA].hitbox[i].y - gs->levi.hitbox.alto;
                    gs->levi.velocidadY = 0;
                    gs->levi.levi_suelo = true; //Habilita levi_suelo, lo que hace que lo habilite a dar un salto
                    gs->levi.doble_salto = true;
                }

            }
        }
}

void colision_ODM(s_GameState *gs)
{
    int pA = gs->pantalla_actual, i;
    float cx, cy;

    gs->levi.hitboxODM.x = gs->input.mouseX / gs->escala;
    gs->levi.hitboxODM.y = gs->input.mouseY / gs->escala;
    gs->levi.hitboxODM.ancho = 5;
    gs->levi.hitboxODM.alto = 5;

    if(gs->input.ClickDer)
    {
        for(i=0;i<gs->pantalla[pA].num_entidades;i++)
            if(colision(gs, gs->levi.hitboxODM, gs->pantalla[pA].entidades[i].hitboxTitan) && gs->pantalla[pA].entidades[i].activo == true)
            {
                /*gs->levi.x = (gs->input.mouseX / gs->escala) - LEVI_HB_RECORTE - gs->levi.hitbox.ancho/2;
                gs->levi.y = gs->input.mouseY / gs->escala;*/

                cx = (gs->input.mouseX/gs->escala) - (gs->levi.x + gs->levi.hitbox.ancho); //Calcula cateto x 
                cy = (gs->input.mouseY/gs->escala) - (gs->levi.y + gs->levi.hitbox.alto); //Calcula cateto y
                gs->levi.ODM.distanciaRestante = sqrt(cx*cx + cy*cy); //Calcula la hipotenusa (distancia de levi al punto)

                gs->levi.ODM.activo = true;
                gs->levi.ODM.dirX = cx/gs->levi.ODM.distanciaRestante;
                gs->levi.ODM.dirY = cy/gs->levi.ODM.distanciaRestante;
                gs->levi.ODM.velocidadODM = 1;
                i = gs->pantalla[pA].num_entidades;
            }
        
        gs->input.ClickDer = false;
    }

    if(gs->levi.ODM.activo)
        {
            gs->levi.x += gs->levi.ODM.dirX * gs->levi.ODM.velocidadODM;
            gs->levi.y += gs->levi.ODM.dirY * gs->levi.ODM.velocidadODM;
            gs->levi.ODM.distanciaRestante -= gs->levi.ODM.velocidadODM;
            gs->levi.ODM.velocidadODM += 0.5f;

            if(gs->levi.ODM.distanciaRestante <= 0)
            {
                gs->levi.ODM.activo = false;
                gs->levi.velocidadX = gs->levi.ODM.dirX * gs->levi.ODM.velocidadODM;
                gs->levi.velocidadY = gs->levi.ODM.dirY * gs->levi.ODM.velocidadODM;
                gs->levi.dash.tiempoRecuperacionDash = 2.5f;
            }
        }
}

void levi_dash(s_GameState *gs)
{
    float cx, cy, distancia; //Catetos e hipotenusa

    if(gs->input.keyF == true && gs->levi.dash.activo == false && gs->levi.dash.cooldown <= 0)
    {
        if(colision(gs, gs->levi.hitboxODM, gs->pantalla[gs->pantalla_actual].hitbox[0]) && gs->levi.levi_suelo) 
            return; //Evita bug de activar dash con el mouse en el suelo

        cx = (gs->input.mouseX/gs->escala) - (gs->levi.x + gs->levi.hitbox.ancho); //Calcula cateto x 
        cy = (gs->input.mouseY/gs->escala) - (gs->levi.y + gs->levi.hitbox.alto); //Calcula cateto y
        distancia = sqrt(cx*cx + cy*cy); //Calcula la hipotenusa (distancia de levi al punto)

        gs->levi.dash.activo = true;
        gs->levi.dash.distanciaRestante = 125; //Distancia fija a recorrer
        gs->levi.dash.cooldown = 1; 
        gs->levi.dash.dashX = cx/distancia; //Direccion x del dash (coseno)
        gs->levi.dash.dashY = cy/distancia; //Direccion y del dash (seno)
        if(gs->levi.dash.dashY < 0) //Si el dash es hacia arriba activa esta variable
            gs->levi.dash.frameActivacion = true; //Sirve para ignorar por 1 frame las colisiones del mapa, asi permite despegar el dash si esta en suelo
    }

    if(gs->levi.dash.activo)
    {
        gs->input.keyF = false;
        gs->levi.x += gs->levi.dash.dashX*20; //Mueve a levi segun la direccion x y lo multiplica por la velocidad 8
        gs->levi.y += gs->levi.dash.dashY*20; //Mueve a levi segun la direccion x y lo multiplica por la velocidad 8
        gs->levi.dash.distanciaRestante -= 20; //Resta la distancia restante 

        if(gs->levi.dash.distanciaRestante <= 0) //Calcula el fin del dash
        {
            gs->levi.dash.activo = false;
            gs->levi.dash.tiempoRecuperacionDash = 0.3f;
            gs->levi.velocidadX = gs->levi.dash.dashX * 12; //Inercia horizontal
            gs->levi.velocidadY = gs->levi.dash.dashY * 8; //Inercia vertical
        }
    }

    if(gs->levi.dash.cooldown > 0) 
        gs->levi.dash.cooldown -= 1.0/FPS;
    
}

void colision_levi_dash(s_GameState *gs)
{
    int pA = gs->pantalla_actual, i;

    gs->levi.dash.hitboxDash.x = gs->levi.x;
    gs->levi.dash.hitboxDash.y = gs->levi.y;
    gs->levi.dash.hitboxDash.ancho = 96;
    gs->levi.dash.hitboxDash.alto = 60;

    for(i=0;i<gs->pantalla[pA].num_entidades;i++)
        {
            if(gs->pantalla[pA].entidades[i].activo == false)
                continue;
            if(colision(gs, gs->levi.dash.hitboxDash, gs->pantalla[pA].entidades[i].hitboxNuca)) //Comprueba si pega en la nuca, si es asi rompe el bucle
            {
                printf("Colision en la nuca\n");
                printf("%d\n", gs->pantalla[pA].entidades[i].vida);
                gs->pantalla[pA].entidades[i].vida = 0;
                gs->pantalla[pA].entidades[i].activo = false;
                printf("%d\n", gs->pantalla[pA].entidades[i].vida);
                continue;
            }

            if(colision(gs, gs->levi.hitboxAtaque, gs->pantalla[pA].entidades[i].hitboxTitan)) //Comprueba si pega en cualquier parte de la hitbox del titan
            {
                printf("Colisiono\n");
                printf("Titan %d vida antes: %d\n", i, gs->pantalla[pA].entidades[i].vida);
                gs->pantalla[pA].entidades[i].vida -= 75;
                printf("Titan %d vida despues:%d\n", i, gs->pantalla[pA].entidades[i].vida);
            }

            if(gs->pantalla[pA].entidades[i].vida <= 0) //Desactiva al titan en caso de que no tenga vida
                gs->pantalla[pA].entidades[i].activo = false;

        }


}

//================================================//