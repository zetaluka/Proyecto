#include "commons.h"

int main(void) {

   srand(time(0));
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
   ALLEGRO_DISPLAY* display = al_create_display(1280, 720);
   al_register_event_source(queue, al_get_display_event_source(display));
   
   al_set_window_title(display,"CodeNoKyojin");

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
   s_Assets assets;
   assets_load(&assets);
   game_init(&gs, &assets, display);

   while (gs.ejecutando) 
   {
      al_wait_for_event(queue, &evento);
      input_update(&gs, &evento);

      if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
         gs.ejecutando = false;

      if(evento.type == ALLEGRO_EVENT_TIMER)
      {
         if(evento.timer.source == timer_juego)
            gs.tiempoJugado.validacion = 1;
         if(evento.timer.source == timer_fps)
         {
            genera_entidades(&gs, &assets);
            update(&gs, &assets);
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