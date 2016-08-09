#pragma once

#define HAL_BOARD_NAME "F4BY"
#define HAL_CPU_CLASS HAL_CPU_CLASS_150
#define HAL_OS_POSIX_IO 1
#define HAL_BOARD_LOG_DIRECTORY "/fs/microsd/APM/LOGS"
#define HAL_BOARD_TERRAIN_DIRECTORY "/fs/microsd/APM/TERRAIN"
#define HAL_PARAM_DEFAULTS_PATH "/etc/defaults.parm"
#define HAL_INS_DEFAULT HAL_INS_F4BY
#define HAL_BARO_DEFAULT HAL_BARO_PX4
#define HAL_COMPASS_DEFAULT HAL_COMPASS_F4BY
#define HAL_SERIAL0_BAUD_DEFAULT 115200
#define CONFIG_HAL_BOARD_SUBTYPE HAL_BOARD_SUBTYPE_NONE
#define HAL_STORAGE_SIZE          16384
#define HAL_HAVE_IMU_HEATER       1 // for F4BY
#define HAL_IMU_TEMP_DEFAULT     -1 // disabled

#define HAL_GPIO_A_LED_PIN        27
#define HAL_GPIO_B_LED_PIN        26
#define HAL_GPIO_C_LED_PIN        25
#define HAL_GPIO_D_LED_PIN        24
#define HAL_GPIO_LED_ON           LOW
#define HAL_GPIO_LED_OFF          HIGH
