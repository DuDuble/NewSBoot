#ifndef MCUBOOT_CONFIG_H
#define MCUBOOT_CONFIG_H

#define MCUBOOT_IMAGE_NUMBER 1

/* Disable crypto for now (temporary just to compile) */
    


/* Basic options */
#define MCUBOOT_VALIDATE_PRIMARY_SLOT 0

#endif

//we use mbedtls
#define MCUBOOT_USE_MBED_TLS 1


#define MCUBOOT_MAX_IMG_SECTORS 3

//WE do not use watchdog
#define MCUBOOT_WATCHDOG_FEED()                 \
    do {                                        \
    } while (0) 

//Overwrite, No SWAP
#define MCUBOOT_OVERWRITE_ONLY 1

//we use RSA
#define MCUBOOT_SIGN_RSA 1
#define MCUBOOT_SIGN_RSA_LEN 2048
#define MCUBOOT_SIGN_RSA_PKCS1_PSS 1


