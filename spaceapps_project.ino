#include <Wire.h>
#include <SeeedGrayOLED.h>
#include <DHT.h>
#include <LED_Bar.h>

const int buzzerPin =  2;
LED_Bar bar(9, 8); // config Io here, (clk, dio)

#define DHTPIN A0     // what pin we're connected to 
#define DHTTYPE DHT11   // DHT 11 
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Wire.begin();	
  dht.begin();
  SeeedGrayOled.init();
  SeeedGrayOled.clearDisplay();           //clear the screen and set start position to top left corner
  SeeedGrayOled.setNormalDisplay();       //Set display to Normal mode
  SeeedGrayOled.setVerticalMode();
}


float Vmin = .075;
float Vmax = 0;

float Vc = 4.95;
float Rs = 0;
float R0 = 0;
float Ratio = 0;

float maxRatio = 1.0;
float barScale = 0;
int barLevel = 0;

void loop()
{
  // VOC Sensor
  int sensorValue = analogRead(A3);
  float Vnow = sensorValue*4.95/1023;

  if(Vnow < Vmin){
    Vmin = Vnow;
  }

  if(Vnow > Vmax){
    Vmax = Vnow;
  }

  SeeedGrayOled.setTextXY(0,0);
  SeeedGrayOled.putString("Vnow= ");
  char VnowChar[6];
  dtostrf(Vnow, 1, 3, VnowChar);
  SeeedGrayOled.putString(VnowChar);

  SeeedGrayOled.setTextXY(1,0);
  SeeedGrayOled.putString("Vmin= ");
  char VminChar[6];
  dtostrf(Vmin, 1, 3, VminChar);
  SeeedGrayOled.putString(VminChar);

  SeeedGrayOled.setTextXY(2,0);
  SeeedGrayOled.putString("Vmax= ");
  char VmaxChar[6];
  dtostrf(Vmax, 1, 3, VmaxChar);
  SeeedGrayOled.putString(VmaxChar);

  Rs = Vc / Vmax - 1; 
  R0 = Vc / Vmin - 1;

  SeeedGrayOled.setTextXY(4,0);
  SeeedGrayOled.putString("Rs/R0=");
  char RatioCharChar[6];
  dtostrf(Rs/R0, 2, 3, RatioCharChar);
  SeeedGrayOled.putString(RatioCharChar);

  // led bar
  barScale = (maxRatio - Vnow) / maxRatio;

  if(barScale < 0){
    barScale = 0;
  } 
  else if(barScale > 1){
    barScale = 1;
  }

  //update LED
  barLevel = ceil(barScale * 10);
  bar.setLevel(barLevel);

  // update OLED
  SeeedGrayOled.setTextXY(6,0);
  SeeedGrayOled.putString("level=");
  char barScaleChar[6];
  dtostrf(barLevel, 4, 1, barScaleChar);
  SeeedGrayOled.putString(barScaleChar);

  // buzzer
  if (barLevel < 3) {     
    // turn buzzer on:    
    digitalWrite(buzzerPin, HIGH);  
  } 
  else {
    // turn buzzer off:
    digitalWrite(buzzerPin, LOW); 
  }

  // T and H sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature() * .96 * 9/5 + 32;
  char tempF[6];
  char rhPct[6];

  SeeedGrayOled.setTextXY(10,0);
  dtostrf(t, 2, 1, tempF);
  SeeedGrayOled.putString("T= ");
  SeeedGrayOled.putString(tempF);
  SeeedGrayOled.putString("F");

  SeeedGrayOled.setTextXY(11,0);
  dtostrf(h, 2, 1, rhPct);
  SeeedGrayOled.putString("H= ");
  SeeedGrayOled.putString(rhPct);
  SeeedGrayOled.putString("%");

  delay(500);

}











