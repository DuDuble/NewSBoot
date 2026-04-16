/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "mbedtls.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "cmsis_gcc.h"
#include "sha512.h"
#include "stm32f7xx_hal.h"
#include "mbedtls/sha256.h"
#include "mbedtls/rsa.h"
#include "mbedtls/bignum.h"
#include "mbedtls/md.h"
#include "stm32f7xx_hal_flash.h"
#include "stm32f7xx_hal_flash_ex.h"
#include "flash_map_backend/flash_map_backend.h"
#include "sysflash/sysflash.h"
#include "bootutil/crypto/sha.h"
#include "bootutil/crypto/rsa.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

// Pointer to function. Used for the Jump 
typedef void (*pFunction ) (void);

// Struct used to read the firmware header  
typedef struct 
{
  /* data */
  uint32_t version;
  uint32_t size;
  uint8_t hash[32];
  uint8_t sign[256];
}Header;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// Memory address, they are not updated automatically
#define BOOT_META 0x8018000
#define APP_ADDR_HEADER 0x8020000
#define APP_ADDR_FLASH (APP_ADDR_HEADER + HEADER_OFFSET)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */


// Public Key , r2 is not used in this project
const uint8_t modulus[] = {189,223,103,217,109,14,149,211,82,50,127,220,222,53,149,55,241,102,173,38,35,139,227,156,159,65,15,120,162,178,31,195,34,11,136,3,72,188,197,58,30,15,20,218,54,204,95,208,49,28,57,150,105,152,254,162,255,14,14,176,168,145,203,208,172,19,90,100,38,127,96,23,54,26,2,92,238,194,111,220,89,249,53,245,4,228,137,8,252,36,158,4,248,17,70,179,220,248,14,56,76,64,67,253,0,26,128,202,66,22,38,160,75,137,218,48,131,253,217,75,206,116,53,91,184,129,84,237,48,243,223,1,83,146,18,100,21,161,13,171,116,16,0,33,135,217,202,242,36,20,243,12,81,174,45,25,148,2,10,62,176,55,16,40,153,62,173,127,51,215,26,11,18,105,127,201,9,61,97,127,92,179,93,67,6,128,114,86,5,144,220,162,57,189,93,14,251,189,79,13,92,5,11,146,82,182,219,137,12,171,186,117,178,85,86,82,197,243,31,157,72,123,108,240,220,80,214,88,124,133,11,243,66,144,192,44,140,56,246,165,245,235,254,175,144,205,150,157,153,248,121,0,130,69,84,55};
const uint8_t r2[] = {139,184,113,204,56,185,87,188,51,23,237,223,76,114,26,231,44,196,197,214,205,97,142,54,228,216,44,222,162,202,49,211,230,116,53,239,174,176,75,176,143,39,14,151,206,226,40,200,85,12,232,131,107,116,97,139,12,84,112,78,80,25,65,32,97,238,207,140,78,225,116,35,151,58,248,15,30,129,254,147,131,173,211,243,12,207,134,140,203,220,6,179,155,145,3,120,240,38,122,66,203,87,10,245,195,251,212,49,93,16,36,66,199,3,150,138,71,164,18,2,63,173,100,190,9,148,102,158,100,219,229,67,161,70,43,151,218,148,176,163,240,235,189,9,31,64,60,214,152,61,137,137,49,102,13,28,195,116,158,28,201,243,72,78,200,30,245,43,186,240,53,14,66,177,116,174,70,79,21,193,67,115,91,96,134,54,88,210,181,211,96,250,238,153,44,112,106,61,143,170,226,187,145,161,153,243,94,110,80,107,12,181,190,126,24,252,234,60,163,220,206,41,139,138,98,58,195,67,196,15,20,95,206,222,12,105,11,69,154,151,185,220,42,76,93,221,220,21,39,40,223,117,214,228,198,140};
const uint8_t exponent[] = {0x00, 0x01, 0x00, 0x01};




void bootJump(void);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



// ------------------------- Jump to Firmware  -----------------------------------------

// Jump to the firmware 
void bootJump(){
  uint32_t JumpAddress;
  pFunction JumpFunction;

  printf("App Starting.. \r\n" );
  HAL_Delay(100);

  // Reset Handler function position. The first 4 bytes are the Stack Pointer 
  JumpAddress = *( volatile uint32_t*) (APP_ADDR_FLASH + 4);
  JumpFunction = (pFunction) JumpAddress;

  __set_MSP(*(volatile uint32_t*) APP_ADDR_FLASH);

  HAL_DeInit();
  __disable_irq();

  JumpFunction();

  
}

// Check if there is an application.
int checkApp(const struct flash_area* app){

  // We know that the SP needs to be in RAM. So it must be a x20000000 address
  printf("BootLoader Start \r\n");
  if( ( ( *(uint32_t*)(app->fa_off + HEADER_OFFSET) ) & 0x2FF00000) == 0x20000000 )
  {
    return 1;
  } 
  else{
    printf("No Application found !!\r\n"); 
    return 0;
  }
}

