#include "flash_map_backend/flash_map_backend.h"
#include "bootutil/bootutil_public.h"
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#include "sysflash/sysflash.h"
#include <stdbool.h>
#include <stddef.h>
#include <string.h>


//------------ Alignment -----------------
#define ALIGN_OFFSET(num,align)  ((num) & ((align)-1))
#define IS_ALIGN(num,align)   (ALIGN_OFFSET((num),(align)) == 0)

#define NULL ((void*)0)
#define ARRAY_SIZE(arr)    (sizeof(arr) / sizeof((arr)[0]))

//-------------- Bootloader ---------------
#define FLASH_AREA_BOOTLOADER  0 // va in sysflash
#define FLASH_DEVICE_INTERNAL_FLASH 0 // va in sysflash
#define BOOTLOADER_START_ADDRESS 0x80000000
#define BOOTLOADER_SIZE 0x20000 // 100KB ( FLASH + META section )

//-------------- Primary Slot ---------------

#define IMAGE_PRIMARY_START_ADDRESS 0x8040000
#define APPLICATION_SIZE 0x12200 // 72K + 512 ( FLASH + HEADER )

//------------- Secondary Slot ---------------#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"

#define IMAGE_SECONDARY_START_ADDRESS 0x8080000

//-------------- Scratch ---------------------
#define SCRATCH_START_ADDRESS  0x80C0000
#define SCRATCH_SIZE 0x40000

//--------------- Aree ----------------------

static const flash_area bootloader = {
    .fa_id = FLASH_AREA_BOOTLOADER,
    .fa_device_id = FLASH_DEVICE_INTERNAL_FLASH,
    .fa_off = BOOTLOADER_START_ADDRESS,
    .fa_size = BOOTLOADER_SIZE, 
};

static const flash_area primary_image = {
    .fa_id = FLASH_AREA_IMAGE_PRIMARY(0),
    .fa_device_id = FLASH_DEVICE_INTERNAL_FLASH,
    .fa_off = IMAGE_PRIMARY_START_ADDRESS,
    .fa_size = APPLICATION_SIZE
};

static const  flash_area secondary_image = {
    .fa_id = FLASH_AREA_IMAGE_SECONDARY(0),
    .fa_device_id = FLASH_DEVICE_INTERNAL_FLASH,
    .fa_off = IMAGE_SECONDARY_START_ADDRESS,
    .fa_size = APPLICATION_SIZE
};

static const  flash_area scratch = {
    .fa_id = FLASH_AREA_IMAGE_SCRATCH,
    .fa_device_id = FLASH_DEVICE_INTERNAL_FLASH,
    .fa_off = SCRATCH_START_ADDRESS,
    .fa_size = SCRATCH_SIZE
};
//------------ Areas Array ------------

static const flash_area* arr_flash_areas[] =
{
    &bootloader,
    &primary_image,
    &secondary_image,
    &scratch,
};

static const flash_area* lookup_flash_area(uint8_t id){
    for (int i = 0; i < ARRAY_SIZE(arr_flash_areas); i++)
    {
        const flash_area* area = arr_flash_areas[i];
        if(area->fa_id == id)
            return area;
    }
    return NULL;
}

//---------- Open flash Area ----------
// given an id, return the area identified by that id 
int flash_area_open(uint8_t id, const struct flash_area **area_outp){
    const flash_area* area = lookup_flash_area(id);
    *area_outp = area;
    return area == NULL ? -1 : 0;
}


//---------- Close flash Area --------
//Placeholder
void flash_area_close(const struct flash_area *area)
{

}


//---------- Flash Area Read --------- 


int flash_area_read(const struct flash_area *fa, uint32_t off, void *dst, uint32_t len){
    if(fa->fa_device_id != FLASH_DEVICE_INTERNAL_FLASH)
        return -1;

    const uint32_t end_offset = off + len;
    if(end_offset > fa->fa_size)
        return -1;
    
    memcpy(dst,(void*)(fa->fa_off + off), len);
    return 0;
}

static uint32_t getSector(uint32_t addr){
    if(addr < 0x8008000) return FLASH_SECTOR_0;
    else if (addr < 0x8010000) return FLASH_SECTOR_1;
    else if (addr < 0x8018000) return FLASH_SECTOR_2;
    else if (addr < 0x8020000) return FLASH_SECTOR_3;
    else if (addr < 0x8040000) return FLASH_SECTOR_4;
    else if (addr < 0x8080000) return FLASH_SECTOR_5;
    else if (addr < 0x80C0000) return FLASH_SECTOR_6;
    return FLASH_SECTOR_7;
}

int flash_area_erase(const struct flash_area *fa, uint32_t off, uint32_t len){
    HAL_FLASH_Unlock();
    FLASH_EraseInitTypeDef erase;
    erase.TypeErase = FLASH_TYPEERASE_SECTORS;
    erase.NbSectors = 1;
    erase.Sector = getSector((fa->fa_off + off));
    erase.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    uint32_t eraseError;
    HAL_FLASHEx_Erase(&erase,&eraseError);
    HAL_FLASH_Lock();
    return 0;
}



