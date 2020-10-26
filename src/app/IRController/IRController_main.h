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
#ifndef _IRCONTROLLER_MAIN_H
    #define _IRCONTROLLER_MAIN_H

    #include <TTGO.h>

    void IRController_main_setup( uint32_t tile_num );

#endif // _EXAMPLE_APP_MAIN_H

#define IR_Sony
//#define IR_Samsung

#ifdef IR_Samsung

// Mute    
const uint16_t S_mute[68]={4650,4350,650,1550,650,1550,700,1550,700,400,700,400,700,400,700,450,650,450,650,1550,700,1500,700,1550,700,400,700,450,650,400,700,450,700,400,700,1500,700,1550,650,1550,700,1500,700,450,700,400,700,400,700,400,700,400,700,450,650,450,700,400,700,1500,700,1550,650,1550,700,1500,700};
#define IR_mute irsend.sendRaw(S_mute, 68, 38);

// Power
const uint16_t S_pwr[68]={4600,4350,700,1550,650,1550,650,1600,650,450,650,450,650,450,650,450,700,400,700,1550,650,1550,650,1600,650,450,650,450,650,450,700,450,650,450,650,450,650,1550,700,450,650,450,650,450,650,450,650,450,700,400,650,1600,650,450,650,1550,650,1600,650,1550,650,1550,700,1550,650,1550,650};
#define IR_pwr irsend.sendRaw(S_pwr, 68, 38);

// Source
const uint16_t S_scr[68]={4600,4350,700,1550,650,1550,700,1500,700,450,650,450,700,400,700,400,700,400,700,1550,700,1500,700,1550,700,400,700,400,700,400,700,400,700,400,700,1550,700,400,700,450,650,450,650,450,700,400,700,400,700,400,700,450,650,1550,700,1500,700,1550,650,1550,700,1500,700,1550,700,1500,700};
#define IR_scr irsend.sendRaw(S_scr, 68, 38);

// channel up
const uint16_t S_pup[68]={4600,4350,700,1500,700,1500,700,1550,700,450,650,400,700,450,650,450,700,400,700,1500,700,1550,650,1550,700,450,650,450,700,400,700,400,700,400,700,400,700,1550,700,400,700,400,700,1550,650,450,700,400,700,400,700,1550,650,450,650,1600,650,1550,650,450,700,1500,700,1500,700,1550,650};
  
// channel down
const uint16_t S_pdown[68]={4650,4300,700,1550,700,1500,700,1550,700,400,700,400,700,400,700,450,650,450,650,1550,700,1500,700,1550,700,400,700,400,700,400,700,450,700,400,700,400,700,400,700,450,650,450,650,1550,700,400,700,450,650,400,700,1550,700,1500,700,1550,700,1500,700,400,700,1550,650,1550,700,1500,700};
  
// volume up
const uint16_t S_vup[68]={4600,4350,650,1550,700,1500,700,1550,700,400,700,400,700,450,650,450,700,400,700,1500,700,1550,650,1550,700,400,700,400,700,450,650,450,700,400,700,1500,700,1550,650,1550,700,400,700,450,700,400,700,400,700,400,700,450,650,450,650,450,650,1550,700,1500,700,1550,700,1500,700,1550,650};
#define IR_vup irsend.sendRaw(S_vup, 68, 38);

// volume down
const uint16_t S_vdown[68]={4600,4350,700,1550,650,1550,700,1500,700,450,650,450,700,400,700,400,700,400,700,1550,700,1500,700,1550,700,400,700,400,700,400,700,450,650,450,650,1550,700,1500,700,450,650,1550,700,400,700,400,700,450,700,400,700,400,700,400,700,1550,700,400,700,1500,700,1500,700,1550,700,1500,700};
#define IR_vdown irsend.sendRaw(S_vdown, 68, 38);

// TV/DTV
const uint16_t S_tv[68]={4600,4350,650,1550,700,1500,700,1550,700,400,700,400,700,400,700,450,700,400,700,1500,700,1500,700,1550,700,400,700,400,700,450,650,450,700,400,700,1500,700,1550,700,400,700,400,700,400,700,400,700,1550,700,400,700,400,700,400,700,1550,700,1500,700,1550,650,1550,700,400,700,1500,700};
  
