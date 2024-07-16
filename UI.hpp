#pragma once

#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

void handleRoot() {
  char html[2048];
  sprintf(html, "<html><body><h1>Tracer-S3 Controler</h1><form action=\"/PGainUp\" method=\"POST\"><button type=\"submit\">P Gain UP</button></form><form action=\"/PGainDown\" method=\"POST\"><button type=\"submit\">P Gain Down</button></form><p>Kp = %f</p><form action=\"/DGainUp\" method=\"POST\"><button type=\"submit\">D Gain Up</button></form><form action=\"/DGainDown\" method=\"POST\"><button type=\"submit\">D Gain Down</button></form><p>Kd = %f</p><form action=\"/BaseUp\" method=\"POST\"><button type=\"submit\">Base Speed Up</button></form><form action=\"/BaseDown\" method=\"POST\"><button type=\"submit\">Base Speed Down</button></form><p>base_speed = %d</p><form action=\"/DiffUp\" method=\"POST\"><button type=\"submit\">Diff Up</button></form><form action=\"/DiffDown\" method=\"POST\"><button type=\"submit\">Diff Down</button></form><p>speed_diff = %d</p></body></html>", Kp, Kd, base_speed, speed_diff);
  server.send(200, "text/html", (String)html);
}

void PGainUp() {
  Kp += 0.05;
  if(Kp > 2.0f) Kp = 2.0f;
  server.sendHeader("Location", "/");
  server.send(303);
}

void PGainDown() {
  Kp -= 0.05;
  if(Kp < 0) Kp = 0.0f;
  server.sendHeader("Location", "/");
  server.send(303);
}

void DGainUp() {
  Kd += 0.05;
  if(Kp > 2.0f) Kp = 2.0f;
  server.sendHeader("Location", "/");
  server.send(303);
}

void DGainDown() {
  Kd -= 0.05;
  if(Kp < 0) Kp = 0.0f;
  server.sendHeader("Location", "/");
  server.send(303);
}

void BaseUp() {
  base_speed += 50;
  if(base_speed > 2000) base_speed = 2000;
  server.sendHeader("Location", "/");
  server.send(303);
}

void BaseDown() {
  base_speed -= 50;
  if(base_speed < 1000) base_speed = 1000;
  server.sendHeader("Location", "/");
  server.send(303);
}

void DiffUp() {
  speed_diff += 10;
  if(speed_diff > 1000) speed_diff = 1000;
  server.sendHeader("Location", "/");
  server.send(303);
}

void DiffDown() {
  speed_diff -= 10;
  if(speed_diff < 0) speed_diff = 0;
  server.sendHeader("Location", "/");
  server.send(303);
}
