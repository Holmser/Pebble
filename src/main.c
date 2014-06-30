#include "pebble.h"

Window *window;  
TextLayer *text_layer;
char buffer[] = "00 00";
//char hour[] = "00"
//char min[] = "00"
InverterLayer *inv_layer;
GBitmap *seahawks_bitmap, *trophy_bitmap;
BitmapLayer *seahawks_layer, *trophy_layer;


void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  //Update watchface display
  strftime(buffer, sizeof("00 00"), "%H %M", tick_time);
  
  text_layer_set_text(text_layer, buffer);
  
}

void window_load(Window *window)
{
  ResHandle font_handle = resource_get_handle(RESOURCE_ID_ORBIT_FONT_42);
  seahawks_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SEAHAWKS_LOGO);
  trophy_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SB_TROPHY);
  
  
  seahawks_layer = bitmap_layer_create(GRect(0,0,144,60));
  bitmap_layer_set_bitmap(seahawks_layer, seahawks_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(seahawks_layer));
  
  
  trophy_layer = bitmap_layer_create(GRect(45,62,50,50));
  bitmap_layer_set_bitmap(trophy_layer, trophy_bitmap);
  //layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(trophy_layer));
  
  
  text_layer = text_layer_create(GRect(15,60,120,168));
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_color(text_layer, GColorBlack);
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_font(text_layer, fonts_load_custom_font(font_handle));
  
  layer_add_child(window_get_root_layer(window), (Layer*) text_layer);
  //text_layer_set_text(text_layer, "Jenn is the bestest");
  
  //inv_layer = inverter_layer_create(GRect(0, 55, 144, 62));
  inv_layer = inverter_layer_create(GRect(0, 0, 144, 156));
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(trophy_layer));
  
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
  gbitmap_destroy(seahawks_bitmap);
  bitmap_layer_destroy(seahawks_layer);
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