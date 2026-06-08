#include "commons.h"


//====Funcion principal====//
void input_update(s_InputState *input, ALLEGRO_EVENT* evento)
{
    if(evento->type == ALLEGRO_EVENT_KEY_DOWN)
    {
        switch(evento->keyboard.keycode)
        {
        case ALLEGRO_KEY_W:
            input->keyW = true;
            break;
        case ALLEGRO_KEY_S:
            input->keyS = true;
            break;
        case ALLEGRO_KEY_A:
            input->keyA = true;
            break;
        case ALLEGRO_KEY_D:
            input->keyD = true;
            break;
        case ALLEGRO_KEY_SPACE:
            input->keySpace = true;
            break;
        case ALLEGRO_KEY_LSHIFT:
            input->keyLShift = true;
            break;
        case ALLEGRO_KEY_L:
            input->keyL = true;
        }
    }

    else if(evento->type == ALLEGRO_EVENT_KEY_UP)
    {
        switch(evento->keyboard.keycode)
        {
            case ALLEGRO_KEY_W:
                input->keyW = false;
                break;
            case ALLEGRO_KEY_S:
                input->keyS = false;
                break;
            case ALLEGRO_KEY_A:
                input->keyA = false;
                break;
            case ALLEGRO_KEY_D:
                input->keyD = false;
                break;
            case ALLEGRO_KEY_SPACE:
                input->keySpace = false;
                break;
            case ALLEGRO_KEY_LSHIFT:
                input->keyLShift = false;
                break;
            case ALLEGRO_KEY_L:
                input->keyL = false;
        }
    }

    if(evento->type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        printf("Mouse x = %d, Mouse y = %d\n", evento->mouse.x, evento->mouse.y);
         

    return;
}