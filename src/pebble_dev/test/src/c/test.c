#include <pebble.h>

static Window *s_window;
static TextLayer *s_text_layer;

int angle=0;

static Layer *my_window_layer;
static GBitmap *my_hour_hand_image;
static RotBitmapLayer *my_hour_hand_layer;


static void prv_select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(s_text_layer, "Select");

  int rad_angle;
  
  angle = (angle + 10) % 360;
  rad_angle = TRIG_MAX_ANGLE * angle / 360;
  rot_bitmap_layer_set_angle(my_hour_hand_layer, rad_angle);

}

static void prv_up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(s_text_layer, "Up");
}

static void prv_down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(s_text_layer, "Down");
}

static void prv_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, prv_select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, prv_up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, prv_down_click_handler);
}

static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_text_layer = text_layer_create(GRect(0, 72, bounds.size.w, 20));
  text_layer_set_text(s_text_layer, "Press right arrow");
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer));
}

static void prv_window_unload(Window *window) {
  text_layer_destroy(s_text_layer);
}


void log_heap() 
{
    int h = (int)heap_bytes_free();
    app_log(APP_LOG_LEVEL_WARNING,
	    __FILE__,
	    __LINE__,	 
            "heap free %d\n", h);
}


static void prv_init(void) {
  s_window = window_create();
  window_set_click_config_provider(s_window, prv_click_config_provider);
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload,
  });
  const bool animated = true;
  window_stack_push(s_window, animated);

    my_window_layer  = window_get_root_layer(s_window);

    // Set up a layer for the hour hand
    my_hour_hand_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_HOUR_HAND);
    app_log(APP_LOG_LEVEL_WARNING,
	    __FILE__,
	    __LINE__,	 
	    "after hour\n");
    log_heap();

    my_hour_hand_layer = rot_bitmap_layer_create(my_hour_hand_image);
    app_log(APP_LOG_LEVEL_WARNING,
	    __FILE__,
	    __LINE__,	 
	    "after hour image\n");

    layer_add_child(my_window_layer,
                    bitmap_layer_get_layer((BitmapLayer *)my_hour_hand_layer));
//    layer_add_child(my_window_layer,(Layer *)my_hour_hand_layer);
    app_log(APP_LOG_LEVEL_WARNING,
	    __FILE__,
	    __LINE__,	 
	    "after hour add\n");
    rot_bitmap_set_src_ic(my_hour_hand_layer, GPoint(13, 27));
  

}

static void prv_deinit(void) {
  window_destroy(s_window);
}

int main(void) {
  prv_init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", s_window);

  app_event_loop();
  prv_deinit();
}
