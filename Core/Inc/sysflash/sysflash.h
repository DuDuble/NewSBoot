#define FLASH_AREA_IMAGE_PRIMARY(x)    (((x) == 0) ?          \
                                         FLASH_AREA_IMAGE_0_PRIMARY : \
                                         FLASH_SLOT_DOES_NOT_EXIST)
#define FLASH_AREA_IMAGE_SECONDARY(x)  (((x) == 0) ?          \
                                         FLASH_AREA_IMAGE_0_SECONDARY : \
                                         FLASH_SLOT_DOES_NOT_EXIST)