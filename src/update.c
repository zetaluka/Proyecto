#include "commons.h"

//====Prototipos====//
void update_jugando(s_GameState *gs, s_Assets *assets);
void update_tiempo_jugado(s_GameState* gs);
void update_levi_movimiento(s_GameState *gs);
void transicion_pantalla(s_GameState *gs, s_Assets *assets);
void hitbox_levi(s_GameState *gs, s_Assets *assets);
void levi_dash(s_GameState *gs);
void camara_scroll(s_GameState *gs);
void cuadrado_prueba (s_GameState *gs);
void colision_levi_dash(s_GameState *gs);
void colision_levi_ataque(s_GameState *gs);
void colision_levi_mapa(s_GameState *gs);
void colision_ODM(s_GameState *gs);
void colision_levi_elementos(s_GameState *gs);
void hitbox_mouse(s_GameState *gs);

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
    hitbox_mouse(gs);
    levi_dash(gs);
    comprueba_colision(gs);
    camara_scroll(gs);
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
    //printf("%f",gs->levi.velocidadY);

    if(gs->levi.ODM.activo == true)
        gs->levi.levi_suelo == false;

    if(gs->levi.dash.activo || gs->levi.ODM.activo)
        return;

    if(gs->levi.dash.tiempoRecuperacionDash >= 0)
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
            gs->levi.velocidadY = -10;
            gs->levi.levi_suelo = false;
            gs->levi.doble_salto = true;
            gs->input.keySpace = 0;
            gs->levi.dash.tiempoRecuperacionDash = 0;
            gs->levi.ODM.activo = false;
        }

    if(gs->input.keySpace == 1 && gs->levi.doble_salto) //Confirma si doble salto esta habilitado
    {
        gs->levi.velocidadY = -15;
        gs->levi.doble_salto = false;
        gs->input.keySpace = 0;
        gs->levi.dash.tiempoRecuperacionDash = 0;
        gs->levi.ODM.activo = false;
    }

    //Gravedad
    if(!gs->levi.dash.activo && !gs->levi.ODM.activo)
    {
        gs->levi.velocidadY += 0.8f; //Lo hace moverse todo el rato hacia abajo
        gs->levi.y = gs->levi.y + gs->levi.velocidadY;
    }
    
}

void transicion_pantalla(s_GameState *gs, s_Assets *assets) //Efecto de transicion por pantallas
{
    float anchoPantalla = gs->pantalla[gs->pantalla_actual].ancho * TAM_CELDA;

    if(gs->levi.x+40 >= anchoPantalla && gs->pantalla_actual < MAXPANTALLAS - 1)
    {
        gs->pantalla_actual++;
        gs->levi.x = -20; 
        gs->camara.x = 0;
    }

    if(gs->levi.x+35 <= 0 && gs->pantalla_actual > 0)
    {
        gs->pantalla_actual--;
        anchoPantalla = gs->pantalla[gs->pantalla_actual].ancho * TAM_CELDA;
        gs->levi.x = anchoPantalla - 70;

        if(gs->pantalla[gs->pantalla_actual].pantallaCargada == false)
        {
            hitbox_init(gs);
            gs->pantalla[gs->pantalla_actual].pantallaCargada = true;
        }
    }

     if(gs->pantalla_actual > gs->variables.carga_pantalla)
    {
        mapa1(gs, assets);
        gs->variables.carga_pantalla++;
        
        if(gs->pantalla[gs->pantalla_actual].pantallaCargada == false)
        {
            hitbox_init(gs);
            gs->pantalla[gs->pantalla_actual].pantallaCargada = true;
        }
    }

}

