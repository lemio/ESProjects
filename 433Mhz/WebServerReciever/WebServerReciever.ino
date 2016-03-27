#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <RCSwitch.h>
extern "C" {
#include "user_interface.h"
}
RCSwitch mySwitch = RCSwitch();
const char* ssid = "aastvej";
const char* password = "1a2b3c4d5e6f7";

ESP8266WebServer server(80);

const int led = 13;
String message = "";
void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/html", "<script type=\"text/javscript\">setInterval(function (){location.reload()},100);</script>hello from esp8266!<br />" + message);
  message = "";
  message.remove(0);
  digitalWrite(led, 0);
}

void handleNotFound(){
  digitalWrite(led, 1);
  /*
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }*/
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void){
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");
  mySwitch.enableReceive(D4);
 Serial.print("Opening port 12 for Messages");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
 
  if (mySwitch.available()) {
    
    int value = mySwitch.getReceivedValue();
    
    if (value == 0) {
      Serial.print("Unknown encoding");
    } else {
      Serial.print("Received ");
      Serial.print( mySwitch.getReceivedValue() );
      Serial.print(" / ");
      Serial.print( mySwitch.getReceivedBitlength() );
      Serial.print("bit ");
      Serial.print("Heap: ");
      message.concat( mySwitch.getReceivedValue());
      message.concat( "<br />");
      Serial.println( system_get_free_heap_size() );
    }

    mySwitch.resetAvailable();
  }else{
     server.handleClient();
    }
}
