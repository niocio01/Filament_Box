#ifndef __PAGE_RUN_H__
#define __PAGE_RUN_H__

lv_obj_t* run_page_init(lv_obj_t *tabs);

void toggleTask(void *pvParameter);

void run_setTab(lv_group_t * group);

void run_toggle_led(void);


#endif // __PAGE_RUN_H__