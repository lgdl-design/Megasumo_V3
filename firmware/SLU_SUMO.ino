#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include "Adafruit_VL53L1X.h"
#include <Wire.h>

#define XSHUT_LEFT  4
#define XSHUT_FRONT 16
#define XSHUT_RIGHT 17

Adafruit_VL53L1X sensorLeft;
Adafruit_VL53L1X sensorFront;
Adafruit_VL53L1X sensorRight;

#define INA1 27
#define INA2 14
#define PWMA 13

#define INB1 25
#define INB2 33
#define PWMB 26

#define SharpLeft 34
#define SharpRight 36

#define Bright 32
#define Bleft 35

IRrecv receiver(5);
decode_results remote;
int count = 0;

int bottom = 0;

void setup() {
  pinMode(INA1, OUTPUT); // DIR_A
  pinMode(INA2, OUTPUT);
  pinMode(INB1, OUTPUT); // DIR_A
  pinMode(INB2, OUTPUT);

  pinMode(Bright, INPUT);
  pinMode(Bleft, INPUT);

  pinMode(SharpLeft, INPUT);
  pinMode(SharpRight, INPUT);


  ledcAttach(13, 1000, 8);
  ledcAttach(26, 1000, 8); 
  receiver.enableIRIn();
  Serial.begin(115200);
  Wire.begin(19, 18);
  sensorboot();
  

}

void loop() {
  if (receiver.decode(&remote)) {
    receiver.resume();
    Serial.println(remote.value, HEX);
    
    if(remote.value == 0xFF6897) {
      count++;
      if(count >= 3) count = 0;
    }

    // motor state changes on IR press
    if(count == 0 || count == 1) {
      stopMotors();
    }
    if(count == 2) {
      Forward();
    }
  }

  if(count == 2) {
      int16_t left  = sensorLeft.distance();
      int16_t front = sensorFront.distance();
      int16_t right = sensorRight.distance();
      float sharpL  = getSharpDistance(34);
      float sharpR  = getSharpDistance(36);

      bool frontDetect = front > 0 && front < 500;
      bool leftDetect  = left > 0  && left < 500;
      bool rightDetect = right > 0 && right < 500;
      bool sharpLDetect = sharpL != -1;
      bool sharpRDetect = sharpR != -1;


    if(digitalRead(Bleft) == 0 && digitalRead(Bright) == 0){
      bottom = 1;
      escapeLeft();
      delay(300);
    }

    else if(digitalRead(Bright) == 0){
      bottom = 1;
      escapeRight();
      delay(300);
    }

    else if (digitalRead(Bleft) == 0){
      bottom = 1;
      escapeLeft();
      delay(300);
    }

    else{
      if(frontDetect && leftDetect && rightDetect) {
        Forward();          // surrounded on all sides
      } else if(frontDetect && leftDetect) {
        TLeft();
        Forward();                // front + left
      } else if(frontDetect && rightDetect) {
        TRight(); 
        Forward();              // front + right
      } else if(frontDetect) {
        Forward();              // front only = charge!
      } else if(leftDetect) {
        Left();                 // left only
      } else if(rightDetect) {
        Right();                // right only
      } else if(sharpLDetect) {
        sharpLeft();            // sharp IR left
      } else if(sharpRDetect) {
        sharpRight();           // sharp IR right
      } 
      else{
        Forward();
      }
    }

  }
}


void stopMotors() {
  digitalWrite(INA1, LOW);
  digitalWrite(INA2, LOW);
  ledcWrite(PWMA, 0);
  digitalWrite(INB1, LOW);
  digitalWrite(INB2, LOW);
  ledcWrite(PWMB, 0);
}

void Forward() {
  digitalWrite(INA1, HIGH);
  digitalWrite(INA2, LOW);
  ledcWrite(PWMA, 220);
  digitalWrite(INB1, HIGH);
  digitalWrite(INB2, LOW);
  ledcWrite(PWMB, 220);
}

