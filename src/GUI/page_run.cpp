#include <Arduino.h>
#include <lvgl.h>

#include "GUI/display.h"
#include "GUI/nav.h"
#include "GUI/page_select.h"
#include "GUI/page_setup.h"
#include "GUI/page_run.h"
#include "GUI/profiles.h"

lv_obj_t *run_page;

lv_obj_t *run_label_temperature_title;
lv_obj_t *run_label_temperature_value;
lv_obj_t *run_label_temperature_setpoint;
lv_obj_t *run_led_temperature;

lv_obj_t *run_label_humidity_title;
lv_obj_t *run_label_humidity_value;
lv_obj_t *run_label_humidity_setpoint;
lv_obj_t *run_led_humidity;

lv_obj_t* run_page_init(lv_obj_t *tabs)
{
    /*------------------------------------Tabs--------------------------------------*/
    run_page = lv_tabview_add_tab(tabs, "Run");
    lv_page_set_scrollbar_mode(run_page, LV_SCROLLBAR_MODE_OFF);

    /*---------------------------------Style-----------------------------------*/

    static lv_style_t font_Large;
    lv_style_init(&font_Large);
    lv_style_set_text_font(&font_Large, LV_STATE_DEFAULT, &lv_font_montserrat_30);

    /*---------------------------------Temperature-----------------------------------*/
    run_label_temperature_title = lv_label_create(run_page, NULL);
    lv_label_set_text(run_label_temperature_title, "Temp:  ");
    lv_obj_align(run_label_temperature_title, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 8);
    
    run_label_temperature_value = lv_label_create(run_page, NULL);
    lv_label_set_text(run_label_temperature_value, "25.8");
    lv_label_set_recolor(run_label_temperature_value, true);
    lv_obj_align(run_label_temperature_value, NULL, LV_ALIGN_IN_TOP_LEFT, 70, 1);
    lv_obj_add_style(run_label_temperature_value, LV_LABEL_PART_MAIN, &font_Large);
    lv_obj_set_style_local_text_color(run_label_temperature_value, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);

    run_label_temperature_setpoint = lv_label_create(run_page, NULL);
    lv_label_set_text(run_label_temperature_setpoint, "  / 45°C");
    lv_obj_align(run_label_temperature_setpoint, run_label_temperature_value, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

    run_led_temperature = lv_led_create(run_page, NULL),
    lv_obj_align(run_led_temperature, NULL, LV_ALIGN_IN_TOP_LEFT, 200, 8);
    lv_obj_set_size(run_led_temperature, 20, 20);

    lv_obj_set_style_local_shadow_spread(run_led_temperature, LV_LED_PART_MAIN, LV_STATE_DEFAULT, 5);
    lv_obj_set_style_local_shadow_opa(run_led_temperature, LV_LED_PART_MAIN, LV_STATE_DEFAULT, 127);

    /*---------------------------------Humidity-----------------------------------*/

    run_label_humidity_title = lv_label_create(run_page, NULL);
    lv_label_set_text(run_label_humidity_title, "Hum:  ");
    lv_obj_align(run_label_humidity_title, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 8+35);
    
    run_label_humidity_value = lv_label_create(run_page, NULL);
    lv_label_set_text(run_label_humidity_value, "25.8");
    lv_label_set_recolor(run_label_humidity_value, true);
    lv_obj_align(run_label_humidity_value, NULL, LV_ALIGN_IN_TOP_LEFT, 70, 1+35);
    lv_obj_add_style(run_label_humidity_value, LV_LABEL_PART_MAIN, &font_Large);
    lv_obj_set_style_local_text_color(run_label_humidity_value, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);

    run_label_humidity_setpoint = lv_label_create(run_page, NULL);
    lv_label_set_text(run_label_humidity_setpoint, "  / 10");
    lv_obj_align(run_label_humidity_setpoint, run_label_humidity_value, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

    run_led_humidity = lv_led_create(run_page, NULL),
    lv_obj_align(run_led_humidity, NULL, LV_ALIGN_IN_TOP_LEFT, 200, 12+35);
    lv_obj_set_size(run_led_humidity, 20, 20);

    lv_obj_set_style_local_shadow_spread(run_led_humidity, LV_LED_PART_MAIN, LV_STATE_DEFAULT, 5);
    lv_obj_set_style_local_shadow_opa(run_led_humidity, LV_LED_PART_MAIN, LV_STATE_DEFAULT, 127);


    xTaskCreate(toggleTask, "Toggle_task", 1024 * 4, NULL, 1, NULL);


    return run_page;
}

void run_setTab(lv_group_t * group)
{
    lv_group_remove_all_objs(group);
}

void run_toggle_led(void)
{
    lv_led_toggle(run_led_temperature);
    lv_led_toggle(run_led_humidity);
}

void toggleTask(void *pvParameter)
{
    while (1)
    {
        run_toggle_led();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

