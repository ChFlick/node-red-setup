#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <Wire.h>

#define wifi_ssid "FRITZ!Box 7490"
#define wifi_password "Flutz2017cc"

#define mqtt_server "192.168.178.56"
#define mqtt_user ""      // if exist
#define mqtt_password ""  //idem

#define temperature_topic "sensor/temperature"  //Topic temperature
#define humidity_topic "sensor/humidity"        //Topic humidity
#define gas_topic "sensor/gas"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//Buffer to decode MQTT messages
char message_buff[100];

long lastMsg = 0;   
long lastRecu = 0;
bool debug = true;  //Display log message if True

#define DHTPIN D4    // DHT Pin 
#define LED_PIN D0

// Un-comment you sensor
//#define DHTTYPE DHT11       // DHT 11 
#define DHTTYPE DHT22         // DHT 22  (AM2302)
#define OLED_RESET 0  // GPIO0

DHT dht(DHTPIN, DHTTYPE);     
WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);     
  pinMode(A0, INPUT);
  pinMode(LED_PIN, OUTPUT);     //Pin 2 for LED
  setup_wifi();           //Connect to Wifi network
  client.setServer(mqtt_server, 1883);    // Configure MQTT connexion
  client.setCallback(callback);           // callback function to execute when a MQTT message   
  dht.begin();

  Serial.println("Initialize display!");
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
    Serial.println("Display error!");
  } else {
    Serial.println("Display connected!");
  }
  
  display.clearDisplay();

  //Add stuff into the 'display buffer'
  //display.setTextWrap(false);
  //display.setTextSize(1);
  //display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("teeest.party");
  //for(int16_t i=max(display.width(),display.height())/2; i>0; i-=5) {
  //  display.fillTriangle(
  //      display.width()/2  , display.height()/2-i,
  //      display.width()/2-i, display.height()/2+i,
  //      display.width()/2+i, display.height()/2+i, INVERSE);
  //}
 
  display.display(); //output 'display buffer' to screen  
  display.startscrollleft(0x00, 0x0F); //make display scroll 
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi OK ");
  Serial.print("=> ESP8266 IP address: ");
  Serial.print(WiFi.localIP());
}

void reconnect() {

  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker ...");
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("OK");
    } else {
      Serial.print("KO, error : ");
      Serial.print(client.state());
      Serial.println(" Wait 5 secondes before to retry");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();

  // Send a message every 10 seconds
  if (now - lastMsg > 1000 * 10) {
    lastMsg = now;
    // Read humidity
    float h = dht.readHumidity();
    // Read temperature in Celcius
    float t = dht.readTemperature();

    // Oh, nothing to send
    if ( isnan(t) || isnan(h)) {
      Serial.println("No climate info, please check DH Sensor!");
      return;
    }

    float gasRaw = analogRead(A0);

    if( isnan(gasRaw) ) {
      Serial.println("No gas info, please check MQ-5 Sensor");
      return;
    }
    
    float gas = (gasRaw/1023.0) * 3.3;
  
    if ( debug ) {
      Serial.print("Temperature : ");
      Serial.print(t);
      Serial.print(" | Humidity : ");
      Serial.print(h);
      Serial.print(" | Gas concentration : ");
      Serial.println(gas);
    }  
    client.publish(temperature_topic, String(t).c_str(), true);   // Publish temperature on temperature_topic
    client.publish(humidity_topic, String(h).c_str(), true);      // and humidity
    client.publish(gas_topic, String(gas).c_str(), true);      // and gas concentration
  }
  if (now - lastRecu > 100 ) {
    lastRecu = now;
    client.subscribe("homeassistant/switch1");
  }
}

// MQTT callback function
void callback(char* topic, byte* payload, unsigned int length) {

  int i = 0;
  if ( debug ) {
    Serial.println("Message =>  topic: " + String(topic));
    Serial.print(" | length: " + String(length,DEC));
  }
  // create character buffer with ending null terminator (string)
  for(i=0; i<length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';
  
  String msgString = String(message_buff);
  if ( debug ) {
    Serial.println("Payload: " + msgString);
  }
  
  if ( msgString == "ON" ) {
    digitalWrite(LED_PIN,HIGH);  
    if ( debug ) {
      Serial.println("Start LED");
    }
  } else {
    digitalWrite(LED_PIN,LOW); 
    if ( debug ) {
      Serial.println("Stop LED");
    } 
  }
}
