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
        case ALLEGRO_KEY_H:
            if(!gs->input.keyH)
                gs->input.keyH = true;
            else if(gs->input.keyH)
                gs->input.keyH = false;
            printf("%d",gs->input.keyH);
            break;
        case ALLEGRO_KEY_G:
            if(!gs->input.keyG)
                gs->input.keyG = true;
            else if(gs->input.keyG)
                gs->input.keyG = false;
            printf("%d",gs->input.keyG);
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
        }
    
    else if(evento->type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        switch(evento->mouse.button)
        {
            case ALLEGRO_MOUSE_BUTTON_LEFT:
                gs->input.ClickIzq = false;
                break;
        }

    if(evento->type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        printf("Mouse x = %d, Mouse y = %d\n", evento->mouse.x, evento->mouse.y);
         

    return;
}