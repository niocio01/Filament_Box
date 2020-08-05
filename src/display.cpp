
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "arduino.h"

/* Littlevgl specific */
#include <lvgl.h>
#include <TFT_eSPI.h>

#include "input.h"
#include "display.h"



TFT_eSPI tft = TFT_eSPI(); /* TFT instance */
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

lv_obj_t *test;
lv_obj_t *test2;
lv_obj_t *buttons;

lv_obj_t *tabs;


lv_obj_t *tabPage1;
lv_obj_t *tabPage2;
lv_obj_t *tabPage3;

lv_group_t *group1;
lv_group_t *group2;

void guiTask(void *pvParameter)
{

    while (1)
    {
        uint32_t waitTime = lv_task_handler(); /* let the GUI do its work */
        vTaskDelay(waitTime / portTICK_PERIOD_MS);
    }

    //A task should NEVER return
    vTaskDelete(NULL);
}

void toggle(void *pvParameter)
{

    while (1)
    {
        vTaskDelay(500/ portTICK_PERIOD_MS);
        lv_switch_toggle(test, LV_ANIM_ON);
        lv_switch_toggle(test2, LV_ANIM_ON);
    }
}

void display_init(void)
{
    lv_init();

    #if LV_USE_LOG != 0
    lv_log_register_print_cb(my_print); /* register print function for debugging */
    #endif

    tft.begin(); /* TFT init */
    tft.setRotation(1); /* Landscape orientation */

    ledcSetup(0, 4000, 8); // setup PWM for Backlight of Screen
    ledcAttachPin(4, 0);
    setScreenBrightness(DEFAULT_SCREEN_BRIGHTNESS);

    lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);

    /*Initialize the display*/
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = dispWidth;
    disp_drv.ver_res = dispHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);

    /*Initialize the (dummy) input device driver*/
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = read_encoder;
    lv_indev_t *encoder = lv_indev_drv_register(&indev_drv);
    initEncoder();


    tabs = lv_tabview_create (lv_scr_act(), NULL);
    tabPage1 = lv_tabview_add_tab(tabs, "Select");
    tabPage2 = lv_tabview_add_tab(tabs, "Setup");
    tabPage3 = lv_tabview_add_tab(tabs, "Run");

    lv_obj_set_style_local_bg_color(tabs, LV_TABVIEW_PART_BG, LV_TABVIEW_PART_TAB_BG, LV_COLOR_GREEN );

    
    //lv_obj_set_size(tabPage1, dispWidth, 80);
    // lv_obj_set_style_local_margin_bottom(tabPage1, LV_STATE_DEFAULT, 0 );
    lv_page_set_scrollbar_mode(tabPage1, LV_SCRLBAR_MODE_AUTO);

    /* Create simple label */
    lv_obj_t *label = lv_label_create(tabPage1, NULL);
    lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 5);
    lv_label_set_text(label, "No programm running  -  Select a preset or create a new one  -");    
    lv_label_set_long_mode(label, LV_LABEL_LONG_SROLL_CIRC);
    lv_label_set_anim_speed(label, 20);    
    lv_obj_set_width(label, dispWidth - 20);


    static const char* btnm_map[] ={ "PLA", "ABS", "PETG", "\n", "TPU-R", "PC", "+", "" };
    buttons = lv_btnmatrix_create(tabPage1, NULL);
    lv_btnmatrix_set_map(buttons, btnm_map);
    lv_obj_align(buttons, label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);

    lv_obj_set_size(buttons, dispWidth-20, 100);
    
    group1 = lv_group_create();

    //lv_group_add_obj(group1, tabPage1);
    lv_group_add_obj(group1, tabPage1);
    lv_group_add_obj(group1, buttons);
    lv_indev_set_group(encoder, group1);

    lv_group_set_focus_cb(group1, test_cb);
    lv_obj_set_event_cb(buttons, current_cb);

    //lv_page_focus(tabPage1, buttons, LV_ANIM_OFF);
    lv_group_focus_obj(buttons);
    lv_event_send(buttons, LV_EVENT_SHORT_CLICKED, NULL );
    //lv_btnmatrix_set_focused_btn(buttons, 0);


    

    xTaskCreate(guiTask, "GuiTask", 1025 * 4, NULL, 1, NULL);
    // xTaskCreate(toggle, "toggleTask", 1025 * 4, NULL, 1, NULL);
}


void setScreenBrightness(uint8_t brightness)
{
    ledcWrite(0, brightness);
}

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(lv_log_level_t level, const char * file, uint32_t line, const char * func, const char * dsc)
{

    Serial.printf("%s@%din%s->%s\r\n", file, line, func, dsc);
    Serial.flush();
}
#endif


/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors(&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}


/* Reading input device (simulated encoder here) */
bool read_encoder(lv_indev_drv_t * indev, lv_indev_data_t * data)
{
    data->enc_diff = readEncMovement();
    data->state = readEncButton();

    return false;
}




void test_cb(_lv_group_t * group)
{

    lv_obj_t *currentFocus = lv_group_get_focused(group1);

    lv_obj_set_event_cb(currentFocus, current_cb);   /*Assign an event callback*/
}

void current_cb(lv_obj_t * obj, lv_event_t event)
{

    switch (event) {
    case LV_EVENT_PRESSED:
        break;

    case LV_EVENT_SHORT_CLICKED:
        break;

    case LV_EVENT_CLICKED:
        {
            Serial.print(lv_btnmatrix_get_active_btn_text(buttons));
            Serial.println(" Clicked\n");

            lv_tabview_set_tab_act(tabs, 1, LV_ANIM_ON);

            //lv_obj_set_hidden(tabPage1, true);
            //lv_group_remove_obj(buttons);
        }
        break;

    case LV_EVENT_LONG_PRESSED:
        break;

    case LV_EVENT_LONG_PRESSED_REPEAT:
        break;

    case LV_EVENT_RELEASED:
        break;
    }
}

