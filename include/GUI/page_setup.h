#ifndef __PAGE_SETUP_H__
#define __PAGE_SETUP_H__

#define MAX_TIME 5*24*60

lv_obj_t* setup_page_init(lv_obj_t *tabs);

void setup_setTab(void);

void setup_updateProfileName(void);

void setup_btn_back_cb(lv_obj_t * obj, lv_event_t event);
void setup_btn_run_cb(lv_obj_t * obj, lv_event_t event);

void setup_slider_temperature_cb(lv_obj_t * obj, lv_event_t event);
void setup_slider_humidity_cb(lv_obj_t * obj, lv_event_t event);
void setup_slider_time_cb(lv_obj_t * obj, lv_event_t event);

#endif // __PAGE_SETUP_H__