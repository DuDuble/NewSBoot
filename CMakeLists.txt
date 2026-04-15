cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME NewSBoot)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# --- Generazione del .bin ---
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary
            $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
            ${CMAKE_PROJECT_NAME}.bin
    COMMENT "Generating .bin file"
)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Add mcuboot


# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/mcuboot/boot/bootutil/src/boot_record.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/mcuboot/boot/bootutil/src/bootutil_find_key.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/mcuboot/boot/bootutil/src/bootutil_img_hash.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/mcuboot/boot/bootutil/src/bootutil_img_security_cnt.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/mcuboot/boot/bootutil/src/bootutil_misc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/mcuboot/boot/bootutil/src/bootutil_area.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/mcuboot/boot/bootutil/src/bootutil_loader.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/mcuboot/boot/bootutil/src/bootutil_public.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/mcuboot/boot/bootutil/src/caps.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/mcuboot/boot/bootutil/src/encrypted.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/mcuboot/boot/bootutil/src/fault_injection_hardening.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/mcuboot/boot/bootutil/src/fault_injection_hardening_delay_rng_mbedtls.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/mcuboot/boot/bootutil/src/image_ecdsa.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/mcuboot/boot/bootutil/src/image_ed25519.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/mcuboot/boot/bootutil/src/image_rsa.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/mcuboot/boot/bootutil/src/image_validate.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/mcuboot/boot/bootutil/src/loader.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/mcuboot/boot/bootutil/src/swap_misc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/mcuboot/boot/bootutil/src/swap_move.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/mcuboot/boot/bootutil/src/swap_offset.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/mcuboot/boot/bootutil/src/swap_scratch.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/mcuboot/boot/bootutil/src/ram_load.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/mcuboot/boot/bootutil/src/tlv.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/mcuboot/boot/bootutil/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc/flash_map_backend
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc/mcuboot_config
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    
    # Add user defined libraries
)

