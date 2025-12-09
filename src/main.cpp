#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

String ssidName = "ESP32S3_AP";
int repeatCount = 1;

const char PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <body>
    <h2>Set WiFi Name & Repeat Count</h2>
    <form action="/set">
      WiFi Name: <input name="name"><br>
      Repeat Count: <input name="count"><br>
      <input type="submit" value="Set">
    </form>
  </body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", PAGE);
}

void handleSet() {
  if (server.hasArg("name")) {
    ssidName = server.arg("name");
  }
  if (server.hasArg("count")) {
    repeatCount = server.arg("count").toInt();
  }
  String msg = "Setting WiFi: " + ssidName + "<br>Repeat: " + String(repeatCount);
  server.send(200, "text/html", msg);
  delay(500);
  WiFi.softAPdisconnect(true);
  for (int i = 0; i < repeatCount; i++) {
    WiFi.softAP(ssidName.c_str());
    delay(1000);
    WiFi.softAPdisconnect(true);
    delay(500);
  }
  WiFi.softAP(ssidName.c_str());
}

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssidName.c_str());
  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.begin();
  Serial.println("Access point started: " + ssidName);
}

void loop() {
  server.handleClient();
}