void hitbox_levi(s_GameState *gs, s_Assets *assets) //Actualiza la hitbox del personaje principal
{
    gs->levi.hitbox.alto = al_get_bitmap_height(assets->levi.levi);
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

void hitbox_mouse(s_GameState *gs)
{
    gs->levi.hitboxODM.x = (gs->input.mouseX/gs->escala) + gs->camara.x;
    gs->levi.hitboxODM.y = (gs->input.mouseY/gs->escala) + gs->camara.y;
    gs->levi.hitboxODM.ancho = 5;
    gs->levi.hitboxODM.alto = 5;

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
    colision_levi_elementos(gs);
    //Hacer funcion de detectar colision entre titan y estructuras(del mapa) para ponerles gravedad

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

            /*else if(gs->pantalla[pA].hitbox[i].tipo == 1)
            {
                //Define las distancias con las paredes
                distancia_izquierda = (gs->levi.hitbox.x + gs->levi.hitbox.ancho) - gs->pantalla[pA].hitbox[i].x;
                distancia_derecha = (gs->pantalla[pA].hitbox[i].x + gs->pantalla[pA].hitbox[i].ancho) - gs->levi.hitbox.x;
                distancia_arriba = (gs->levi.hitbox.y + gs->levi.hitbox.alto) - gs->pantalla[pA].hitbox[i].y;
                distancia_abajo = (gs->pantalla[pA].hitbox[i].y + gs->pantalla[pA].hitbox[i].alto) - gs->levi.hitbox.y;

                //Comprueba la menor distancia entre las paredes para detectar por donde choco
                /*if(distancia_izquierda < distancia_derecha && distancia_izquierda < distancia_arriba && distancia_izquierda < distancia_abajo)
                    gs->levi.x = gs->pantalla[pA].hitbox[i].x - gs->levi.hitbox.ancho - LEVI_HB_RECORTE;

                else if(distancia_derecha < distancia_izquierda && distancia_derecha < distancia_arriba && distancia_derecha < distancia_abajo)
                    gs->levi.x = gs->pantalla[pA].hitbox[i].x + gs->pantalla[pA].hitbox[i].ancho - LEVI_HB_RECORTE;

                else if(distancia_abajo < distancia_arriba && distancia_abajo < distancia_izquierda && distancia_abajo < distancia_derecha)
                    gs->levi.y = gs->pantalla[pA].hitbox[i].y + gs->pantalla[pA].hitbox[i].alto;

                if(distancia_arriba < distancia_abajo && distancia_arriba < distancia_izquierda && distancia_arriba < distancia_derecha)
                {
                    gs->levi.y = gs->pantalla[pA].hitbox[i].y - gs->levi.hitbox.alto;
                    gs->levi.velocidadY = 0;
                    gs->levi.levi_suelo = true; //Habilita levi_suelo, lo que hace que lo habilite a dar un salto
                    gs->levi.doble_salto = true;
                }*/
            }

}

void colision_ODM(s_GameState *gs)
{
    int pA = gs->pantalla_actual, i;
    float cx, cy;
    float mouseX = gs->levi.hitboxODM.x, mouseY = gs->levi.hitboxODM.y;

    if(gs->input.ClickDer)
    {
        for(i=0;i<gs->pantalla[pA].num_entidades;i++)
            if(colision(gs, gs->levi.hitboxODM, gs->pantalla[pA].entidades[i].hitboxTitan) && gs->pantalla[pA].entidades[i].activo == true)
            {
                /*gs->levi.x = (gs->input.mouseX / gs->escala) - LEVI_HB_RECORTE - gs->levi.hitbox.ancho/2;
                gs->levi.y = gs->input.mouseY / gs->escala;*/

                cx = mouseX - (gs->levi.x + gs->levi.hitbox.ancho); //Calcula cateto x 
                cy = mouseY - (gs->levi.y + gs->levi.hitbox.alto); //Calcula cateto y
                gs->levi.ODM.distanciaRestante = sqrt(cx*cx + cy*cy); //Calcula la hipotenusa (distancia de levi al punto)

                gs->levi.ODM.activo = true;
                gs->levi.ODM.dirX = cx/gs->levi.ODM.distanciaRestante;
                gs->levi.ODM.dirY = cy/gs->levi.ODM.distanciaRestante;
                gs->levi.ODM.velocidadODM = 1;
            }

        for(i = 0; i < gs->pantalla[pA].num_elementos; i++)
            if(gs->pantalla[pA].elementos[i].tipo == 1 && colision(gs, gs->levi.hitboxODM, gs->pantalla[pA].elementos[i].hitbox))
            {
                /*gs->levi.x = (gs->input.mouseX / gs->escala) - LEVI_HB_RECORTE - gs->levi.hitbox.ancho/2;
                gs->levi.y = gs->input.mouseY / gs->escala;*/

                cx = mouseX - (gs->levi.x + gs->levi.hitbox.ancho); //Calcula cateto x 
                cy = mouseY - (gs->levi.y + gs->levi.hitbox.alto); //Calcula cateto y
                gs->levi.ODM.distanciaRestante = sqrt(cx*cx + cy*cy); //Calcula la hipotenusa (distancia de levi al punto)

                gs->levi.ODM.activo = true;
                gs->levi.ODM.dirX = cx/gs->levi.ODM.distanciaRestante;
                gs->levi.ODM.dirY = cy/gs->levi.ODM.distanciaRestante;
                gs->levi.ODM.velocidadODM = 1;
            }
        
        gs->input.ClickDer = false;
    }

    if(gs->levi.ODM.activo)
        {
            printf("ODM Activo\n");
            gs->levi.x += gs->levi.ODM.dirX * gs->levi.ODM.velocidadODM;
            gs->levi.y += gs->levi.ODM.dirY * gs->levi.ODM.velocidadODM;
            gs->levi.ODM.distanciaRestante -= gs->levi.ODM.velocidadODM;
            gs->levi.ODM.velocidadODM += 0.5f;

            if(gs->levi.ODM.distanciaRestante <= 0)
            {
                gs->levi.ODM.activo = false;
                gs->levi.velocidadX = gs->levi.ODM.dirX * gs->levi.ODM.velocidadODM;
                gs->levi.velocidadY = gs->levi.ODM.dirY * gs->levi.ODM.velocidadODM;
                gs->levi.dash.tiempoRecuperacionDash = 1;
            }
        }
}

