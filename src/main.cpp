#include<Arduino.h>
#include<LiquidCrystal.h>
#include<OneWire.h>
#include<DallasTemperature.h>

//Sensor Pin
#define DS18B20 8

//Parameters
const int v1Temp = 80;
const int v2Temp = 85;
const int v3Temp = 90;
const int maxTemperature = 95;
const int fanMaxDelay = 120;
const int fan1Pin = 9;
const int fan2Pin = 10;
const int fan3Pin = 11;
const int buzzerPin = 12;

//Updates
unsigned long nextUpdate1;
unsigned long nextUpdate2;
unsigned long nextUpdate3;

//Global Variables
bool showTemperature = false;
//bool alert = false;
bool fanAlternance = true; //on
bool tempAlternance = true; //on
float tempC;
int temp;
int fanSpeed = 0;
int fanPos = 1;
int fanT = 0;

//initialize the library with the numbers of the interface pins
//LiquidCrystal Lcd(12, 11, 5, 4, 3, 2);
LiquidCrystal Lcd(2, 3, 4, 5, 6, 7);
OneWire ourWire(DS18B20);
DallasTemperature sensors(&ourWire);

void OverlapDisplay(int overlapWith) {

  for(int i = 0; i < 20; i++) {

    //First row
    Lcd.setCursor(i, 0);

    if(overlapWith == 32) {

      String bgTxt = "   Feito por:   ";
      Lcd.write(bgTxt[i]);
    }
    else {

      Lcd.print((char) overlapWith);
    }

    //Second row
    if(i > 3) {

      Lcd.setCursor(i - 4, 1);

      if(overlapWith == 32) {

        String bgTxt2 = " Cris and JeffG ";
        Lcd.write(bgTxt2[i - 4]);
      }
      else {

        Lcd.print((char) overlapWith);
      }
    }

    delay(25);
  }
}

void Println(String txt, int line) {

  for(unsigned int i = 0; i <= 15; i++) {

    Lcd.setCursor(i, line);

    if(i > txt.length() - 1) {
      
      Lcd.write(32);
    }
    else {

      Lcd.write(txt[i]);
    }
  }
}

void Start() {
  
  //Presentation
  Println("      S.O.S     ", 0);
  Println("  AUTO ELETRICA ", 1);
  delay(3000);
  OverlapDisplay(255); //0
  OverlapDisplay(32);
  delay(1000);
  Lcd.clear();
  showTemperature = true;
}

void UpdateTemperature() { 

  //Time since last fan operation
  fanT++;

  //Temperature Calculation (Analogic)
  /*int sensorValue = analogRead(A0);
  tempC = (float) sensorValue / 1024.0;
  tempC = tempC * 5;
  tempC = tempC - 0.5;
  tempC = tempC * 100;*/

  //Temperature Calculation (Digital)
  tempC = sensors.getTempCByIndex(0);
  temp = (int) tempC;

  if(showTemperature) {

    //The whole display will be updated each update
    String displayTxt = "Temperatura: ";

    //fan icon
    if(fanSpeed == 0) {

      displayTxt += "   ";
    }
    else {

      displayTxt += "[ ]";
    }

    //Temperature bar
    for(int i = 0; i < 10; i++) {

      if(i < temp / (maxTemperature / 10)) {

        displayTxt += (char)255; //1
      }
      else {

        displayTxt += " ";
      }
    }

    //Temperature alert
    if(temp >= maxTemperature) {

      if(tempAlternance) {

        displayTxt += "!";
      }
      else {

        displayTxt += " ";
      }

      tempAlternance = !tempAlternance;
    }
    else {

      displayTxt += " ";
    }

    //Temperature
    displayTxt += temp;
    displayTxt += (char)223; //178
    displayTxt += "c";
    Print(displayTxt);
  }

  //TempDebug
  Serial.println(temp);
}

