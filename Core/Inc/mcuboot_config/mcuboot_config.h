#ifndef MCUBOOT_CONFIG_H
#define MCUBOOT_CONFIG_H

#define MCUBOOT_IMAGE_NUMBER 1

/* Disable crypto for now (temporary just to compile) */
#define MCUBOOT_SIGN_RSA 0
#define MCUBOOT_SIGN_EC256 0
#define MCUBOOT_SIGN_ED25519 0

/* Basic options */
#define MCUBOOT_VALIDATE_PRIMARY_SLOT 0

#endif

#define MCUBOOT_USE_MBED_TLS 1

#define MCUBOOT_MAX_IMG_SECTORS 3

#define MCUBOOT_WATCHDOG_FEED()                 \
    do {                                        \
    } while (0) 

#define MCUBOOT_OVERWRITE_ONLY 1
