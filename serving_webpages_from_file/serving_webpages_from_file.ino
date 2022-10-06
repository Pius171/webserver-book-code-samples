

#include <Arduino.h>
#include <FS.h>
#ifdef ESP32 // check if the board is ESP32
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
// to disable brownout detector for esp32
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <LittleFS.h>
#include <ESP8266WebServer.h>
#endif

const char* AP_ssid = "Pius Electronics"; // the name of your AP wifi
const char* AP_password = "12345678";

// create a server object, you can call it anything, but i will be calling it server
//WebServer server(80); // for esp32
ESP8266WebServer server(80); // uncomment if you are using esp8266


void handleRoot() {
 send_file("index.html","text/html");
  // do other stuff
  
}

void handleNotFound() {
  server.send(404, "text/plain", "page not found");

}

void hello() {
 
  server.send(200, "text/plain", "Hello Pius");     
}

void send_file(const char* filename, String content_type) {
  File file1 = LittleFS.open(filename, "r");
  if (!file1) {
    char buffer[100];
    sprintf(buffer, "failed to open %s for reading", filename);
    Serial.println(buffer);

  } else {
    server.streamFile(file1, content_type);
    char buffer[100];
    sprintf(buffer, "%s file sent", filename);
    Serial.println(buffer);
  }
}

void setup(void) {
  // WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // disable brownout detector of esp32
  // comment out line 40 if using esp8266
  Serial.begin(115200);
  delay(500); // pause for a while to allow Serial to initialise
  Serial.println();// print a new line

  // mount LITTLEFS
  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed");
    return;
  }

  //WiFi.mode(WIFI_STA); //uncomment if your are using station mode

  //Uncomment one of the lines depending on the mode you want to use
  // uncomment line 47-52 if suing STA mode
  //WiFi.begin("your router ssid" , "your router's password");// for station mode
  //  Serial.println("connecting");
  //  while(WiFi.status()!= WL_CONNECTED){
  //    Serial.print(".");
  //  }
  //  Serial.println("connected");

  WiFi.softAP(AP_ssid, AP_password); // for AP mode

  Serial.print("IP address: ");
  //Uncomment one of the lines depending on the mode you want to use
  //Serial.println(WiFi.localIP()); // STA mode
  Serial.println(WiFi.softAPIP()); // AP mode



  // call your server from your setup
  server.on("/", handleRoot);
  // The "/" represents your home page
  // This is the page you are sent to
  // when you type in your IP address
  // so when you type in your ip address handleRoot function is called

  server.on("/hello", hello);
  //type your_ip_address/hello in your browser trigger this response

  server.onNotFound(handleNotFound); // when the page your enetered is not found, handleNotFound function is called

  server.begin(); // start your server
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient(); // keep the server running in the loop
  delay(2);//allow the cpu to switch to other tasks
}
