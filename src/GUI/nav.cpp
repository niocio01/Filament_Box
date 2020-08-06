#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <Arduino.h>

#include <lvgl.h>

#include "GUI/page_select.h"
#include "GUI/page_select.h"
#include "GUI/page_select.h"

#include "GUI/input.h"
#include "GUI/nav.h"

lv_obj_t *tabs;
lv_group_t *group;

lv_obj_t *page_select;
lv_obj_t *page_setup;
lv_obj_t *page_run;

void nav_init(void)
{
    /* ---------------------------------------- input ---------------------------------------------*/
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = read_encoder;
    lv_indev_t *encoder = lv_indev_drv_register(&indev_drv);
    initEncoder();

    /* ---------------------------------------- tabs ---------------------------------------------*/
    tabs = lv_tabview_create(lv_scr_act(), NULL);
    
    page_select = page_setup_init(tabs);
    page_setup = lv_tabview_add_tab(tabs, "Setup");
    page_run = lv_tabview_add_tab(tabs, "Run");

    lv_obj_set_style_local_pad_top(tabs, LV_TABVIEW_PART_TAB_BTN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_pad_bottom(tabs, LV_TABVIEW_PART_TAB_BTN, LV_STATE_DEFAULT, 0);
    //lv_obj_set_style_local_size(tabs, LV_TABVIEW_PART_INDIC, LV_STATE_DEFAULT, 5);

    //lv_style_plain

    lv_obj_set_style_local_pad_top(tabs, LV_TABVIEW_PART_TAB_BG, LV_STATE_DEFAULT, 5);
    lv_obj_set_style_local_pad_bottom(tabs, LV_TABVIEW_PART_TAB_BG, LV_STATE_DEFAULT, 10);

    //lv_obj_set_event_cb(tabs, tabs_cb);

    /* ---------------------------------------- group ---------------------------------------------*/
    group = lv_group_create();

    lv_group_add_obj(group, tabs);
    lv_indev_set_group(encoder, group);

    //lv_group_focus_obj(buttons);
    //lv_group_set_editing(group1, true);
    
    xTaskCreate(updateTask, "UpdateTask", 1024 * 4, NULL, 1, NULL);
}

void updateTask(void *pvParameter)
{
    while (1)
    {
        uint32_t waitTime = lv_task_handler(); /* let the GUI do its work */
        vTaskDelay(waitTime / portTICK_PERIOD_MS);
    }
}

bool read_encoder(lv_indev_drv_t * indev, lv_indev_data_t * data)
{
    data->enc_diff = readEncMovement();
    data->state = readEncButton();
    return false;
}

void tabs_cb(lv_obj_t * obj, lv_event_t event)
{
    Serial.print("tabs: ");
    Serial.println(event);

    switch (event)
    {
    case LV_EVENT_SHORT_CLICKED:
        if (lv_tabview_get_tab_act(tabs) == 0)
        {
            //enterbutton = true;
            //lv_group_focus_obj(buttons);
            //lv_group_set_editing(group1, true);
        }
        break;
    }
}
