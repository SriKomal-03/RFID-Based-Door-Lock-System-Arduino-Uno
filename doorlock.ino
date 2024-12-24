#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
String UID = "3E ED 8E 59";
byte lock = 0;

Servo servo;
MFRC522 rfid(SS_PIN, RST_PIN);


void setup() {
  Serial.begin(9600);
  servo.write(70);
  servo.attach(3);
  SPI.begin();
  rfid.PCD_Init();
}

void loop() {
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  String ID = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    ID.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    ID.concat(String(rfid.uid.uidByte[i], HEX));
    delay(300);
  }
  ID.toUpperCase();

  if (ID.substring(1) == UID && lock == 0 ) {
  Serial.println("Door Lock");
    servo.write(70);
    delay(1500);
    lock = 1;
  } else if (ID.substring(1) == UID && lock == 1 ) {
  Serial.println("Door Open");
    servo.write(160);
    delay(1500);
    lock = 0;
  } else {
    Serial.println("Wrong Card");
  }
}