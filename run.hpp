#pragma once

void run0(void) {
  diff_t sensor;

  Serial.print("sensor Calibration will run While \"EXEC\" is pressing.\n");

  while(digitalRead(SW_EXEC_IN) == HIGH);
  while(digitalRead(SW_EXEC_IN) ==  LOW) {
    sensor = get_sensor_status();

    char buffer[64];
    sprintf(buffer, "%5d, %5d, %5d, %5d, %5d\n", sensor.diff.s1, sensor.diff.s2, sensor.diff.s3, sensor.diff.s4, sensor.diff.s5);
    Serial.print(buffer);

    if(sensor.diff.s1 < data_min.s1) {
      data_min.s1 = sensor.diff.s1;
    }
    if(sensor.diff.s2 < data_min.s2) {
      data_min.s2 = sensor.diff.s2;
    }
    if(sensor.diff.s3 < data_min.s3) {
      data_min.s3 = sensor.diff.s3;
    }
    if(sensor.diff.s4 < data_min.s4) {
      data_min.s4 = sensor.diff.s4;
    }
    if(sensor.diff.s5 < data_min.s5) {
      data_min.s5 = sensor.diff.s5;
    }

    if(sensor.diff.s1 > data_max.s1) {
      data_max.s1 = sensor.diff.s1;
    }
    if(sensor.diff.s2 > data_max.s2) {
      data_max.s2 = sensor.diff.s2;
    }
    if(sensor.diff.s3 > data_max.s3) {
      data_max.s3 = sensor.diff.s3;
    }
    if(sensor.diff.s4 > data_max.s4) {
      data_max.s4 = sensor.diff.s4;
    }
    if(sensor.diff.s5 > data_max.s5) {
      data_max.s5 = sensor.diff.s5;
    }

    delay(50);
  }
  
  Serial.print("\nsensor level :   min /   max\n");
  char buffer[40];
  sprintf(buffer, "     sensor%d : %5d / %5d\n", 1, data_min.s1, data_max.s1);
  Serial.print(buffer);
  sprintf(buffer, "     sensor%d : %5d / %5d\n", 2, data_min.s2, data_max.s2);
  Serial.print(buffer);
  sprintf(buffer, "     sensor%d : %5d / %5d\n", 3, data_min.s3, data_max.s3);
  Serial.print(buffer);
  sprintf(buffer, "     sensor%d : %5d / %5d\n", 4, data_min.s4, data_max.s4);
  Serial.print(buffer);
  sprintf(buffer, "     sensor%d : %5d / %5d\n", 5, data_min.s5, data_max.s5);
  Serial.print(buffer);

  return;
}

void run1(void) {
  change_motor_voltage( 3000,  3000);
  delay(1000);
  change_motor_voltage(-3000, -3000);
  delay(1000);
  change_motor_voltage(    0,     0);

  return;
}

void run2(void) {
  while((digitalRead(SW_MODE_IN) == HIGH) && (digitalRead(SW_EXEC_IN) == HIGH)) {
    normalized_line_t sensor = get_normalized_sensor_value();

    flash_LED();

    char buffer[64];
    sprintf(buffer, "%f, %f, %f, %f, %f\n", sensor.s1, sensor.s2, sensor.s3, sensor.s4, sensor.s5);
    Serial.print(buffer);

    delay(50);
  }

  delay(250);

  return;
}

void run3(void) {
  while((digitalRead(SW_EXEC_IN) == HIGH) && (digitalRead(SW_EXEC_IN) == HIGH)) {
    diff_t sensor = get_sensor_status();

    char buffer[64];
    sprintf(buffer, "%5d, %5d, %5d, %5d, %5d\n", sensor.diff.s1, sensor.diff.s2, sensor.diff.s3, sensor.diff.s4, sensor.diff.s5);
    Serial.print(buffer);

    delay(50);
  }

  delay(250);

  return;
}

void run4(void) {
  change_motor_voltage(2500, 2500);  // 始動トルク
  delay(200);
  while((digitalRead(SW_MODE_IN) == HIGH) && (digitalRead(SW_EXEC_IN) == HIGH)) {
    static float prev_error;

    normalized_line_t sensor = get_normalized_sensor_value();

    float L = sensor.s2;
    float R = sensor.s4;

    float error = L - R;
    float error_diff = error - prev_error;
          prev_error = error;

    float PD = Kp * error + Kd * error_diff;

    char buffer[32];
    sprintf(buffer, "%f, %f, %f\n", L, R, PD);
    Serial.print(buffer);

    change_motor_voltage(base_speed - uint16_t(speed_diff * PD), base_speed + uint16_t(speed_diff * PD));
    flash_LED();

    delay(1);
  }

  change_motor_voltage(0, 0);

  return;
}

void run5(void) {
}

void run6(void) {
}

void run7(void) {
  while((digitalRead(SW_MODE_IN) == HIGH) && (digitalRead(SW_EXEC_IN) == HIGH)) {
    server.handleClient();
  }
}