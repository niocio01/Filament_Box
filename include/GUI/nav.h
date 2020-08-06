#ifndef __NAV_H__
#define __NAV_H__


void nav_init(void);

void updateTask(void *pvParameter);
bool read_encoder(lv_indev_drv_t * indev, lv_indev_data_t * data);
void tabs_cb(lv_obj_t * obj, lv_event_t event);

#endif // __NAV_H__