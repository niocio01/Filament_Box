#ifndef __PAGE_SELECT_H__
#define __PAGE_SELECT_H__

lv_obj_t* page_select_init(lv_obj_t *tabs);
// void page_cb(lv_obj_t * obj, lv_event_t event);

void setTab_select(lv_group_t * group);
uint8_t get_selected_profile_id(void);


#endif // __PAGE_SELECT_H__