// guide
const uint16_t S_guide[68]={4600,4350,700,1500,700,1550,700,1500,700,450,650,450,700,400,700,400,700,400,700,1550,650,1550,700,1500,700,450,650,450,700,400,700,400,700,400,700,1550,700,1500,700,1550,650,1550,700,400,700,400,700,1550,700,400,700,400,700,400,700,450,700,400,650,1550,700,1550,650,450,700,1500,700};
 
// exit
const uint16_t S_exit[68]={4650,4300,700,1550,650,1550,700,1550,700,400,700,400,700,450,650,450,650,450,650,1550,700,1500,700,1550,700,450,650,400,700,450,650,450,700,400,700,1500,700,400,700,1550,700,1500,700,400,700,1550,700,450,650,400,700,450,650,1550,700,400,700,400,700,1550,650,450,650,1550,700,1500,700};
#define IR_exit irsend.sendRaw(S_exit, 68, 38);

#endif

#ifdef IR_Sony
#define IR_pwr irsend.sendSony(0x00005422, 15);
#define IR_mute irsend.sendSony(0x00004E26, 15);  //PAUSE instead of MUTE
#define IR_scr irsend.sendSony(0x00004422, 15); //MODE instead of SOURCE
#define IR_vup irsend.sendSony(0x00006422, 15);
#define IR_vdown irsend.sendSony(0x00006422, 15);
#define IR_exit irsend.sendSony(0x00002626, 15); // PLAY instead of EXIT
/*
irsend.sendSony(0x00002122, 15); // AnalysIR IR Protocol: SONY, Key:  CD
irsend.sendSony(0x00004122, 15); // AnalysIR IR Protocol: SONY, Key:  Radio
irsend.sendSony(0x00000122, 15); // AnalysIR IR Protocol: SONY, Key:  Tape
irsend.sendSony(0x00001626, 15); // AnalysIR IR Protocol: SONY, Key:  Loop
irsend.sendSony(0x00003222, 15); // AnalysIR IR Protocol: SONY, Key:  Tone L
irsend.sendSony(0x00005222, 15); // AnalysIR IR Protocol: SONY, Key:  Tone H
irsend.sendSony(0x00007E22, 15); // AnalysIR IR Protocol: SONY, Key:  MegaBass
irsend.sendSony(0x00004422, 15); // AnalysIR IR Protocol: SONY, Key:  Mode
irsend.sendSony(0x00005826, 15); // AnalysIR IR Protocol: SONY, Key:  >10
irsend.sendSony(0x00001826, 15); // AnalysIR IR Protocol: SONY, Key:  0/10
irsend.sendSony(0x00000826, 15); // AnalysIR IR Protocol: SONY, Key:  9
irsend.sendSony(0x00007026, 15); // AnalysIR IR Protocol: SONY, Key:  8
irsend.sendSony(0x00003026, 15); // AnalysIR IR Protocol: SONY, Key:  7
irsend.sendSony(0x00005026, 15); // AnalysIR IR Protocol: SONY, Key:  6
irsend.sendSony(0x00001026, 15); // AnalysIR IR Protocol: SONY, Key:  5
irsend.sendSony(0x00006026, 15); // AnalysIR IR Protocol: SONY, Key:  4
irsend.sendSony(0x00002026, 15); // AnalysIR IR Protocol: SONY, Key:  3
irsend.sendSony(0x00004026, 15); // AnalysIR IR Protocol: SONY, Key:  2
irsend.sendSony(0x00000026, 15); // AnalysIR IR Protocol: SONY, Key:  1
irsend.sendSony(0x00004626, 15); // AnalysIR IR Protocol: SONY, Key:  FWD
irsend.sendSony(0x00000626, 15); // AnalysIR IR Protocol: SONY, Key:  REW
irsend.sendSony(0x00000E26, 15); // AnalysIR IR Protocol: SONY, Key:  Stop
irsend.sendSony(0x00004E26, 15); // AnalysIR IR Protocol: SONY, Key:  Pause
irsend.sendSony(0x00002626, 15); // AnalysIR IR Protocol: SONY, Key:  Play
irsend.sendSony(0x00006422, 15); // AnalysIR IR Protocol: SONY, Key:  VOL-
irsend.sendSony(0x00002422, 15); // AnalysIR IR Protocol: SONY, Key:  VOL+
irsend.sendSony(0x00005422, 15); // AnalysIR IR Protocol: SONY, Key:  ON/OFF
*/
#endif