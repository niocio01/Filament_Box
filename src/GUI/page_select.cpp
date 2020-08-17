#include <Arduino.h>
#include <lvgl.h>

#include "GUI/display.h"
#include "GUI/nav.h"
#include "GUI/profiles.h"
#include "GUI/page_select.h"
#include "GUI/page_setup.h"
#include "GUI/page_run.h"

lv_obj_t *select_Page;
lv_obj_t *select_btn_mtrx;
uint8_t lastButton = 0;

const uint8_t btnm_id_map[] = {PLA, PETG, ABS, TPU, 0,  PC, PP, PVA, NYLON, 0, DESSICANT, KEEP_DRY};
uint8_t btnm_id_map_stripped[(sizeof(btnm_id_map)/sizeof(btnm_id_map[0]))];

lv_obj_t* select_page_init(lv_obj_t * tabs)
{
    select_Page = lv_tabview_add_tab(tabs, "Select");
    lv_page_set_scrollbar_mode(select_Page, LV_SCROLLBAR_MODE_OFF);

    static const char* linebreak = {"\n"};
    static const char* end = {""};
    static const char* btnm_map[(sizeof(btnm_id_map)/sizeof(btnm_id_map[0]))+1];

    uint8_t cnt = 0;

    for (int i = 0; i < (sizeof(btnm_id_map)/sizeof(btnm_id_map[0])); i++)
    {
        if (btnm_id_map[i] == 0)
        {
            btnm_map[i] = linebreak;
        }
        else
        {
            btnm_map[i] = profiles_getProfile(btnm_id_map[i])->name;
            btnm_id_map_stripped[cnt] = btnm_id_map[i];
            cnt++;
        }
    }
    btnm_map[sizeof(btnm_id_map)/sizeof(btnm_id_map[0])] = end;

    select_btn_mtrx = lv_btnmatrix_create(select_Page, NULL);
    lv_btnmatrix_set_map(select_btn_mtrx, btnm_map);
    lv_obj_align(select_btn_mtrx, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 4);
    lv_obj_set_size(select_btn_mtrx, dispWidth-10, 93);

    static lv_style_t style_btn;
    lv_style_init(&style_btn);
    lv_style_set_radius(&style_btn, LV_STATE_DEFAULT, 10);

    lv_style_set_border_width(&style_btn, LV_STATE_DEFAULT, 2);

    lv_style_set_pad_inner(&style_btn, LV_STATE_DEFAULT, 5);
    lv_style_set_pad_top(&style_btn, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_bottom(&style_btn, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_left(&style_btn, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_right(&style_btn, LV_STATE_DEFAULT, 0);
    lv_style_set_radius(&style_btn, LV_STATE_DEFAULT, 10);

    // lv_obj_reset_style_list(select_btn_mtrx, LV_BTNMATRIX_PART_BTN);         /*Remove the styles coming from the theme*/
    lv_obj_add_style(select_btn_mtrx, LV_BTN_PART_MAIN, &style_btn);
    lv_obj_add_style(select_btn_mtrx, LV_BTNMATRIX_PART_BTN, &style_btn);
    lv_obj_set_style_local_border_opa(select_btn_mtrx, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_bg_opa(select_btn_mtrx, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);

    lv_obj_set_event_cb(select_btn_mtrx, select_btn_mtrx_cb);

    return select_Page;
}

void select_setTab(void)
{

    lv_group_remove_all_objs(group);
    lv_group_add_obj(group, select_btn_mtrx);
    lv_group_focus_obj(select_btn_mtrx);
    lv_group_set_editing(group, true);
    lv_btnmatrix_set_focused_btn(select_btn_mtrx, lastButton);
}

void select_btn_mtrx_cb(lv_obj_t * obj, lv_event_t event)
{
    switch (event) {

    case LV_EVENT_SHORT_CLICKED:
        {
            uint16_t button =  lv_btnmatrix_get_active_btn(select_btn_mtrx);
            lastButton = button;

            profiles_setCurrentProfile_byPreset(btnm_id_map_stripped[button]);

            set_tab(TAB_SETUP);
        }
        break;
    
    default:
        break;
    }
}

uint8_t select_get_selected_profile_id(void)
{
    return selectedProfileId;
}
