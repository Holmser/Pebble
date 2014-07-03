#include "pebble.h"

Window *window;  
TextLayer *text_layer;
char buffer[] = "0000";
InverterLayer *inv_layer;

GBitmap *seahawks_bitmap, *trophy_bitmap, *feather_bitmap;
BitmapLayer *seahawks_layer, *trophy_layer, *feather_layer_top, *feather_layer_bottom;


void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  //Update watchface display
  
  strftime(buffer, sizeof("0000"), "%I%M", tick_time);
  
  vibes_double_pulse();
  
  
  if (buffer[0] == '0') {
    // Remove the leading zero when 12-hour clock is used
    memmove(buffer, &buffer[1], sizeof(buffer) - 1);
  }
  
  
  text_layer_set_text(text_layer, buffer);
  
}

void window_load(Window *window)
{
  ResHandle font_handle = resource_get_handle(RESOURCE_ID_SEAHAWKS_FONT_46);
  seahawks_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SEAHAWKS_LOGO);
  feather_bitmap = gbitmap_create_with_resource(RESOURCE_ID_FEATHER);
  
  
  seahawks_layer = bitmap_layer_create(GRect(0,30,144,60));
  bitmap_layer_set_bitmap(seahawks_layer, seahawks_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(seahawks_layer));
  
  //feather
  feather_layer_bottom = bitmap_layer_create(GRect(0,144,144,24));
  bitmap_layer_set_bitmap(feather_layer_bottom, feather_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(feather_layer_bottom));
 
  feather_layer_top = bitmap_layer_create(GRect(0,0,144,24));

  bitmap_layer_set_bitmap(feather_layer_top, feather_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(feather_layer_top));
  
  
  text_layer = text_layer_create(GRect(15,85,120,168));
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_color(text_layer, GColorBlack);
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_font(text_layer, fonts_load_custom_font(font_handle));
  
  layer_add_child(window_get_root_layer(window), (Layer*) text_layer);
  
  inv_layer = inverter_layer_create(GRect(0, 0, 144, 168));
  
  
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
  //text_layer_destroy(hour_layer);
  
  inverter_layer_destroy(inv_layer);
  gbitmap_destroy(seahawks_bitmap);
  bitmap_layer_destroy(seahawks_layer);
  gbitmap_destroy(trophy_bitmap);
  bitmap_layer_destroy(trophy_layer);
  bitmap_layer_destroy(feather_layer_top);
  bitmap_layer_destroy(feather_layer_bottom);
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