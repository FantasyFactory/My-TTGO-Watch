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

#include "my_basic_app.h"
#include "my_basic_app_main.h"

#include "gui/mainbar/app_tile/app_tile.h"
#include "gui/mainbar/main_tile/main_tile.h"
#include "gui/mainbar/mainbar.h"
#include "gui/statusbar.h"

#include "bas_arduino.h"
#include "file_module.h"
#include "hw_bindings.h"
#include "lvgl_bindings.h"
#include "various_module.h"

lv_obj_t *my_basic_app_main_tile = NULL;
lv_style_t my_basic_app_main_style;
lv_task_t * _my_basic_app_task;

LV_IMG_DECLARE(exit_32px);
LV_IMG_DECLARE(setup_32px);
LV_IMG_DECLARE(refresh_32px);
LV_FONT_DECLARE(Ubuntu_16px);

static void exit_my_basic_app_main_event_cb( lv_obj_t * obj, lv_event_t event );
static void enter_my_basic_app_setup_event_cb( lv_obj_t * obj, lv_event_t event );
static void refresh_output_event_cb( lv_obj_t * obj, lv_event_t event );
void my_basic_app_task( lv_task_t * task );

 //#define UseOutputLabel 

static lv_obj_t *my_basic_cont;
static lv_style_t my_basic_cont_main_style;
#ifdef UseOutputLabel
static lv_obj_t *my_basic_output_label;
static lv_style_t my_basic_output_style;
#endif 
struct mb_interpreter_t* bas = NULL;
char * buffer = NULL;
#define MyBasicThreads 4
const char *BasFileName = "/spiffs/myfile.bas";
#define dbg(x) Serial.println(x)

void my_basic_app_main_setup( uint32_t tile_num ) {

    my_basic_app_main_tile = mainbar_get_tile_obj( tile_num );
    lv_style_copy( &my_basic_app_main_style, mainbar_get_style() );

    lv_obj_t * exit_btn = lv_imgbtn_create( my_basic_app_main_tile, NULL);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_RELEASED, &exit_32px);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_PRESSED, &exit_32px);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_CHECKED_RELEASED, &exit_32px);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_CHECKED_PRESSED, &exit_32px);
    lv_obj_add_style(exit_btn, LV_IMGBTN_PART_MAIN, &my_basic_app_main_style );
    lv_obj_align(exit_btn, my_basic_app_main_tile, LV_ALIGN_IN_BOTTOM_LEFT, 10, -10 );
    lv_obj_set_event_cb( exit_btn, exit_my_basic_app_main_event_cb );

    lv_obj_t * setup_btn = lv_imgbtn_create( my_basic_app_main_tile, NULL);
    lv_imgbtn_set_src(setup_btn, LV_BTN_STATE_RELEASED, &setup_32px);
    lv_imgbtn_set_src(setup_btn, LV_BTN_STATE_PRESSED, &setup_32px);
    lv_imgbtn_set_src(setup_btn, LV_BTN_STATE_CHECKED_RELEASED, &setup_32px);
    lv_imgbtn_set_src(setup_btn, LV_BTN_STATE_CHECKED_PRESSED, &setup_32px);
    lv_obj_add_style(setup_btn, LV_IMGBTN_PART_MAIN, &my_basic_app_main_style );
    lv_obj_align(setup_btn, my_basic_app_main_tile, LV_ALIGN_IN_BOTTOM_RIGHT, -10, -10 );
    lv_obj_set_event_cb( setup_btn, enter_my_basic_app_setup_event_cb );


    /************ my_basic_cont main container (for lvgl integration) *********/
 /*   lv_obj_t *my_basic_cont = lv_obj_create( my_basic_app_main_tile, NULL );
    lv_obj_set_size( my_basic_cont, lv_disp_get_hor_res( NULL ) , 200);
    lv_style_copy( &my_basic_cont_main_style, mainbar_get_style() );
    lv_style_set_bg_color( &my_basic_cont_main_style, LV_OBJ_PART_MAIN, LV_COLOR_BLUE);
    lv_obj_add_style( my_basic_cont, LV_OBJ_PART_MAIN, &my_basic_cont_main_style  );
    lv_obj_align( my_basic_cont, my_basic_app_main_tile, LV_ALIGN_IN_TOP_MID, 0, 10 );
*/
    /************ my_basic_output_label label for  "PRINT" redirection *********/
