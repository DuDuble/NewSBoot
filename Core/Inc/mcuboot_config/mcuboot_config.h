#ifndef MCUBOOT_CONFIG_H
#define MCUBOOT_CONFIG_H

#define MCUBOOT_IMAGE_NUMBER 1

/* Disable crypto for now (temporary just to compile) */
    


/* Basic options */
#define MCUBOOT_VALIDATE_PRIMARY_SLOT 1

#endif

//we use mbedtls
#define MCUBOOT_USE_MBED_TLS 1


//-------------------- Encryption Image -------------------
#define MCUBOOT_ENC_IMAGES 1
#define MCUBOOT_ENCRYPT_RSA 1


#define MCUBOOT_MAX_IMG_SECTORS 3

//WE do not use watchdog
#define MCUBOOT_WATCHDOG_FEED()                 \
    do {                                        \
    } while (0) 

#define MCUBOOT_SWAP_USING_SCRATCH 1

//we use RSA
#define MCUBOOT_SIGN_RSA 1
#define MCUBOOT_SIGN_RSA_LEN 2048
#define MCUBOOT_SIGN_RSA_PKCS1_PSS 1

#define MCUBOOT_USE_FLASH_AREA_GET_SECTORS 1
