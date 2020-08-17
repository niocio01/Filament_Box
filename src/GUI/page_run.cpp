#include <Arduino.h>
#include <lvgl.h>

#include "GUI/display.h"
#include "GUI/nav.h"
#include "GUI/profiles.h"
#include "GUI/page_select.h"
#include "GUI/page_setup.h"
#include "GUI/page_run.h"

lv_obj_t *run_page;

lv_obj_t *run_label_temperature_title;
lv_obj_t *run_label_temperature_value;
lv_obj_t *run_label_temperature_setpoint;
lv_obj_t *run_led_temperature;

lv_obj_t *run_label_humidity_title;
lv_obj_t *run_label_humidity_value;
lv_obj_t *run_label_humidity_setpoint;
lv_obj_t *run_led_humidity;

lv_obj_t *run_btn_back;
lv_obj_t *run_label_back;

lv_obj_t *run_btn_materials;
lv_obj_t *run_label_materials;

lv_obj_t *run_container_materialList;
lv_obj_t *run_label_materialList;
lv_obj_t *run_list_materialList;

lv_obj_t* run_page_init(lv_obj_t *tabs)
{
    /*------------------------------------Tabs--------------------------------------*/
    run_page = lv_tabview_add_tab(tabs, "Run");
    lv_page_set_scrollbar_mode(run_page, LV_SCROLLBAR_MODE_OFF);

    /*---------------------------------Style-----------------------------------*/

    static lv_style_t font_Large;
    lv_style_init(&font_Large);
    lv_style_set_text_font(&font_Large, LV_STATE_DEFAULT, &lv_font_montserrat_30);

    static lv_style_t style_btn;
    lv_style_init(&style_btn);
    lv_style_set_outline_width(&style_btn, LV_STATE_FOCUSED, 1);
    lv_style_set_border_width(&style_btn, LV_STATE_DEFAULT, 2);
    lv_style_set_border_color(&style_btn, LV_STATE_DEFAULT, lv_color_hex(0xd6dde3));
    lv_style_set_border_color(&style_btn, LV_STATE_FOCUSED, LV_COLOR_RED);
    lv_style_set_radius(&style_btn, LV_STATE_DEFAULT, 7);

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
    lv_label_set_text(run_label_temperature_setpoint, "/ 45°C");
    lv_obj_align(run_label_temperature_setpoint, NULL, LV_ALIGN_IN_TOP_LEFT, 137, 8);

    run_led_temperature = lv_led_create(run_page, NULL),
    lv_obj_align(run_led_temperature, NULL, LV_ALIGN_IN_TOP_LEFT, 200, 8);
    lv_obj_set_size(run_led_temperature, 20, 18);

    lv_obj_set_style_local_shadow_spread(run_led_temperature, LV_LED_PART_MAIN, LV_STATE_DEFAULT, 5);
    lv_obj_set_style_local_shadow_opa(run_led_temperature, LV_LED_PART_MAIN, LV_STATE_DEFAULT, 127);

    /*---------------------------------Humidity-----------------------------------*/

    run_label_humidity_title = lv_label_create(run_page, NULL);
    lv_label_set_text(run_label_humidity_title, "Hum:   ");
    lv_obj_align(run_label_humidity_title, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 8+30);
    
    run_label_humidity_value = lv_label_create(run_page, NULL);
    lv_label_set_text(run_label_humidity_value, "12.7");
    lv_label_set_recolor(run_label_humidity_value, true);
    lv_obj_align(run_label_humidity_value, NULL, LV_ALIGN_IN_TOP_LEFT, 70, 1+30);
    lv_obj_add_style(run_label_humidity_value, LV_LABEL_PART_MAIN, &font_Large);
    lv_obj_set_style_local_text_color(run_label_humidity_value, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);

    run_label_humidity_setpoint = lv_label_create(run_page, NULL);
    lv_label_set_text(run_label_humidity_setpoint, "/ 10%");
    lv_obj_align(run_label_humidity_setpoint, NULL, LV_ALIGN_IN_TOP_LEFT, 137, 8+30);

    run_led_humidity = lv_led_create(run_page, NULL),
    lv_obj_align(run_led_humidity, NULL, LV_ALIGN_IN_TOP_LEFT, 200, 8+30);
    lv_obj_set_size(run_led_humidity, 20, 18);

    lv_obj_set_style_local_shadow_spread(run_led_humidity, LV_LED_PART_MAIN, LV_STATE_DEFAULT, 5);
    lv_obj_set_style_local_shadow_opa(run_led_humidity, LV_LED_PART_MAIN, LV_STATE_DEFAULT, 127);
    lv_obj_set_style_local_bg_color(run_led_humidity, LV_LED_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);
    lv_obj_set_style_local_shadow_color(run_led_humidity, LV_LED_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);

    /*---------------------------------button Back-----------------------------------*/
    run_btn_back = lv_btn_create (run_page, NULL);
    lv_obj_align(run_btn_back, NULL, LV_ALIGN_IN_TOP_LEFT, 7, 70);
    lv_obj_set_size(run_btn_back, dispWidth-(14+8+80), 22);
    lv_obj_set_event_cb(run_btn_back, setup_btn_back_cb);
    lv_obj_add_style(run_btn_back, LV_BTN_PART_MAIN, &style_btn);

    run_label_back = lv_label_create(run_btn_back, NULL);
    lv_obj_set_event_cb(run_btn_back, run_btn_back_cb);

    /*---------------------------------button materials-----------------------------------*/
    run_btn_materials = lv_btn_create (run_page, NULL);
    //lv_obj_align(run_btn_materials, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 75);
    lv_obj_align(run_btn_materials, NULL, LV_ALIGN_IN_TOP_LEFT, dispWidth-(14+8+80)+14, 70);
    lv_obj_set_size(run_btn_materials, 80, 22);
    lv_obj_set_event_cb(run_btn_materials, setup_btn_back_cb);
    lv_obj_add_style(run_btn_materials, LV_BTN_PART_MAIN, &style_btn);

    run_label_materials = lv_label_create(run_btn_materials, NULL);
    lv_label_set_text(run_label_materials, "Materials");
    lv_obj_set_event_cb(run_btn_materials, run_btn_materials_cb);

    /*---------------------------------material list-----------------------------------*/

    run_container_materialList = lv_cont_create(lv_scr_act(), NULL);
    lv_obj_align(run_container_materialList, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 5);
    lv_obj_set_size(run_container_materialList, dispWidth - 10, dispHeight - 10);
    lv_obj_set_hidden(run_container_materialList, true);

    run_label_materialList = lv_label_create(run_container_materialList, NULL);
    lv_obj_align(run_label_materialList, NULL, LV_ALIGN_IN_TOP_MID, 0, 8);
    lv_label_set_align(run_label_materialList, LV_LABEL_ALIGN_CENTER);
    lv_label_set_text(run_label_materialList, "Compatible Materials:");

    run_list_materialList = lv_table_create(run_container_materialList, NULL);
    lv_obj_align(run_label_materialList, run_label_materialList, LV_ALIGN_IN_TOP_LEFT, 10, 20);
    lv_table_set_col_width(run_list_materialList, 0, dispWidth - 20 - 30);
    lv_table_set_col_width(run_list_materialList, 1, 30);
    lv_table_set_cell_type(run_list_materialList, 0, 0, 1);
    lv_table_set_cell_type(run_list_materialList, 0, 1, 1);

    lv_table_set_cell_value(run_list_materialList, 0, 0, "Material");
    lv_table_set_cell_value(run_list_materialList, 0, 1, "Compatibility");

    lv_table_set_cell_value(run_list_materialList, 1, 0, "PLA");
    lv_table_set_cell_value(run_list_materialList, 1, 1, LV_SYMBOL_OK);



    xTaskCreate(toggleTask, "Toggle_task", 1024 * 4, NULL, 1, NULL);

    return run_page;
}

void run_setTab(void)
{
    lv_group_remove_all_objs(group);
    lv_group_add_obj(group, run_btn_back);
    lv_group_add_obj(group, run_btn_materials);

    char str1[40];
    char str2[40];
    strcpy(str1, "Stop: ");
    strcpy(str2, profilesByID[CUSTOM]->name);
    strcat(str1, str2);

    lv_label_set_text(run_label_back, str1);
}

void run_btn_back_cb(lv_obj_t * obj, lv_event_t event)
{
    switch (event)
    {
    case LV_EVENT_CLICKED:
        set_tab(TAB_SETUP);
        break;
    
    default:
        break;
    }
}

void run_btn_materials_cb(lv_obj_t * obj, lv_event_t event)
{
    switch (event)
    {
    case LV_EVENT_CLICKED:
        lv_obj_set_hidden(run_container_materialList, false);
        break;
    
    default:
        break;
    }
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

