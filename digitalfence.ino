
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

bool outarea = false;
int soundpin = 8;
static const int RXPin = 0, TXPin = 1;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup() {
  
  //pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  ss.begin(GPSBaud);

  Serial.println(F("Sats HDOP  Latitude   Longitude   Fix   Chars Sentences Checksum"));
  Serial.println(F("           (deg)      (deg)       Age --  RX    RX        Fail"));
  Serial.println(F("------------------------------------------------------------------"));

}

void loop() {

  /*
  printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
  printFloat(gps.hdop.hdop(), gps.hdop.isValid(), 6, 1);
  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
    printInt(gps.charsProcessed(), true, 6);
  printInt(gps.sentencesWithFix(), true, 10);
  printInt(gps.failedChecksum(), true, 9);
  Serial.println();

  */
  outarea = false;

  if(gps.satellites.value()>7 && gps.satellites.isValid()){

    printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
    printFloat(gps.hdop.hdop(), gps.hdop.isValid(), 6, 1);
    printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
    printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
    printInt(gps.charsProcessed(), true, 6);
    printInt(gps.sentencesWithFix(), true, 10);
    printInt(gps.failedChecksum(), true, 9);
    Serial.println();
  
     if(gps.location.isValid()){
        if(gps.location.lng() < 4.696600 ){
         outarea = true;
         Serial.println(F("over algemene west lijn"));
        }
        if(gps.location.lat() > 50.954612  ){
          Serial.println(F("te ver naar het noorden"));
          if(gps.location.lng() < 4.697926){
            outarea = true;
            Serial.println(F("te ver naar het west en noorden tegelijk"));
          }
        }
        if(gps.location.lat() < 50.953618 ){
          outarea = true;
          Serial.println(F("over algemene zuid lijn"));
        }
     }
  

    /*
    if(gps.location.lng() < 4.696600 || gps.location.lat() > 50.954612 && gps.location.lng() < 4.697926  )){ // noord + west-lijn
     outarea = true;
    }else {
      outarea = false;
    }
    */
  }

  if(outarea){
   // digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      tone(soundpin, 2000);
  }else{
   noTone(soundpin);
   //   digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  }
  
  smartDelay(1000);
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}


static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
}

static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  smartDelay(0);
}

static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartDelay(0);
}
