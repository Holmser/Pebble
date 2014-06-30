#include "pebble.h"

Window *window;  
TextLayer *text_layer;
char buffer[] = "00:00";
InverterLayer *inv_layer;

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  //Update watchface display
  strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  
  text_layer_set_text(text_layer, buffer);
  
}

void window_load(Window *window)
{
  seahawks_bitmap = gbitmap_create_with_resource(SEAHAWKS_LOGO);
  seahawks_layer = bitmap_layer_create(GRect(0,0,144,50));
  bitmap_layer_set_bitmap(seahawks_layer, seahawks_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(seahawks_layer));
  
  text_layer = text_layer_create(GRect(0,53,132,168));
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_color(text_layer, GColorBlack);
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  
  layer_add_child(window_get_root_layer(window), (Layer*) text_layer);
  text_layer_set_text(text_layer, "Jenn is the bestest");
  
  inv_layer = inverter_layer_create(GRect(0, 50, 144, 62));
  layer_add_child(window_get_root_layer(window), (Layer*) inv_layer);
  
  struct tm *t;
  time_t temp;
  temp = time(NULL);
  t= localtime(&temp);
  
  tick_handler(t, MINUTE_UNIT);
  
  //add window elements
}

void window_unload(Window *window)
{
  //destroy window elements
  text_layer_destroy(text_layer);
  inverter_layer_destroy(inv_layer);
}


void init()
{
  //inititalize app elements
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true);
  tick_timer_service_subscribe(MINUTE_UNIT, (TickHandler) tick_handler);
}

void deinit()
{
  //De-Initialize
  window_destroy(window);
  tick_timer_service_unsubscribe();
}

int main(void)
{
  init();
  app_event_loop();
  deinit();
}