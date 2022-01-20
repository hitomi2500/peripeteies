#include <yaul.h>
#include <cartridge.h>
#include "svin.h"

uint16_t lfsr;

uint16_t lfsr_fib(void)
{
    
    uint16_t bit;                    /* Must be 16-bit to allow bit<<15 later in the code */

    /* taps: 16 14 13 11; feedback polynomial: x^16 + x^14 + x^13 + x^11 + 1 */
    bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1u;
    lfsr = (lfsr >> 1) | (bit << 15);

    return lfsr;
}

bool test_ram_range(void * address, int size, int speed, bool preread)
{
    uint16_t * p16 = (uint16_t *)address;
    uint16_t s;
    //setting speed
    if (true == preread)
        MEMORY_WRITE(32, SCU(ASR0), 0x90001FF0 | (0x1100000*speed));
    else
        MEMORY_WRITE(32, SCU(ASR0), 0x10001FF0 | (0x1100000*speed));
    //fill RAM with test data
    lfsr = 0xACE1u;
    for (unsigned int i=0;i<(size/sizeof(uint16_t));i++)
    {
        s = lfsr_fib();
        p16[i] = s;
    }
    //verifying
    bool bOk = true;
    lfsr = 0xACE1u;
    for (unsigned int i=0;((i<(size/sizeof(uint16_t))) && (bOk == true));i++)
    {
        s = lfsr_fib();
        if (p16[i] != s)
            bOk = false;  
    }
    return bOk;
}

bool test_ram_range_aliased(void * write_address, void *  read_address, int size, int speed, bool preread)
{
    uint16_t * p16 = (uint16_t *)write_address;
    uint16_t s;
    //setting speed
    if (true == preread)
        MEMORY_WRITE(32, SCU(ASR0), 0x90001FF0 | (0x1100000*speed));
    else
        MEMORY_WRITE(32, SCU(ASR0), 0x10001FF0 | (0x1100000*speed));
    //fill RAM with test data
    lfsr = 0xACE1u;
    for (unsigned int i=0;i<(size/sizeof(uint16_t));i++)
    {
        s = lfsr_fib();
        p16[i] = s;
    }
    //verifying
    p16 = (uint16_t *)read_address;
    bool bOk = true;
    lfsr = 0xACE1u;
    for (unsigned int i=0;((i<(size/sizeof(uint16_t))) && (bOk == true));i++)
    {
        s = lfsr_fib();
        if (p16[i] != s)
            bOk = false;  
    }
    return bOk;
}

