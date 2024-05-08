

// Blynk Connection Details Definition
#define BLYNK_TEMPLATE_ID "TMPL6Uainf7Cg"
#define BLYNK_TEMPLATE_NAME "Weather Station"
#define BLYNK_AUTH_TOKEN "RnGgbFRHwYjvzF-gZq3ZxIKn9xWOAaUs"


//Required Package Declaration
#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <DHT.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "************"; //WIFI SSID (Redacted in public view for privacy reasons)
char pass[] = "**************"; //WIFI Password (Redacted in public view for privacy reasons)

BlynkTimer timer;

#define DHT_PIN 4           // Pin where the DHT11 is connected
#define WATER_LEVEL_PIN 36  // Pin where the Water Level sensor is connected

DHT dht(DHT_PIN, DHT11); // DHT11 Pin Configuration

LiquidCrystal lcd(22, 23, 14, 27, 26, 25); //LCD Pin Configuration

float humidity; //Global humidity value
float temperature; //Global temperature value
int waterLevel; //Global water level value
int selectedOption;  //Selected Option value

void sendSensor() //Sensor send to Blynk App
{

  Blynk.virtualWrite(V1, waterLevel);
  Blynk.virtualWrite(V2, temperature);
  Blynk.virtualWrite(V3, humidity);
  

}


void setup() {

  
  connectToWiFi(); // Connect to Wi-Fi
  Serial.begin(115200); //Begin serial transmission for debugging purposes

  

  
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass); // Initialize Blynk

  
  timer.setInterval(100L, sendSensor); // Initialize Sensor Data Send Stream 
  

}

BLYNK_WRITE(V0){ // Blynk write send for Temperature / Humidity / Water Level option selection
  lcd.clear();
  int optionsSliderPin = param.asInt();

  selectedOption = param.asInt();

}

BLYNK_WRITE(V4){ //Blynk write send for ALl OPTION option selection
  lcd.clear();

  selectedOption = 4;



void loop() {
  
  
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();        //Continous Sensor Read
  waterLevel = analogRead(WATER_LEVEL_PIN);



  lcd.clear(); // Clear Currently Showing LCD Characters



  if(selectedOption == 1){  //Water Level Option (Slider to the Left)
    lcd.print("WL ");
    lcd.print(waterLevel);
  }else if(selectedOption == 2){ //Temperature Option (Slider to the Middle)
    lcd.print("T: ");
    lcd.print(temperature);
    lcd.print("C");
  }else if (selectedOption ==3){   //Humidity Option (Slider to the Right)
    lcd.print("Hum:  ");
    lcd.print(humidity);
  } else { //ALL OPTIONS Option (Middle button pressed)
    lcd.print(waterLevel);
    lcd.print(" ");
    lcd.print(temperature);
    lcd.print(" ");
    lcd.print(humidity);
    
  }



  Blynk.run(); // Run Blink 
  timer.run(); // Timer Run

  delay(500); // Half second delay in reading and data send
}



void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi");

  WiFi.begin(ssid, pass);  //Attempt Connect to Wifi using previous declared WIFI Details
                                              
  lcd.print("Connecting...");

  while (WiFi.status() != WL_CONNECTED) {  //Attempt Wifi Connection every second
    
    Serial.print(".");
    delay(1000);  
  }

  Serial.println("\nConnected to Wi-Fi");  // Serial to show succesful connection to WIFI
  lcd.clear();  // Clear the LCD screen
  
  lcd.print("WiFi Connected"); // LCD to show succesful connection to WIFI

}
