/****************************************************************************
 *   Aug 3 12:17:11 2020
 *   Copyright  2020  Dirk Brosswick
 *   Email: dirk.brosswick@googlemail.com
 ****************************************************************************/
 
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#include "config.h"
#include <TTGO.h>

#include "image_ticker.h"
#include "image_ticker_main.h"
#include "image_ticker_fetch.h"

#include "gui/mainbar/app_tile/app_tile.h"
#include "gui/mainbar/main_tile/main_tile.h"
#include "gui/mainbar/mainbar.h"
#include "gui/statusbar.h"

#include "hardware/wifictl.h"


lv_obj_t *image_ticker_main_tile = NULL;
lv_style_t image_ticker_main_style;

lv_task_t * _image_ticker_task;

LV_IMG_DECLARE(exit_32px);
LV_IMG_DECLARE(setup_32px);
LV_IMG_DECLARE(refresh_32px);
LV_FONT_DECLARE(Ubuntu_72px);

static void exit_image_ticker_main_event_cb( lv_obj_t * obj, lv_event_t event );
static void enter_image_ticker_setup_event_cb( lv_obj_t * obj, lv_event_t event );
void image_ticker_task( lv_task_t * task );


void image_ticker_main_setup( uint32_t tile_num ) {

    image_ticker_main_tile = mainbar_get_tile_obj( tile_num );
    lv_style_copy( &image_ticker_main_style, mainbar_get_style() );

    /*Create an image obejct and use the converter png file*/
    log_i("lv_img_create..");
    lv_obj_t * img_obj = lv_img_create(image_ticker_main_tile, NULL);
    //lv_style_copy( &image_ticker_main_style, mainbar_get_style() );
    lv_img_set_src(img_obj, "/imgtick.png");
    lv_obj_align(img_obj, image_ticker_main_tile, LV_ALIGN_IN_TOP_MID, 0, 0 );
    log_i("lv_img_created");

    lv_obj_t * exit_btn = lv_imgbtn_create( image_ticker_main_tile, NULL);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_RELEASED, &exit_32px);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_PRESSED, &exit_32px);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_CHECKED_RELEASED, &exit_32px);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_CHECKED_PRESSED, &exit_32px);
    lv_obj_add_style(exit_btn, LV_IMGBTN_PART_MAIN, &image_ticker_main_style );
    lv_obj_align(exit_btn, image_ticker_main_tile, LV_ALIGN_IN_BOTTOM_LEFT, 10, -10 );
    lv_obj_set_event_cb( exit_btn, exit_image_ticker_main_event_cb );

    lv_obj_t * setup_btn = lv_imgbtn_create( image_ticker_main_tile, NULL);
    lv_imgbtn_set_src(setup_btn, LV_BTN_STATE_RELEASED, &setup_32px);
    lv_imgbtn_set_src(setup_btn, LV_BTN_STATE_PRESSED, &setup_32px);
    lv_imgbtn_set_src(setup_btn, LV_BTN_STATE_CHECKED_RELEASED, &setup_32px);
    lv_imgbtn_set_src(setup_btn, LV_BTN_STATE_CHECKED_PRESSED, &setup_32px);
    lv_obj_add_style(setup_btn, LV_IMGBTN_PART_MAIN, &image_ticker_main_style );
    lv_obj_align(setup_btn, image_ticker_main_tile, LV_ALIGN_IN_BOTTOM_RIGHT, -10, -10 );
    lv_obj_set_event_cb( setup_btn, enter_image_ticker_setup_event_cb );

    wifictl_register_cb( WIFICTL_CONNECT_IP | WIFICTL_OFF_REQUEST | WIFICTL_OFF | WIFICTL_DISCONNECT , image_ticker_wifictl_event_cb, "imageticker" );

    // create an task that runs every secound
    //_image_ticker_task = lv_task_create( image_ticker_task, 10000, LV_TASK_PRIO_MID, NULL );
}

static void enter_image_ticker_setup_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       statusbar_hide( true );
                                        mainbar_jump_to_tilenumber( image_ticker_get_app_setup_tile_num(), LV_ANIM_ON );
                                        break;
    }
}

static void exit_image_ticker_main_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       mainbar_jump_to_maintile( LV_ANIM_OFF );
                                        break;
    }
}

void image_ticker_task( lv_task_t * task ) {
     //image_ticker_fetch_jpg();
}

bool image_ticker_wifictl_event_cb( EventBits_t event, void *arg ) {
    image_ticker_config_t *image_ticker_config = image_ticker_get_config();
    switch( event ) {
        case WIFICTL_CONNECT_IP:    if (image_ticker_config->autosync ) {
                                        image_ticker_fetch_jpg();
                                        }
                                    break;
        case WIFICTL_OFF_REQUEST:
                                    break;
        case WIFICTL_OFF:
                                    break;
        case WIFICTL_DISCONNECT: 
                                    break;
    }
    return( true );
}


