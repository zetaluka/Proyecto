#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
 
#define ANCHO_PANTALLA 800
#define ALTO_PANTALLA  600
#define TAM_CUADRADO   40
#define VELOCIDAD      4.0f
#define FPS            60
 
int main(void) {
    /* --- Init Allegro --- */
    if (!al_init()) {
        fprintf(stderr, "Error: al_init() fallo\n");
        return 1;
    }
    if (!al_install_keyboard()) {
        fprintf(stderr, "Error: al_install_keyboard() fallo\n");
        return 1;
    }
    if (!al_init_primitives_addon()) {
        fprintf(stderr, "Error: al_init_primitives_addon() fallo\n");
        return 1;
    }
 
    ALLEGRO_DISPLAY *display = al_create_display(ANCHO_PANTALLA, ALTO_PANTALLA);
    if (!display) {
        fprintf(stderr, "Error: al_create_display() fallo\n");
        return 1;
    }
    al_set_window_title(display, "Cuadrado WASD - Allegro 5");
 
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        fprintf(stderr, "Error: al_create_timer() fallo\n");
        return 1;
    }
 
    ALLEGRO_EVENT_QUEUE *cola = al_create_event_queue();
    if (!cola) {
        fprintf(stderr, "Error: al_create_event_queue() fallo\n");
        return 1;
    }
 
    al_register_event_source(cola, al_get_display_event_source(display));
    al_register_event_source(cola, al_get_timer_event_source(timer));
    al_register_event_source(cola, al_get_keyboard_event_source());
 
    /* --- Estado del juego --- */
    float x = (ANCHO_PANTALLA - TAM_CUADRADO) / 2.0f;
    float y = (ALTO_PANTALLA  - TAM_CUADRADO) / 2.0f;
 
    /* Teclas presionadas actualmente */
    int tecla_w = 0, tecla_a = 0, tecla_s = 0, tecla_d = 0;
 
    int corriendo    = 1;
    int redibujar    = 1;
 
    al_start_timer(timer);
 
    /* --- Bucle principal --- */
    while (corriendo) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(cola, &ev);
 
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            corriendo = 0;
        }
 
        /* Tecla presionada */
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_W: tecla_w = 1; break;
                case ALLEGRO_KEY_A: tecla_a = 1; break;
                case ALLEGRO_KEY_S: tecla_s = 1; break;
                case ALLEGRO_KEY_D: tecla_d = 1; break;
                case ALLEGRO_KEY_ESCAPE: corriendo = 0; break;
            }
        }
 
        /* Tecla soltada */
        else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_W: tecla_w = 0; break;
                case ALLEGRO_KEY_A: tecla_a = 0; break;
                case ALLEGRO_KEY_S: tecla_s = 0; break;
                case ALLEGRO_KEY_D: tecla_d = 0; break;
            }
        }
 
        /* Tick del timer: actualizar logica */
        else if (ev.type == ALLEGRO_EVENT_TIMER) {
            if (tecla_w) y -= VELOCIDAD;
            if (tecla_s) y += VELOCIDAD;
            if (tecla_a) x -= VELOCIDAD;
            if (tecla_d) x += VELOCIDAD;
 
            /* Limitar al borde de la pantalla */
            if (x < 0)                          x = 0;
            if (y < 0)                          y = 0;
            if (x > ANCHO_PANTALLA - TAM_CUADRADO) x = ANCHO_PANTALLA - TAM_CUADRADO;
            if (y > ALTO_PANTALLA  - TAM_CUADRADO) y = ALTO_PANTALLA  - TAM_CUADRADO;
 
            redibujar = 1;
        }
 
        /* Dibujar solo cuando la cola esta vacia y hay cambios */
        if (redibujar && al_is_event_queue_empty(cola)) {
            redibujar = 0;
 
            al_clear_to_color(al_map_rgb(20, 20, 20));
 
            /* Sombra del cuadrado */
            al_draw_filled_rectangle(
                x + 4, y + 4,
                x + TAM_CUADRADO + 4, y + TAM_CUADRADO + 4,
                al_map_rgba(0, 0, 0, 100)
            );
 
            /* Cuadrado principal */
            al_draw_filled_rectangle(
                x, y,
                x + TAM_CUADRADO, y + TAM_CUADRADO,
                al_map_rgb(70, 180, 255)
            );
 
            /* Borde del cuadrado */
            al_draw_rectangle(
                x, y,
                x + TAM_CUADRADO, y + TAM_CUADRADO,
                al_map_rgb(255, 255, 255), 2.0f
            );
 
            al_flip_display();
        }
    }
 
    /* --- Limpieza --- */
    al_destroy_event_queue(cola);
    al_destroy_timer(timer);
    al_destroy_display(display);
 
    return 0;
}
 