#ifndef __NAV_H__
#define __NAV_H__

typedef enum
{
    TAB_SELECT,
    TAB_SETUP,
    TAB_RUN
} tabs_t;


extern lv_group_t *group;


void nav_init(void);

void updateTask(void *pvParameter);
bool read_encoder(lv_indev_drv_t * indev, lv_indev_data_t * data);
void set_tab(tabs_t tab);
#endif // __NAV_H__