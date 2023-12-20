/* Projektnamn:"höjdmätaren 1.0"
   Skapare av projekt: Elin Söderström
   Slutdatum: 13/12/2023
   Projektbeskrivning: En maskin som kan mäta lufttryck och med hjälp av skillnader u lufftryck från läge a och läge b kan räkna ut höjdskillanden mellan dessa två positioner
*/



#include "U8glib.h" //Inkluderar bibloteket U8glib.h som hanterar olika typer av grafiska displayer, t.ex min OLED-skärm
U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NO_ACK); //skapar ett objekt av klassen U8Glib vilket gör att jag får tillgång Oled-biblotekets funktioner
#include <Wire.h> //inkluderar Wire-bibloteket för att möjliggöra I2C-kommunikation
#include <SPI.h> //Inkluderar SPI-bubloteket för att möjliggöra för SPI-kommunikation
#include <Adafruit_Sensor.h> //inkluderar Adafruit_Sensor-bibloteket 
#include "Adafruit_BMP3XX.h" //inkluderar "Adafruit_BMP3XX"-bibloteket för att få tillgång till funktioner och definitioner relaterade till sensorn

#define SEALEVELPRESSURE_HPA (1013.25) //definerar konstanten SEALEVELPRESSURE_HPA och ger den värdet 1013.25, fungerar som en referenspunkt för lufttrycket vid havsnivån

Adafruit_BMP3XX bmp; //skapar ett objekt av klassen "Adafruit_BMP3XX" som namnges bmp, detta är för att kunna mäta lufttryck med min Adafruit-sensor

//initierar mina variariabler Counter, pressureOne, pressureTwo och heightChange och ger de värdet 0
int counter = 0;
int pressureOne = 0 ;
int pressureTwo = 0;
int heightChange = 0;

void setup() {

  Serial.begin(115200);

  pinMode(2, INPUT); //gör pin 2 till en input så den kan läsa av knappens signaler
  oled.setFont(u8g_font_helvB10); //sätter ett typsnitt till oled-skärmen
  Wire.begin(); //initierar I2C-kommuniaktionen

  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X); //Anger trycköversampling till X4, förbättrar nogrannheten i mätningen
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3); //Sätter koeffecient för IIR, det används för att filtrera/jämna ut mätvärden
  bmp.setOutputDataRate(BMP3_ODR_50_HZ); //ställer in hur ofta sensorn ger nya mätvärden

}

void loop() {

  if (digitalRead(2) == 1) { //En if-sats som säger att om knappen är nedtryckt så ska variablen "counter2 inkrementeras med 1
    counter = counter + 1;
  }

  if (counter == 1) { // en if-sats som säger att så länge knappen tryckts ned en gång (counter = 1) så ska "pressureOne" inkrementeras med värdet som mätaren läser av
    pressureOne = (bmp.pressure / 100.0);
    updateOled("Start Pressure: " , String(pressureOne), "PRESS BUTTON", "TO CONTINUE"); // funtionen updateOled tillkallas
    delay(1000);
  }

  if (counter == 2) { //if-sats som säger att så länge villkoret counter==2 är true, alltså om knappen tryckts ned två ggr, ska ett meddelande skrivas ut på skärmen
    updateOled("Good my lord!", "", "PLEASE MOVE &", "PRESS BUTTON"); // funtionen updateOled tillkallas

  }

  if (counter == 3) { //if-sats som så länge villkoret counter==3 är true ger variabeln "pressureTwo" värdet barometern läser av, sedan räknas skillnaden i höjd ut med hjälp av "pressureOne" och "presureTwo" och detta värde ges till varibeln "heightChange". Värdet på "pressureTwo" och "heightChange" skrivs sedan ut på skärmen
    pressureTwo = (bmp.pressure / 100.0);
    heightChange = (pressureTwo - pressureOne) * 9.144;
    updateOled("New Pressure:" , String(pressureTwo), "Height change: " , String(heightChange)); // funtionen updateOled tillkallas
    delay(1000);
  }

  if (counter >= 4 || counter == 0) { // if-sats som så länge counter >=4 stämmer, alltså om knappen tryckts den fyra eller fler gånger, nollställer alla variabler och skriver ut ett meddelande på skärmen.
    pressureOne = 0;
    pressureTwo = 0;
    counter = 0;
    updateOled("Welcome my lord!", "", "PRESS BUTTON", "TO START (please)"); // funtionen updateOled tillkallas
    heightChange = 0;
  }
  


}

void updateOled(String text1, String text2, String text3, String text4) { // Här definieras funktionen updateOled, den har fyra olika argument, alla strings med ett visst radavstånd emellan sig på den lilla skärmen
  oled.firstPage();
  do {
    oled.drawStr(0, 11, text1.c_str());
    oled.drawStr(0, 26, text2.c_str());
    oled.drawStr(0, 41, text3.c_str());
    oled.drawStr(0, 56, text4.c_str());
  } while (oled.nextPage());
}
