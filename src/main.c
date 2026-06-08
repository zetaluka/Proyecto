#include "commons.h"

int main(void) {

   al_init();
   al_init_font_addon();
   al_init_ttf_addon();
   al_init_primitives_addon();
   al_install_mouse();
   al_init_image_addon();
   al_install_keyboard();

   ALLEGRO_TIMER* timer_juego = al_create_timer(1.0);
   ALLEGRO_TIMER* timer_fps = al_create_timer(1.0/FPS);
   ALLEGRO_EVENT evento;
   ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
   ALLEGRO_DISPLAY* screen = al_create_display(SCREEN_X, SCREEN_Y);
   al_register_event_source(queue, al_get_display_event_source(screen));
   
   al_set_window_title(screen,"CodeNoKyojin");

   al_register_event_source(queue, al_get_mouse_event_source());
   al_register_event_source(queue, al_get_keyboard_event_source());
   al_register_event_source(queue , al_get_timer_event_source(timer_juego));
   al_register_event_source(queue , al_get_timer_event_source(timer_fps));

   al_start_timer(timer_juego);
   al_start_timer(timer_fps);

   /* ------------------------------------------------------------
      4. INICIALIZAR EL ESTADO DEL JUEGO Y LOS ASSETS
      ------------------------------------------------------------ */
   s_GameState gs = {0};
   s_InputState input_state = {0};
   s_Assets assets;
   assets_load(&assets);
   game_init(&gs);

   while (gs.ejecutando) 
   {
      al_wait_for_event(queue, &evento);
      input_update(&input_state, &evento);

      if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
         gs.ejecutando = false;

      if(evento.type == ALLEGRO_EVENT_TIMER)
      {
         if(evento.timer.source == timer_juego)
            gs.tiempoJugado.validacion = 1;
         if(evento.timer.source == timer_fps)
         {
            update(&gs, &input_state, &assets);
            render_gameview(&gs,&assets);
            render_ui(&gs, &assets);
         }
      }
     
   }


    /* ------------------------------------------------------------
       6. CIERRE Y LIBERACIÓN DE RECURSOS
       ------------------------------------------------------------ */

    return 0;
}