void Left() {
  digitalWrite(INA1, HIGH);
  digitalWrite(INA2, LOW);
  ledcWrite(PWMA, 100);
  digitalWrite(INB1, LOW);
  digitalWrite(INB2, HIGH);
  ledcWrite(PWMB, 100);
}

void Right() {
  digitalWrite(INA1, LOW);
  digitalWrite(INA2, HIGH);
  ledcWrite(PWMA, 100);
  digitalWrite(INB1, HIGH);
  digitalWrite(INB2, LOW);
  ledcWrite(PWMB, 100);
}

void TLeft() {
  digitalWrite(INA1, HIGH);
  digitalWrite(INA2, LOW);
  ledcWrite(PWMA, 50);
  digitalWrite(INB1, LOW);
  digitalWrite(INB2, HIGH);
  ledcWrite(PWMB, 50);
}

void TRight() {
  digitalWrite(INA1, LOW);
  digitalWrite(INA2, HIGH);
  ledcWrite(PWMA, 50);
  digitalWrite(INB1, HIGH);
  digitalWrite(INB2, LOW);
  ledcWrite(PWMB, 50);
}

void sharpRight() {
  digitalWrite(INA1, LOW);
  digitalWrite(INA2, HIGH);
  ledcWrite(PWMA, 200);
  digitalWrite(INB1, HIGH);
  digitalWrite(INB2, LOW);
  ledcWrite(PWMB, 150);
}

void sharpLeft() {
  digitalWrite(INA1, HIGH);
  digitalWrite(INA2, LOW);
  ledcWrite(PWMA, 200);
  digitalWrite(INB1, LOW);
  digitalWrite(INB2, HIGH);
  ledcWrite(PWMB, 200);
}

void escapeLeft() {
  digitalWrite(INA1, HIGH);
  digitalWrite(INA2, LOW);
  ledcWrite(PWMA, 200);
  digitalWrite(INB1, LOW);
  digitalWrite(INB2, HIGH);
  ledcWrite(PWMB, 200);
}

void escapeRight(){
  digitalWrite(INA1, LOW);
  digitalWrite(INA2, HIGH);
  ledcWrite(PWMA, 200);
  digitalWrite(INB1, HIGH);
  digitalWrite(INB2, LOW);
  ledcWrite(PWMB, 200);
}


void sensorboot(){
  // Shut all sensors down
  pinMode(XSHUT_LEFT, OUTPUT);
  pinMode(XSHUT_FRONT, OUTPUT);
  pinMode(XSHUT_RIGHT, OUTPUT);
  digitalWrite(XSHUT_LEFT, LOW);
  digitalWrite(XSHUT_FRONT, LOW);
  digitalWrite(XSHUT_RIGHT, LOW);
  delay(10);

  // Boot left L1X
  digitalWrite(XSHUT_LEFT, HIGH);
  delay(10);
  sensorLeft.begin(0x01);
  sensorLeft.startRanging();

  // Boot front L0X
  digitalWrite(XSHUT_FRONT, HIGH);
  delay(10);
  sensorFront.begin(0x02);
  sensorFront.startRanging();

  // Boot right L1X
  digitalWrite(XSHUT_RIGHT, HIGH);
  delay(10);
  sensorRight.begin(0x03);
  sensorRight.startRanging();

  // in sensorboot() after startRanging()
  sensorLeft.setTimingBudget(20);   // 20ms fastest
  sensorFront.setTimingBudget(20);
  sensorRight.setTimingBudget(20);
}

float getSharpDistance(int pin) {
  int numReadings = 10;
  long total = 0;

  for(int i = 0; i < numReadings; i++) {
    total += analogRead(pin);
  }

  float average = total / numReadings;
  float voltage = average * (3.3 / 4095.0);
  float cm = 27.86 / (voltage - 0.42);

  // ignore readings above 25cm with nothing there
  if(cm < 0 || cm > 25) return -1;

  return cm;
}
