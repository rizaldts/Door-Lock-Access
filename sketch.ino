#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define PASSWORD_LENGTH 4
#define SERVO_PIN 10
#define BUZZER_PIN 11

char Data[PASSWORD_LENGTH + 1];
char Master[PASSWORD_LENGTH + 1] = "1234";
byte data_count = 0;
char customKey;

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  myServo.attach(SERVO_PIN);
  pinMode(BUZZER_PIN, OUTPUT);
  ServoClose();
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Masukkan PIN:");

  customKey = customKeypad.getKey();
  if (customKey) {
    Data[data_count] = customKey;
    lcd.setCursor(data_count, 1);
    lcd.print("*");
    data_count++;
  }

  if (data_count == PASSWORD_LENGTH) {
    Data[data_count] = '\0';
    Serial.print("PIN dimasukkan: ");
    Serial.println(Data);
    
    if (strcmp(Data, Master) == 0) {
      lcd.clear();
      lcd.print("PIN Benar");
      Serial.println("PIN Benar");
      ServoOpen();
      delay(3000);
      ServoClose();
    }
    else {
      lcd.clear();
      lcd.print("PIN Salah");
      Serial.println("PIN Salah");
      Buzz();
      delay(1000);
    }
    lcd.clear();
    clearData();
  }
}

void clearData() {
  while (data_count != 0) {
    Data[data_count--] = 0;
  }
  return;
}

void ServoOpen() {
  myServo.write(90);
}

void ServoClose() {
  myServo.write(0);
}

void Buzz() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(1000);
  digitalWrite(BUZZER_PIN, LOW);
}