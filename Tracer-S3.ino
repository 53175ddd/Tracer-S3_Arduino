#include "constant.hpp"
#include "S3.hpp"
#include "UI.hpp"
#include "run.hpp"

void setup() {
  Serial.begin(115200);
  delay(100);
  LED_init();
  ADC_init();
  IR_init();
  motor_init();
  SW_init();
  buzzer_init();

  /* WebServer ここから */
  WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/PGainUp",   HTTP_POST, PGainUp);
  server.on("/PGainDown", HTTP_POST, PGainDown);
  server.on("/DGainUp",   HTTP_POST, DGainUp);
  server.on("/DGainDown", HTTP_POST, DGainDown);
  server.on("/BaseUp",    HTTP_POST, BaseUp);
  server.on("/BaseDown",  HTTP_POST, BaseDown);
  server.on("/DiffUp",    HTTP_POST, DiffUp);
  server.on("/DiffDown",  HTTP_POST, DiffDown);

  server.begin();
  Serial.println("HTTP server started");

  /* WebServer ここまで */

  Serial.print("All setup sequences done\n");
}

void loop() {
  check_battery(false);  // true 指定で過放電検出無効

  uint8_t mode = get_mode_number();

  switch(mode) {
    case 0:
      run0();  // センサの補正
      break;

    case 1:
      run1();  // モータテスト
      break;

    case 2:
      run2();  // ADC デバッグ（正規化）
      break;

    case 3:
      run3();  // ADC デバッグ（生の値）
      break;

    case 4:
      run4();  // PD 制御
      break;

    case 5:
      run5();
      break;

    case 6:
      run6();
      break;

    case 7:
      run7();  // 制御用 Web サーバ実行
      break;
  }
}