void levi_dash(s_GameState *gs)
{
    float cx, cy, distancia; //Catetos e hipotenusa
    float mouseX = gs->levi.hitboxODM.x, mouseY = gs->levi.hitboxODM.y;

    if(gs->input.keyF == true && gs->levi.dash.activo == false && gs->levi.dash.cooldown <= 0)
    {
        if(colision(gs, gs->levi.hitboxODM, gs->pantalla[gs->pantalla_actual].hitbox[0]) && gs->levi.levi_suelo) 
            return; //Evita bug de activar dash con el mouse en el suelo

        cx = mouseX - (gs->levi.x + gs->levi.hitbox.ancho); //Calcula cateto x 
        cy = mouseY - (gs->levi.y + gs->levi.hitbox.alto); //Calcula cateto y
        distancia = sqrt(cx*cx + cy*cy); //Calcula la hipotenusa (distancia de levi al punto)

        gs->levi.dash.activo = true;
        gs->levi.dash.distanciaRestante = 125; //Distancia fija a recorrer
        gs->levi.dash.cooldown = 1; 
        gs->levi.dash.dashX = cx/distancia; //Direccion x del dash (coseno)
        gs->levi.dash.dashY = cy/distancia; //Direccion y del dash (seno)
        gs->levi.ODM.activo = false;
        if(gs->levi.dash.dashY < 0 && gs->levi.levi_suelo == true) //Si el dash es hacia arriba activa esta variable
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

void colision_levi_elementos(s_GameState *gs)
{
    int i, pA = gs->pantalla_actual, nE = gs->pantalla[pA].num_elementos;

    for(i=0;i<nE;i++)
        if(colision(gs, gs->levi.hitbox, gs->pantalla[pA].elementos[i].hitbox) && gs->pantalla[pA].elementos[i].activo && gs->pantalla[pA].elementos[i].tipo == 2)
        {
            gs->levi.inventario.escudos++;
            gs->pantalla[pA].elementos[i].activo = false;
        }
}

void camara_scroll(s_GameState *gs)
{
    float bordeDer = gs->camara.x + SCREEN_X - BORDE_CAM - 150; //Limite derecho donde empieza a scrollear la camara
    float bordeIzq = gs->camara.x + BORDE_CAM; //Limite izquierdo donde empieza a scrollear la camara
    float leviX = gs->levi.x + 48;
    float anchoPantalla = gs->pantalla[gs->pantalla_actual].ancho * TAM_CELDA;

    if(leviX < bordeIzq) //Calcula cuando toca el limite izquierdo para que empiece a scrollear la camara, para luego r
        gs->camara.x = leviX - BORDE_CAM; //Obtiene un valor de la camara para luego restarlo al dibujo y que la posicion de levi quede en donde deberia estar en una pantalla normal

    else if(leviX > bordeDer) //Calcula cuando toca el limite derecho para que empiece a scrollear la camara
        gs->camara.x = leviX - (SCREEN_X - BORDE_CAM - 150); //Obtiene un valor de la camara para luego restarlo al dibujo y que la posicion de levi quede en donde deberia estar en una pantalla normal

    if(gs->camara.x < 0) //Evita bugs
        gs->camara.x = 0;

    if(anchoPantalla > SCREEN_X) 
    {
        if(gs->camara.x > anchoPantalla - SCREEN_X) //Si levi se encuentra en el borde del mundo total no se mueve la camara
            gs->camara.x = anchoPantalla - SCREEN_X;
    }
    else
        gs->camara.x = 0;

}

//================================================//