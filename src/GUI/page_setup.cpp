#include <Arduino.h>
#include <lvgl.h>

#include "GUI/nav.h"
#include "GUI/page_setup.h"
// #include "GUI/icons/temperature.c"

LV_IMG_DECLARE(temperature);

static lv_obj_t * page;

lv_obj_t * slider_temperature;
lv_obj_t * label_temperature;

lv_obj_t * slider_humidity;
lv_obj_t * label_humidity;

lv_obj_t * slider_time;
lv_obj_t * label_time;

lv_obj_t * switch_keepDryAfter;
lv_obj_t * label_keepDryAfter;


lv_obj_t* page_setup_init(lv_obj_t * tabs)
{
    /* --------------------------------------------Style-----------------------------------------*/
    static lv_style_t style_slider_knob;
    lv_style_init(&style_slider_knob);
    lv_style_set_bg_opa(&style_slider_knob, LV_STATE_DEFAULT, 0);

    static lv_style_t style_slider;
    lv_style_init(&style_slider);
    lv_style_set_outline_width(&style_slider, LV_STATE_DEFAULT, 2);
    lv_style_set_outline_width(&style_slider, LV_STATE_FOCUSED, 3);
    lv_style_set_outline_width(&style_slider, LV_STATE_EDITED, 3);
    lv_style_set_outline_opa(&style_slider, LV_STATE_DEFAULT, 255);
    lv_style_set_outline_color(&style_slider, LV_STATE_DEFAULT, LV_COLOR_GRAY);
    lv_style_set_outline_color(&style_slider, LV_STATE_FOCUSED, LV_COLOR_RED);
    lv_style_set_outline_color(&style_slider, LV_STATE_EDITED, LV_COLOR_BLUE);

    /* --------------------------------------------Page-----------------------------------------*/
    page = lv_tabview_add_tab(tabs, "Setup");
    lv_page_set_scrollbar_mode(page, LV_SCROLLBAR_MODE_OFF);

    /* --------------------------------------------Temperature-----------------------------------------*/
    slider_temperature = lv_slider_create(page, NULL);
    lv_obj_align(slider_temperature, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 12);
    lv_slider_set_type(slider_temperature, LV_SLIDER_TYPE_NORMAL);
    lv_obj_set_width(slider_temperature, 115);
    lv_obj_set_height(slider_temperature, 8);
    lv_slider_set_range(slider_temperature, 0, 13);
    lv_obj_add_style(slider_temperature, LV_SLIDER_PART_KNOB, &style_slider_knob);
    lv_obj_add_style(slider_temperature, LV_SLIDER_PART_BG, &style_slider);

    label_temperature = lv_label_create(page, NULL);    
    //lv_label_set_text(label_temperature, "Temp:  00°C");
    lv_obj_align(label_temperature, slider_temperature, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

    lv_obj_set_event_cb(slider_temperature, slider_temp_cb);
    lv_event_send(slider_temperature, LV_EVENT_VALUE_CHANGED, NULL);

    /* --------------------------------------------humidity----------------------------------------------*/
    slider_humidity = lv_slider_create(page, NULL);
    lv_obj_align(slider_humidity, slider_temperature, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_slider_set_type(slider_humidity, LV_SLIDER_TYPE_NORMAL);
    lv_obj_set_width(slider_humidity, 115);
    lv_obj_set_height(slider_humidity, 8);
    lv_slider_set_range(slider_humidity, 0, 20);
    lv_obj_add_style(slider_humidity, LV_SLIDER_PART_KNOB, &style_slider_knob);
    lv_obj_add_style(slider_humidity, LV_SLIDER_PART_BG, &style_slider);

    label_humidity = lv_label_create(page, NULL);
    lv_obj_align(label_humidity, slider_humidity, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

    lv_obj_set_event_cb(slider_humidity, slider_hum_cb);
    lv_event_send(slider_humidity, LV_EVENT_VALUE_CHANGED, NULL);

    /* --------------------------------------------Time----------------------------------------------*/
    slider_time = lv_slider_create(page, NULL);
    lv_obj_align(slider_time, slider_humidity, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_slider_set_type(slider_time, LV_SLIDER_TYPE_NORMAL);
    lv_obj_set_width(slider_time, 80);
    lv_obj_set_height(slider_time, 8);
    lv_slider_set_range(slider_time, 0, MAX_TIME);
    lv_obj_add_style(slider_time, LV_SLIDER_PART_KNOB, &style_slider_knob);
    lv_obj_add_style(slider_time, LV_SLIDER_PART_BG, &style_slider);

    label_time = lv_label_create(page, NULL);
    lv_obj_align(label_time, slider_time, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

    lv_obj_set_event_cb(slider_time, slider_time_cb);
    lv_event_send(slider_time, LV_EVENT_VALUE_CHANGED, NULL);

    return page;
}

void setTab_setup(lv_group_t * group)
{
    lv_group_remove_all_objs(group);
    lv_group_add_obj(group, slider_temperature);
    lv_group_add_obj(group, slider_humidity);
    lv_group_add_obj(group, slider_time);
    lv_group_focus_obj(slider_temperature);
    lv_group_set_editing(group, false);
}

void slider_temp_cb(lv_obj_t * obj, lv_event_t event)
{
    switch (event)
    {
    case LV_EVENT_VALUE_CHANGED:
    {
        char str[20] = {0} ;
        uint8_t value = lv_slider_get_value(slider_temperature);
        uint8_t temperature = 0;

        if (value == 0)
        {
            sprintf(str, "Temp: OFF");
        }
        else
        {
            value == 0 ? temperature = 0 : temperature = value*5+15;
            sprintf(str, "Temp: %02d°C", temperature);
        }
        lv_label_set_text(label_temperature, str);
    }
    break;

    default:
    break;
    }
}

void slider_hum_cb(lv_obj_t * obj, lv_event_t event)
{
    switch (event)
    {
    case LV_EVENT_VALUE_CHANGED:
    {
        char str[20] = {0} ;
        uint8_t value = lv_slider_get_value(slider_humidity);
        uint8_t humidity = 0;

        if (value == 0)
        {
            sprintf(str, "Hum: ON");
        }
        else if (value == 20)
        {
            sprintf(str, "Hum: OFF");
        }
        else
        {
            value == 0 ? humidity = 0 : humidity = value*5;
            sprintf(str, "Hum:  %02d%%", humidity);
        }
        lv_label_set_text(label_humidity, str);
    }
    break;

    default:
    break;
    }
}

void slider_time_cb(lv_obj_t * obj, lv_event_t event)
{
    switch (event)
    {
    case LV_EVENT_VALUE_CHANGED:
    {
        uint8_t minutes = 0, hours = 0, days = 0;
        uint8_t value = lv_slider_get_value(slider_time);
        char str[20] = {0} ;
        if (value == 0 || value == MAX_TIME)
        {
            sprintf(str, "Time: unlimited");
        }
        else
        {
            if(value < 4*4) // go in 15 min steps
            {
                minutes = (value % 4)*15;
                hours = (value%(24*4))/4;
            }
            else if (value < 4*4+20*2) //after 4h go in 30min steps
            {
                minutes = (value % 2)*30;
                hours = (((value-4*4)%(24*2))/2)+4;
            }
            else // after 24h go in 1h steps
            {
                hours = (value-(4*4+20*2))%24;
                days = ((value-(4*4+20*2))/24)+1;
            }
            sprintf(str, "Time: %d:%02d:%02d D", days, hours, minutes);
        }
        lv_label_set_text(label_time, str);
    }
    break;

    default:
    break;
    }
}