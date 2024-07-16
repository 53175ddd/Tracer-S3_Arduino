#pragma once

// #include <BTAddress.h>
// #include <BTAdvertisedDevice.h>
// #include <BTScan.h>
// #include <BluetoothSerial.h>

#include "driver/adc.h"

#define SENSOR_C 0
#define SENSOR_L 1
#define SENSOR_M 2
#define SENSOR_R 3
#define SENSOR_G 4
#define POWER_LEVEL 5

#define BATTERY_CELL_COUNT 2

#define MOTOR_VOLTAGE_LIMIT 3000

// #define Serial SerialBT

// BluetoothSerial SerialBT;

typedef struct {
  float s1;
  float s2;
  float s3;
  float s4;
  float s5;
} normalized_line_t;

typedef struct {
  int16_t s1;
  int16_t s2;
  int16_t s3;
  int16_t s4;
  int16_t s5;
} sensor_t;

typedef struct {
  sensor_t on;
  sensor_t off;
  sensor_t diff;
} diff_t;

sensor_t data_min = {32767, 32767, 32767, 32767, 32767};
sensor_t data_max = {0, 0, 0, 0, 0};

void LED_init(void) {
  /* 初期状態を設定 */
  pinMode(LED1_OUT, OUTPUT);
  pinMode(LED2_OUT, OUTPUT);
  pinMode(LED3_OUT, OUTPUT);

  Serial.print("LED Initialization done.\n");
}

void ADC_init(void) {
  pinMode(SENSOR_C_IN   ,  INPUT);
  pinMode(SENSOR_L_IN   ,  INPUT);
  pinMode(SENSOR_M_IN   ,  INPUT);
  pinMode(SENSOR_R_IN   ,  INPUT);
  pinMode(SENSOR_G_IN   ,  INPUT);
  pinMode(POWER_LEVEL_IN,  INPUT);
  analogReadResolution(12);
  analogSetAttenuation((adc_attenuation_t)ADC_ATTEN_DB_11);

  Serial.print("ADC Input Initialization done\n");
}

void IR_init(void) {
  pinMode(TR_OUT, OUTPUT);

  Serial.print("IR Transister Control Initialization done.\n");
}

void motor_init(void) {
  pinMode(DRV8835_MODE_OUT, OUTPUT);  digitalWrite(DRV8835_MODE_OUT,  LOW);
  pinMode(DRV8835_AIN1_OUT, OUTPUT);  digitalWrite(DRV8835_AIN1_OUT,  LOW);
  pinMode(DRV8835_AIN2_OUT, OUTPUT);  digitalWrite(DRV8835_AIN2_OUT,  LOW);
  pinMode(DRV8835_BIN1_OUT, OUTPUT);  digitalWrite(DRV8835_BIN1_OUT,  LOW);
  pinMode(DRV8835_BIN2_OUT, OUTPUT);  digitalWrite(DRV8835_BIN2_OUT,  LOW);

  Serial.print("Motor Driver Output Initialization done\n");
}

void SW_init(void) {
  pinMode(SW_MODE_IN,  INPUT_PULLUP);
  pinMode(SW_EXEC_IN,  INPUT_PULLUP);

  Serial.print("SW Input Initialization done.\n");
}

void buzzer_init(void) {
  pinMode(BUZZER_OUT, OUTPUT);

  Serial.print("Buzzer Output Initialization done.\n");
}

void LED_num(uint8_t num) {
  /* 対応するビットの値に合わせて LED を光らせる */
  digitalWrite(LED1_OUT, (num & 0b100) == 0 ? LOW : HIGH);
  digitalWrite(LED2_OUT, (num & 0b010) == 0 ? LOW : HIGH);
  digitalWrite(LED3_OUT, (num & 0b001) == 0 ? LOW : HIGH);
}

void buzzer(uint16_t freq) {
  tone(BUZZER_OUT, freq);
}

void buzzer_off(void) {
  noTone(BUZZER_OUT);
}

