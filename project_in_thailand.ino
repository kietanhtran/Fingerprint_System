#include <Adafruit_Fingerprint.h>
#include "LiquidCrystal_I2C.h"
#include "TU_TAO.h"
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial mySerial(2, 3);
int relay = 1;
int mode = 0;
int count = 2;
uint8_t p;
uint8_t id = 2;
bool flag_enroll = false;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
//  Serial.begin(9600);
  lcd.begin();
  lcd.clear();
  intro();
  lcd.createChar(1, LT);
  lcd.createChar(2, UB);
  lcd.createChar(3, RT);
  lcd.createChar(4, LL);
  lcd.createChar(5, LB);
  lcd.createChar(6, LR);
  lcd.createChar(7, MB);
  lcd.createChar(8, BLOCK);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
//  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }

  finger.getTemplateCount();
//  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
//  Serial.println("Waiting for valid finger...");
}

void loop() {

  if (digitalRead(4) == LOW) {
    delay(20);
    if (digitalRead(4) == LOW) {
      mode++;
      if (mode > 2) mode = 0;
    }
  } while (digitalRead(4) == LOW);
  
 

  

  if (mode == 0) {
    intro();

    p = finger.getImage();
    if (p != FINGERPRINT_OK)  return -1;
    p = finger.image2Tz();
    if (p != FINGERPRINT_OK)  return -1;
    p = finger.fingerFastSearch();
    if (p == FINGERPRINT_OK)  {
      lcd.setCursor(0, 0);
      lcd.print("  Match finger!  ");
      
      if (finger.fingerID > 0) {
        //        lcd.setCursor(0, 1);
        //        lcd.print("                ");
        //        lcd.setCursor(0, 1);
        //        lcd.print("ID: 1   Khac Huu");

        lcd.setCursor(0, 1);
        lcd.print("   Hello ID ");
        lcd.print(finger.fingerID);
        lcd.print("   ");

      }
      //      else if (finger.fingerID == 2) {
      //        lcd.setCursor(0, 1);
      //        lcd.print("                ");
      //        lcd.setCursor(0, 1);
      //        lcd.print("ID: 2   Anh Kiet");
      //      }
      //       else if (finger.fingerID == 3) {
      //        lcd.setCursor(0, 1);
      //        lcd.print("                ");
      //        lcd.setCursor(0, 1);
      //        lcd.print("ID: 3   Khac Huu");
      //      }
      //
      digitalWrite(relay, LOW);
      delay(5000);
      digitalWrite(relay, HIGH);
      intro();
    }
    else {
      lcd.setCursor(0, 0);
      lcd.print("   Not match!   ");
      lcd.setCursor(0, 1);
      lcd.print("Please try again!");
      delay(1000);
      intro();
    }
  }

  else if (mode == 1) {
    lcd.setCursor(0, 0);
    lcd.print("    Add Mode   ");
    lcd.setCursor(0, 1);
    lcd.print("  Confirm again  ");
    
    p = finger.getImage();
    if (p != FINGERPRINT_OK)  return -1;
    p = finger.image2Tz();
    if (p != FINGERPRINT_OK)  return -1;
    p = finger.fingerFastSearch();
    
    if (p == FINGERPRINT_OK) {
      flag_enroll = true;
      lcd.setCursor(0, 1);
      lcd.print("   Confirm OK   ");
      delay(1000);
      finger.getTemplateCount();
      if (flag_enroll == true) {
        max(finger.templateCount, 1);
        finger.templateCount++;
        lcd.setCursor(0, 1);
        lcd.print("Place new finger");
//        Serial.println("Ready to enroll a fingerprint!");
//        Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");

        if (id == 0) {// ID #0 not allowed, try again!
          return;
        }
//        Serial.print("Enrolling ID #");
//        Serial.println(id);

        getFingerprintEnroll();
        flag_enroll = false;

      }

    }
  }
  else if (mode == 2) {
    lcd.setCursor(0, 0);
    lcd.print("   Delete Mode  ");
    lcd.setCursor(0, 1);
    lcd.print("  Selected ID ");
    lcd.print(count);
     if (digitalRead(5) == LOW)
  {
    delay(20);
    if (digitalRead(5) == LOW)
    {
      count++;
    }
  } while (digitalRead(5) == LOW);

  if (digitalRead(6) == LOW)
  {
    delay(20);
    if (digitalRead(6) == LOW)
    {
      count--;
      if (count < 2) count = 2;
    }
  } while (digitalRead(6) == LOW);
    if (digitalRead(7) == LOW)
  {
    delay(20);
    if (digitalRead(7) == LOW)
    {
      p = finger.deleteModel(count);
      lcd.setCursor(0, 0);
      lcd.print("Deleting ID ");
      lcd.print(count);
      lcd.print("...");
      lcd.setCursor(0, 1);
      lcd.print("  Please wait!  ");
      delay(2000);
      if (p == FINGERPRINT_OK) {
        lcd.setCursor(0, 0);
        lcd.print("   Successful!  ");
        lcd.setCursor(0, 1);
         lcd.print("                ");
        delay(2000);
      }
    }
  } while (digitalRead(7) == LOW);
  }


}
void intro(void) {
  lcd.setCursor(0, 0);
  lcd.print("  Please place ");
  lcd.setCursor(0, 1);
  lcd.print("your fingerprint");


}

uint8_t getFingerprintEnroll() {

  int p = -1;
//  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:

        break;
      case FINGERPRINT_NOFINGER:

        break;
      case FINGERPRINT_PACKETRECIEVEERR:

        break;
      case FINGERPRINT_IMAGEFAIL:

        break;
      default:

        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:

      break;
    case FINGERPRINT_IMAGEMESS:

      return p;
    case FINGERPRINT_PACKETRECIEVEERR:

      return p;
    case FINGERPRINT_FEATUREFAIL:

      return p;
    case FINGERPRINT_INVALIDIMAGE:

      return p;
    default:

      return p;
  }
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("  Remove finger  ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  Serial.println("Remove finger");
  delay(1000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  lcd.setCursor(0, 0);
  lcd.print("   Place same   ");
  lcd.setCursor(0, 1);
  lcd.print("  finger again  ");
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:

        break;
      case FINGERPRINT_NOFINGER:

        break;
      case FINGERPRINT_PACKETRECIEVEERR:

        break;
      case FINGERPRINT_IMAGEFAIL:

        break;
      default:

        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:

      break;
    case FINGERPRINT_IMAGEMESS:

      return p;
    case FINGERPRINT_PACKETRECIEVEERR:

      return p;
    case FINGERPRINT_FEATUREFAIL:

      return p;
    case FINGERPRINT_INVALIDIMAGE:

      return p;
    default:

      return p;
  }

  // OK converted!
  lcd.setCursor(0, 0);
  lcd.print("Creating ID ");
  lcd.print(finger.templateCount);
  
  lcd.print("...");
  lcd.setCursor(0, 1);
  lcd.print("  Please wait!  ");
  delay(2000);
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");

  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {

    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    lcd.setCursor(0, 0);
    lcd.print("  Fingerprint   ");
    lcd.setCursor(0, 1);
    lcd.print(" did not match ");
    finger.templateCount--;
    delay(2000);
    return p;
  } else {

    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(finger.templateCount);
  if (p == FINGERPRINT_OK) {

    lcd.setCursor(0, 0);
    lcd.print("   Successful!  ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    delay(2000);

  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {

    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {

    return p; p = finger.createModel();
  } else if (p == FINGERPRINT_FLASHERR) {

    return p;
  } else {

    return p;
  }
}
