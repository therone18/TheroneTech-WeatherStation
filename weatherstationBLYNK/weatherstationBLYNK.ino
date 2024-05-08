#define BLYNK_TEMPLATE_ID "TMPL6Uainf7Cg"
#define BLYNK_TEMPLATE_NAME "Weather Station"
#define BLYNK_AUTH_TOKEN "RnGgbFRHwYjvzF-gZq3ZxIKn9xWOAaUs"

#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <DHT.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "************";
char pass[] = "**************";

BlynkTimer timer;

#define DHT_PIN 4           // Pin where the DHT11 is connected
#define WATER_LEVEL_PIN 36  // Pin where the Water Level sensor is connected
#define LCD_RS 12
#define LCD_EN 11
#define LCD_D4 5
#define LCD_D5 4
#define LCD_D6 3
#define LCD_D7 2

DHT dht(DHT_PIN, DHT11);
//LiquidCrystal lcd(22, 23, 5, 18, 19, 21);
//LiquidCrystal lcd(22, 23, 25, 32, 35, 34);
LiquidCrystal lcd(22, 23, 14, 27, 26, 25);

float humidity;
float temperature;int waterLevel;
int selectedOption;

void sendSensor()
{

  Blynk.virtualWrite(V1, waterLevel);
  Blynk.virtualWrite(V2, temperature);
  Blynk.virtualWrite(V3, humidity);
  

}


void setup() {

  // Connect to Wi-Fi
  connectToWiFi();
  Serial.begin(115200);

  

  // Initialize Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Initialize Sensor Data Send Stream 
  timer.setInterval(100L, sendSensor);
  

}

BLYNK_WRITE(V0){
  lcd.clear();
  int optionsSliderPin = param.asInt();

  selectedOption = param.asInt();


  //lcd.print(optionsSliderPin);
}

BLYNK_WRITE(V4){
  lcd.clear();
  //int optionsSliderPin = param.asInt();

  selectedOption = 4;


  //lcd.print(optionsSliderPin);
}

void loop() {
  
  humidity = dht.readHumidity();
  temperature = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  waterLevel = analogRead(WATER_LEVEL_PIN);
  lcd.clear();
  if(selectedOption == 1){
    lcd.print("WL ");
    lcd.print(waterLevel);
  }else if(selectedOption == 2){
    lcd.print("T: ");
    lcd.print(temperature);
    lcd.print("C");
  }else if (selectedOption ==3){
    lcd.print("Hum:  ");
    lcd.print(humidity);
  } else {
    lcd.print(waterLevel);
    lcd.print(" ");
    lcd.print(temperature);
    lcd.print(" ");
    lcd.print(humidity);
    
  }


  
  //lcd.print(selectedOption);

  

  Blynk.run();
  timer.run();

  delay(500);
}

void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi");

  WiFi.begin(ssid, pass);

  lcd.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    
    Serial.print(".");
    delay(1000);  // Adjust delay if needed
  }

  Serial.println("\nConnected to Wi-Fi");
  lcd.clear();  // Clear the LCD screen
  
  lcd.print("WiFi Connected");

  if (Blynk.connected()) {
    Serial.println("Connected to Blynk");
    lcd.print("WiFi-Blynk Connected");
  } else {
    Serial.println("Not connected to Blynk");
  }
}
