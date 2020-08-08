#ifndef __PAGE_SETUP_H__
#define __PAGE_SETUP_H__

#define MAX_TIME 81

lv_obj_t* page_setup_init(lv_obj_t *tabs);

void setTab_setup(lv_group_t *group);

void slider_temp_cb(lv_obj_t * obj, lv_event_t event);
void slider_hum_cb(lv_obj_t * obj, lv_event_t event);
void slider_time_cb(lv_obj_t * obj, lv_event_t event);

#endif // __PAGE_SETUP_H__