void Events() {

  //Temperature;
  if(temp >= v1Temp && temp < v2Temp) {

    fanSpeed = 1;
  }
  else if(temp >= v2Temp && temp < v3Temp) {

    fanSpeed = 2;
  }
  else if(temp >= v3Temp && temp < maxTemperature) {

    fanSpeed = 3;
  }
  else if(temp >= maxTemperature) {

    fanSpeed = 3;
    digitalWrite(buzzerPin, HIGH);
  }

  //Fan
  if(fanSpeed == 1) {

    //Turn on first pin and turn off others
    digitalWrite(fan1Pin, LOW);
    digitalWrite(fan2Pin, HIGH);
    digitalWrite(fan3Pin, HIGH);
  }
  else if(fanSpeed == 2) {

    //Turn on second pin and turn off others
    digitalWrite(fan1Pin, HIGH);
    digitalWrite(fan2Pin, LOW);
    digitalWrite(fan3Pin, HIGH);
  }
  else if(fanSpeed >= 3 || fanSpeed <= -1) {

    //Turn on Thirth pin and turn off others
    digitalWrite(fan1Pin, HIGH);
    digitalWrite(fan2Pin, HIGH);
    digitalWrite(fan3Pin, LOW);

    if(fanSpeed <= -1) {

      digitalWrite(buzzerPin, HIGH);
    }
  }
  else {

    digitalWrite(fan1Pin, HIGH);
    digitalWrite(fan2Pin, HIGH);
    digitalWrite(fan3Pin, HIGH);
  }
}

void UpdateFan() {
  
  char fan = ' ';

    if(fanSpeed == -1) {
      
      if(fanAlternance) {

        fan = '!';
      }
      else {

        fan = ' ';
      }

      fanAlternance = !fanAlternance;
    }
    else if (fanSpeed > 0) {

      if(fanPos == 1) {

        fan = 47;
      }
      else if(fanPos == 2) {

        fan = 45;
      }
      else if(fanPos == 3) {

        fan = 92;
      }
      else if(fanPos == 4) {

        fan = 124;
        fanPos = 0;
      }

      fanPos++;
      fanT = 0;
    }

    if(showTemperature) {

      Lcd.setCursor(14, 0);
      Lcd.write(fan);
    }
}

void Print(String txt) {

  for(unsigned int i = 0; i <= 15; i++) {

    Lcd.setCursor(i, 0);

    if(i > txt.length() - 1) {

      Lcd.write(32);
    }
    else {

      Lcd.write(txt[i]);
    }

    if(txt.length() > 16) {

      Lcd.setCursor(i, 1);

      if(i + 16 > txt.length() - 1) {

        Lcd.write(32);
      }
      else {

        Lcd.write(txt[i + 16]);
      }
    }
  }
}

void setup() {

  Serial.begin(2400); //Temporaly

  //ModuleSetup
  Lcd.begin(16, 2);
  sensors.begin();

  //Pin Setup
  pinMode(fan1Pin, OUTPUT); //GND
  pinMode(fan2Pin, OUTPUT); //GND
  pinMode(fan3Pin, OUTPUT); //GND
  pinMode(buzzerPin, OUTPUT); //+5v

  Start();
  //ListCharTable();
}

void loop() {

  unsigned long currentTime = millis();

  //1 Frame
  if(currentTime >= nextUpdate1) {

    UpdateTemperature();
    Events();

    if(fanSpeed >= 1) {

      UpdateFan();
    }

    nextUpdate1 += 1000;
  }

  //2 Frame
  if(currentTime >= nextUpdate2) {

    digitalWrite(buzzerPin, LOW);

    if(fanSpeed == 2) {

      UpdateFan();
    }

    nextUpdate2 += 500;
  }

  //4 Frame
  if(currentTime >= nextUpdate3) {

    if(fanSpeed == 3) {

      UpdateFan();
    }

    nextUpdate3 += 250;
  }

  //Exit//
  if(fanT > fanMaxDelay) {

    showTemperature = false;
    fanSpeed = -1;
    Lcd.setCursor(0, 0);
    Lcd.print("FAN ERROR!");
    digitalWrite(buzzerPin, HIGH);
  }

  if(temp <= -127) {

    showTemperature = false;
    fanSpeed = -1;
    Lcd.setCursor(0, 0);
    Lcd.print("TEMPERATURE ERROR!");
    digitalWrite(buzzerPin, HIGH);
  }
}

void ListCharTable() {

  for(int i = 0; i < 256; i++) {

    Lcd.setCursor(0, 0);
    Lcd.print((char) i);
    Lcd.setCursor(0, 1);
    Lcd.print(i);
    delay(500);
  }
}