uint16_t ADC_read(uint8_t ch) {
  uint16_t adc;

  switch(ch) {
    case SENSOR_C:
      adc = analogRead(SENSOR_C_IN);
      break;

    case SENSOR_L:
      adc = analogRead(SENSOR_L_IN);
      break;

    case SENSOR_M:
      adc = analogRead(SENSOR_M_IN);
      break;

    case SENSOR_R:
      adc = analogRead(SENSOR_R_IN);
      break;

    case SENSOR_G:
      adc = analogRead(SENSOR_G_IN);
      break;

    case POWER_LEVEL:
      adc = analogRead(POWER_LEVEL_IN);
      break;
  }

  return adc;
}

sensor_t sensor_read(uint8_t led_status) {
  sensor_t data;

  digitalWrite(TR_OUT, led_status);
  delayMicroseconds(200);
  data.s1 = ADC_read(SENSOR_C);
  data.s2 = ADC_read(SENSOR_L);
  data.s3 = ADC_read(SENSOR_M);
  data.s4 = ADC_read(SENSOR_R);
  data.s5 = ADC_read(SENSOR_G);
  digitalWrite(TR_OUT,  LOW);

  return data;
}

diff_t get_sensor_status(void) {
  diff_t data;

  data.on  = sensor_read(HIGH);
  data.off = sensor_read(LOW);
  data.diff.s1 = data.on.s1 - data.off.s1;
  data.diff.s2 = data.on.s2 - data.off.s2;
  data.diff.s3 = data.on.s3 - data.off.s3;
  data.diff.s4 = data.on.s4 - data.off.s4;
  data.diff.s5 = data.on.s5 - data.off.s5;

  return data;
}

normalized_line_t get_normalized_sensor_value(void) {
  normalized_line_t data;
  diff_t diff;

  diff = get_sensor_status();

  data.s1 = (float)(diff.diff.s1 - data_min.s1)/(data_max.s1 - data_min.s1);
  data.s2 = (float)(diff.diff.s2 - data_min.s2)/(data_max.s2 - data_min.s2);
  data.s3 = (float)(diff.diff.s3 - data_min.s3)/(data_max.s3 - data_min.s3);
  data.s4 = (float)(diff.diff.s4 - data_min.s4)/(data_max.s4 - data_min.s4);
  data.s5 = (float)(diff.diff.s5 - data_min.s5)/(data_max.s5 - data_min.s5);

  return data;
}

uint8_t get_mode_number(void) {
  int8_t num = 0;
  uint32_t mode, exec;
  bool number_isDeterdata_mined = false;

  Serial.print("Please select a mode and execute\n");

  while(!number_isDeterdata_mined) {
    /* スイッチの状態を読み取って変数をいじる: */
    mode = (mode << 1) + digitalRead(SW_MODE_IN);
    exec = (exec << 1) + digitalRead(SW_EXEC_IN);

    /* EXEC スイッチが押されたらループを抜ける */
    if((exec & 0b111) == 0b100) {
      uint16_t flash_delay = 200;
      LED_num(0);
      delay(flash_delay);
      LED_num(7);
      delay(flash_delay);
      LED_num(0);
      delay(flash_delay);
      LED_num(7);
      delay(flash_delay);
      LED_num(0);
      
      number_isDeterdata_mined = true;
    }
    
    /* 現在の num の値を表示 */
    LED_num(num);

    /* MODE スイッチが押されたら num を加算 */
    if((mode & 0b111) == 0b100) {
      num += 1;
    }

    /* MODE スイッチが長押しされたら num を加算 */
    if((mode & 0xffff) == 0) {
      num += 1;
      delay(75);
    }
    
    /* num が範囲外に出たらもとに戻す */
    if(num >= 8) {
      num = 0;
    }

    delay(25);
  }

  char buffer[8];
  sprintf(buffer, "mode%2d\n", num);
  Serial.print(buffer);

  delay(500);

  LED_num(0);

  return num;
}

float get_battery_voltage(void) {
  uint16_t bat_raw = ADC_read(POWER_LEVEL);

  float voltage = float(bat_raw * 4 * 3.3) / 4096.0;

  return voltage;
}

