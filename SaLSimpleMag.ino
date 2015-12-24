
#include "Baro.h"
#include "Filter.h"
#include "Memory.h"

int startup[] = {
  262, 196, 196, 220, 196, 0, 247, 262
};
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

#define sense_A A3
#define sense_B A4
#define sense_Bat A5
#define SS_baro 8
#define SS_mem 10
#define MOSI 6
#define MISO 9
#define SCK 7
#define samples 4

uint64_t contTimer = 0;
uint64_t batTimer = 0;
uint8_t samplesPerSecond = 0;
uint64_t beginingSamples = 0;
int cont = 0;
float bat = 0;
float groundAlt = 0;
float peakAltitude = 0;
float Apogee = 0;
boolean launchDetected = false;
boolean apogeeDetected = false;
uint8_t apogeeCheck = 0;
uint8_t currentLog = 0;
Baro baro(SS_baro, MOSI, MISO, SCK);
Filter filter(1.0);
Memory memory(SS_mem, MOSI, MISO, SCK);



void setup() {
  Serial.begin(57600);

  baro.reset_Baro();
  baro.readCoefficients();
  baro.readCoefficients();
  
  for (uint8_t i = 0; i < 50; ++i) {
    groundAlt += baro.getPres();
  }
  groundAlt = 0;
  startTone();
  for (uint8_t i = 0; i < 50; ++i) {
    groundAlt += baro.getPres();
  }
  groundAlt = (groundAlt / 50) / 30.48;
  delay(1000);
}

int ContCheck() {

  int _result = cont;
  if (millis() - contTimer > 5000) {
    contTimer = millis();
    int read_A = analogRead(sense_A);
    int read_B = analogRead(sense_B);
    if (read_A < 10 && read_B < 10) {
      _result = 0;
      tone(5, 400, 1000);
    }
    else if (read_A > 10 && read_B < 10) {
      _result = 1;
      tone(5, 400, 200);
    }
    else if (read_A < 10 && read_B > 10) {
      _result = 2;
      tone(5, 400, 200);
      delay(450);
      tone(5, 400, 200);
    }
    else if (read_A > 10 && read_B > 10) {
      _result = 3;
      tone(5, 400, 200);
      delay(450);
      tone(5, 400, 200);
      delay(450);
      tone(5, 400, 200);
    }
  }
  return _result;
}

float batCheck() {
  float _batLevel = bat;

  if (millis() - batTimer > 5000) {
    batTimer = millis();
    int batRaw = analogRead(sense_Bat);
    _batLevel = (float)batRaw * 0.01527;
    if (_batLevel < 3.7) {
      tone(5, 400, 4000);
    }
  }
  return _batLevel;
}


void startTone() {
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(5, startup[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(5);
  }
}

void logData( float alt) {

  unsigned long curTime = millis();
  float hundreth = curTime % 1000;
  int tenth = hundreth / 100;

  if (tenth == 0) {
    if (currentLog == 0) {
      currentLog++;
      memory.storeData(curTime, alt);
    }
  }
  else if (tenth == 1) {
    if (currentLog == 1) {
      currentLog++;
      memory.storeData(curTime, alt);
    }
  }
  else if (tenth == 2) {
    if (currentLog == 2) {
      currentLog++;
      memory.storeData(curTime, alt);
    }
  }
  else if (tenth == 3) {
    if (currentLog == 3) {
      currentLog++;
      memory.storeData(curTime, alt);
    }
  }
  else if (tenth == 4) {
    if (currentLog == 4) {
      currentLog++;
      memory.storeData(curTime, alt);
    }
  }
  else if (tenth == 5) {
    if (currentLog == 5) {
      currentLog++;
      memory.storeData(curTime, alt);
    }
  }
  else if (tenth == 6) {
    if (currentLog == 6) {
      currentLog++;
      memory.storeData(curTime, alt);
    }
  }
  else if (tenth == 7) {
    if (currentLog == 7) {
      currentLog++;
      memory.storeData(curTime, alt);
    }
  }
  else if (tenth == 8) {
    if (currentLog == 8) {
      currentLog++;
      memory.storeData(curTime, alt);
    }
  }
  else if (tenth == 9) {
    if (currentLog == 9) {
      currentLog = 0;
      memory.storeData(curTime, alt);
    }
  }
}

void loop() {
  /*

    if (!launchDetected) {
    // cont = ContCheck();
    //  bat = batCheck();
    }

    float curAlt = (baro.getPres() / 30.48) - groundAlt;
    float correctedAlt = filter.filterAlt(curAlt);

    if (correctedAlt > peakAltitude) {
    peakAltitude = correctedAlt;
    }
    if (!launchDetected && (peakAltitude > 6)) {
    launchDetected = true;
    //tone(5, 400, 4000);
    }

    if (launchDetected && !apogeeDetected) {
    if (correctedAlt < peakAltitude) {
      apogeeCheck++;
      if (apogeeCheck > 10) {
        apogeeDetected = true;
        tone(5, 400, 4000);
      }
    }
    else if (correctedAlt > peakAltitude) {
      apogeeCheck = 0;
    }
    }

    Serial.print(peakAltitude);
    Serial.print(" ");
    Serial.print(correctedAlt);
    Serial.println();

  */
   float curAlt = (baro.getPres() / 30.48) - groundAlt;
    float correctedAlt = filter.filterAlt(curAlt);
    
  ////logData(correctedAlt);
}

