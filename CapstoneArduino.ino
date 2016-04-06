#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <NewPing.h>

#define TRIGGER_PIN_L  A0
#define ECHO_PIN_L A0
#define TRIGGER_PIN_R A1
#define ECHO_PIN_R A1
#define MAX_DISTANCE 200

NewPing sonarRight (TRIGGER_PIN_R, ECHO_PIN_R, MAX_DISTANCE);
NewPing sonarLeft(TRIGGER_PIN_L, ECHO_PIN_L, MAX_DISTANCE);
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

int intensity = 2, contrast = 50, option = 1, vibrMode = 1, vibrationIntensity=70, intensityLevel = 1, vibrationIntensityRight = 70, intensityLevelRight = 1;
int intensityValues[]= {80, 100, 130, 150, 160};
char input;
const int screenRefreshRate = 500;
int displayRevert = 0;
long lastScreenRefreshTime = 0;
//long displayRevert = 0;
const int analogOutPin = 9;
//const int analogOutPinRight = 10;
const int BASE_INTENSITY = 30;


void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13,LOW);
  display.begin();
  display.setContrast(contrast);
  display.display();
}

void loop() {

  if (Serial.available() > 0) {
    settingsUpdate();
  }

   delay(50);
   unsigned int uSL = sonarLeft.ping();
   unsigned int uSR = sonarRight.ping();
   int readingLeft = uSL / US_ROUNDTRIP_CM;
   int readingRight = uSR / US_ROUNDTRIP_CM;
   vibration(readingLeft, readingRight);
  
  //Normal screen refresh
  if (((millis() - lastScreenRefreshTime) > screenRefreshRate) && ((millis() - lastScreenRefreshTime) > displayRevert)) {
    lastScreenRefreshTime = millis();
    displayOutput(readingLeft, readingRight);
    displayRevert =0;

  }
    if (vibrMode == 1 && (readingLeft != 0 || readingRight != 0)) {
    //int vibrationIntensity = intensity * 4;
    analogWrite(analogOutPin, vibrationIntensity);
 //   analogWrite(analogOutPinRight, vibrationIntensityRight);
    }
    else
    {
          analogWrite(analogOutPin, 0);
          // analogWrite(analogOutPinRight, 0);

    }

}

void vibration(int readingLeft, int readingRight) {
  if (readingLeft <= 30)
   intensityLevel = 4; 
  else if (readingLeft > 30 && readingLeft <= 50)
    intensityLevel = 3;
  else if (readingLeft > 50 && readingLeft <= 90)
    intensityLevel = 2;
  else if  (readingLeft > 90 && readingLeft <= 130)
    intensityLevel = 1;
   else if (readingLeft > 130 && readingLeft <= 170)
    intensityLevel = 0;

if (readingRight <= 30)
   intensityLevelRight = 4; 
  else if (readingRight > 30 && readingRight <= 50)
    intensityLevelRight = 3;
  else if (readingRight > 50 && readingRight <= 90)
    intensityLevelRight = 2;
  else if  (readingRight > 90 && readingRight <= 130)
    intensityLevelRight = 1;
   else if (readingRight > 130 && readingRight <= 170)
    intensityLevelRight = 0;

    int finalValue = intensityLevel > intensityLevelRight? intensityLevel:intensityLevelRight;
    vibrationIntensity = intensityValues[finalValue];
    //vibrationIntensityRight = intensityValues[intensityLevelRight];
  //vibrationIntensity = (intensityLevel * intensity) + BASE_INTENSITY;
  
}


void settingsUpdate() {
  input = Serial.read();
  if (input == '1') {
    if (option == 2) {
      if (contrast > 0) {
        --contrast;
        display.setContrast(contrast);
      }
    }
    else if (option == 1) {
      if (intensity > 1)
      {
        --intensity;
        updateIntensityValues();

      }
    }
    else if (option  == 3) {
      if(vibrMode == 1 )
        vibrMode = 0;
    }
  }
  else if (input == '2') {
    if (option == 2) {
      if (contrast < 100 ) {
        ++contrast;
        display.setContrast(contrast);
      }
    }
    else if (option == 1) {
      if (intensity < 5){
        ++intensity;
        updateIntensityValues();
    }
    }
    else if (option == 3) {
      if (vibrMode == 0)
        vibrMode = 1;
    }
  }
  else if (input == '3') {
  
    if (option == 1)
      option = 2;
    else if (option == 2)
      option = 3;
    else 
    option =1;
  }
  //Refresh for update to reflect
  lastScreenRefreshTime = millis();
//  displayRevert = millis() + 12000;
  displayRevert = 3000;
  settingsDisplay();
}
void updateIntensityValues() {
  
      if (intensity == 1) {
            intensityValues[0] = 70;
            intensityValues[1] = 90;
            intensityValues[2] = 120;
            intensityValues[3] = 140;
            intensityValues[4] = 150;
       }
       else if (intensity == 2) {
            intensityValues[0] = 80;
            intensityValues[1] = 100;
            intensityValues[2] = 130;
            intensityValues[3] = 150;
            intensityValues[4] = 160;
       }
       else if (intensity == 3) {
            intensityValues[0] = 90;
            intensityValues[1] = 110;
            intensityValues[2] = 140;
            intensityValues[3] = 160;
            intensityValues[4] = 170;
       }
       else if (intensity == 4) {
            intensityValues[0] = 100;
            intensityValues[1] = 120;
            intensityValues[2] = 150;
            intensityValues[3] = 170;
            intensityValues[4] = 180;
       }
       else if (intensity == 5) {
            intensityValues[0] = 110;
            intensityValues[1] = 130;
            intensityValues[2] = 160;
            intensityValues[3] = 180;
            intensityValues[4] = 190;
       }  
}

void settingsDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  if (option == 1) {
    display.fillRect(3, 12, 3, 3, BLACK);
  }
  else if (option == 2) {
    display.fillRect(3, 22, 3, 3, BLACK);
  }
  else if (option == 3) {
    display.fillRect(3, 32, 3, 3, BLACK);
  }
  display.setCursor(10, 10);
  display.setTextWrap(true);
  display.println("Vibr: " + String(intensity));
  display.setCursor(10, 20);
  display.println("Contrast: " + String(contrast));
   display.setCursor(10, 30);
  if (vibrMode == 1)
    display.println("Vibr: ON");
  else
    display.println("Vibr: OFF");
  display.display();
}

void displayOutput(int readingLeft, int readingRight) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(2, 10);
  display.setTextWrap(true);
  display.println("Dist. L: " + String (readingLeft) + "cm");
  display.setCursor(2,20);
  display.println("Dist. R: " + String (readingRight) + "cm");
  display.display();
}