int flash_area_write(const struct flash_area *fa, uint32_t off, const void *src, uint32_t len){
    
  HAL_FLASH_Unlock();
  
 
  for (uint32_t i = 0; i < len; i++)
  {
      /* code */
          HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, (fa->fa_off + off) + i , ((uint8_t*)src)[i]);
    }
  

    HAL_FLASH_Lock();
    return 0;
} 


uint32_t flash_area_align(const struct flash_area *area){
    if( area->fa_device_id == FLASH_DEVICE_INTERNAL_FLASH )
        return FLASH_ALIGN;
    return 0;
}



uint8_t flash_area_erased_val(const struct flash_area *area){
    if(area->fa_device_id == FLASH_DEVICE_INTERNAL_FLASH)
        return ERASED_VAL;
    return 0;
}


int flash_area_get_sectors(int fa_id, uint32_t *count,struct flash_sector *sectors){
    if(fa_id == FLASH_AREA_BOOTLOADER)
    {
        *count = 4;
        sectors[0].fs_off = 0;
        sectors[0].fs_size = 0x8000;

        sectors[1].fs_off = 0x8000;
        sectors[1].fs_size = 0x8000;

        sectors[2].fs_off = 0x10000;
        sectors[2].fs_size = 0x8000;

        sectors[3].fs_off = 0x18000;
        sectors[3].fs_size = 0x8000;

        return 0;
    }
    if(fa_id == FLASH_AREA_IMAGE_PRIMARY(0)){
        *count = 1;
        sectors[0].fs_off = 0;
        sectors[0].fs_size = 0x40000;
        return 0;
    }
    if(fa_id == FLASH_AREA_IMAGE_SECONDARY(0)){
        *count = 1;
        sectors[0].fs_off = 0;
        sectors[0].fs_size = 0x40000;
        return 0;
    }
    if(fa_id == FLASH_AREA_IMAGE_SCRATCH){
        *count = 1; 
        sectors[0].fs_off = 0;
        sectors[0].fs_size = 0x40000;
        return 0;
    }
    return -1;
}

//! Retrieve the flash sector a given offset belongs to.
int flash_area_sector_from_off(uint32_t off, struct flash_sector *sector){
    return -1;
}

//! Retrieve the flash sector a given offset belongs to.
int flash_area_get_sector(const struct flash_area *area, uint32_t off,struct flash_sector *sector){
    
    switch (getSector(area->fa_off + off))
    {
    case FLASH_SECTOR_0:
       {
            sector->fs_off = 0x8000000 - area->fa_off;
            sector->fs_size = 0x8000;
            return 0;
            break;
        }
    case FLASH_SECTOR_1:
       {
            sector->fs_off = 0x8008000- area->fa_off;
            sector->fs_size = 0x8000;
            return 0;
            break;
        }
    case FLASH_SECTOR_2:
       {
            sector->fs_off = 0x8010000 - area->fa_off;
            sector->fs_size = 0x8000;
            return 0;
            break;
        }
    case FLASH_SECTOR_3:
        {
            sector->fs_off = 0x8018000 - area->fa_off;
            sector->fs_size = 0x8000;
            return 0;
            break;
        }
    case FLASH_SECTOR_4:
    {
        sector->fs_off = 0x8020000 - area->fa_off;
        sector->fs_size = 0x20000;
        return 0;
        break;
    }
    case FLASH_SECTOR_5:
    {
        sector->fs_off = 0x8040000 - area->fa_off;
        sector->fs_size = 0x40000;
        return 0;
        break;
    }
    case FLASH_SECTOR_6:
    {
        sector->fs_off = 0x8080000 - area->fa_off;
        sector->fs_size = 0x40000;
        return 0;
        break;
    }
    case FLASH_SECTOR_7:
    {
        sector->fs_off = 0x80C0000 - area->fa_off;
        sector->fs_size = 0x40000;
        return 0;
        break;
    }
    default:
        break;
    }
    return -1;
    
}


//---------------- Image ------------------


int flash_area_id_from_multi_image_slot(int image_index, int slot){
    switch (slot) {
      case 0:
        return FLASH_AREA_IMAGE_PRIMARY(image_index);
      case 1:
        return FLASH_AREA_IMAGE_SECONDARY(image_index);
    }
    return -1;
}

int flash_area_id_from_image_slot(int slot){
    if(slot == 0 )
        return FLASH_AREA_IMAGE_PRIMARY(0);
    if(slot == 1)
        return FLASH_AREA_IMAGE_SECONDARY(0);
    return -1;
}

int flash_area_to_sectors(int idx, int *cnt, struct flash_area *fa){
    
    return -1;
}

int flash_area_id_to_multi_image_slot(int image_index,uint8_t id){
    if(id == FLASH_AREA_IMAGE_PRIMARY(image_index) )
        return 0;
    if(id == FLASH_AREA_IMAGE_SECONDARY(image_index))
        return 1;
    return -1;
}