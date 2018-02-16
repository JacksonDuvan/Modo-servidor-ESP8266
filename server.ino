#include <ESP8266WiFi.h>

const char * ssid = "ARRIS-2692";
const char * password = "**********";
const int Bombillo = 16;

WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  delay(10);

  pinMode(Bombillo,OUTPUT);
  digitalWrite(Bombillo,LOW);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  Serial.println();

  Serial.print("Conectando con ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP address: ");
  IPAddress sensor_ip = WiFi.localIP();
  digitalWrite(Bombillo,HIGH);

  server.begin();
  Serial.print("Servidor WEB activo: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  if (!client){
    return;
    }  

  Serial.println("Cliente Conectado");
  while (!client.available()){
    delay(1);
    }
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  int val;
  if (req.indexOf("/off") != -1)
     val = 0;
  else if (req.indexOf("/on") != -1)
     val = 1;
  else {
    Serial.print("Peticion Invalida");
    client.stop();
    return;
    }
  digitalWrite(Bombillo,val);
  client.flush();

  String s = "HTTP/1.1 200 0K\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nBombillo ";
  s += (val)?"encendido" : "apagado";
  s += "</html>\n";

  client.print(s);
  delay(1);
  Serial.println("Cliente Desconectado");
}
