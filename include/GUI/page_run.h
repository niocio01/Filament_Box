#ifndef __PAGE_RUN_H__
#define __PAGE_RUN_H__

lv_obj_t* run_page_init(lv_obj_t *tabs);

void run_setTab(void);

void run_group_cb (lv_group_t * group);

void run_btn_back_cb(lv_obj_t * obj, lv_event_t event);
void run_btn_pause_cb(lv_obj_t * obj, lv_event_t event);
void run_btn_materials_cb(lv_obj_t * obj, lv_event_t event);
void run_page_materialList_cb(lv_obj_t * obj, lv_event_t event);

void run_timeTask(void *pvParameter);
void toggleTask(void *pvParameter);


#endif // __PAGE_RUN_H__