void cartridge_memory_test()
{
    uint8_t * buf = malloc(64*1024);
    char text[256];
    //clear VDP1
    _svin_background_set("BLACK.BG");
    //set VDP1 with fornt palette
    uint8_t temp_pal[3 * 256];

    //setting wasca to RAM mode
    //uint16_t * pWasca = (uint16_t *)0x23FFF000;
    //pWasca[0x7FA] = 0x0004;//RAM mode

    //pallete 7 is special. it's 16 gradients from solid colors to "backgroung frame" color  
    //reverse grayscale
    int iBaseR,iBaseG,iBaseB;
    int iStepR,iStepG,iStepB;
    for (int iColor=0; iColor<16; iColor++)
    {
        iBaseR = 0x7F; iBaseG = 0x7F; iBaseB = 0x7F; 
        switch(iColor)
        {
            case 0: //quasi-black
                iStepR = -8; iStepG = -8; iStepB = -8;
                break;
            case 1: //red
                iStepR = 8; iStepG = -7; iStepB = -7;
                break;
            case 2: //green
                iStepR = -7; iStepG = 8; iStepB = -7;
                break;
            case 3: //blue
                iStepR = -7; iStepG = -7; iStepB = 8;
                break;
            case 4: //cyan
                iStepR =-7; iStepG = 8; iStepB = 8;
                break;
            case 5: //magenta
                iStepR = 8; iStepG = -7; iStepB = 8;
                break;
            case 6: //yellow
                iStepR = 8; iStepG = 8; iStepB = -7;
                break;
            case 7: //white
                iStepR = 8; iStepG = 8; iStepB = 8;
                break;
            case 8: //orange
                iStepR = 6; iStepG = 0; iStepB = -6;
                break;
            case 9: //olive
                iStepR = 0; iStepG = 4; iStepB = -4;
                break;
            case 10: //brown
                iStepR = 2; iStepG = 0; iStepB = -7;
                break;
            case 11: //lightblue
                iStepR = 0; iStepG = 4; iStepB = 8;
                break;
            case 12: //meaty
                iStepR = 6; iStepG = 0; iStepB = 0;
                break;
            case 13: //khaki
                iStepR = -7; iStepG = -2; iStepB = 0;
                break;
            case 14: //grey
                iStepR = 2; iStepG = 2; iStepB = 2;
                break;
            case 15: //coffee
                iStepR = 6; iStepG = 4; iStepB = -2;
                break;
        }

        for (int i = 15; i >= 0; i--)
        {
            temp_pal[iColor*48 + i * 3] = iBaseR;     
            temp_pal[iColor*48 + i * 3 + 1] = iBaseG; 
            temp_pal[iColor*48 + i * 3 + 2] = iBaseB; 
            iBaseR += iStepR;
            iBaseG += iStepG;
            iBaseB += iStepB;
        }
    }

    _svin_set_palette(0, temp_pal);

    vdp1_sync();
    vdp1_sync_wait();

    vdp1_vram_partitions_t vdp1_vram_partitions;
    vdp1_vram_partitions_get(&vdp1_vram_partitions);

           
    sprintf(text," TESTING CART...1234567890");
    int heigth = _svin_text_render(buf,352,text,"Robtronika10");
    uint8_t * p8a = (uint8_t *)(vdp1_vram_partitions.texture_base + 704*7 );
    uint8_t * p8b = (uint8_t *)(vdp1_vram_partitions.texture_base + 704*231 );
    uint8_t c;
    for (int j=0;j<(heigth/2);j++)
    {
        for (int i=0;i<352;i++)
        {
            c = buf[352*2*j+i]>>4;
            if (0x0F == c)
                p8a[352*j+i] = 0x01;
            else
                p8a[352*j+i] = 0x70+c;
            c = buf[352*2*j+352+i]>>4;
            if (0x0F == c)
                p8b[352*j+i] = 0x01;
            else
                p8b[352*j+i] = 0x70+c;
        }
    }

    bool bRAM;
    int ram_speed;
    int ram_speed_preread;
    bool bUnmapped;
    bool bOk;
    //do CS0 tests for each 1M region
    for (int chunk = 0; chunk<32; chunk++)
    {
        //do test
        //first let's check if it's ROM or RAM
        if (true == test_ram_range((void*)(CS0(chunk*0x100000)),16,0xF,false))
        {
            bRAM = true;
            bUnmapped = false;
        }
        else
        {
            volatile uint16_t *p16 = (uint16_t *)(CS0(chunk*0x100000));
            bRAM = false;
            //it's a ROM or unmapped, deciding which one
            bUnmapped = true;
            for (int i=1;((i<0x80000) && (bUnmapped == true));i++)
                if (p16[0] != p16[i])
                    bUnmapped = false;
        }

        int ram_size = 0;
        bool ram_aliased = false;
        if (true == bRAM)
        {
            //detecting ram type first at minimal speed
            if (true == test_ram_range((void*)(CS0(chunk*0x100000)),0x100000,0xF,false))
            {
                //1M 
                ram_size = 0x100000;
            }
            else if (true == test_ram_range((void*)(CS0(chunk*0x100000)),0xF0000,0xF,false))
            {
                //0.93M 
                ram_size = 0xF0000;
            }
            else if ( (true == test_ram_range((void*)(CS0(chunk*0x100000)),0x80000,0xF,false)) )
            {
                //0.5M 
                ram_size = 0x80000;
                if (true == test_ram_range_aliased((void*)(CS0(chunk*0x100000)),(void*)(CS0(chunk*0x100000+0x80000)),0x80000,0xF,false))
                    ram_aliased = true;
            }

            //measuring speed for a small chunk (16K)
            ram_speed = 0x0F;
            bOk = true;
            while ((true == bOk) && (ram_speed > 0) )
            {
                ram_speed--;
                bOk = test_ram_range((void*)(CS0(chunk*0x100000)),0x4000,0xF,false);
            }   
            if (false == bOk)
                ram_speed++;       
            //verifying for a full range
            bOk = test_ram_range((void*)(CS0(chunk*0x100000)),ram_size,0xF,false);
            //if failed, choosing a slower speed
            while ((false == bOk) && (ram_speed < 0xF) )
            {
                ram_speed++;
                bOk = test_ram_range((void*)(CS0(chunk*0x100000)),ram_size,0xF,false);
            }   

            //same stuff for speed with preread enabled
            ram_speed_preread = 0x0F;
            bOk = true;
            while ((true == bOk) && (ram_speed_preread > 0) )
            {
                ram_speed_preread--;
                bOk = test_ram_range((void*)(CS0(chunk*0x100000)),0x4000,0xF,true);
            }   
            if (false == bOk)
                ram_speed_preread++;       
            //verifying for a full range
            bOk = test_ram_range((void*)(CS0(chunk*0x100000)),ram_size,0xF,true);
            //if failed, choosing a slower speed
            while ((false == bOk) && (ram_speed_preread < 0xF) )
            {
                ram_speed_preread++;
                bOk = test_ram_range((void*)(CS0(chunk*0x100000)),ram_size,0xF,true);
            }  
        }

        //output results
        if (bUnmapped)
            sprintf(text," %X : UNMAPPED",CS0(chunk*0x100000));
        else if (bRAM)
        {
            switch (ram_size)
            {
                case 0x100000:
                    sprintf(text," %X: RAM 1M SPD %i PR %i",CS0(chunk*0x100000),ram_speed,ram_speed_preread);
                    break;
                case 0xF0000:
                    sprintf(text," %X: RAM .93M SPD %i PR %i",CS0(chunk*0x100000),ram_speed,ram_speed_preread);
                    break;
                case 0x80000:
                    if (ram_aliased)
                        sprintf(text," %X: RAM 0.5Ma SPD %i PR %i",CS0(chunk*0x100000),ram_speed,ram_speed_preread);
                    else
                        sprintf(text," %X: RAM 0.5M SPD %i PR %i",CS0(chunk*0x100000),ram_speed,ram_speed_preread);
                    break;
                default:
                    sprintf(text," %X: RAM ??? SPD %i PR %i",CS0(chunk*0x100000),ram_speed,ram_speed_preread);
                    break;
            }
        }
        else
            sprintf(text,"  %X : ROM SPD=X PRE=X",CS0(chunk*0x100000));
        
        int heigth = _svin_text_render(buf,352,text,"Robtronika10");
        //assert (heigth <= 12);
        //copy text to VDP1 sprites
        uint8_t * p8a = (uint8_t *)(vdp1_vram_partitions.texture_base + 704*10 + (chunk*6)*352);
        uint8_t * p8b = (uint8_t *)(vdp1_vram_partitions.texture_base + 704*234 + (chunk*6)*352);
        uint8_t c;
        for (int j=0;j<(heigth/2);j++)
        {
            for (int i=0;i<352;i++)
            {
                c = buf[352*2*j+i]>>4;
                if (0x0F == c)
                    p8a[352*j+i] = 0x01;
                else
                    p8a[352*j+i] = 0x70+c;
                c = buf[352*2*j+352+i]>>4;
                if (0x0F == c)
                    p8b[352*j+i] = 0x01;
                else
                    p8b[352*j+i] = 0x70+c;
            }
            //print at 1st field
            //memcpy((uint8_t *)(vdp1_vram_partitions.texture_base + (iChunk*7 + j)*352), (uint8_t *)&(buf[352*2*j]), 352);
            //print at 2nd field
            //memcpy((uint8_t *)(vdp1_vram_partitions.texture_base + 704*224 + (iChunk*7 + j)*352), (uint8_t *)&(buf[352*2*j+352]), 352);
        }

    }
    free (buf);
}