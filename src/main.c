#include "pebble.h"

Window *window;  
TextLayer *text_layer, *hour_layer, *minute_layer;
char buffer[] = "00 00";
char hour[] = "00";
char min[] = "00";
InverterLayer *inv_layer;
GBitmap *seahawks_bitmap, *trophy_bitmap, *feather_bitmap;
BitmapLayer *seahawks_layer, *trophy_layer, *feather_layer;


void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  //Update watchface display
  
  //strftime(buffer, sizeof("00 00"), "%I %M", tick_time);
  strftime(hour, sizeof("00"), "%I", tick_time);
  strftime(min, sizeof("00"), "%M", tick_time);
  
  if (hour[0] == '0') {
    // Remove the leading zero when 12-hour clock is used
    memmove(hour, &hour[1], sizeof(hour) - 1);
  }
  
  //text_layer_set_text(text_layer, buffer);
  text_layer_set_text(hour_layer, hour);
  text_layer_set_text(minute_layer, min);
  
}

void window_load(Window *window)
{
  ResHandle font_handle = resource_get_handle(RESOURCE_ID_ORBIT_FONT_42);
  seahawks_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SEAHAWKS_LOGO);
  trophy_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SB_TROPHY);
  feather_bitmap = gbitmap_create_with_resource(RESOURCE_ID_FEATHER);
  
  
  seahawks_layer = bitmap_layer_create(GRect(0,0,144,60));
  bitmap_layer_set_bitmap(seahawks_layer, seahawks_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(seahawks_layer));
  
  //feather
  feather_layer = bitmap_layer_create(GRect(0,128,144,50));
  bitmap_layer_set_bitmap(feather_layer, feather_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(feather_layer));
  
  
  
  trophy_layer = bitmap_layer_create(GRect(47,64,50,50));
  bitmap_layer_set_bitmap(trophy_layer, trophy_bitmap);
  //layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(trophy_layer));
  
  //large text layer
  /*
  text_layer = text_layer_create(GRect(15,60,120,168));
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_color(text_layer, GColorBlack);
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_font(text_layer, fonts_load_custom_font(font_handle));
  */
  //layer_add_child(window_get_root_layer(window), (Layer*) text_layer);
  //text_layer_set_text(text_layer, "Jenn is the bestest");
  
  //hour layer
  hour_layer = text_layer_create(GRect(-10,60,70,60));
  text_layer_set_background_color(hour_layer, GColorClear);
  text_layer_set_text_color(hour_layer, GColorBlack);
  text_layer_set_text_alignment(hour_layer, GTextAlignmentRight);
  text_layer_set_font(hour_layer, fonts_load_custom_font(font_handle));
  layer_add_child(window_get_root_layer(window), (Layer*) hour_layer);
  
  //minute layer
  minute_layer = text_layer_create(GRect(80,60,80,60));
  text_layer_set_background_color(minute_layer, GColorClear);
  text_layer_set_text_color(minute_layer, GColorBlack);
  text_layer_set_text_alignment(minute_layer, GTextAlignmentLeft);
  text_layer_set_font(minute_layer, fonts_load_custom_font(font_handle));
  layer_add_child(window_get_root_layer(window), (Layer*) minute_layer);
  
  
  //inv_layer = inverter_layer_create(GRect(0, 55, 144, 62));
  inv_layer = inverter_layer_create(GRect(0, 0, 144, 168));
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
  //text_layer_destroy(text_layer);
  text_layer_destroy(hour_layer);
  
  inverter_layer_destroy(inv_layer);
  gbitmap_destroy(seahawks_bitmap);
  bitmap_layer_destroy(seahawks_layer);
  gbitmap_destroy(trophy_bitmap);
  bitmap_layer_destroy(trophy_layer);
  bitmap_layer_destroy(feather_layer);
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