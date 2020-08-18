#include <Arduino.h>
#include <lvgl.h>

#include "GUI/display.h"
#include "GUI/nav.h"
#include "GUI/profiles.h"
#include "GUI/page_select.h"
#include "GUI/page_setup.h"
#include "GUI/page_run.h"


LV_IMG_DECLARE(temperature);

lv_obj_t * setup_page;

lv_obj_t * setup_btn_back;
lv_obj_t * setup_label_back;

lv_obj_t * setup_btn_run;
lv_obj_t * setup_label_run;

lv_obj_t * setup_slider_temperature;
lv_obj_t * setup_label_temperature;

lv_obj_t * setup_slider_humidity;
lv_obj_t * setup_label_humidity;

lv_obj_t * setup_slider_time;
lv_obj_t * setup_label_time;

int last_time_slider_Value = 0;

lv_obj_t* setup_page_init(lv_obj_t * tabs)
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
    setup_page = lv_tabview_add_tab(tabs, "Setup");
    lv_page_set_scrollbar_mode(setup_page, LV_SCROLLBAR_MODE_OFF);

    /* --------------------------------------------Buttons-----------------------------------------*/
    setup_btn_back = lv_btn_create(setup_page, NULL);
    lv_obj_align(setup_btn_back, setup_page, LV_ALIGN_IN_TOP_LEFT, 8, 5);
    lv_obj_set_size(setup_btn_back, dispWidth-(16+10+50), 25);
    lv_obj_set_event_cb(setup_btn_back, setup_btn_back_cb);
    lv_obj_add_style(setup_btn_back, LV_BTN_PART_MAIN, &style_btn);

    setup_label_back = lv_label_create(setup_btn_back, NULL);
    lv_label_set_text(setup_label_back, "Select:    ");
    // lv_label_set_long_mode(setup_label_back, LV_LABEL_LONG_SROLL_CIRC);

    setup_btn_run = lv_btn_create(setup_page, NULL);
    lv_obj_align(setup_btn_run, setup_page, LV_ALIGN_IN_TOP_LEFT, dispWidth-(16+10+50)+18, 5);
    lv_obj_set_size(setup_btn_run, 50, 25);
    lv_obj_set_event_cb(setup_btn_run, setup_btn_run_cb);
    lv_obj_add_style(setup_btn_run, LV_BTN_PART_MAIN, &style_btn);

    setup_label_run = lv_label_create(setup_btn_run, NULL);
    lv_label_set_text(setup_label_run, "RUN");

    /* --------------------------------------------Temperature-----------------------------------------*/
    setup_slider_temperature = lv_slider_create(setup_page, NULL);
    lv_obj_align(setup_slider_temperature, setup_btn_back, LV_ALIGN_OUT_BOTTOM_LEFT, 5, 9);
    lv_slider_set_type(setup_slider_temperature, LV_SLIDER_TYPE_NORMAL);
    lv_obj_set_width(setup_slider_temperature, 115);
    lv_obj_set_height(setup_slider_temperature, 10);
    lv_slider_set_range(setup_slider_temperature, 0, 13);
    lv_obj_add_style(setup_slider_temperature, LV_SLIDER_PART_KNOB, &style_slider_knob);
    lv_obj_add_style(setup_slider_temperature, LV_SLIDER_PART_BG, &style_slider);

    setup_label_temperature = lv_label_create(setup_page, NULL);
    lv_obj_align(setup_label_temperature, setup_slider_temperature, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

    lv_obj_set_event_cb(setup_slider_temperature, setup_slider_temperature_cb);
    lv_event_send(setup_slider_temperature, LV_EVENT_VALUE_CHANGED, NULL);

    /* --------------------------------------------humidity----------------------------------------------*/
    setup_slider_humidity = lv_slider_create(setup_page, NULL);
    lv_obj_align(setup_slider_humidity, setup_slider_temperature, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 12);
    lv_slider_set_type(setup_slider_humidity, LV_SLIDER_TYPE_NORMAL);
    lv_obj_set_width(setup_slider_humidity, 115);
    lv_obj_set_height(setup_slider_humidity, 10);
    lv_slider_set_range(setup_slider_humidity, 0, 20);
    lv_obj_add_style(setup_slider_humidity, LV_SLIDER_PART_KNOB, &style_slider_knob);
    lv_obj_add_style(setup_slider_humidity, LV_SLIDER_PART_BG, &style_slider);

    setup_label_humidity = lv_label_create(setup_page, NULL);
    lv_obj_align(setup_label_humidity, setup_slider_humidity, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

    lv_obj_set_event_cb(setup_slider_humidity, setup_slider_humidity_cb);
    lv_event_send(setup_slider_humidity, LV_EVENT_VALUE_CHANGED, NULL);

    /* --------------------------------------------Time----------------------------------------------*/
    setup_slider_time = lv_slider_create(setup_page, NULL);
    lv_obj_align(setup_slider_time, setup_slider_humidity, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 12);
    lv_slider_set_type(setup_slider_time, LV_SLIDER_TYPE_NORMAL);
    lv_obj_set_width(setup_slider_time, 80);
    lv_obj_set_height(setup_slider_time, 10);
    lv_slider_set_range(setup_slider_time, 0, MAX_TIME+2);
    lv_obj_add_style(setup_slider_time, LV_SLIDER_PART_KNOB, &style_slider_knob);
    lv_obj_add_style(setup_slider_time, LV_SLIDER_PART_BG, &style_slider);

    setup_label_time = lv_label_create(setup_page, NULL);
    lv_obj_align(setup_label_time, setup_slider_time, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

    lv_obj_set_event_cb(setup_slider_time, setup_slider_time_cb);
    lv_event_send(setup_slider_time, LV_EVENT_VALUE_CHANGED, NULL);

    return setup_page;
}

void setup_setTab(void)
{
    lv_group_remove_all_objs(group);
    lv_group_add_obj(group, setup_btn_back);
    lv_group_add_obj(group, setup_btn_run);
    lv_group_add_obj(group, setup_slider_temperature);
    lv_group_add_obj(group, setup_slider_humidity);
    lv_group_add_obj(group, setup_slider_time);
    lv_group_focus_obj(setup_btn_run);
    lv_group_set_editing(group, false);
    lv_group_set_wrap(group, false);

    char str1[40];
    char str2[40];
    strcpy(str1, "Selected: ");
    strcpy(str2, profilesByID[profilesByID[CUSTOM]->id]->name);
    strcat(str1, str2);

    lv_label_set_text(setup_label_back, str1);
    lv_slider_set_value(setup_slider_temperature, (profilesByID[CUSTOM]->temperature-15)/5, LV_ANIM_ON);
    lv_slider_set_value(setup_slider_humidity, profilesByID[CUSTOM]->humidity, LV_ANIM_ON);

    int8_t minutes = (profilesByID[CUSTOM]->time.minutes/15)*15;
    int8_t hours = profilesByID[CUSTOM]->time.hours;
    int8_t days = profilesByID[CUSTOM]->time.days;

    int16_t time = minutes + hours*60 + days*24*60;
    last_time_slider_Value = time;
    lv_slider_set_value(setup_slider_time, time, LV_ANIM_ON);

    lv_event_send(setup_slider_temperature, LV_EVENT_VALUE_CHANGED, NULL);
    lv_event_send(setup_slider_humidity, LV_EVENT_VALUE_CHANGED, NULL);
    lv_event_send(setup_slider_time, LV_EVENT_VALUE_CHANGED, NULL);
}

void setup_updateProfileName(void)
{
    static bool changedTagSet = false;

    bool profilesAreTheSame = profiles_Compare_CurrentToPreset();

    if( !profilesAreTheSame && !changedTagSet) // if not the same and tag not set
    {
        changedTagSet = true;

        char str1[40];
        char str2[40];

        strcpy(str1, "Selected: ");
        strcpy(str2, profilesByID[CUSTOM]->name);
        
        strcat(str1, str2);

        lv_label_set_text(setup_label_back, str1);
        
    }

    else if(profilesAreTheSame && changedTagSet) // if profiles are the same and the tag is set
    {
        changedTagSet = false;

        char str1[40];
        char str2[40];

        strcpy(str1, "Selected: ");
        strcpy(str2, profilesByID[profilesByID[CUSTOM]->id]->name);
        strcat(str1, str2);

        lv_label_set_text(setup_label_back, str1);
    }
}

void setup_btn_back_cb(lv_obj_t * obj, lv_event_t event)
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

void setup_btn_run_cb(lv_obj_t * obj, lv_event_t event)
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

void setup_slider_temperature_cb(lv_obj_t * obj, lv_event_t event)
{
    switch (event)
    {
    case LV_EVENT_VALUE_CHANGED:
    {
        char str[20] = {0} ;
        uint8_t value = lv_slider_get_value(setup_slider_temperature);
        uint8_t temperature = 0;

        value == 0 ? temperature = 0 : temperature = value*5+15;

        if (value == 0)
        {
            sprintf(str, "Temp: OFF");
        }
        else
        {
            sprintf(str, "Temp: %02d°C", temperature);
        }

        profilesByID[CUSTOM]->temperature = temperature;
        setup_updateProfileName();
        lv_label_set_text(setup_label_temperature, str);
    }
    break;

    default:
    break;
    }
}

void setup_slider_humidity_cb(lv_obj_t * obj, lv_event_t event)
{
    switch (event)
    {
    case LV_EVENT_VALUE_CHANGED:
    {
        char str[20] = {0} ;
        uint8_t value = lv_slider_get_value(setup_slider_humidity);
        uint8_t humidity = 0;

        value == 0 ? humidity = 0 : humidity = value*5;

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
            sprintf(str, "Hum:  %02d%%", humidity);
        }

        profilesByID[CUSTOM]->humidity = humidity;
        setup_updateProfileName();
        lv_label_set_text(setup_label_humidity, str);
    }
    break;

    default:
    break;
    }
}

void setup_slider_time_cb(lv_obj_t * obj, lv_event_t event)
{
    switch (event)
    {
    case LV_EVENT_VALUE_CHANGED:
    {
        
        int16_t value = lv_slider_get_value(setup_slider_time);
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

        lv_slider_set_value(setup_slider_time, value, LV_ANIM_ON);
        last_time_slider_Value = value;
        
        uint8_t minutes = value%60;
        uint8_t hours = (value/60)%24;
        uint8_t days = value/(24*60);

        profilesByID[CUSTOM]->time.minutes = minutes;
        profilesByID[CUSTOM]->time.hours = hours;
        profilesByID[CUSTOM]->time.days = days;
        setup_updateProfileName();

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
        lv_label_set_text(setup_label_time, str);
    }
    break;

    default:
    break;
    }
}