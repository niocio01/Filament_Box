#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <Arduino.h>

#include <lvgl.h>

#include "GUI/page_select.h"
#include "GUI/page_setup.h"
#include "GUI/page_run.h"
#include "GUI/profiles.h"

#include "GUI/input.h"
#include "GUI/nav.h"

lv_obj_t *tabs;
lv_group_t *group;

lv_obj_t *page_select;
lv_obj_t *page_setup;
lv_obj_t *page_run;

lv_obj_t *slider_temperature2;

void nav_init(void)
{
    /* ---------------------------------------- input ---------------------------------------------*/
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = read_encoder;
    lv_indev_t *encoder = lv_indev_drv_register(&indev_drv);
    initEncoder();

    /* ---------------------------------------- profiles ---------------------------------------------*/
    profiles_init();

    /* ---------------------------------------- tabs ---------------------------------------------*/
    tabs = lv_tabview_create(lv_scr_act(), NULL);

    page_select = select_page_init(tabs);
    page_setup = setup_page_init(tabs);
    page_run = run_page_init(tabs);

    lv_obj_set_style_local_pad_top(tabs, LV_TABVIEW_PART_TAB_BTN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_pad_bottom(tabs, LV_TABVIEW_PART_TAB_BTN, LV_STATE_DEFAULT, 0);

    lv_obj_set_style_local_pad_top(tabs, LV_TABVIEW_PART_TAB_BG, LV_STATE_DEFAULT, 5);
    lv_obj_set_style_local_pad_bottom(tabs, LV_TABVIEW_PART_TAB_BG, LV_STATE_DEFAULT, 10);

    /* ---------------------------------------- group ---------------------------------------------*/
    group = lv_group_create();
    lv_indev_set_group(encoder, group);

    set_tab(TAB_SELECT);

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

void set_tab(tabs_t tab)
{
    lv_tabview_set_tab_act(tabs, tab, LV_ANIM_ON);
    switch (tab)
        {
        case TAB_SELECT:
            select_setTab(group);
            break;

        case TAB_SETUP:
            setup_setTab(group);
            break;

        case TAB_RUN:
            break;

        default:
            break;
        }
}