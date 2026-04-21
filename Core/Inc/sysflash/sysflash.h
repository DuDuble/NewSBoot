#define FLASH_SLOT_DOES_NOT_EXIST 255

#define FLASH_AREA_IMAGE_PRIMARY_ID 1
#define FLASH_AREA_IMAGE_SECONDARY_ID 2
#define FLASH_AREA_IMAGE_SCRATCH 3

#define FLASH_AREA_IMAGE_PRIMARY(x)    (((x) == 0) ? FLASH_AREA_IMAGE_PRIMARY_ID : FLASH_SLOT_DOES_NOT_EXIST   )   
                                   

#define FLASH_AREA_IMAGE_SECONDARY(x)  (((x) == 0) ?          \
                                         FLASH_AREA_IMAGE_SECONDARY_ID : \
                                         FLASH_SLOT_DOES_NOT_EXIST)

#define HEADER_OFFSET 512

#define FLASH_ALIGN 4 
#define ERASED_VAL 0xFF
