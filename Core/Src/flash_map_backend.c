#include "flash_map_backend/flash_map_backend.h"
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

//------------ Alignment -----------------
#define ALIGN_OFFSET(num,align)  ((num) & ((align)-1))
#define IS_ALIGN(num,align)   (ALIGN_OFFSET((num),(align)) == 0)

#define NULL ((void*)0)
#define ARRAY_SIZE(arr)    (sizeof(arr) / sizeof((arr)[0]))

//-------------- Bootloader ---------------
#define FLASH_AREA_BOOTLOADER_ID  0 // va in sysflash
#define FLASH_DEVICE_INTERNAL_FLASH 0 // va in sysflash
#define BOOTLOADER_START_ADDRESS 0x80000000
#define BOOTLOADER_SIZE 0x19000 // 100KB ( FLASH + META section )

//-------------- Primary Slot ---------------
#define FLASH_AREA_PRIMARY_ID 1
#define IMAGE_PRIMARY_START_ADDRESS 0x8020000
#define APPLICATION_SIZE 0x12200 // 72K + 512 ( FLASH + HEADER )

// //------------- Secondary Slot ---------------
// #define FLASH_AREA_SECONDARY_ID 2
// #define IMAGE_SECONDARY_START_ADDRESS 0x8032200

//--------------- Aree ----------------------

static const flash_area bootloader = {
    .fa_id = FLASH_AREA_BOOTLOADER_ID,
    .fa_device_id = FLASH_DEVICE_INTERNAL_FLASH,
    .fa_off = BOOTLOADER_START_ADDRESS,
    .fa_size = BOOTLOADER_SIZE, 
};

static const flash_area primary_image = {
    .fa_id = FLASH_AREA_PRIMARY_ID,
    .fa_device_id = FLASH_DEVICE_INTERNAL_FLASH,
    .fa_off = IMAGE_PRIMARY_START_ADDRESS,
    .fa_size = APPLICATION_SIZE
};

// static const  flash_area secondary_image = {
//     .fa_id = FLASH_AREA_SECONDARY_ID,
//     .fa_device_id = FLASH_DEVICE_INTERNAL_FLASH,
//     .fa_off = IMAGE_SECONDARY_START_ADDRESS,
//     .fa_size = APPLICATION_SIZE
// };

//------------ Areas Array ------------

static const flash_area* arr_flash_areas[] =
{
    &bootloader,
    &primary_image,
    //&secondary_image,
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
    return area == NULL ? 0 : -1;
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

int flash_area_erase(const struct flash_area *fa, uint32_t off, uint32_t len){
    return -1;
}

int flash_area_write(const struct flash_area *fa, uint32_t off, const void *src, uint32_t len){
    return -1;
} 


uint32_t flash_area_align(const struct flash_area *area){
    return -1;
}



uint8_t flash_area_erased_val(const struct flash_area *area){
    return -1;
}


int flash_area_get_sectors(int fa_id, uint32_t *count,struct flash_sector *sectors){
    return -1;
}

//! Retrieve the flash sector a given offset belongs to.
int flash_area_sector_from_off(uint32_t off, struct flash_sector *sector){
    return -1;
}

//! Retrieve the flash sector a given offset belongs to.
int flash_area_get_sector(const struct flash_area *area, uint32_t off,struct flash_sector *sector){
    return -1;
}


//---------------- Image ------------------


int flash_area_id_from_multi_image_slot(int image_index, int slot){
    return -1;
}

int flash_area_id_from_image_slot(int slot){
    if(slot == 0 )
        return FLASH_AREA_PRIMARY_ID;
    // if(slot == 1)
    //     return FLASH_AREA_SECONDARY_ID;
    return -1;
}

int flash_area_to_sectors(int idx, int *cnt, struct flash_area *fa){
    return -1;
}