#include <yaul.h>

#include <assert.h>
#include <stdlib.h>

#include "peripeteies.h"

#include "svin.h"
#include "cartridge.h"
#include <smpc/peripheral.h>

#define MENU_ENTRY_COUNT 16

extern vdp1_cmdt_list_t *_svin_cmdt_list;
extern smpc_peripheral_port_t * smpc_peripheral_raw_port(uint8_t port);

//static smpc_peripheral_keyboard_t * _keyboard;
static smpc_peripheral_port_t* _raw_port1;
static smpc_peripheral_port_t* _raw_port2;
static smpc_peripheral_t* _raw_per1;
static smpc_peripheral_t* _raw_per2;

//extern uint8_t root_romdisk[];

extern _key_area key_areas[KEY_AREAS_NUMBER];

//void *_romdisk = NULL;

void
mark_tile(int x, int y)
{
        int * _pointer32 = (int *)_SVIN_NBG1_PNDR_START;
        int i;
        if (x<64)
                i = y*64+x;
        else
                i = (y+64)*64+(x-64);
        _pointer32[i] = 0x10200000 + _SVIN_NBG1_CHPNDR_SPECIALS_INDEX+2*2; //palette 2, transparency on
}

int
main(void)
{
        char tmp[256];

        MEMORY_WRITE(32, SCU(ASR0), 0x23301FF0);

        //_svin_filelist_fill(); //move this to init probably

        _svin_init();
        _svin_textbox_disable();

        //MEMORY_WRITE(32, SCU(ASR0), 0x23301FF0);

        //load logo
        _svin_filelist_fill();
        _svin_background_set("BOOTLOGO.BG");
        _svin_delay(1000);
        cartridge_memory_test();
        while(1);
        _svin_background_fade_to_black();
        
        //_svin_background_set("jap_kbd.bg");  
        //_svin_background_romdisk_set("bus_stop.bg");  
        //_svin_textbox_print("  ","Waiting for keypress...","Lato_Black12",7,7);
        //_svin_textbox_print("  ","Waiting for keypress...","NotoSans_Black16",7,7);
        _svin_textbox_print("  ","Detecting devices...","Lato_Black12",7,7);

        //writing pattern names for nbg0
        //starting with plane 0
        int * _pointer32 = (int *)_SVIN_NBG1_PNDR_START;
        /*for (unsigned int i = 0; i < _SVIN_NBG1_PNDR_SIZE / 11; i++)
        {
                _pointer32[i] = 0x10200000 + _SVIN_NBG1_CHPNDR_SPECIALS_INDEX+2*2; //palette 2, transparency on
        }*/

        //setting up "semi-transparent green" character for nbg1
        _pointer32 = (int *)(_SVIN_NBG1_CHPNDR_SPECIALS_ADDR + _SVIN_CHARACTER_BYTES*2);
        for (unsigned int i = 0; i < _SVIN_CHARACTER_BYTES / sizeof(int); i++)
        {
                _pointer32[i] = 0x7F7F7F7F;
        }
                _raw_port1 = smpc_peripheral_raw_port(1);
                _raw_port2 = smpc_peripheral_raw_port(2);
                _raw_per1 = _raw_port1->peripheral;
                _raw_per2 = _raw_port2->peripheral;
                
        while(1)
        {
                smpc_peripheral_process();

                //smpc_peripheral_port_1;

                /*if (_raw_per1->type == 0x34)
                        _keyb = _raw_per1;
                else if (_raw_per1->type == 34)
                        _keyb = _raw_per2;
                else
                        _keyb = NULL;*/

                /*sprintf(tmp,"PORT1 : cap=%i con=%i port=%i type=%X size=%i \r\n PORT2 : cap=%i con=%i port=%i type=%X size=%i",
                        _raw_per1->capacity, _raw_per1->connected, _raw_per1->port, _raw_per1->type, _raw_per1->size,
                        _raw_per2->capacity, _raw_per2->connected, _raw_per2->port, _raw_per2->type, _raw_per2->size
                        );*/
                _svin_textbox_print("  ",tmp,"Lato_Black12",7,7);

                vdp1_sync();
                vdp2_sync();

                /*if (_keyb != NULL)
                {
                        if ( 0 == (_keyb->data[2] & 0x08))
                        {
                                sprintf(tmp,"con=%i port=%i type=%X size=%i data = %X %X %X %X %X %X",
                                _keyb->connected, _keyb->port, _keyb->type, _keyb->size, 
                                _keyb->data[0], _keyb->data[1], _keyb->data[2], _keyb->data[3], _keyb->data[4], _keyb->data[5]
                                );
                                uint8_t keycode = _keyb->data[3];
                                keycode = ~ keycode;
                                _svin_textbox_print("  ",tmp,"NotoSans_Black16",7,7);

                                for (int i=0;i<KEY_AREAS_NUMBER;i++)
                                {
                                        if (key_areas[i].keycode == keycode)
                                                for (int y=key_areas[i].y_start;y<key_areas[i].y_end;y++)
                                                        for (int x=key_areas[i].x_start;x<key_areas[i].x_end;x++)
                                                                mark_tile(x,y);
                                }

                        }
                        
                }*/

        }

}

