#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <SimpleDHT.h>

#define CHANNEL_ID 1161684     // replace 0000000 with your channel number
#define WRITE_APIKEY "YTP7PWZGO0G2RU00"   // replace XYZ with your channel write API Key

char ssid[]= "dragino-1c0ad8";
char pass[]= "dragino-dragino";
//int keyIndex = 0;    // indice el la KEY de red en caso de WEP
WiFiClient  client;

unsigned long myChannelNumber = CHANNEL_ID;
const char * myWriteAPIKey = WRITE_APIKEY;
unsigned int counterFieldNumber = 1;

int x;
String texto;

// Inicializa valores
int temperature = 0;
int humidity = 0;
int number = 100;
int pinDHT11 = 2;
SimpleDHT11 dht11;

void setup() {
  Serial.begin(9600);  //Inicializa serie
  
  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);  // Inicializa ThingSpeak 

     // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Intentando conectarse a la red con SSID: ");
    Serial.println(ssid);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Conecta a la red con WPA/WPA2. Cambiar esta línea en caso de red abierta o con WEP.
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConectado.");
  }
}

void loop() {
  int statusCode = 0;

// read with raw sample data.
  byte temperature = 0;
  byte humidity = 0;
  byte data[40] = {0};
  dht11.read(pinDHT11, &temperature, &humidity, data);
  Serial.print((int)temperature); Serial.print(","); Serial.println((int)humidity);
  
 // DHT11 sampling rate is 1HZ.
  delay(2000);

//************ Envios
// Guarda los valores correspondientes a los 4 primeros campos
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  // envia y escribe en el canal ThingSpeak
  x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Datos múltiples enviados correctamente");
  }
  else{
    Serial.println("Problema al enviar los datos múltiples. HTTP error code " + String(x));
  }

  delay(20000); // Espera 20 segundos antes de volver a enviar
  
  x = ThingSpeak.writeField(myChannelNumber, 5, number, myWriteAPIKey);
  if(x == 200){
    Serial.print("Dato enviado correctamente: ");
    Serial.println(number);
  }
  else{
    Serial.println("Problema al enviar el dato. HTTP error code " + String(x));
  }

  delay(20000); // Espera 20 segundos antes de volver a enviar 
}