// ------------------------- Integrity Check -----------------------------------------

// Calculate SHA256 of the firmware in flash
void sha256Calc(uint8_t* outputHash,uint32_t size,const struct flash_area* app){
   

    bootutil_sha_context ctx;
    bootutil_sha_init(&ctx);
    bootutil_sha_update(&ctx, (uint8_t*) (app->fa_off + HEADER_OFFSET), size);
    bootutil_sha_finish(&ctx,outputHash);
    bootutil_sha_drop(&ctx);
    // mbedtls_sha256_init(&ctx);
    // mbedtls_sha256_starts(&ctx,0);
    // mbedtls_sha256_update(&ctx,(uint8_t* )APP_ADDR_FLASH,size);
    // mbedtls_sha256_finish(&ctx,outputHash);
    // mbedtls_sha256_free(&ctx);
}

// Compare calculated hash with the hash stored in the firmware header
int checkIntegrity(uint8_t* hash){
    return memcmp(hash,(uint8_t*) APP_ADDR_HEADER + 8,32);
}

// ------------------------- Auth Check -----------------------------------------

//Sign control
int checkAuth(uint8_t* hash,uint8_t* sign){

  mbedtls_rsa_context rsa;

  mbedtls_rsa_init(&rsa,MBEDTLS_RSA_PKCS_V15,0);

  int moduleSize = sizeof(modulus)/sizeof(modulus[0]);
  int expSize = sizeof(exponent) / sizeof(exponent[0]);

  //uint8_t output[256];

  //Import public key 
  mbedtls_rsa_import_raw(&rsa,modulus,moduleSize,NULL,0,NULL,0,NULL,0,exponent,expSize);


  //Verify 
  int ret = mbedtls_rsa_pkcs1_verify(&rsa,NULL,NULL,MBEDTLS_RSA_PUBLIC,MBEDTLS_MD_SHA256,32,hash,sign);

  printf("RET ---> %d\r\n",ret);
  return ret;
}

// ------------------------- Anti-RollBack -----------------------------------------

//return the current or last firmware version that is/was installed
uint32_t getCurrentVersion(){
    uint32_t version = *(uint32_t*) BOOT_META;
    return version;
}

//update version value stored in META section in the flash. 

void updateVersion(uint32_t newVersion){

  HAL_FLASH_Unlock();
  
  FLASH_EraseInitTypeDef erase;
  erase.TypeErase = FLASH_TYPEERASE_SECTORS;
  erase.NbSectors = 1;
  erase.Sector = FLASH_SECTOR_3;
  erase.VoltageRange = FLASH_VOLTAGE_RANGE_3;

  uint32_t eraseError;
  HAL_FLASHEx_Erase(&erase,&eraseError);
  HAL_FLASH_Program(TYPEPROGRAM_WORD, BOOT_META, newVersion);

  HAL_FLASH_Lock();
}

// -------------------------- UART -----------------------------------------------

//for the print
int _write(int file, char* ptr, int len){
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
    HAL_UART_Transmit(&huart3, (uint8_t* )ptr++,1,100);

  }

  return len;
  
}

// -------------------------- Main -----------------------------------------------

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_MBEDTLS_Init();
  /* USER CODE BEGIN 2 */






  
  //Does even an application exists ?  
  const struct flash_area* firmware;
  int ret = flash_area_open(1,&firmware);
  printf("Ret --> %d\r\n",ret);
  printf("Value ---> %ld\r\n",firmware->fa_off);

  if(checkApp(firmware)){


    uint8_t outputHash[32];

    //Get all the data present in the header

    Header header;
    flash_area_read(firmware,0,&header,sizeof(header));

    //Calc a hash from scratch, from the application, excluding header
    sha256Calc(outputHash,header.size,firmware);

    //Check if the sign is correct 
    if(!checkAuth(outputHash,header.sign))
    {
      printf("Firma VERIFICATA \r\n");
    
      
      //check integrity ( hash )
      if (!checkIntegrity(outputHash))
      {
        printf("Confronto avvenuto con successo,Hash corrispondono \r\n");
       

        printf("New Version ---> %ld \r\n",header.version);
        uint32_t current_version = getCurrentVersion();
        printf("Versione corrente -------> %ld \r\n",current_version);

        //anti - rollback 
        if(current_version < header.version){
          updateVersion(header.version);
          bootJump();
        }
        else{
          if(current_version == header.version)
              bootJump();
          else
            printf("Roll-back rilevato \r\n");
        }
      }
      else
        printf("Hash NON corrispondono \r\n");


    }
    else{
      printf("Autenticazione non avvenuta \r\n");
    }

  
  }
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
