#ifndef __PAGE_SELECT_H__
#define __PAGE_SELECT_H__

lv_obj_t* select_page_init(lv_obj_t *tabs);

void select_setTab(lv_group_t * group);
void select_btn_mtrx_cb(lv_obj_t * obj, lv_event_t event);
uint8_t select_get_selected_profile_id(void);


#endif // __PAGE_SELECT_H__