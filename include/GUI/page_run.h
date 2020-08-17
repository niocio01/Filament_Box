#ifndef __PAGE_RUN_H__
#define __PAGE_RUN_H__

lv_obj_t* run_page_init(lv_obj_t *tabs);

void toggleTask(void *pvParameter);

void run_setTab(void);

void run_btn_back_cb(lv_obj_t * obj, lv_event_t event);
void run_btn_materials_cb(lv_obj_t * obj, lv_event_t event);

void run_toggle_led(void);


#endif // __PAGE_RUN_H__