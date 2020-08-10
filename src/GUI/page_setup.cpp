#include <Arduino.h>
#include <lvgl.h>

#include "GUI/display.h"
#include "GUI/nav.h"
#include "GUI/page_setup.h"
#include "GUI/page_select.h"
#include "GUI/profiles.h"
// #include "GUI/icons/temperature.c"

LV_IMG_DECLARE(temperature);

static lv_obj_t * page;

lv_obj_t * btn_back;
lv_obj_t * label_back;

lv_obj_t * btn_run;
lv_obj_t * label_run;

lv_obj_t * slider_temperature;
lv_obj_t * label_temperature;

lv_obj_t * slider_humidity;
lv_obj_t * label_humidity;

lv_obj_t * slider_time;
lv_obj_t * label_time;

lv_obj_t * switch_keepDryAfter;
lv_obj_t * label_keepDryAfter;

profile_t * currentProfile;
int last_time_slider_Value = 0;

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

    static lv_style_t style_btn;
    lv_style_init(&style_btn);
    lv_style_set_outline_width(&style_btn, LV_STATE_FOCUSED, 1);
    lv_style_set_border_width(&style_btn, LV_STATE_DEFAULT, 2);
    lv_style_set_border_color(&style_btn, LV_STATE_DEFAULT, lv_color_hex(0xd6dde3));
    lv_style_set_border_color(&style_btn, LV_STATE_FOCUSED, LV_COLOR_RED);
    lv_style_set_radius(&style_btn, LV_STATE_DEFAULT, 7);

    /* --------------------------------------------Page-----------------------------------------*/
    page = lv_tabview_add_tab(tabs, "Setup");
    lv_page_set_scrollbar_mode(page, LV_SCROLLBAR_MODE_OFF);

    /* --------------------------------------------Buttons-----------------------------------------*/
    btn_back = lv_btn_create(page, NULL);
    lv_obj_align(btn_back, page, LV_ALIGN_IN_TOP_LEFT, 8, 5);
    lv_obj_set_size(btn_back, dispWidth-(16+10+50), 20);
    lv_obj_set_event_cb(btn_back, btn_back_cb);
    lv_obj_add_style(btn_back, LV_BTN_PART_MAIN, &style_btn);

    label_back = lv_label_create(btn_back, NULL);
    lv_label_set_text(label_back, "Select:    ");
    // lv_label_set_long_mode(label_back, LV_LABEL_LONG_SROLL_CIRC);

    btn_run = lv_btn_create(page, NULL);
    lv_obj_align(btn_run, page, LV_ALIGN_IN_TOP_LEFT, dispWidth-(16+10+50)+18, 5);
    lv_obj_set_size(btn_run, 50, 20);
    lv_obj_set_event_cb(btn_run, btn_run_cb);
    lv_obj_add_style(btn_run, LV_BTN_PART_MAIN, &style_btn);

    label_run = lv_label_create(btn_run, NULL);
    lv_label_set_text(label_run, "RUN");

    /* --------------------------------------------Temperature-----------------------------------------*/
    slider_temperature = lv_slider_create(page, NULL);
    lv_obj_align(slider_temperature, btn_back, LV_ALIGN_OUT_BOTTOM_LEFT, 5, 12);
    lv_slider_set_type(slider_temperature, LV_SLIDER_TYPE_NORMAL);
    lv_obj_set_width(slider_temperature, 115);
    lv_obj_set_height(slider_temperature, 10);
    lv_slider_set_range(slider_temperature, 0, 13);
    lv_obj_add_style(slider_temperature, LV_SLIDER_PART_KNOB, &style_slider_knob);
    lv_obj_add_style(slider_temperature, LV_SLIDER_PART_BG, &style_slider);

    label_temperature = lv_label_create(page, NULL);
    lv_obj_align(label_temperature, slider_temperature, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

    lv_obj_set_event_cb(slider_temperature, slider_temp_cb);
    lv_event_send(slider_temperature, LV_EVENT_VALUE_CHANGED, NULL);

    /* --------------------------------------------humidity----------------------------------------------*/
    slider_humidity = lv_slider_create(page, NULL);
    lv_obj_align(slider_humidity, slider_temperature, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 12);
    lv_slider_set_type(slider_humidity, LV_SLIDER_TYPE_NORMAL);
    lv_obj_set_width(slider_humidity, 115);
    lv_obj_set_height(slider_humidity, 10);
    lv_slider_set_range(slider_humidity, 0, 20);
    lv_obj_add_style(slider_humidity, LV_SLIDER_PART_KNOB, &style_slider_knob);
    lv_obj_add_style(slider_humidity, LV_SLIDER_PART_BG, &style_slider);

    label_humidity = lv_label_create(page, NULL);
    lv_obj_align(label_humidity, slider_humidity, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

    lv_obj_set_event_cb(slider_humidity, slider_hum_cb);
    lv_event_send(slider_humidity, LV_EVENT_VALUE_CHANGED, NULL);

    /* --------------------------------------------Time----------------------------------------------*/
    slider_time = lv_slider_create(page, NULL);
    lv_obj_align(slider_time, slider_humidity, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 12);
    lv_slider_set_type(slider_time, LV_SLIDER_TYPE_NORMAL);
    lv_obj_set_width(slider_time, 80);
    lv_obj_set_height(slider_time, 10);
    lv_slider_set_range(slider_time, 0, MAX_TIME+2);
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
    lv_group_add_obj(group, btn_back);
    lv_group_add_obj(group, btn_run);
    lv_group_add_obj(group, slider_temperature);
    lv_group_add_obj(group, slider_humidity);
    lv_group_add_obj(group, slider_time);
    lv_group_focus_obj(btn_run);
    lv_group_set_editing(group, false);
    lv_group_set_wrap(group, false);

    currentProfile = get_profile(get_selected_profile_id());

    char str1[40];
    char str2[40];
    strcpy(str1, "Selected: ");
    strcpy(str2, currentProfile->name);
    strcat(str1, str2);

    lv_label_set_text(label_back, str1);
    lv_slider_set_value(slider_temperature, (currentProfile->temperature-15)/5, LV_ANIM_ON);
    lv_slider_set_value(slider_humidity, currentProfile->humidity, LV_ANIM_ON);

    int8_t minutes = (currentProfile->time.minutes/15)*15;
    int8_t hours = currentProfile->time.hours;
    int8_t days = currentProfile->time.days;

    int16_t time = minutes + hours*60 + days*24*60;
    last_time_slider_Value = time;
    lv_slider_set_value(slider_time, time, LV_ANIM_ON);

    lv_event_send(slider_temperature, LV_EVENT_VALUE_CHANGED, NULL);
    lv_event_send(slider_humidity, LV_EVENT_VALUE_CHANGED, NULL);
    lv_event_send(slider_time, LV_EVENT_VALUE_CHANGED, NULL);
}

void btn_back_cb(lv_obj_t * obj, lv_event_t event)
{
    switch (event)
    {
    case LV_EVENT_CLICKED:
        set_tab(TAB_SELECT);
        break;
    
    default:
        break;
    }
}

void btn_run_cb(lv_obj_t * obj, lv_event_t event)
{
    switch (event)
    {
    case LV_EVENT_CLICKED:
        set_tab(TAB_RUN);
        break;
    
    default:
        break;
    }
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
            sprintf(str, "Hum:  ON");
        }
        else if (value == 20)
        {
            sprintf(str, "Hum:  OFF");
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
        
        int16_t value = lv_slider_get_value(slider_time);
        char str[20] = {0} ;

        // set weather to increase or decrease the time
        int8_t direction = 0; // if value == last_time_slider_Value direction is 0 (stay)
        direction = value - last_time_slider_Value;

        if (last_time_slider_Value > MAX_TIME && direction > 0)
        {
            value = 15;
        }
        else if (last_time_slider_Value > MAX_TIME && direction < 0)
        {
            value = MAX_TIME;
        }
        else if (last_time_slider_Value == MAX_TIME && direction > 0)
        {
            value = MAX_TIME+1;
        }
        else if ( (last_time_slider_Value <= 15 && direction < 0) || value < 15)
        {
            value = MAX_TIME+1;
        }

        else if ( (last_time_slider_Value >= 1*24*60 && direction > 0) || (last_time_slider_Value > 1*24*60 && direction < 0) ) // after 1d
        {
            value = last_time_slider_Value+(direction*60);
        }
        else if( (last_time_slider_Value >= 4*60 && direction > 0 ) || (last_time_slider_Value > 4*60 && direction < 0 ) ) //after 4h
        {
            value = last_time_slider_Value+(direction*30);
        }
        else
        {
            value = last_time_slider_Value+(direction*15);
        }

        lv_slider_set_value(slider_time, value, LV_ANIM_ON);
        last_time_slider_Value = value;
        
        uint8_t minutes = value%60;
        uint8_t hours = (value/60)%24;
        uint8_t days = value/(24*60);

        if (value > MAX_TIME)
        {
            sprintf(str, "Time: unlimited");
        }
        else if (value < 24*60)
        {
            sprintf(str, "Time: %02dh %02dmin", hours, minutes);
        }        
        else
        {
            sprintf(str, "Time: %dd %02dh", days, hours);
        }
        lv_label_set_text(label_time, str);

        Serial.println(value);
    }
    break;

    default:
    break;
    }
}