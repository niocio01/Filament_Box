#include <Arduino.h>
#include <lvgl.h>

#include "temp.h"
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

lv_obj_t *run_label_time_title;
lv_obj_t *run_label_time_value;

lv_obj_t *run_btn_back;
lv_obj_t *run_label_back;

lv_obj_t *run_btn_pause;
lv_obj_t *run_label_pause;

lv_obj_t *run_btn_materials;
lv_obj_t *run_label_materials;

lv_obj_t *run_container_materialList;
lv_obj_t *run_page_materialList;
lv_obj_t *run_label_materialList;
lv_obj_t *run_line_materialList;
lv_obj_t *run_table_materialList;

uint8_t run_remainingTime_days = 0;
uint8_t run_remainingTime_hours = 0;
uint8_t run_remainingTime_minutes = 0;
uint8_t run_remainingTime_seconds = 0;

bool run_paused = true;

lv_obj_t* run_page_init(lv_obj_t *tabs)
{

    /*------------------------------------Tabs--------------------------------------*/
    run_page = lv_tabview_add_tab(tabs, "Run");
    
    lv_page_set_scrollbar_mode(run_page, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_local_pad_top(run_page, LV_PAGE_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_pad_bottom(run_page, LV_PAGE_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_pad_left(run_page, LV_PAGE_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_pad_right(run_page, LV_PAGE_PART_BG, LV_STATE_DEFAULT, 0);

    lv_obj_set_style_local_pad_top(run_page, LV_PAGE_PART_SCROLLABLE, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_pad_bottom(run_page, LV_PAGE_PART_SCROLLABLE, LV_STATE_DEFAULT, 7);
    lv_obj_set_style_local_pad_left(run_page, LV_PAGE_PART_SCROLLABLE, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_pad_right(run_page, LV_PAGE_PART_SCROLLABLE, LV_STATE_DEFAULT, 0);

    lv_obj_set_style_local_pad_right(run_page, LV_PAGE_PART_SCROLLBAR, LV_STATE_DEFAULT, 0);

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
    lv_obj_align(run_led_temperature, NULL, LV_ALIGN_IN_TOP_LEFT, 197, 8);
    lv_obj_set_size(run_led_temperature, 20, 18);

    lv_obj_set_style_local_shadow_spread(run_led_temperature, LV_LED_PART_MAIN, LV_STATE_DEFAULT, 4);
    lv_obj_set_style_local_shadow_opa(run_led_temperature, LV_LED_PART_MAIN, LV_STATE_DEFAULT, 127);

    /*---------------------------------Humidity-----------------------------------*/

    run_label_humidity_title = lv_label_create(run_page, NULL);
    lv_label_set_text(run_label_humidity_title, "Hum:   ");
    lv_obj_align(run_label_humidity_title, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 8+33);
    
    run_label_humidity_value = lv_label_create(run_page, NULL);
    lv_label_set_text(run_label_humidity_value, "12.7");
    lv_label_set_recolor(run_label_humidity_value, true);
    lv_obj_align(run_label_humidity_value, NULL, LV_ALIGN_IN_TOP_LEFT, 70, 1+33);
    lv_obj_add_style(run_label_humidity_value, LV_LABEL_PART_MAIN, &font_Large);
    lv_obj_set_style_local_text_color(run_label_humidity_value, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);

    run_label_humidity_setpoint = lv_label_create(run_page, NULL);
    lv_label_set_text(run_label_humidity_setpoint, "/ 10%");
    lv_obj_align(run_label_humidity_setpoint, NULL, LV_ALIGN_IN_TOP_LEFT, 137, 8+33);

    run_led_humidity = lv_led_create(run_page, NULL),
    lv_obj_align(run_led_humidity, NULL, LV_ALIGN_IN_TOP_LEFT, 197, 8+33);
    lv_obj_set_size(run_led_humidity, 20, 18);

    lv_obj_set_style_local_shadow_spread(run_led_humidity, LV_LED_PART_MAIN, LV_STATE_DEFAULT, 4);
    lv_obj_set_style_local_shadow_opa(run_led_humidity, LV_LED_PART_MAIN, LV_STATE_DEFAULT, 127);
    lv_obj_set_style_local_bg_color(run_led_humidity, LV_LED_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);
    lv_obj_set_style_local_shadow_color(run_led_humidity, LV_LED_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);

    /*---------------------------------Time-----------------------------------*/

    run_label_time_title = lv_label_create(run_page, NULL);
    lv_label_set_text(run_label_time_title, "Time:");
    lv_obj_align(run_label_time_title, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 8+33+33);
    
    run_label_time_value = lv_label_create(run_page, NULL);
    lv_label_set_text(run_label_time_value, "0:00:00:00");
    lv_label_set_recolor(run_label_time_value, true);
    lv_obj_align(run_label_time_value, NULL, LV_ALIGN_IN_TOP_LEFT, 70, 1+33+33);
    lv_obj_add_style(run_label_time_value, LV_LABEL_PART_MAIN, &font_Large);
    lv_obj_set_style_local_text_color(run_label_time_value, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);

    /*---------------------------------button Back-----------------------------------*/
    run_btn_back = lv_btn_create (run_page, NULL);
    lv_obj_align(run_btn_back, NULL, LV_ALIGN_IN_TOP_LEFT, 8, 105);
    lv_obj_set_size(run_btn_back, dispWidth-(16+8+70), 25);
    lv_obj_set_event_cb(run_btn_back, setup_btn_back_cb);
    lv_obj_add_style(run_btn_back, LV_BTN_PART_MAIN, &style_btn);

    run_label_back = lv_label_create(run_btn_back, NULL);
    lv_obj_set_event_cb(run_btn_back, run_btn_back_cb);

    /*---------------------------------button pause-----------------------------------*/
    run_btn_pause = lv_btn_create (run_page, NULL);
    //lv_obj_align(run_btn_materials, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 75);
    lv_obj_align(run_btn_pause, NULL, LV_ALIGN_IN_TOP_LEFT, dispWidth-(16+8+70)+16, 105);
    lv_obj_set_size(run_btn_pause, 70, 25);
    lv_obj_set_event_cb(run_btn_pause, run_btn_pause_cb);
    lv_obj_add_style(run_btn_pause, LV_BTN_PART_MAIN, &style_btn);
    lv_btn_set_checkable(run_btn_pause, true);

    run_label_pause = lv_label_create(run_btn_pause, NULL);
    lv_label_set_text(run_label_pause, "Pause");
    lv_obj_set_event_cb(run_label_pause, run_btn_materials_cb);

    /*---------------------------------button materials-----------------------------------*/
    run_btn_materials = lv_btn_create (run_page, NULL);
    //lv_obj_align(run_btn_materials, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 75);
    lv_obj_align(run_btn_materials, NULL, LV_ALIGN_IN_TOP_LEFT, 8, 105+32);
    lv_obj_set_size(run_btn_materials, dispWidth - 16, 25);
    lv_obj_set_event_cb(run_btn_materials, setup_btn_back_cb);
    lv_obj_add_style(run_btn_materials, LV_BTN_PART_MAIN, &style_btn);

    run_label_materials = lv_label_create(run_btn_materials, NULL);
    lv_label_set_text(run_label_materials, "show compatibility list");
    lv_obj_set_event_cb(run_btn_materials, run_btn_materials_cb);

    /*---------------------------------material list-----------------------------------*/

    run_container_materialList = lv_cont_create(lv_scr_act(), NULL);
    lv_obj_align(run_container_materialList, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 5);
    lv_obj_set_size(run_container_materialList, dispWidth - 10, dispHeight - 10);
    lv_obj_set_hidden(run_container_materialList, true);

    run_label_materialList = lv_label_create(run_container_materialList, NULL);
    lv_label_set_text(run_label_materialList, "Compatible Materials:");
    lv_obj_align(run_label_materialList, run_container_materialList, LV_ALIGN_IN_TOP_MID, 0, 8);
    lv_label_set_align(run_label_materialList, LV_LABEL_ALIGN_CENTER);


    run_line_materialList = lv_line_create(run_container_materialList, NULL);
    static lv_point_t line_points[] = { {0,0} , {dispWidth - (10+20), 0} };

    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, LV_STATE_DEFAULT, 3);
    lv_style_set_line_color(&style_line, LV_STATE_DEFAULT, lv_color_hex(0xD4D7D9));
    lv_style_set_line_rounded(&style_line, LV_STATE_DEFAULT, true);

    lv_line_set_points(run_line_materialList, line_points, 2); 
    lv_obj_add_style(run_line_materialList, LV_LINE_PART_MAIN, &style_line);
    lv_obj_align(run_line_materialList, run_container_materialList, LV_ALIGN_IN_TOP_LEFT, 10, 30);


    run_page_materialList = lv_page_create(run_container_materialList, NULL);
    lv_obj_set_size(run_page_materialList, dispWidth - 10, dispHeight - (10 + 33 + 5) );
    lv_obj_align(run_page_materialList, run_container_materialList, LV_ALIGN_IN_TOP_LEFT, 0, 30);
    lv_obj_set_event_cb(run_page_materialList, run_page_materialList_cb);
    lv_page_set_anim_time(run_page_materialList, 120);

    lv_obj_set_style_local_pad_top(run_page_materialList, LV_PAGE_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_pad_bottom(run_page_materialList, LV_PAGE_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_pad_left(run_page_materialList, LV_PAGE_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_pad_right(run_page_materialList, LV_PAGE_PART_BG, LV_STATE_DEFAULT, 0);

    lv_obj_set_style_local_pad_top(run_page_materialList, LV_PAGE_PART_SCROLLABLE, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_pad_bottom(run_page_materialList, LV_PAGE_PART_SCROLLABLE, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_pad_left(run_page_materialList, LV_PAGE_PART_SCROLLABLE, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_pad_right(run_page_materialList, LV_PAGE_PART_SCROLLABLE, LV_STATE_DEFAULT, 0);

    lv_obj_set_style_local_pad_right(run_page_materialList, LV_PAGE_PART_SCROLLBAR, LV_STATE_DEFAULT, 8);

    lv_obj_set_style_local_border_width(run_page_materialList, LV_PAGE_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_width(run_page_materialList, LV_PAGE_PART_SCROLLABLE, LV_STATE_DEFAULT, 0);


    run_table_materialList = lv_table_create(run_page_materialList, NULL);
    lv_table_set_col_cnt(run_table_materialList, 3);
    lv_table_set_row_cnt(run_table_materialList, noOfProfiles);
    lv_obj_align(run_table_materialList, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

    lv_obj_set_style_local_pad_top(run_table_materialList, LV_TABLE_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_pad_bottom(run_table_materialList, LV_TABLE_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_pad_left(run_table_materialList, LV_TABLE_PART_BG, LV_STATE_DEFAULT, 8);
    lv_obj_set_style_local_pad_right(run_table_materialList, LV_TABLE_PART_BG, LV_STATE_DEFAULT, 8);
    lv_obj_set_style_local_border_width(run_table_materialList, LV_TABLE_PART_BG, LV_STATE_DEFAULT, 0);

    static lv_style_t style_basic_table;
    lv_style_init(&style_basic_table);

    lv_style_set_pad_top(&style_basic_table, LV_STATE_DEFAULT, 7);
    lv_style_set_pad_bottom(&style_basic_table, LV_STATE_DEFAULT, 7);
    lv_style_set_pad_left(&style_basic_table, LV_STATE_DEFAULT, 8);
    lv_style_set_pad_right(&style_basic_table, LV_STATE_DEFAULT, 8);

    lv_obj_add_style(run_table_materialList, LV_TABLE_PART_CELL1, &style_basic_table);
    lv_obj_add_style(run_table_materialList, LV_TABLE_PART_CELL2, &style_basic_table); // green Text
    lv_obj_add_style(run_table_materialList, LV_TABLE_PART_CELL3, &style_basic_table); // Red Text

    lv_obj_set_style_local_text_color(run_table_materialList, LV_TABLE_PART_CELL2, LV_STATE_DEFAULT, LV_COLOR_GREEN);
    lv_obj_set_style_local_text_color(run_table_materialList, LV_TABLE_PART_CELL3, LV_STATE_DEFAULT, LV_COLOR_RED);


    for (int row = 0; row < noOfProfiles; row++)
    {
        lv_table_set_cell_align(run_table_materialList, row, 0, LV_LABEL_ALIGN_LEFT);
        lv_table_set_cell_align(run_table_materialList, row, 1, LV_LABEL_ALIGN_RIGHT);
        lv_table_set_cell_align(run_table_materialList, row, 2, LV_LABEL_ALIGN_CENTER);
    }
    
    lv_table_set_col_width(run_table_materialList, 0, dispWidth - (10+16+17 + 60 + 25));
    lv_table_set_col_width(run_table_materialList, 1, 60);
    lv_table_set_col_width(run_table_materialList, 2, 25);

    lv_obj_move_foreground(run_line_materialList);

    xTaskCreate(run_UpdateValues_Task, "Run_TimeTask", 1024 * 4, NULL, 5, NULL);

    return run_page;
}

void run_setTab(void)
{
    lv_group_remove_all_objs(group);
    lv_group_set_focus_cb(group, run_group_cb);
    lv_group_add_obj(group, run_label_temperature_title);
    lv_group_add_obj(group, run_btn_back);
    lv_group_add_obj(group, run_btn_pause);
    lv_group_add_obj(group, run_btn_materials);

    lv_label_set_text_fmt(run_label_temperature_setpoint, "/ %d°C", profilesByID[CUSTOM]->temperature);
    lv_label_set_text_fmt(run_label_humidity_setpoint, "/ %d%%", profilesByID[CUSTOM]->humidity);
    lv_label_set_text_fmt(run_label_time_value, "%d:%02d:%02d:00", profilesByID[CUSTOM]->time.days, profilesByID[CUSTOM]->time.hours, profilesByID[CUSTOM]->time.minutes);

    run_remainingTime_days = profilesByID[CUSTOM]->time.days;
    run_remainingTime_hours = profilesByID[CUSTOM]->time.hours;
    run_remainingTime_minutes = profilesByID[CUSTOM]->time.minutes;
    run_remainingTime_seconds = 0;

    run_paused = false;


    bool profilesAreTheSame = profiles_Compare_CurrentToPreset();

    char str[40];
    strcpy(str, "Stop: ");

    profilesAreTheSame ? strcat(str, profilesByID[profilesByID[CUSTOM]->id]->name) : strcat(str, profilesByID[CUSTOM]->name) ;

    lv_label_set_text(run_label_back, str);


    for (int row = 0; row < noOfProfiles; row++)
    {
        lv_table_set_cell_value(run_table_materialList, row, 0, profilesByTemperature[row]->name);

        char str[40];
        sprintf(str, "%d°C", profilesByTemperature[row]->temperature);
        lv_table_set_cell_value(run_table_materialList, row, 1, str);

        if (profilesByID[CUSTOM]->temperature >= profilesByTemperature[row]->temperature )
        {
            lv_table_set_cell_value(run_table_materialList, row, 2, LV_SYMBOL_OK );
            lv_table_set_cell_type(run_table_materialList, row, 2, 2); // Green
        }
        else
        {
            lv_table_set_cell_value(run_table_materialList, row, 2, LV_SYMBOL_CLOSE);
            lv_table_set_cell_type(run_table_materialList, row, 2, 3); // Red
        }
    }
    lv_page_scroll_ver(run_page_materialList, 500); // go to top
}

void run_group_cb (lv_group_t * group)
{
    lv_obj_t * focusedObj = lv_group_get_focused(group);
     if(focusedObj == run_btn_back)
     {
        lv_page_scroll_ver(run_page, -100);
     }
     else if (focusedObj == run_label_temperature_title)
     {
        lv_page_scroll_ver(run_page, 100);
     }
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

void run_btn_pause_cb(lv_obj_t * obj, lv_event_t event)
{
    switch (event)
    {
    case LV_EVENT_CLICKED:
        {
            lv_btn_state_t state = lv_btn_get_state(run_btn_pause);

            if (state == LV_BTN_STATE_CHECKED_RELEASED)
            {
                run_paused = true;
                lv_label_set_text(run_label_pause, "Resume");
            }
            else if (state == LV_BTN_STATE_RELEASED)
            {
                run_paused = false;
                lv_label_set_text(run_label_pause, "Pause");
            }
        }
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
        lv_group_remove_all_objs(group);
        lv_group_add_obj(group, run_page_materialList);
        lv_group_set_editing(group, true);
        break;
    
    default:
        break;
    }
}

void run_page_materialList_cb(lv_obj_t * obj, lv_event_t event)
{
    switch (event)
    {
    case LV_EVENT_CLICKED:
        lv_obj_set_hidden(run_container_materialList, true);

        lv_group_remove_all_objs(group);
        lv_group_add_obj(group, run_label_temperature_title);
        lv_group_add_obj(group, run_btn_back);
        lv_group_add_obj(group, run_btn_pause);
        lv_group_add_obj(group, run_btn_materials);
        lv_group_focus_obj(run_btn_materials);
        lv_page_scroll_ver(run_page, -100);
        break;
    
    default:
        break;
    }
}

void run_UpdateValues_Task(void *pvParameter)
{

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount ();

    while (1)
    {
        if (run_paused == false)
        {
            if (run_remainingTime_seconds > 0)
            {
                run_remainingTime_seconds--;
            }
            else
            {
                if (run_remainingTime_minutes > 0)
                {
                    run_remainingTime_seconds = 59;
                    run_remainingTime_minutes--;
                }
                else
                {
                    if (run_remainingTime_hours > 0)
                    {
                        run_remainingTime_minutes = 59;
                        run_remainingTime_seconds = 59;
                        run_remainingTime_hours--;
                    }
                    else
                    {
                        if (run_remainingTime_days > 0)
                        {
                            run_remainingTime_hours = 23;
                            run_remainingTime_minutes = 59;
                            run_remainingTime_seconds = 59;
                            run_remainingTime_days--;
                        }
                        else
                        {
                            Serial.println("time Over.");
                        }
                    }
                }
            }
        lv_label_set_text_fmt(run_label_time_value, "%d:%02d:%02d:%02d", run_remainingTime_days, run_remainingTime_hours, run_remainingTime_minutes, run_remainingTime_seconds);
        }

        char str[10];

        sprintf(str, "%.1f", getTemperature(AHT10_1));
        lv_label_set_text(run_label_temperature_value, str);

        sprintf(str, "%.1f", getHumitity());
        lv_label_set_text(run_label_humidity_value, str);

        vTaskDelayUntil( &xLastWakeTime, 1000 / portTICK_PERIOD_MS);

    }
}