void change_motor_voltage(int16_t L, int16_t R) {
  int16_t Vmotor_L, Vmotor_R;
  float Vbat = get_battery_voltage();

  /* 指令値がリミットを超えたときの処理 */
  if(Vmotor_L >  MOTOR_VOLTAGE_LIMIT) Vmotor_L =  MOTOR_VOLTAGE_LIMIT;
  if(Vmotor_R >  MOTOR_VOLTAGE_LIMIT) Vmotor_R =  MOTOR_VOLTAGE_LIMIT;
  if(Vmotor_L < -MOTOR_VOLTAGE_LIMIT) Vmotor_L = -MOTOR_VOLTAGE_LIMIT;
  if(Vmotor_R < -MOTOR_VOLTAGE_LIMIT) Vmotor_R = -MOTOR_VOLTAGE_LIMIT;
  
  Vmotor_L = (L * 256) / (Vbat * 1000);
  Vmotor_R = (R * 256) / (Vbat * 1000);

  /* 左側のモータの制御 */
  if(Vmotor_L > 0) {
    digitalWrite(DRV8835_AIN1_OUT,  LOW); analogWrite(DRV8835_AIN2_OUT,  Vmotor_L);
  }else if(Vmotor_L < 0) {
    digitalWrite(DRV8835_AIN1_OUT, HIGH); analogWrite(DRV8835_AIN2_OUT, -Vmotor_L);
  }else if(Vmotor_L == 0) {
    digitalWrite(DRV8835_AIN1_OUT,  LOW); analogWrite(DRV8835_AIN2_OUT,         0);
  }

  /* 右側のモータの制御 */
  if(Vmotor_R > 0) {
    digitalWrite(DRV8835_BIN1_OUT,  LOW); analogWrite(DRV8835_BIN2_OUT,  Vmotor_R);
  }else if(Vmotor_R < 0) {
    digitalWrite(DRV8835_BIN1_OUT, HIGH); analogWrite(DRV8835_BIN2_OUT, -Vmotor_R);
  }else if(Vmotor_R == 0) {
    digitalWrite(DRV8835_BIN1_OUT,  LOW); analogWrite(DRV8835_BIN2_OUT,         0);
  }

  /* モータ電圧指令値のデバッグ */
  char buffer[64];
  sprintf(buffer, "L = %5d, R = %5d, Vmotor_L = %4d, Vmotor_R = %4d\n", L, R, Vmotor_L, Vmotor_R);
  Serial.print(buffer);
}

void check_battery(bool skip_enable) {
  float Vbat = get_battery_voltage();
  const float thres = 3.0 * BATTERY_CELL_COUNT;

  const float th1 = 3.1 * BATTERY_CELL_COUNT;
  const float th2 = 3.5 * BATTERY_CELL_COUNT;
  const float th3 = 3.9 * BATTERY_CELL_COUNT;

  digitalWrite(LED1_OUT, (Vbat > th1) ? HIGH : LOW);
  delay(150);
  digitalWrite(LED2_OUT, (Vbat > th2) ? HIGH : LOW);
  delay(150);
  digitalWrite(LED3_OUT, (Vbat > th3) ? HIGH : LOW);
  delay(300);
  
  digitalWrite(LED1_OUT,  LOW);
  digitalWrite(LED2_OUT,  LOW);
  digitalWrite(LED3_OUT,  LOW);

  while((digitalRead(SW_MODE_IN) == HIGH) && (digitalRead(SW_EXEC_IN) == HIGH) && (Vbat < thres) && !skip_enable) {
    buzzer(440);
    LED_num(7);
    delay(50);
    LED_num(0);
    delay(50);
    buzzer_off();
    LED_num(7);
    delay(50);
    LED_num(0);
    delay(50);
  }

  delay(100);
}

void flash_LED(void) {
  normalized_line_t sensor = get_normalized_sensor_value();

  digitalWrite(LED1_OUT, sensor.s2 > 0.5 ? HIGH : LOW);
  digitalWrite(LED2_OUT, sensor.s3 > 0.5 ? HIGH : LOW);
  digitalWrite(LED3_OUT, sensor.s4 > 0.5 ? HIGH : LOW);
}