#ifdef UseOutputLabel    
    lv_style_copy( &my_basic_output_style, &my_basic_cont_main_style );
    lv_style_set_text_font( &my_basic_output_style, LV_STATE_DEFAULT, &Ubuntu_16px);
    lv_style_set_text_color(&my_basic_output_style, LV_STATE_DEFAULT, LV_COLOR_BLUE);
    my_basic_output_label = lv_label_create(my_basic_cont, NULL);
    lv_obj_add_style( my_basic_output_label, LV_OBJ_PART_MAIN, &my_basic_output_style );
    //lv_label_set_long_mode(my_basic_output_label, LV_LABEL_LONG_SROLL);            /*Automatically scroll long lines*/
    lv_obj_set_width(my_basic_output_label, lv_page_get_width_fit(my_basic_cont));          /*Set the label width to max value to not show hor. scroll bars*/
    lv_label_set_text(my_basic_output_label, "");
#endif


    lv_obj_t * reload_btn = lv_imgbtn_create( my_basic_app_main_tile, NULL);
    lv_imgbtn_set_src(reload_btn, LV_BTN_STATE_RELEASED, &refresh_32px);
    lv_imgbtn_set_src(reload_btn, LV_BTN_STATE_PRESSED, &refresh_32px);
    lv_imgbtn_set_src(reload_btn, LV_BTN_STATE_CHECKED_RELEASED, &refresh_32px);
    lv_imgbtn_set_src(reload_btn, LV_BTN_STATE_CHECKED_PRESSED, &refresh_32px);
    lv_obj_add_style(reload_btn, LV_IMGBTN_PART_MAIN, &my_basic_app_main_style );
    lv_obj_align(reload_btn, my_basic_app_main_tile, LV_ALIGN_IN_BOTTOM_MID, 0 , -10 );
    lv_obj_set_event_cb( reload_btn, refresh_output_event_cb );



    InitBasic();


    // create an task that runs every secound
    _my_basic_app_task = lv_task_create( my_basic_app_task, 1000, LV_TASK_PRIO_MID, NULL );
}

static void enter_my_basic_app_setup_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       statusbar_hide( true );
                                        mainbar_jump_to_tilenumber( my_basic_app_get_app_setup_tile_num(), LV_ANIM_ON );
                                        break;
    }
}

static void exit_my_basic_app_main_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       CloseBasic();
                                        mainbar_jump_to_maintile( LV_ANIM_OFF );
                                        break;
    }
}

static void refresh_output_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       DoBasic();
                                        break;
    }
}

void my_basic_app_task( lv_task_t * task ) {
    // put your code her
}




bool InitBasic ( void ) {


    FILE * pFile;
    long lSize;

    size_t result;
log_i("Loading %s\r\n", BasFileName);
    pFile = fopen ( BasFileName, "r" );
    if (pFile==NULL) {Serial.printf ("File error"); return false;}

    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);

    // allocate memory to contain the whole file:
    buffer = (char*) malloc (sizeof(char)*lSize+1);
    if (buffer == NULL) {Serial.printf ("Memory error"); return false;}

    // copy the file into the buffer:
    result = fread (buffer,1,lSize,pFile);
    buffer[lSize]=0;
    if (result != lSize) {Serial.printf ("Reading error"); return false;}

    // terminate
    fclose (pFile);
log_i("Loaded %d bytes of code\r\n", lSize);

    log_i("Free heap: %d\r\n", ESP.getFreeHeap());
    log_i("Free PSRAM: %d\r\n", ESP.getFreePsram());
    log_i("My Basic RUN\n");

#ifdef piripillo
    MyBasic.begin(MyBasicThreads);
    MyBasic.loadProgram(buffer, BasFileName);
    MyBasic.setLv(BasFileName, my_basic_app_main_tile, &my_basic_app_main_style);

#else
	mb_init();
	mb_open(&bas);
    enableArduinoBindings(bas);
#ifdef UseOutputLabel
    enableLVGLprint(bas, my_basic_output_label);
#else
    enableSerialPrint(bas);
#endif
  enableLVGL(bas, my_basic_app_main_tile, &my_basic_app_main_style);
  enableFileModule(bas);
  enableVariousModule(bas);
  mb_load_string(bas, buffer, true);
#endif

  return true;
}

bool DoBasic( void ) {
#ifdef piripillo
  MyBasic.runLoaded(BasFileName);  
#else
	mb_run(bas, true);
#endif

}

void CloseBasic (void) {
#ifdef piripillo
  MyBasic.closeProgram(BasFileName);
#else
  mb_close(&bas);
  mb_dispose();
#endif
  
  log_i("My Basic END\r\n");

  log_i("Free heap: %d\r\n", ESP.getFreeHeap());
  log_i("Free PSRAM: %d\r\n", ESP.getFreePsram());
  free (buffer);
}
