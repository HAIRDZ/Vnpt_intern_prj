#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "TEST01";      // id va pass
const char* password = "12345678";

WebServer server(80);                  // 80 = default unencrypted HTTP traffic
const int ledPin   = 2;                // led tren board

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_AP);             // apmode= phat wifi, dia chi ip mac dinh apmode = 192.168.4.1
  bool result = WiFi.softAP(ssid, password);  // ham softap nhan 2 tham so ssid, pass , khoi tao thanh cong => result = true
  if (result) {
    Serial.print("Dia chi IP: ");
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("khoi tao fail");
  }

  pinMode(ledPin, OUTPUT);

  // Dat ten route
  server.on("/",         HTTP_GET, handleRoot);    // => route = http://192.168.4.1/  function = handleRoot => Trang chu
  server.on("/led/on",   HTTP_GET, handleLedOn);   // bat LED
  server.on("/led/off",  HTTP_GET, handleLedOff);  // tat LED

  server.begin();
}

void loop() {
  server.handleClient();  // neu co client moi => ham ghi lai dia chi ip cua client,
                          // status cua client chuyen tu none sang read
                          // neu khong co client => ham tao 1 ham loop lien tuc kiem tra xem co client
}

void handleRoot() {
  String html = "<html><body><h1>LED CONTROL ESP32</h1>";
  html += "<a href='/led/on'>BAT LED</a><br>";
  html += "<a href='/led/off'>TAT LED</a><br>";
  html += "</body></html>";
  server.send(200, "text/html", html);  // send(code, content_type, String& content)
                                         // code = 200 => flag xu ly thanh cong
}

// Bat led
void handleLedOn() {
  digitalWrite(ledPin, HIGH);
  server.send(200, "text/html",
              "<html><body><h1>LED BAT</h1><a href='/'>back</a></body></html>"); //tra ve trang html
}

// tat led
void handleLedOff() {
  digitalWrite(ledPin, LOW);
  server.send(200, "text/html",
              "<html><body><h1>LED TAT</h1><a href='/'>Quay lai</a></body></html>"); 
}
