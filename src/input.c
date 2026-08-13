#include "commons.h"

//====Funcion principal====//
void input_update(s_GameState *gs, ALLEGRO_EVENT* evento)
{
    if(evento->type == ALLEGRO_EVENT_KEY_DOWN)
    {
        switch(evento->keyboard.keycode)
        {
            case ALLEGRO_KEY_W:
                gs->input.keyW = true;
                break;
            case ALLEGRO_KEY_S:
                gs->input.keyS = true;
                break;
            case ALLEGRO_KEY_A:
                gs->input.keyA = true;
                break;
            case ALLEGRO_KEY_D:
                gs->input.keyD = true;
                break;
            case ALLEGRO_KEY_SPACE:
                gs->input.keySpace = true;
                break;
            case ALLEGRO_KEY_LSHIFT:
                gs->input.keyLShift = true;
                break;
            case ALLEGRO_KEY_L:
                gs->input.keyL = true;
                break;
            case ALLEGRO_KEY_H: //Usado para debugg, muestra hitboxes
                if(!gs->input.keyH)
                    gs->input.keyH = true;
                else if(gs->input.keyH)
                    gs->input.keyH = false;
                break;
            case ALLEGRO_KEY_G:
                if(!gs->input.keyG)
                    gs->input.keyG = true;
                else if(gs->input.keyG)
                    gs->input.keyG = false;
                break;
            case ALLEGRO_KEY_F:
                gs->input.keyF = true;
                break;
            case ALLEGRO_KEY_E:
                gs->input.keyE = true;
                break;
            case ALLEGRO_KEY_R:
                gs->input.keyR = true;
                break;
            case ALLEGRO_KEY_X:
                gs->input.keyX = true;
                break;
            case ALLEGRO_KEY_C:
                gs->input.keyC = true;
                break;
            case ALLEGRO_KEY_1:
                gs->input.key1 = true;
                break;
            case ALLEGRO_KEY_2:
                gs->input.key2 = true;
                break;
            case ALLEGRO_KEY_3:
                gs->input.key3 = true;
                break;
            case ALLEGRO_KEY_ENTER: 
                gs->input.keyEnter = true;
                break;
            case ALLEGRO_KEY_ESCAPE:
                gs->input.keyEsc = true;
                break; 
        }
    }

    else if(evento->type == ALLEGRO_EVENT_KEY_UP)
    {
        switch(evento->keyboard.keycode)
        {
            case ALLEGRO_KEY_W:
                gs->input.keyW = false;
                break;
            case ALLEGRO_KEY_S:
                gs->input.keyS = false;
                break;
            case ALLEGRO_KEY_A:
                gs->input.keyA = false;
                break;
            case ALLEGRO_KEY_D:
                gs->input.keyD = false;
                break;
            case ALLEGRO_KEY_SPACE:
                gs->input.keySpace = false;
                break;
            case ALLEGRO_KEY_LSHIFT:
                gs->input.keyLShift = false;
                break;
            case ALLEGRO_KEY_L:
                gs->input.keyL = false;
                break;
            case ALLEGRO_KEY_F:
                gs->input.keyF = false;
                break;
            case ALLEGRO_KEY_E:
                gs->input.keyE = false;
                break;
            case ALLEGRO_KEY_R:
                gs->input.keyR = false;
                break;
            case ALLEGRO_KEY_X:
                gs->input.keyX = false;
                break;
            case ALLEGRO_KEY_C:
                gs->input.keyC = true;
                break;
            case ALLEGRO_KEY_1:
                gs->input.key1 = false;
                break;
            case ALLEGRO_KEY_2:
                gs->input.key2 = false;
                break;
            case ALLEGRO_KEY_3:
                gs->input.key3 = false;
                break;
            case ALLEGRO_KEY_ENTER: 
                gs->input.keyEnter = false;
                break;
            case ALLEGRO_KEY_ESCAPE:
                gs->input.keyEsc = false;
                break;
            /*case ALLEGRO_KEY_H:
                gs->input.keyH = false;
                break;*/
        }
    }

    else if(evento->type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        switch(evento->mouse.button)
        {
            case ALLEGRO_MOUSE_BUTTON_LEFT:
                gs->input.ClickIzq = true;
                break;
            case ALLEGRO_MOUSE_BUTTON_RIGHT:
                gs->input.ClickDer = true;
                break;
        }
    
    else if(evento->type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        switch(evento->mouse.button)
        {
            case ALLEGRO_MOUSE_BUTTON_LEFT:
                gs->input.ClickIzq = false;
                break;
            case ALLEGRO_MOUSE_BUTTON_RIGHT:
                gs->input.ClickDer = false;
                break;
        }

    else if(evento->type == ALLEGRO_EVENT_MOUSE_AXES)
    {
        gs->input.mouseX = evento->mouse.x;
        gs->input.mouseY = evento->mouse.y;
    }

    return;
}