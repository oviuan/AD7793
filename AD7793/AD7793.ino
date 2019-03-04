/*
Referinte setup:
https://electronics.stackexchange.com/questions/21017/how-to-convert-0-to-10v-analog-signal-to-0-to-2-5v-for-adc-input
https://electronics.stackexchange.com/questions/6053/voltage-dividers-and-adcs
Modulul pe care mi l-ai trimis e destul de ok poate fi folosit in multe moduri, dar acum depinde si de ce o sa faci acolo exact.
Oricum, inainte de software trebuie sa te joci un pic cu partea hardware (interfatare senzor cu convertor).
Senzorul scoate 0 - 10V, dar in convertor poti sa bagi maxim vreo 5V (dupa fisa de specificatii, in functie de configurare).
De asemenea trebuie sa ai in vedere interferentele pentru ca intrarea este destul de sensibila.
Dar astea sunt chestii de trial and error, faci circuitul, vezi cum merge... Ideal ar fi sa ai un osciloscop sa vezi semnalele.


Referinta librarie:
https://create.arduino.cc/projecthub/iamsolar/a-library-for-getting-the-best-out-of-the-ad7793-adc-cf01af
Partea de software e destul de simpla.
Ce am scris mai jos compileaza, dar nu am cum sa testez sa vad ca merge end-to-end.
S-ar putea sa mai fie necesare modificari.
Pentru versiunea ta de arduino trebuie vazut care sunt pinii (si eventual setati in communication.h)

Arduino R3 pin#    AD7793 pin#   	Pullup resistors
    8   GPIO        3   ~CS		          50 KOhm
    9   RDY         15  DOUT/~RDY  
    12  MISO        15  DOUT/~RDY
    11  MOSI        16  DIN            	50 KOhm
    10  CS          not connected   
    13  SCK         1   CLK            	50 KOhm
*/

#include "ad7793.h"
#include "communication.h"
#include <SPI.h>

// Afiseaza registrul de stare
void printStatus()
{
  Serial.print("Ready: ");     Serial.println(AD7793_Ready());
  Serial.print("Error: ");     Serial.println(AD7793_Error());
  Serial.print("Channel 3: "); Serial.println(AD7793_Channel3());
  Serial.print("Channel 2: "); Serial.println(AD7793_Channel2());
  Serial.print("Channel 1: "); Serial.println(AD7793_Channel1());
}

void setup()
{
  Serial.begin(9600);
  while (!Serial);

  // Initializare
  unsigned char hwStatus = AD7793_Init();

  // "AD7793 hardware status = 1" - inseamna ca a detectat chipul
  Serial.print("AD7793 hardware status = ");
  Serial.println(hwStatus);

  printStatus();
  
  // Setare conversie continua
  AD7793_SetMode(AD7793_MODE_CONT);
  // Setare sursa de ceas interna (64 kHz)
  AD7793_SetClockSource(AD7793_CLK_INT);
  // Setare referinta de tensiune interna
  AD7793_SetIntReference(AD7793_REFSEL_INT);
  // Selectare intrare (AIN1+ - AIN1-)
  AD7793_SetChannel(AD7793_CH_AIN1P_AIN1M);
}

void loop()
{
  // Citeste o singura conversie
  unsigned long reading = AD7793_SingleConversion();

  Serial.print("Conversion result: "); Serial.println(reading);

  delay(1000);
}
