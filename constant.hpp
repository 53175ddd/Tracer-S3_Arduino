#pragma once

#define LED1_OUT 42
#define LED2_OUT 41
#define LED3_OUT 40

#define BUZZER_OUT 16

#define SENSOR_C_IN 1
#define SENSOR_L_IN 5
#define SENSOR_M_IN 6
#define SENSOR_R_IN 7
#define SENSOR_G_IN 2

#define POWER_LEVEL_IN 3

#define TR_OUT 8

#define SW_MODE_IN 17
#define SW_EXEC_IN 18

#define DRV8835_MODE_OUT 35
#define DRV8835_AIN1_OUT 36
#define DRV8835_AIN2_OUT 37
#define DRV8835_BIN1_OUT 38
#define DRV8835_BIN2_OUT 39

extern float Kp = 1.200f;  // 比例ゲイン
extern float Kd = 0.400f;  // 微分ゲイン

extern int16_t base_speed = 1200;
extern int16_t speed_diff = 350;

extern const char* SSID = "Your SSID here";
extern const char* PASS = "Your Password";
