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
   ALLEGRO_TIMER* timer_fps = al_create_timer(1.0/60.0);
   ALLEGRO_EVENT evento;
   ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
   ALLEGRO_DISPLAY* screen = al_create_display(SCREEN_X, SCREEN_Y);
   
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
   s_InputState input_state;
   s_Assets assets;
   //game_init(&gs);
   gs.ejecutando = 1;
   assets_load(&assets);


   while (gs.ejecutando) 
   {
      al_wait_for_event(queue, &evento);

      if(evento.type == ALLEGRO_EVENT_TIMER){
         if(evento.timer.source == timer_juego)
            gs.tiempoJugado.validacion = 1;
         /*if(evento.timer.source == timer_fps){
            input_update(&input_state, &evento);
            update(&gs, &input_state);
            render_gameview(&gs);
            render_ui(&gs, &assets);
         }*/
      }
      input_update(&input_state, &evento);
      update(&gs, &input_state);
      render_gameview(&gs);
      render_ui(&gs, &assets);
      //else 
         //input_update(&input_state, &evento);
     
   }


    /* ------------------------------------------------------------
       6. CIERRE Y LIBERACIÓN DE RECURSOS
       ------------------------------------------------------------ */

    return 0;
}